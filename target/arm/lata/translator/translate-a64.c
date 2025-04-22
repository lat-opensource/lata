/*
 *  AArch64 translation
 *
 *  Copyright (c) 2013 Alexander Graf <agraf@suse.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
#include "qemu/osdep.h"

#include "target/arm/tcg/translate.h"
#include "qemu/log.h"
#include "disas/disas.h"
#include "target/arm/tcg/arm_ldst.h"
#include "semihosting/semihost.h"
#include "cpregs.h"

#ifdef CONFIG_LATA
#include "target/arm/lata/include/translate.h"
#include "target/arm/lata/include/insts-pattern.h"
#include "ir1.h"
#include "ir1-arg.h"
#include "translate-a64.h"
#include "tu.h"
#endif

static TCGv_i64 cpu_X[32];
static TCGv_i64 cpu_pc;

/* Load/store exclusive handling */
static TCGv_i64 cpu_exclusive_high;

static const char *regnames[] = {
    "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7",
    "x8", "x9", "x10", "x11", "x12", "x13", "x14", "x15",
    "x16", "x17", "x18", "x19", "x20", "x21", "x22", "x23",
    "x24", "x25", "x26", "x27", "x28", "x29", "lr", "sp"
};

enum a64_shift_type {
    A64_SHIFT_TYPE_LSL = 0,
    A64_SHIFT_TYPE_LSR = 1,
    A64_SHIFT_TYPE_ASR = 2,
    A64_SHIFT_TYPE_ROR = 3
};

/*
 * Helpers for extracting complex instruction fields
 */

/*
 * For load/store with an unsigned 12 bit immediate scaled by the element
 * size. The input has the immediate field in bits [14:3] and the element
 * size in [2:0].
 */
static int uimm_scaled(DisasContext *s, int x)
{
    unsigned imm = x >> 3;
    unsigned scale = extract32(x, 0, 3);
    return imm << scale;
}

/* For load/store memory tags: scale offset by LOG2_TAG_GRANULE */
static int scale_by_log2_tag_granule(DisasContext *s, int x)
{
    return x << LOG2_TAG_GRANULE;
}

/*
 * Include the generated decoders.
 */

// #include "decode-sme-fa64.c.inc"
#ifdef CONFIG_LATA
#include "../ir1/decode_ir1_a64.c.inc"
#endif
/* Table based decoder typedefs - used when the relevant bits for decode
 * are too awkwardly scattered across the instruction (eg SIMD).
 */
typedef void AArch64DecodeFn(DisasContext *s, uint32_t insn);

typedef struct AArch64DecodeTable {
    uint32_t pattern;
    uint32_t mask;
    AArch64DecodeFn *disas_fn;
} AArch64DecodeTable;

/* initialize TCG globals.  */
void a64_translate_init(void)
{
    int i;

    cpu_pc = tcg_global_mem_new_i64(cpu_env,
                                    offsetof(CPUARMState, pc),
                                    "pc");
    for (i = 0; i < 32; i++) {
        cpu_X[i] = tcg_global_mem_new_i64(cpu_env,
                                          offsetof(CPUARMState, xregs[i]),
                                          regnames[i]);
    }

    cpu_exclusive_high = tcg_global_mem_new_i64(cpu_env,
        offsetof(CPUARMState, exclusive_high), "exclusive_high");
}

uint64_t asimd_imm_const(uint32_t imm, int cmode, int op)
{
    /* Expand the encoded constant as per AdvSIMDExpandImm pseudocode */
    switch (cmode) {
    case 0: case 1:
        /* no-op */
        break;
    case 2: case 3:
        imm <<= 8;
        break;
    case 4: case 5:
        imm <<= 16;
        break;
    case 6: case 7:
        imm <<= 24;
        break;
    case 8: case 9:
        imm |= imm << 16;
        break;
    case 10: case 11:
        imm = (imm << 8) | (imm << 24);
        break;
    case 12:
        imm = (imm << 8) | 0xff;
        break;
    case 13:
        imm = (imm << 16) | 0xffff;
        break;
    case 14:
        if (op) {
            /*
             * This and cmode == 15 op == 1 are the only cases where
             * the top and bottom 32 bits of the encoded constant differ.
             */
            uint64_t imm64 = 0;
            int n;

            for (n = 0; n < 8; n++) {
                if (imm & (1 << n)) {
                    imm64 |= (0xffULL << (n * 8));
                }
            }
            return imm64;
        }
        imm |= (imm << 8) | (imm << 16) | (imm << 24);
        break;
    case 15:
        if (op) {
            /* Reserved encoding for AArch32; valid for AArch64 */
            uint64_t imm64 = (uint64_t)(imm & 0x3f) << 48;
            if (imm & 0x80) {
                imm64 |= 0x8000000000000000ULL;
            }
            if (imm & 0x40) {
                imm64 |= 0x3fc0000000000000ULL;
            } else {
                imm64 |= 0x4000000000000000ULL;
            }
            return imm64;
        }
        imm = ((imm & 0x80) << 24) | ((imm & 0x3f) << 19)
            | ((imm & 0x40) ? (0x1f << 25) : (1 << 30));
        break;
    }
    if (op) {
        imm = ~imm;
    }
    return dup_const(MO_32, imm);
}

/*
 * The imm8 encodes the sign bit, enough bits to represent an exponent in
 * the range 01....1xx to 10....0xx, and the most significant 4 bits of
 * the mantissa; see VFPExpandImm() in the v8 ARM ARM.
 */
uint64_t vfp_expand_imm(int size, uint8_t imm8)
{
    uint64_t imm;

    switch (size) {
    case MO_64:
        imm = (extract32(imm8, 7, 1) ? 0x8000 : 0) |
            (extract32(imm8, 6, 1) ? 0x3fc0 : 0x4000) |
            extract32(imm8, 0, 6);
        imm <<= 48;
        break;
    case MO_32:
        imm = (extract32(imm8, 7, 1) ? 0x8000 : 0) |
            (extract32(imm8, 6, 1) ? 0x3e00 : 0x4000) |
            (extract32(imm8, 0, 6) << 3);
        imm <<= 16;
        break;
    case MO_16:
        imm = (extract32(imm8, 7, 1) ? 0x8000 : 0) |
            (extract32(imm8, 6, 1) ? 0x3000 : 0x4000) |
            (extract32(imm8, 0, 6) << 6);
        break;
    default:
        g_assert_not_reached();
    }
    return imm;
}

static void gen_pc_plus_diff(DisasContext *s, TCGv_i64 dest, target_long diff)
{
    assert(s->pc_save != -1);
    if (tb_cflags(s->base->tb) & CF_PCREL) {
        tcg_gen_addi_i64(dest, cpu_pc, (s->pc_curr - s->pc_save) + diff);
    } else {
        tcg_gen_movi_i64(dest, s->pc_curr + diff);
    }
}

void lata_gen_exit_tb_ret_0(DisasContext *s);
void lata_gen_exit_tb_ret_0(DisasContext *s)
{
    IR2_OPND ir2_opnd_addr;

    int64_t curr_ins_pos = (unsigned long)s->base->tb->tc.ptr + 
                            (lsenv->tr_data->real_ir2_inst_num << 2);
    int64_t exit_offset = context_switch_native_to_bt_ret_0 - curr_ins_pos;

    ir2_opnd_build(&ir2_opnd_addr, IR2_OPND_IMM, exit_offset >> 2);
    la_b(ir2_opnd_addr);
}

void lata_gen_a64_update_pc(DisasContext *s, target_long diff);
void lata_gen_a64_update_pc(DisasContext *s, target_long diff)
{
    target_ulong dest = s->pc_curr + diff;

    li_d(a0_ir2_opnd, dest);
    la_st_d(a0_ir2_opnd, env_ir2_opnd, env_offset_pc());
}

void gen_a64_update_pc(DisasContext *s, target_long diff)
{
    gen_pc_plus_diff(s, cpu_pc, diff);
    s->pc_save = s->pc_curr + diff;
}

static void lata_unallocated_encoding(DisasContext *s){

    if (clearGprHigh) {
        for (int i = 0; i < 32; ++i) {
            if (arm_la_map[i] >= 0) {
                clear_gpr_high(i);
            }
        }
    }

    IR2_OPND temp = ra_alloc_itemp();

    li_d(temp, s->pc_curr);
    la_st_d(temp, env_ir2_opnd, env_offset_pc());
    lata_gen_call_helper_prologue(tcg_ctx);

    //CPUARMState *env, uint32_t excp, uint32_t syndrome
    la_mov64(a0_ir2_opnd, env_ir2_opnd);
    li_d(a1_ir2_opnd, EXCP_UDEF);
    li_d(a2_ir2_opnd, syn_uncategorized());

    li_d(temp, (uint64_t)helper_exception_with_syndrome);
    la_jirl(ra_ir2_opnd, temp, 0);

    s->base->is_jmp = DISAS_NORETURN;

    free_alloc_gpr(temp);
    return;
}

static void lata_clean_data_tbi(DisasContext *s, IR2_OPND *dst, IR2_OPND *src, int tbi)
{
    if (tbi == 0) {
        /* Load unmodified address */
        la_mov64(*dst, *src);
    } else if (!regime_has_2_ranges(s->mmu_idx)) {
        /* Force tag byte to all zero */
        la_bstrpick_d(*dst, *src, 56, 0);
    } else {
        /* Sign-extend from bit 55.  */
        la_slli_d(*dst, *src, 8);
        la_srai_d(*dst, *dst, 8);

        switch (tbi) {
        case 1:
            /* tbi0 but !tbi1: only use the extension if positive */
            la_and(*dst, *dst, *src);
            break;
        case 2:
            /* !tbi0 but tbi1: only use the extension if negative */
            la_or(*dst, *dst, *src);
            break;
        case 3:
            /* tbi0 and tbi1: always use the extension */
            break;
        default:
            g_assert_not_reached();
        }
    }
}

typedef struct DisasCompare64 {
    TCGCond cond;
    TCGv_i64 value;
} DisasCompare64;

static inline bool use_goto_tb(DisasContext *s, uint64_t dest)
{
    if (s->ss_active) {
        return false;
    }
    return translator_use_goto_tb(s->base, dest);
}

static void gen_goto_tb_indirect(DisasContext *s, uint32_t rn)
{
    IR2_OPND reg_n = alloc_gpr_src(rn);

#ifdef CONFIG_LATA_INDIRECT_JMP
        IR2_OPND guest_pc = ra_alloc_itemp();
        IR2_OPND host_pc  = ra_alloc_itemp();
        IR2_OPND exit = ir2_opnd_new_type(IR2_OPND_LABEL);

        if (option_fam_jmp_cache) {
            li_d(host_pc, (uint64_t)(current_cpu->env_ptr->pc_map_cache));
            la_alsl_d(host_pc, reg_n, host_pc, 2);
            la_ld_d(host_pc, host_pc, 0);
            la_beqz(host_pc, exit);
            la_jirl(zero_ir2_opnd, host_pc, 0);
            la_label(exit);
            la_st_d(reg_n, env_ir2_opnd, env_offset_pc());
            lata_gen_exit_tb_ret_0(s);
            return;
        }

        if (indirect_jmp_opt_profile) {
            la_ld_d(guest_pc, env_ir2_opnd, env_offset(jr_cnt));
            la_addi_d(guest_pc, guest_pc, 1);
            la_st_d(guest_pc, env_ir2_opnd, env_offset(jr_cnt));
        }

        la_bstrpick_d(guest_pc, reg_n, LATA_PC_LOW_BIT + TB_JMP_CACHE_BITS - 1, LATA_PC_LOW_BIT);
        li_d(host_pc, (uint64_t)(current_cpu->env_ptr->pc_map_cache));
        la_alsl_d(host_pc, guest_pc, host_pc, 3);
        la_ld_d(guest_pc, host_pc, 0); // guest pc
        la_ld_d(host_pc, host_pc, 8); // host pc
        la_bne(reg_n, guest_pc, exit);

        if (indirect_jmp_opt_profile) {
            la_ld_d(guest_pc, env_ir2_opnd, env_offset(jr_hit));
            la_addi_d(guest_pc, guest_pc, 1);
            la_st_d(guest_pc, env_ir2_opnd, env_offset(jr_hit));
        }

        la_jirl(zero_ir2_opnd, host_pc, 0);
        la_label(exit);

        free_alloc_gpr(guest_pc);
        free_alloc_gpr(host_pc);
#endif

    la_st_d(reg_n, env_ir2_opnd, env_offset_pc());

    /* do not link, but context_switch_native_to_bt_ret_0 will do this */
    lata_gen_exit_tb_ret_0(s);

    free_alloc_gpr(reg_n);
}

static inline void li_arm_addr(IR2_OPND opnd2, int64_t value){

    uint32_t /* hi32 ,*/ lo32;

    lo32 = value & 0xffffffff;
    //hi32 = value >> 32;

    la_lu12i_w(opnd2, lo32 >> 12);
    la_ori(opnd2, opnd2, lo32 & 0xfff);
};

static inline void li_tb(IR2_OPND opnd2, int64_t value){

    uint32_t hi32, lo32;

    lo32 = value & 0xffffffff;
    hi32 = value >> 32;

    la_lu12i_w(opnd2, lo32 >> 12);
    la_ori(opnd2, opnd2, lo32 & 0xfff);
    la_lu32i_d(opnd2, hi32 & 0xfffff);
};

static void helper_tb_link_debug(vaddr pc, CPUARMState *env, 
                                    const TranslationBlock *tb){
        CPUState *cpu = env_cpu(env);
        if (qemu_log_in_addr_range(pc)) {
        // qemu_log_mask(CPU_LOG_EXEC,
        //               "Trace %d: [ /%016" VADDR_PRIx "/%08x/%08x] %s\n",
        //               cpu->cpu_index,   pc,
        //               tb->flags, tb->cflags, lookup_symbol(pc));
        qemu_log_mask(CPU_LOG_EXEC,
                      "Trace %d: %p [%08" PRIx64
                      "/%016" VADDR_PRIx "/%08x/%08x] %s\n",
                      cpu->cpu_index, tb->tc.ptr, tb->cs_base, pc,
                      tb->flags, tb->cflags, lookup_symbol(pc));

        if (qemu_loglevel_mask(CPU_LOG_TB_CPU)) {
            FILE *logfile = qemu_log_trylock();
            if (logfile) {
                int flags = 0;

                if (qemu_loglevel_mask(CPU_LOG_TB_FPU)) {
                    flags |= CPU_DUMP_FPU;
                }

                if (qemu_loglevel_mask(CPU_LOG_TB_VPU)) {
                    flags |= CPU_DUMP_VPU;
                }
                cpu_dump_state(cpu, logfile, flags);
                qemu_log_unlock(logfile);
            }
        }
    }
}

static void tb_link_debug(target_ulong pc, TranslationBlock *tb){
    IR2_OPND temp = ra_alloc_itemp();

    li_d(temp, pc);
    la_st_d(temp, env_ir2_opnd, env_offset_pc());
    lata_gen_call_helper_prologue(tcg_ctx);

    li_d(a0_ir2_opnd, pc);
    la_mov64(a1_ir2_opnd, env_ir2_opnd);
    li_d(a2_ir2_opnd, (uint64_t)tb);

    li_d(temp, (uint64_t)helper_tb_link_debug);
    la_jirl(ra_ir2_opnd, temp, 0);

    lata_gen_call_helper_epilogue(tcg_ctx);
    free_alloc_gpr(temp);
}

static void gen_goto_tb(DisasContext *s, int n, int64_t diff)
{
    IR2_OPND goto_label = ir2_opnd_new_type(IR2_OPND_LABEL);
    IR2_OPND ir2_opnd_addr;
    TranslationBlock *tb = lsenv->tr_data->curr_tb;
    
    if(option_fam_jmp_cache){
        uint64_t guest_dest = s->pc_curr + diff;
        uint64_t* pc_map_cache_ptr = (uint64_t*) current_cpu->env_ptr->pc_map_cache;
        uint64_t host_dest = pc_map_cache_ptr[guest_dest];

        if(host_dest){
            uint64_t curr_ins_pos = (unsigned long)tb->tc.ptr + (lsenv->tr_data->real_ir2_inst_num << 2);
            uint64_t exit_offset = host_dest - curr_ins_pos;

            ir2_opnd_build(&ir2_opnd_addr, IR2_OPND_IMM, exit_offset >> 2);
            la_b(ir2_opnd_addr);
            return;
        }
    }
    
    // tb_link_debug(dest, tb);
    la_label(goto_label);
    tb->jmp_reset_offset[n] = ir2_opnd_label_id(&goto_label);
    ir2_opnd_build(&ir2_opnd_addr, IR2_OPND_IMM, 1);
    la_b(ir2_opnd_addr); // nop

    lata_gen_a64_update_pc(s, diff);

    if (qemu_loglevel_mask(CPU_LOG_TB_NOCHAIN)) {
        li_d(a0_ir2_opnd, 0); // do not link
    } else {
        li_tb(a0_ir2_opnd, (uint64_t)tb | n);
    }

    int64_t curr_ins_pos = (unsigned long)tb->tc.ptr + (lsenv->tr_data->real_ir2_inst_num << 2);
    int64_t exit_offset = context_switch_native_to_bt - curr_ins_pos;

    ir2_opnd_build(&ir2_opnd_addr, IR2_OPND_IMM, exit_offset >> 2);
    la_b(ir2_opnd_addr);
}

/* dest = T0 - T1; compute C, N, V and Z flags */

static void gen_add_CC(IR2_OPND* src1, IR2_OPND *src2, int sf){
    if(sf){
        la_x86add_d(*src1, *src2);
    }else{
        la_x86add_w(*src1, *src2);
    }
}

static void gen_sub_CC(IR2_OPND* src1, IR2_OPND *src2, int sf){
    if(sf){
        /*  bits(datasize) operand1 = *src1;
            bits(datasize) operand2 = NOT(*src2);
            (-, nzcv) = AddWithCarry(operand1, operand2, '1');
        */
        IR2_OPND temp = ra_alloc_itemp();
        la_lu12i_w(temp, 0x20000);
        la_armmtflag(temp, 0x39); // pstate.c = 1
        la_orn(temp, zero_ir2_opnd, *src2); // NOT(*src2)
        la_x86adc_d(*src1, temp);
        free_alloc_gpr(temp);
    }else{
        /*  bits(datasize) operand1 = *src1;
            bits(datasize) operand2 = NOT(*src2);
            (-, nzcv) = AddWithCarry(operand1, operand2, '1');
        */
        IR2_OPND temp = ra_alloc_itemp();
        la_lu12i_w(temp, 0x20000);
        la_armmtflag(temp, 0x39); // pstate.c = 1
        la_orn(temp, zero_ir2_opnd, *src2); // NOT(*src2)
        la_x86adc_w(*src1, temp);
        free_alloc_gpr(temp);
    }
}

static void gen_add(IR2_OPND* dst, IR2_OPND* src1, IR2_OPND *src2, int sf){
    if(sf){
        la_add_d(*dst, *src1, *src2);
    }else{
        la_add_w(*dst, *src1, *src2);
    }
}

static void gen_sub(IR2_OPND* dst, IR2_OPND* src1, IR2_OPND *src2, int sf){
    if(sf){
        la_sub_d(*dst, *src1, *src2);
    }else{
        la_sub_w(*dst, *src1, *src2);
    }
}

/* Check that FP/Neon access is enabled. If it is, return
 * true. If not, emit code to generate an appropriate exception,
 * and return false; the caller should not emit any code for
 * the instruction. Note that this check must happen after all
 * unallocated-encoding checks (otherwise the syndrome information
 * for the resulting exception will be incorrect).
 */
static bool fp_access_check_only(DisasContext *s)
{
    if (s->fp_excp_el) {
        assert(!s->fp_access_checked);
        s->fp_access_checked = true;

        assert(0);
        return false;
    }
    s->fp_access_checked = true;
    return true;
}

static bool fp_access_check(DisasContext *s)
{
    if (!fp_access_check_only(s)) {
        return false;
    }
    if (s->sme_trap_nonstreaming && s->is_nonstreaming) {
        assert(0);
        return false;
    }
    return true;
}

/*
 * Check that SME access is enabled, raise an exception if not.
 * Note that this function corresponds to CheckSMEAccess and is
 * only used directly for cpregs.
 */
static bool sme_access_check(DisasContext *s)
{
    if (s->sme_excp_el) {
        assert(0);
        return false;
    }
    return true;
}

/*
 * Check that SVE access is enabled.  If it is, return true.
 * If not, emit code to generate an appropriate exception and return false.
 * This function corresponds to CheckSVEEnabled().
 */
bool sve_access_check(DisasContext *s)
{
    if (s->pstate_sm || !dc_isar_feature(aa64_sve, s)) {
        assert(0);
    } else if (s->sve_excp_el) {
        assert(0);
        goto fail_exit;
    }
    s->sve_access_checked = true;
    return fp_access_check(s);

 fail_exit:
    /* Assert that we only raise one exception per instruction. */
    assert(!s->sve_access_checked);
    s->sve_access_checked = true;
    return false;
}

static inline void gen_check_sp_alignment(DisasContext *s)
{
    /* The AArch64 architecture mandates that (if enabled via PSTATE
     * or SCTLR bits) there is a check that SP is 16-aligned on every
     * SP-relative load or store (with an exception generated if it is not).
     * In line with general QEMU practice regarding misaligned accesses,
     * we omit these checks for the sake of guest program performance.
     * This function is provided as a hook so we can more easily add these
     * checks in future (possibly as a "favour catching guest program bugs
     * over speed" user selectable option).
     */
}

/*
 * This provides a simple table based table lookup decoder. It is
 * intended to be used when the relevant bits for decode are too
 * awkwardly placed and switch/if based logic would be confusing and
 * deeply nested. Since it's a linear search through the table, tables
 * should be kept small.
 *
 * It returns the first handler where insn & mask == pattern, or
 * NULL if there is no match.
 * The table is terminated by an empty mask (i.e. 0)
 */
static inline AArch64DecodeFn *lookup_disas_fn(const AArch64DecodeTable *table,
                                               uint32_t insn)
{
    const AArch64DecodeTable *tptr = table;

    while (tptr->mask) {
        if ((insn & tptr->mask) == tptr->pattern) {
            return tptr->disas_fn;
        }
        tptr++;
    }
    return NULL;
}

/*
 * The instruction disassembly implemented here matches
 * the instruction encoding classifications in chapter C4
 * of the ARM Architecture Reference Manual (DDI0487B_a);
 * classification names and decode diagrams here should generally
 * match up with those in the manual.
 */

static bool trans_B(DisasContext *s)
{
    arg_i *a = &(s->arg.f_i);
    if (clearGprHigh) {
        for (int i = 0; i < 32; ++i) {
            if (arm_la_map[i] >= 0) {
                clear_gpr_high(i);
            }
        }
    }

    gen_goto_tb(s, 1, a->imm);
    // s->base->is_jmp = DISAS_NORETURN;

    return true;
}

static bool trans_BL(DisasContext *s)
{
    arg_i *a = &(s->arg.f_i);
    if (clearGprHigh) {
        for (int i = 0; i < 32; ++i) {
            if (arm_la_map[i] >= 0) {
                clear_gpr_high(i);
            }
        }
    }

    IR2_OPND x30 = alloc_gpr_dst(30);
    li_d(x30, s->base->pc_next); /* BL setting X30 to PC+4 */
    store_gpr_dst(30, x30);

    gen_goto_tb(s, 1, a->imm);
    // s->base->is_jmp = DISAS_NORETURN;

    return true;
}


static bool trans_CBZ(DisasContext *s)
{
    arg_cbz *a = &(s->arg.f_cbz);
    if (clearGprHigh) {
        for (int i = 0; i < 32; ++i) {
            if (arm_la_map[i] >= 0) {
                clear_gpr_high(i);
            }
        }
    }

    IR2_OPND reg_t = alloc_gpr_src(a->rt);
    IR2_OPND label = ir2_opnd_new_type(IR2_OPND_LABEL); 
    IR2_OPND temp;

    if(a->sf){
        if(a->nz){ // CBNZ
            la_bnez(reg_t, label);
        }else{ // CBZ
            la_beqz(reg_t, label);
        }
    }else{
        temp = ra_alloc_itemp();
        la_bstrpick_d(temp, reg_t, 31, 0);
        if(a->nz){ // CBNZ
            la_bnez(temp, label);
        }else{ // CBZ
            la_beqz(temp, label);
        }
        free_alloc_gpr(temp);
    }

    gen_goto_tb(s, 0, 4);
    la_label(label);
    gen_goto_tb(s, 1, a->imm);
    // s->base->is_jmp = DISAS_NORETURN;

    free_alloc_gpr(reg_t);
    return true;
}

static bool trans_TBZ(DisasContext *s)
{
    arg_tbz *a = &(s->arg.f_tbz);
    if (clearGprHigh) {
        for (int i = 0; i < 32; ++i) {
            if (arm_la_map[i] >= 0) {
                clear_gpr_high(i);
            }
        }
    }

    IR2_OPND reg_t = alloc_gpr_src(a->rt);
    IR2_OPND label = ir2_opnd_new_type(IR2_OPND_LABEL); 
    IR2_OPND temp = ra_alloc_itemp();

    la_bstrpick_d(temp, reg_t, a->bitpos, a->bitpos);
    if(a->nz){ // TBNZ
        la_bnez(temp, label);
    }else{ // TBZ
        la_beqz(temp, label);
    }
    gen_goto_tb(s, 0, 4);
    la_label(label);
    gen_goto_tb(s, 1, a->imm);
    // s->base->is_jmp = DISAS_NORETURN;

    free_alloc_gpr(reg_t);
    free_alloc_gpr(temp);
    return true;
}

static bool trans_B_cond(DisasContext *s)
{    
    arg_B_cond *a = &(s->arg.f_decode_insn3211);
    if (clearGprHigh) {
        for (int i = 0; i < 32; ++i) {
            if (arm_la_map[i] >= 0) {
                clear_gpr_high(i);
            }
        }
    }

    if (a->cond < 0x0e) {
        /* genuinely conditional branches */
        IR2_OPND label = ir2_opnd_new_type(IR2_OPND_LABEL);
        IR2_OPND reg_t = ra_alloc_itemp();
        la_setarmj(reg_t, a->cond);
        la_bnez(reg_t, label);
        gen_goto_tb(s, 0, 4);
        la_label(label);    
        gen_goto_tb(s, 1, a->imm);
        free_alloc_gpr(reg_t);
    } else {
        /* 0xe and 0xf are both "always" conditions */
        gen_goto_tb(s, 0, a->imm);
    }
    // s->base->is_jmp = DISAS_NORETURN;
    return true;
}

static bool trans_BR(DisasContext *s)
{
    arg_r *a = &(s->arg.f_r);
    if (clearGprHigh) {
        for (int i = 0; i < 32; ++i) {
            if (arm_la_map[i] >= 0) {
                clear_gpr_high(i);
            }
        }
    }
    gen_goto_tb_indirect(s, a->rn);
    // s->base->is_jmp = DISAS_NORETURN;

    return true;
}

static bool trans_BLR(DisasContext *s)
{
    arg_r *a = &(s->arg.f_r);
    if (clearGprHigh) {
        for (int i = 0; i < 32; ++i) {
            if (arm_la_map[i] >= 0) {
                clear_gpr_high(i);
            }
        }
    }

    IR2_OPND x30 = alloc_gpr_dst(30);
    li_d(x30, s->base->pc_next); /* BL setting X30 to PC+4 */
    store_gpr_dst(30, x30);

    gen_goto_tb_indirect(s, a->rn);
    // s->base->is_jmp = DISAS_NORETURN;

    return true;
}

static bool trans_RET(DisasContext *s)
{
    arg_r *a = &(s->arg.f_r);
    if (clearGprHigh) {
        for (int i = 0; i < 32; ++i) {
            if (arm_la_map[i] >= 0) {
                clear_gpr_high(i);
            }
        }
    }
    gen_goto_tb_indirect(s, a->rn);
    // s->base->is_jmp = DISAS_NORETURN;

    return true;
}

static bool trans_BRAZ(DisasContext *s)
{
    assert(0);
}

static bool trans_BLRAZ(DisasContext *s)
{
    assert(0);
}

static bool trans_RETA(DisasContext *s)
{
    assert(0);
}

static bool trans_BRA(DisasContext *s)
{
    assert(0);
}

static bool trans_BLRA(DisasContext *s)
{
    assert(0);
}

static bool trans_ERET(DisasContext *s)
{
    assert(0);
}

static bool trans_ERETA(DisasContext *s)
{
    assert(0);
}

static bool trans_NOP(DisasContext *s)
{
    return true;
}

static bool trans_YIELD(DisasContext *s)
{
    /*
     * When running in MTTCG we don't generate jumps to the yield and
     * WFE helpers as it won't affect the scheduling of other vCPUs.
     * If we wanted to more completely model WFE/SEV so we don't busy
     * spin unnecessarily we would need to do something more involved.
     */
    if (!(tb_cflags(s->base->tb) & CF_PARALLEL)) {
        // s->base->is_jmp = DISAS_YIELD;
    }
    return true;
}

static bool trans_WFI(DisasContext *s)
{
    s->base->is_jmp = DISAS_WFI;
    return true;
}

static bool trans_WFE(DisasContext *s)
{
    /*
     * When running in MTTCG we don't generate jumps to the yield and
     * WFE helpers as it won't affect the scheduling of other vCPUs.
     * If we wanted to more completely model WFE/SEV so we don't busy
     * spin unnecessarily we would need to do something more involved.
     */
    if (!(tb_cflags(s->base->tb) & CF_PARALLEL)) {
        s->base->is_jmp = DISAS_WFE;
    }
    return true;
}

static bool trans_XPACLRI(DisasContext *s)
{
    return true;
}

static bool trans_PACIA1716(DisasContext *s)
{
    return true;
}

static bool trans_PACIB1716(DisasContext *s)
{
    return true;
}

static bool trans_AUTIA1716(DisasContext *s)
{
    return true;
}

static bool trans_AUTIB1716(DisasContext *s)
{
    return true;
}

static bool trans_ESB(DisasContext *s)
{
    return true;
}

static bool trans_PACIAZ(DisasContext *s)
{
    return true;
}

static bool trans_PACIASP(DisasContext *s)
{
    return true;
}

static bool trans_PACIBZ(DisasContext *s)
{
    return true;
}

static bool trans_PACIBSP(DisasContext *s)
{
    return true;
}

static bool trans_AUTIAZ(DisasContext *s)
{
    return true;
}

static bool trans_AUTIASP(DisasContext *s)
{
    return true;
}

static bool trans_AUTIBZ(DisasContext *s)
{
    return true;
}

static bool trans_AUTIBSP(DisasContext *s)
{
    return true;
}

static bool trans_CLREX(DisasContext *s)
{
    assert(0);
}

static void lata_gen_mb(void){
    bool parallel = tcg_ctx->gen_tb->cflags & CF_PARALLEL;
    if (parallel) {
        la_dbar(0);
    }
}

static bool trans_DSB_DMB(DisasContext *s)
{
    // /* We handle DSB and DMB the same way */
    // TCGBar bar;

    // switch (a->types) {
    // case 1: /* MBReqTypes_Reads */
    //     bar = TCG_BAR_SC | TCG_MO_LD_LD | TCG_MO_LD_ST;
    //     break;
    // case 2: /* MBReqTypes_Writes */
    //     bar = TCG_BAR_SC | TCG_MO_ST_ST;
    //     break;
    // default: /* MBReqTypes_All */
    //     bar = TCG_BAR_SC | TCG_MO_ALL;
    //     break;
    // }
    lata_gen_mb();
    return true;
}

static bool trans_ISB(DisasContext *s)
{
    assert(0);
}

static bool trans_SB(DisasContext *s)
{
    if (!dc_isar_feature(aa64_sb, s)) {
        return false;
    }
    /*
     * TODO: There is no speculation barrier opcode for TCG;
     * MB and end the TB instead.
     */
    tcg_gen_mb(TCG_MO_ALL | TCG_BAR_SC);
    gen_goto_tb(s, 0, 4);
    return true;
}

static bool trans_CFINV(DisasContext *s)
{
    assert(0);
}

static bool trans_XAFLAG(DisasContext *s)
{
    assert(0);
}

static bool trans_AXFLAG(DisasContext *s)
{
    assert(0);
}

static bool trans_MSR_i_UAO(DisasContext *s)
{
    assert(0);
}

static bool trans_MSR_i_PAN(DisasContext *s)
{
    assert(0);
}

static bool trans_MSR_i_SPSEL(DisasContext *s)
{
    assert(0);
}

static bool trans_MSR_i_SBSS(DisasContext *s)
{
    assert(0);
}

static bool trans_MSR_i_DIT(DisasContext *s)
{
    assert(0);
}

static bool trans_MSR_i_TCO(DisasContext *s)
{
    assert(0);
}

static bool trans_MSR_i_DAIFSET(DisasContext *s)
{
    assert(0);
}

static bool trans_MSR_i_DAIFCLEAR(DisasContext *s)
{
    assert(0);
}

static bool trans_MSR_i_SVCR(DisasContext *s)
{
    assert(0);
}

static void lata_helper_get_sysReg(DisasContext *ctx, uint32_t key, int rt)
{
    IR2_OPND temp = ra_alloc_itemp();
    li_d(temp, ctx->base->pc_next);
    la_st_d(temp, env_ir2_opnd, env_offset_pc());
    lata_gen_call_helper_prologue(tcg_ctx);

    la_mov64(a0_ir2_opnd, env_ir2_opnd);
    li_d(a1_ir2_opnd, key);
    li_d(temp, (uint64_t)helper_lookup_cp_reg);
    la_jirl(ra_ir2_opnd, temp, 0);

    la_mov64(a1_ir2_opnd, a0_ir2_opnd);
    la_mov64(a0_ir2_opnd, env_ir2_opnd);
    li_d(temp, (uint64_t)helper_get_cp_reg64);
    la_jirl(ra_ir2_opnd, temp, 0);
    
    la_st_d(a0_ir2_opnd, env_ir2_opnd, env_offset_gpr(rt));

    lata_gen_call_helper_epilogue(tcg_ctx);
    free_alloc_gpr(temp);

    return ;
}

static void lata_helper_set_sysReg(DisasContext *ctx, uint32_t key, int rt)
{
    IR2_OPND temp = ra_alloc_itemp();
    li_d(temp, ctx->base->pc_next);
    la_st_d(temp, env_ir2_opnd, env_offset_pc());
    lata_gen_call_helper_prologue(tcg_ctx);

    la_mov64(a0_ir2_opnd, env_ir2_opnd);
    li_d(a1_ir2_opnd, key);
    li_d(temp, (uint64_t)helper_lookup_cp_reg);
    la_jirl(ra_ir2_opnd, temp, 0);

    la_mov64(a1_ir2_opnd, a0_ir2_opnd);
    la_mov64(a0_ir2_opnd, env_ir2_opnd);
    la_ld_d(a2_ir2_opnd, env_ir2_opnd, env_offset_gpr(rt));
    li_d(temp, (uint64_t)helper_set_cp_reg64);
    la_jirl(ra_ir2_opnd, temp, 0);

    lata_gen_call_helper_epilogue(tcg_ctx);
    free_alloc_gpr(temp);
    return ;
}


static void lata_helper_dc_zva(DisasContext *s, int rt)
{
    IR2_OPND temp = ra_alloc_itemp();
    li_d(temp, s->base->pc_next);
    la_st_d(temp, env_ir2_opnd, env_offset_pc());
    lata_gen_call_helper_prologue(tcg_ctx);
    
    if (s->mte_active[0]) {
        assert(0);
    } else {
        la_ld_d(temp, env_ir2_opnd, env_offset_gpr(rt));
        lata_clean_data_tbi(s, &temp, &temp, s->tbid);
    }

    la_mov64(a0_ir2_opnd, env_ir2_opnd);
    la_mov64(a1_ir2_opnd, temp);
    li_d(temp, (uint64_t)helper_dc_zva);
    la_jirl(ra_ir2_opnd, temp, 0);

    lata_gen_call_helper_epilogue(tcg_ctx);
    free_alloc_gpr(temp);
    return ;
}

/* MRS - move from system register
 * MSR (register) - move to system register
 * SYS
 * SYSL
 * These are all essentially the same insn in 'read' and 'write'
 * versions, with varying op0 fields.
 */
static void handle_sys(DisasContext *s, bool isread,
                       unsigned int op0, unsigned int op1, unsigned int op2,
                       unsigned int crn, unsigned int crm, unsigned int rt)
{
    uint32_t key = ENCODE_AA64_CP_REG(CP_REG_ARM64_SYSREG_CP,
                                      crn, crm, op0, op1, op2);
    const ARMCPRegInfo *ri = get_arm_cp_reginfo(s->cp_regs, key);
    bool need_exit_tb = false;

    if (!ri) {
        /* Unknown register; this might be a guest error or a QEMU
         * unimplemented feature.
         */
        assert(0);
    }

    /* Check access permissions */
    if (!cp_access_ok(s->current_el, ri, isread)) {
        assert(0);
    }

    if (ri->accessfn || (ri->fgt && s->fgt_active)) {
        /* Emit code to perform further access permissions checks at
         * runtime; this may result in an exception.
         */
        assert(0);
    } else if (ri->type & ARM_CP_RAISES_EXC) {
        /*
         * The readfn or writefn might raise an exception;
         * synchronize the CPU state in case it does.
         */
        assert(0);
    }

    IR2_OPND reg_t;
    if(clearGprHigh && !isread && arm_la_map[rt] >= 0 && rt != 31){
        clear_gpr_high(rt);
    }
    /* Handle special cases first */
    switch (ri->type & ARM_CP_SPECIAL_MASK) {
    case 0:
        break;
    case ARM_CP_NOP:
        return;
    case ARM_CP_NZCV:
        // tcg_rt = cpu_reg(s, rt);
        if (isread) {
            // gen_get_nzcv(tcg_rt);
            reg_t = alloc_gpr_dst(rt);
            la_armmfflag(reg_t, 0x39);
            
            store_gpr_dst(rt, reg_t);
        } else {
            reg_t = alloc_gpr_src(rt);
            la_armmtflag(reg_t, 0x39);
        }
        free_alloc_gpr(reg_t);
        return;
    case ARM_CP_CURRENTEL:
        assert(0);
        /* Reads as current EL value from pstate, which is
         * guaranteed to be constant by the tb flags.
         */
        return;
    case ARM_CP_DC_ZVA:
        lata_helper_dc_zva(s, rt);
        return;
    case ARM_CP_DC_GVA:
        {
            assert(0);
        }
        return;
    case ARM_CP_DC_GZVA:
        {
            assert(0);
        }
        return;
    default:
        g_assert_not_reached();
    }
    if ((ri->type & ARM_CP_FPU) && !fp_access_check_only(s)) {
        assert(0);
        return;
    } else if ((ri->type & ARM_CP_SVE) && !sve_access_check(s)) {
        assert(0);
        return;
    } else if ((ri->type & ARM_CP_SME) && !sme_access_check(s)) {
        assert(0);
        return;
    }

    if (ri->type & ARM_CP_IO) {
        assert(0);
        /* I/O operations must end the TB here (whether read or write) */
    }


    if (isread) {
        reg_t = alloc_gpr_dst(rt);
        if (ri->type & ARM_CP_CONST) {
            li_d(reg_t, ri->resetvalue);
            store_gpr_dst(rt, reg_t);
        } else if (ri->readfn) {    
            lata_helper_get_sysReg(s, key, rt);
        } else {
            if(ri->fieldoffset > 0x7ff) {
                IR2_OPND temp = ra_alloc_itemp();
                li_d(temp, ri->fieldoffset);  
                la_add_d(temp, env_ir2_opnd, temp);
                la_ld_d(reg_t, temp, 0);
                free_alloc_gpr(temp);
            } else {
                la_ld_d(reg_t, env_ir2_opnd, ri->fieldoffset);
            }
            store_gpr_dst(rt, reg_t);
        }
        free_alloc_gpr(reg_t);
    } else {
        reg_t = alloc_gpr_src(rt);
        if (ri->type & ARM_CP_CONST) {
            assert(0); /* 需要释放reg_t */
            return;
        } else if (ri->writefn) {
            lata_helper_set_sysReg(s, key, rt);
        } else {
            if(ri->fieldoffset > 0x7ff) {
                IR2_OPND temp = ra_alloc_itemp();
                li_d(temp, ri->fieldoffset);  
                la_add_d(temp, env_ir2_opnd, temp);
                la_st_d(reg_t, temp, 0);
                free_alloc_gpr(temp);
            } else {
                la_st_d(reg_t, env_ir2_opnd, ri->fieldoffset);
            }  
        }

        free_alloc_gpr(reg_t);
    }

    if(isread && arm_la_map[rt] >= 0 && rt != 31 && clearGprHigh){
        set_w_write_flag(rt, TRUE);
    }

    if (!isread && !(ri->type & ARM_CP_SUPPRESS_TB_END)) {
        /*
         * A write to any coprocessor register that ends a TB
         * must rebuild the hflags for the next TB.
         */
        // gen_rebuild_hflags(s);

        /*
         * We default to ending the TB on a coprocessor register write,
         * but allow this to be suppressed by the register definition
         * (usually only necessary to work around guest bugs).
         */
        need_exit_tb = true;
    }
    if (need_exit_tb) {
        s->base->is_jmp = DISAS_UPDATE_EXIT;
    }
}

static bool trans_SYS(DisasContext *s)
{
    arg_SYS *a = &(s->arg.f_decode_insn3218);
    handle_sys(s, a->l, a->op0, a->op1, a->op2, a->crn, a->crm, a->rt);
    return true;
}

static bool trans_SVC(DisasContext *s)
{
    arg_i *a = &(s->arg.f_i);
    if (clearGprHigh) {
        for (int i = 0; i < 32; ++i) {
            if (arm_la_map[i] >= 0) {
                clear_gpr_high(i);
            }
        }
    }

    /*
     * For SVC, HVC and SMC we advance the single-step state
     * machine before taking the exception. This is architecturally
     * mandated, to ensure that single-stepping a system call
     * instruction works properly.
     */
    IR2_OPND temp = ra_alloc_itemp();

    li_d(temp, s->base->pc_next);
    la_st_d(temp, env_ir2_opnd, env_offset_pc());
    lata_gen_call_helper_prologue(tcg_ctx);

    //CPUARMState *env, uint32_t excp, uint32_t syndrome
    la_mov64(a0_ir2_opnd, env_ir2_opnd);
    li_d(a1_ir2_opnd, EXCP_SWI);
    li_d(a2_ir2_opnd, syn_aa64_svc(a->imm));

    li_d(temp, (uint64_t)helper_exception_with_syndrome);
    la_jirl(ra_ir2_opnd, temp, 0);

    // s->base->is_jmp = DISAS_NORETURN;

    free_alloc_gpr(temp);
    return true;
}

static bool trans_HVC(DisasContext *s)
{
    assert(0);
}

static bool trans_SMC(DisasContext *s)
{
    assert(0);
}

static bool trans_BRK(DisasContext *s)
{
    return true;
}

static bool trans_HLT(DisasContext *s)
{
    /*
     * HLT. This has two purposes.
     * Architecturally, it is an external halting debug instruction.
     * Since QEMU doesn't implement external debug, we treat this as
     * it is required for halting debug disabled: it will UNDEF.
     * Secondly, "HLT 0xf000" is the A64 semihosting syscall instruction.
     */
    assert(0);
}
/*
 * Compute the ISS.SF bit for syndrome information if an exception
 * is taken on a load or store. This indicates whether the instruction
 * is accessing a 32-bit or 64-bit register. This logic is derived
 * from the ARMv8 specs for LDR (Shared decode for all encodings).
 */
static bool ldst_iss_sf(int size, bool sign, bool ext)
{

    if (sign) {
        /*
         * Signed loads are 64 bit results if we are not going to
         * do a zero-extend from 32 to 64 after the load.
         * (For a store, sign and ext are always false.)
         */
        return !ext;
    } else {
        /* Unsigned loads/stores work at the specified size */
        return size == MO_64;
    }
}

static void lata_load_exclusive(DisasContext *s, int rt, int rt2, int rn,
                               int size, bool is_pair)
{
    s->is_ldex = true;
    IR2_OPND reg_n = alloc_gpr_src_sp(rn);
    IR2_OPND reg_t = alloc_gpr_dst(rt);
    IR2_OPND reg_t2;

    if(clearGprHigh && arm_la_map[rn] >= 0){ /* all reg_n is 64-bit size*/
        clear_gpr_high(rn);
    }

    g_assert(size <= 3);
    if (is_pair) {
        g_assert(size >= 2);
        reg_t2 = alloc_gpr_dst(rt2);
        if (size == 2) {
            if (s->be_data == MO_LE) {
                la_ll_d(reg_t, reg_n, 0);
                la_bstrpick_d(reg_t2, reg_t, 63, 32);
                la_bstrpick_d(reg_t, reg_t, 31, 0);
            } else {
                la_ll_d(reg_t2, reg_n, 0);
                la_bstrpick_d(reg_t, reg_t2, 63, 32);
                la_bstrpick_d(reg_t2, reg_t2, 31, 0);
            }
        } else {
            assert(0);
            /* FIX ME: LL-SC粒度是针对缓存行的，连续的两个SC，前者一定会造成后者失效？ */
            if (s->be_data == MO_LE) {
                la_ll_d(reg_t, reg_n, 0);
                la_ll_d(reg_t2, reg_n, 8);
            } else {
                la_ll_d(reg_t2, reg_n, 0);
                la_ll_d(reg_t, reg_n, 8);
            }
        }
        store_fpr_dst(rt2, reg_t2);
        free_alloc_gpr(reg_t2);
    } else {
        // if(size == 2){
        //     la_ll_w(reg_t, reg_n, 0);
        // }else{
        //     la_ll_d(reg_t, reg_n, 0);
        // }
        /*  LL支持4字节和8字节，1字节和2字节使用普通的访存指令
            在单线程下不会出现问题。
            */
        IR2_OPND aligned_mem = ra_alloc_itemp();
        IR2_OPND offset = ra_alloc_itemp();
        switch(size){
            case 0://deal with the situation when memory is not aligned
                la_bstrpick_d(offset, reg_n, 2, 0);
                la_or(aligned_mem, reg_n, zero_ir2_opnd);
                la_bstrins_d(aligned_mem, zero_ir2_opnd, 2, 0);
                la_ll_d(reg_t, aligned_mem, 0);
                la_st_d(reg_t, env_ir2_opnd, env_offset(exclusive_memdata));
                la_slli_d(offset, offset, 3);
                la_srl_d(reg_t, reg_t, offset);
                la_bstrpick_d(reg_t, reg_t, 7, 0);
                break;
            case 1:
                la_bstrpick_d(offset, reg_n, 2, 0);
                la_or(aligned_mem, reg_n, zero_ir2_opnd);
                la_bstrins_d(aligned_mem, zero_ir2_opnd, 2, 0);
                la_ll_d(reg_t, aligned_mem, 0);
                la_st_d(reg_t, env_ir2_opnd, env_offset(exclusive_memdata));
                la_slli_d(offset, offset, 3);
                la_srl_d(reg_t, reg_t, offset);
                la_bstrpick_d(reg_t, reg_t, 15, 0);
                break;
            case 2:
                la_ll_w(reg_t, reg_n, 0);
                la_bstrpick_d(reg_t, reg_t, 31, 0);
                break;
            case 3:
                la_ll_d(reg_t, reg_n, 0);
                break;
        }
        free_alloc_gpr(aligned_mem);
        free_alloc_gpr(offset);
    }

    if(arm_la_map[rt] >= 0 && rt != 31 && clearGprHigh){
        set_w_write_flag(rt, TRUE);
    }

    store_gpr_dst(rt, reg_t);
    free_alloc_gpr(reg_t);
    free_alloc_gpr(reg_n);
}

static void lata_store_exclusive(DisasContext *s, int rs, int rt, int rt2,
                                int rn, int size, int is_pair)
{
    assert(!(rs == rn && rn != 31));

    IR2_OPND reg_s = alloc_gpr_dst(rs);
    IR2_OPND reg_n = alloc_gpr_src_sp(rn);
    IR2_OPND reg_t = alloc_gpr_src(rt);
    IR2_OPND reg_t2;

    if(clearGprHigh && arm_la_map[rn] >= 0){ /* all reg_n is 64-bit size*/
        clear_gpr_high(rn);
    }

    if(clearGprHigh && size == 3 && rt != 31 && arm_la_map[rt] >= 0){
        clear_gpr_high(rt);
    }

    if (is_pair) {
        assert(rs != rt && rs != rt2);
        reg_t2 = alloc_gpr_src(rt2);
        if (size == 2) {
            if (s->be_data == MO_LE) {
                la_bstrpick_d(reg_s, reg_t, 31, 0);
                la_bstrins_d(reg_s, reg_t2, 63, 32);
                la_sc_d(reg_s, reg_n, 0);
            } else {
                la_bstrpick_d(reg_s, reg_t2, 31, 0);
                la_bstrins_d(reg_s, reg_t, 63, 32);
                la_sc_d(reg_s, reg_n, 0);
            }
        } else {
            assert(0);
            /* FIX ME: LL-SC粒度是针对缓存行的，连续的两个SC，前者一定会造成后者失效？ */
            if (s->be_data == MO_LE) {
                la_or(reg_s, reg_t, zero_ir2_opnd);
                la_sc_d(reg_s, reg_n, 0);
                la_or(reg_s, reg_t2, zero_ir2_opnd);
                la_sc_d(reg_s, reg_n, 8);
            } else {
                la_or(reg_s, reg_t2, zero_ir2_opnd);
                la_sc_d(reg_s, reg_n, 0);
                la_or(reg_s, reg_t, zero_ir2_opnd);
                la_sc_d(reg_s, reg_n, 8);
            }
        }
        free_alloc_gpr(reg_t2);
    } else {
        assert(rs != rt);
        /*  LL/SC支持4字节和8字节，1字节和2字节使用普通的访存指令
            在单线程下不会出现问题。
        */
        IR2_OPND offset = ra_alloc_itemp();
        IR2_OPND aligned_mem = ra_alloc_itemp();
        IR2_OPND mem_data = ra_alloc_itemp();
        switch(size){
            case 0:
                /* get aligned memory*/
                la_bstrpick_d(offset, reg_n, 2, 0);
                la_slli_d(offset, offset, 3);
                la_or(aligned_mem, reg_n, zero_ir2_opnd);
                la_bstrins_d(aligned_mem, zero_ir2_opnd, 2, 0);

                /*merge data*/
                la_bstrpick_d(reg_s, reg_t, 7, 0);
                la_sll_d(reg_s, reg_s, offset);
                
                li_d(mem_data, 0xff);
                la_sll_d(offset, mem_data, offset);
                la_ld_d(mem_data, env_ir2_opnd, env_offset(exclusive_memdata));
                la_orn(mem_data, mem_data, offset);

                la_or(mem_data, reg_s, mem_data);              

                la_sc_d(reg_s, aligned_mem, 0);
                la_xori(reg_s, reg_s, 1);
                break;
            case 1:
                /* get aligned memory*/
                la_bstrpick_d(offset, reg_n, 2, 0);
                la_slli_d(offset, offset, 3);
                la_or(aligned_mem, reg_n, zero_ir2_opnd);
                la_bstrins_d(aligned_mem, zero_ir2_opnd, 2, 0);

                /*merge data*/
                la_bstrpick_d(reg_s, reg_t, 15, 0);
                la_sll_d(reg_s, reg_s, offset);
                
                li_d(mem_data, 0xffff);
                la_sll_d(offset, mem_data, offset);
                la_ld_d(mem_data, env_ir2_opnd, env_offset(exclusive_memdata));
                la_orn(mem_data, mem_data, offset);

                la_or(mem_data, reg_s, mem_data);              

                la_sc_d(reg_s, aligned_mem, 0);
                la_xori(reg_s, reg_s, 1);
                break;
            case 2:
                la_or(reg_s, reg_t, zero_ir2_opnd);
                la_sc_w(reg_s, reg_n, 0);
                la_xori(reg_s, reg_s, 1); /* 龙芯的SC返回值和arm相反 */
                break;
            case 3:
                la_or(reg_s, reg_t, zero_ir2_opnd);
                la_sc_d(reg_s, reg_n, 0);
                la_xori(reg_s, reg_s, 1); /* 龙芯的SC返回值和arm相反 */
                break;
        }
        free_alloc_gpr(aligned_mem);
        free_alloc_gpr(offset);
        free_alloc_gpr(mem_data);
    }

    /*  the value of reg_s must be 0 or 1, 
        it's no need to clear high bits.
    */
    if(arm_la_map[rs] >= 0 && rs != 31 && clearGprHigh){ 
        set_w_write_flag(rs, TRUE);
    }

    store_gpr_dst(rs, reg_s);
    free_alloc_gpr(reg_s);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(reg_t);
}

static bool trans_STXR(DisasContext *s)
{
    arg_stxr *a = &(s->arg.f_stxr);
    if (a->rn == 31) {
        gen_check_sp_alignment(s);
    }
    if (a->lasr) {
        lata_gen_mb();
    }
    lata_store_exclusive(s, a->rs, a->rt, a->rt2, a->rn, a->sz, false);
    return true;
}

static bool trans_LDXR(DisasContext *s)
{
    arg_stxr *a = &(s->arg.f_stxr);
    if (a->rn == 31) {
        gen_check_sp_alignment(s);
    }
    lata_load_exclusive(s, a->rt, a->rt2, a->rn, a->sz, false);
    if (a->lasr) {
        lata_gen_mb();
    }
    return true;
}

static bool trans_STLR(DisasContext *s)
{
    arg_stlr *a = &(s->arg.f_stlr);
    bool iss_sf = ldst_iss_sf(a->sz, false, false);
    int offset;
    /*
     * StoreLORelease is the same as Store-Release for QEMU, but
     * needs the feature-test.
     */
    if (!a->lasr && !dc_isar_feature(aa64_lor, s)) {
        return false;
    }
    /* Generate ISS for non-exclusive accesses including LASR.  */
    if (a->rn == 31) {
        gen_check_sp_alignment(s);
    }
    lata_gen_mb();
    IR2_OPND reg_n = alloc_gpr_src_sp(a->rn);
    IR2_OPND reg_t = alloc_gpr_src(a->rt);

    if(clearGprHigh && arm_la_map[a->rn] >= 0){ /* all reg_n is 64-bit size*/
        clear_gpr_high(a->rn);
    }

    if(clearGprHigh && a->sz == 3 && arm_la_map[a->rt] >= 0 && a->rt != 31){
        clear_gpr_high(a->rt);
    }

    if(a->lasr){ /* no offset */
        offset = 0;
        // if(iss_sf){
        //     la_st_d(reg_t, reg_n, offset);
        // }else{
        //     la_st_w(reg_t, reg_n, offset);
        // }
        switch (a->sz)
        {
        case 0:
            la_st_b(reg_t, reg_n, offset);
            break;
        case 1:
            la_st_h(reg_t, reg_n, offset);
            break;
        case 2:
            la_st_w(reg_t, reg_n, offset);
            break;
        case 3:
            la_st_d(reg_t, reg_n, offset);
            break;
        default:
            break;
        }
    }else{ /* pre-index */
        assert(0);
        offset = - (1 << a->sz);
        la_addi_d(reg_n, reg_n, offset);
        if(iss_sf){
            la_st_d(reg_t, reg_n, 0);
        }else{
            la_st_w(reg_t, reg_n, 0);
        }
        store_gpr_dst(a->rn, reg_n);
    }

    free_alloc_gpr(reg_n);
    free_alloc_gpr(reg_t);
    return true;
}

static bool trans_LDAR(DisasContext *s)
{
    arg_stlr *a = &(s->arg.f_stlr);
    IR2_OPND reg_t = alloc_gpr_dst(a->rt);
    IR2_OPND reg_n = alloc_gpr_src_sp(a->rn);

    if(clearGprHigh && arm_la_map[a->rn] >= 0){ /* all reg_n is 64-bit size*/
        clear_gpr_high(a->rn);
    }

    /* LoadLOAcquire is the same as Load-Acquire for QEMU.  */
    if (!a->lasr && !dc_isar_feature(aa64_lor, s)) {
        return false;
    }
    /* Generate ISS for non-exclusive accesses including LASR.  */
    if (a->rn == 31) {
        gen_check_sp_alignment(s);
    }

    switch(a->sz){
        case 0:
            la_ld_bu(reg_t, reg_n, 0);
            break;
        case 1:
            la_ld_hu(reg_t, reg_n, 0);
            break;
        case 2:
            la_ld_wu(reg_t, reg_n, 0);
            break;
        case 3:
            la_ld_d(reg_t, reg_n, 0);
            break;
    }

    lata_gen_mb();

    if(a->rt != 31 && arm_la_map[a->rt] >= 0 && clearGprHigh){
        set_w_write_flag(a->rt, TRUE);
    }

    store_gpr_dst(a->rt, reg_t);
    free_alloc_gpr(reg_t);
    free_alloc_gpr(reg_n);
    return true;
}

static bool trans_STXP(DisasContext *s)
{
    assert(0);
}

static bool trans_LDXP(DisasContext *s)
{
    assert(0);
}

static bool trans_CASP(DisasContext *s)
{
    assert(0);
}

static bool trans_CAS(DisasContext *s)
{
    assert(0);
}

static bool trans_LD_lit(DisasContext *s)
{
    arg_ldlit *a = &(s->arg.f_ldlit);
    IR2_OPND reg_d = alloc_gpr_dst(a->rt);
    MemOp memop = a->sz + a->sign * MO_SIGN;
    uint64_t addr = s->pc_curr + a->imm;
    switch(memop)
    {
        case MO_64:
            //ld_d只能处理si12
            la_lu12i_w(reg_d, addr >> 12);
            if(addr & 0x800){
                la_ori(reg_d, reg_d, addr & 0xfff);
                la_ld_d(reg_d,reg_d,0);
            } else {
                la_ld_d(reg_d,reg_d,addr & 0x7ff);
            }

            break;
        default:
            assert(0);
            break;
    }

    if (arm_la_map[a->rt] >= 0 && a->rt != 31 && clearGprHigh) {
        set_w_write_flag(a->rt, TRUE);
    }

    store_gpr_dst(a->rt,reg_d);
    free_alloc_gpr(reg_d);
    return true;
}

static bool trans_LD_lit_v(DisasContext *s)
{
    /* Load register (literal), vector version */
    assert(0);
    return true;
}

static bool trans_STP(DisasContext *s)
{
    
    /*3 classes: Post-index , Pre-index and Signed offset
    *     
    *               Post-index      Pre-index       Signed offset
    * postindex     true            false           false
    *   wback       true            true            false           
    */

    arg_ldstpair *a = &(s->arg.f_ldstpair);
    uint64_t offset = a->imm << a->sz;
    int dbytes = (8 << a->sz) / 8;
    if(a->w && (a->rt == a->rn || a->rt2 == a->rn) && a->rn !=31 )
    {
        assert(0);
        return true;
    }

    IR2_OPND reg_n = alloc_gpr_src_sp(a->rn);
    IR2_OPND reg_t = alloc_gpr_src(a->rt);
    IR2_OPND reg_t2 = alloc_gpr_src(a->rt2);
    IR2_OPND temp = ra_alloc_itemp();

    if(clearGprHigh && arm_la_map[a->rn] >= 0){ /* all reg_n is 64-bit size*/
        clear_gpr_high(a->rn);
    }

    if(clearGprHigh && a->sz == 3 && arm_la_map[a->rt] >= 0 && a->rt != 31){
        clear_gpr_high(a->rt);
    }

    if(clearGprHigh && a->sz == 3 && arm_la_map[a->rt2] >= 0 && a->rt2 != 31){
        clear_gpr_high(a->rt2);
    }

    if(!a->w && offset){ // wback = false && offset!=0
        la_addi_d(temp, reg_n, offset);
        switch(dbytes){
        case 4:
            la_st_w(reg_t, temp, 0);
            la_st_w(reg_t2, temp, dbytes);
            break;
        case 8:
            la_st_d(reg_t, temp, 0);
            la_st_d(reg_t2, temp, dbytes);
            break;
        default:
            assert(0);
        }
    }else{//Post-index
        if(!a->p && offset){
            la_addi_d(reg_n, reg_n, offset);
        }

        switch(dbytes){
        case 4:
            la_st_w(reg_t, reg_n, 0);
            la_st_w(reg_t2, reg_n, dbytes);
            break;
        case 8:
            la_st_d(reg_t, reg_n, 0);
            la_st_d(reg_t2, reg_n, dbytes);
            break;
        default:
            assert(0);
        }

        if(a->p && offset){
            la_addi_d(reg_n,reg_n,offset);
        }       
        if(a->w)
            store_gpr_dst(a->rn, reg_n);
    }

    free_alloc_gpr(reg_n);
    free_alloc_gpr(reg_t);
    free_alloc_gpr(reg_t2);
    free_alloc_gpr(temp);
    return true;
}

static bool trans_LDP(DisasContext *s)
{
    
    /*3 classes: Post-index , Pre-index and Signed offset
    *     
    *               Post-index      Pre-index       Signed offset
    * postindex     true            false           false
    *   wback       true            true            false           
    */

    arg_ldstpair *a = &(s->arg.f_ldstpair);
    uint64_t offset = a->imm << a->sz;
    int dbytes = (8 << a->sz) / 8; // dbytes = datasize / 8;
    if(a->w && (a->rt == a->rn || a->rt2 == a->rn) && a->rn !=31 )
    {
        assert(0);
        return true;
    }

    IR2_OPND reg_n = alloc_gpr_src_sp(a->rn);
    IR2_OPND reg_t = alloc_gpr_dst(a->rt);
    IR2_OPND reg_t2 = alloc_gpr_dst(a->rt2);
    IR2_OPND temp = ra_alloc_itemp();

    if(clearGprHigh && arm_la_map[a->rn] >= 0){ /* all reg_n is 64-bit size*/
        clear_gpr_high(a->rn);
    }

    if(!a->w && offset){ // wback = false && offset!=0
        la_addi_d(temp, reg_n, offset);
        switch(dbytes){
        case 4:
            if(a->sign){
                la_ld_w(reg_t, temp, 0);
                la_ld_w(reg_t2, temp, dbytes);
            }else{
                la_ld_wu(reg_t, temp, 0);
                la_ld_wu(reg_t2, temp, dbytes);
            }
            break;
        case 8:
            la_ld_d(reg_t, temp, 0);
            la_ld_d(reg_t2, temp, dbytes);
            break;
        default:
            assert(0);
        }
    }else{//Post-index
        if(!a->p && offset){
            la_addi_d(reg_n, reg_n, offset);
        }

        switch(dbytes){
        case 4:
            if(a->sign){
                if(a->rn != a->rt && a->rn != a->rt2){ 
                    la_ld_w(reg_t, reg_n, 0);
                    la_ld_w(reg_t2, reg_n, dbytes);
                }else{//read vs write
                    la_or(temp,reg_n, zero_ir2_opnd);
                    la_ld_w(reg_t, temp, 0);
                    la_ld_w(reg_t2, temp, dbytes);
                }
            }else{
                if(a->rn != a->rt && a->rn != a->rt2){
                    la_ld_wu(reg_t, reg_n, 0);
                    la_ld_wu(reg_t2, reg_n, dbytes);
                }else{
                    la_or(temp,reg_n, zero_ir2_opnd);
                    la_ld_wu(reg_t, temp, 0);
                    la_ld_wu(reg_t2, temp, dbytes);
                }
            }
            break;
        case 8:
            if(a->rn != a->rt && a->rn != a->rt2){
                la_ld_d(reg_t, reg_n, 0);
                la_ld_d(reg_t2, reg_n, dbytes);
            }else{
                la_or(temp,reg_n, zero_ir2_opnd);
                la_ld_d(reg_t, temp, 0);
                la_ld_d(reg_t2, temp, dbytes);
            }
            break;
        default:
            assert(0);
        }

        if(a->p && offset){
            la_addi_d(reg_n,reg_n,offset);
        }       
        if(a->w)
            store_gpr_dst(a->rn, reg_n);
    }

    if (a->rt != 31 && arm_la_map[a->rt] >= 0 && clearGprHigh) {
        set_w_write_flag(a->rt, TRUE);
    }

    if (a->rt != 31 && arm_la_map[a->rt2] >= 0 && clearGprHigh) {
        set_w_write_flag(a->rt2, TRUE);
    }

    store_gpr_dst(a->rt, reg_t);
    store_gpr_dst(a->rt2, reg_t2);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(reg_t);
    free_alloc_gpr(reg_t2);
    free_alloc_gpr(temp);
    return true;
}

static bool trans_STP_v(DisasContext *s)
{
    /*3 classes: Post-index , Pre-index and Signed offset
    *     
    *               Post-index      Pre-index       Signed offset
    * postindex     true            false           false
    *   wback       true            true            false           
    */

    arg_ldstpair *a = &(s->arg.f_ldstpair);
    uint64_t offset = a->imm << a->sz;
    int dbytes = (8 << a->sz) / 8; // dbytes = datasize / 8;

    /* TODO: CheckFPAdvSIMDEnabled64(s) */

    IR2_OPND reg_n = alloc_gpr_src_sp(a->rn);
    IR2_OPND vreg_t = alloc_fpr_src(a->rt);
    IR2_OPND vreg_t2 = alloc_fpr_src(a->rt2);
    IR2_OPND temp = ra_alloc_itemp();

    if(clearGprHigh && arm_la_map[a->rn] >= 0){ /* all reg_n is 64-bit size*/
        clear_gpr_high(a->rn);
    }

    if(!a->w && offset){ // wback = false && offset!=0
        la_addi_d(temp, reg_n, offset);
        switch(dbytes){
        case 4:
            la_fst_s(vreg_t, temp, 0);
            la_fst_s(vreg_t2, temp, dbytes);
            break;
        case 8:
            la_fst_d(vreg_t, temp, 0);
            la_fst_d(vreg_t2, temp, dbytes);
            break;
        case 16:
            la_vst(vreg_t, temp, 0);
            la_vst(vreg_t2, temp, dbytes);
            break;
        default:
            assert(0);
        }
    }else{//Post-index
        if(!a->p && offset){
            la_addi_d(reg_n, reg_n, offset);
        }

        switch(dbytes){
        case 4:
            la_fst_s(vreg_t, reg_n, 0);
            la_fst_s(vreg_t2, reg_n, dbytes);
            break;
        case 8:
            la_fst_d(vreg_t, reg_n, 0);
            la_fst_d(vreg_t2, reg_n, dbytes);
            break;
        case 16:
            la_vst(vreg_t, reg_n, 0);
            la_vst(vreg_t2, reg_n, dbytes);
            break;
        default:
            assert(0);
        }

        if(a->p && offset){
            la_addi_d(reg_n,reg_n,offset);
        }       
        if(a->w)
            store_gpr_dst(a->rn, reg_n);
    }

    free_alloc_gpr(reg_n);
    free_alloc_fpr(vreg_t);
    free_alloc_fpr(vreg_t2);
    free_alloc_gpr(temp);
    return true;
}

static bool trans_LDP_v(DisasContext *s)
{
    /*3 classes: Post-index , Pre-index and Signed offset
    *     
    *               Post-index      Pre-index       Signed offset
    * postindex     true            false           false
    *   wback       true            true            false           
    */

    arg_ldstpair *a = &(s->arg.f_ldstpair);
    uint64_t offset = a->imm << a->sz;
    int dbytes = (8 << a->sz) / 8; // dbytes = datasize / 8;
    /* TODO: CheckFPAdvSIMDEnabled64(s) */

    IR2_OPND reg_n = alloc_gpr_src_sp(a->rn);
    IR2_OPND vreg_t = alloc_fpr_dst(a->rt);
    IR2_OPND vreg_t2 = alloc_fpr_dst(a->rt2);
    IR2_OPND temp = ra_alloc_itemp();

    if(clearGprHigh && arm_la_map[a->rn] >= 0){ /* all reg_n is 64-bit size*/
        clear_gpr_high(a->rn);
    }

    if(!a->w && offset){ // wback = false && offset!=0
        la_addi_d(temp, reg_n, offset);
        switch(dbytes){
        case 4:
            la_vsub_d(vreg_t,vreg_t,vreg_t);
            la_vsub_d(vreg_t2,vreg_t2,vreg_t2);
            la_fld_s(vreg_t, temp, 0);
            la_fld_s(vreg_t2, temp, dbytes);
            break;
        case 8:
            la_vsub_d(vreg_t,vreg_t,vreg_t);
            la_vsub_d(vreg_t2,vreg_t2,vreg_t2);
            la_fld_d(vreg_t, temp, 0);
            la_fld_d(vreg_t2, temp, dbytes);
            break;
        case 16:
            la_vld(vreg_t, temp, 0);
            la_vld(vreg_t2, temp, dbytes);
            break;
        default:
            assert(0);
        }
    }else{//Post-index
        if(!a->p && offset){
            la_addi_d(reg_n, reg_n, offset);
        }

        switch(dbytes){
        case 4:
            la_vsub_d(vreg_t,vreg_t,vreg_t);
            la_vsub_d(vreg_t2,vreg_t2,vreg_t2);
            if(a->rn != a->rt && a->rn != a->rt2){ 
                la_fld_s(vreg_t, reg_n, 0);
                la_fld_s(vreg_t2, reg_n, dbytes);
            }else{//read vs write
                la_or(temp,reg_n, zero_ir2_opnd);
                la_fld_s(vreg_t, temp, 0);
                la_fld_s(vreg_t2, temp, dbytes);
            }
            break;
        case 8:
            la_vsub_d(vreg_t,vreg_t,vreg_t);
            la_vsub_d(vreg_t2,vreg_t2,vreg_t2);
            if(a->rn != a->rt && a->rn != a->rt2){ 
                la_fld_d(vreg_t, reg_n, 0);
                la_fld_d(vreg_t2, reg_n, dbytes);
            }else{//read vs write
                la_or(temp,reg_n, zero_ir2_opnd);
                la_fld_d(vreg_t, temp, 0);
                la_fld_d(vreg_t2, temp, dbytes);
            }
            break;
        case 16:
            if(a->rn != a->rt && a->rn != a->rt2){ 
                la_vld(vreg_t, reg_n, 0);
                la_vld(vreg_t2, reg_n, dbytes);
            }else{//read vs write
                la_or(temp,reg_n, zero_ir2_opnd);
                la_vld(vreg_t, temp, 0);
                la_vld(vreg_t2, temp, dbytes);
            }
            break;
        default:
            assert(0);
        }

        if(a->p && offset){
            la_addi_d(reg_n,reg_n,offset);
        }       
        if(a->w)
            store_gpr_dst(a->rn, reg_n);
    }

    store_fpr_dst(a->rt, vreg_t);
    store_fpr_dst(a->rt2, vreg_t2);
    free_alloc_gpr(reg_n);
    free_alloc_fpr(vreg_t);
    free_alloc_fpr(vreg_t2);
    free_alloc_gpr(temp);
    return true;
}

static bool trans_STGP(DisasContext *s)
{
    assert(0);
}

static void assist_trans_STR_i(IR2_OPND* reg_t, IR2_OPND* reg_n, uint64_t offset, uint64_t sz){
    switch(sz){
        case 0: 
            la_st_b(*reg_t, *reg_n, offset);
            break;
        case 1: 
            la_st_h(*reg_t, *reg_n, offset);
            break;
        case 2:
            la_st_w(*reg_t, *reg_n, offset);
            break;
        case 3:
            la_st_d(*reg_t, *reg_n, offset);
            break;
        default:
            assert(0);
    }
}

static bool trans_STR_i(DisasContext *s)
{
    /*3 classes: Post-index , Pre-index and Unsigned offset
    *     
    *               Post-index      Pre-index       Unsigned offset
    * postindex     true            false           false
    *   wback       true            true            false           
    */

    arg_ldst_imm *a = &(s->arg.f_ldst_imm);
    int64_t offset = a->imm;
    if(a->w && a->rt == a->rn && a->rn !=31 )
    {
        assert(0);
        return true;
    }

    IR2_OPND reg_n = alloc_gpr_src_sp(a->rn);
    IR2_OPND reg_t = alloc_gpr_src(a->rt);

    /*  # STR (immediate)
        ## Post-index                       ## Pre-index                         ## Unsigned offset
        32-bit (size == 10)                 32-bit (size == 10)                  32-bit (size == 10)
        STR <Wt>, [<Xn|SP>], #<simm>        STR <Wt>, [<Xn|SP>, #<simm>]!        STR <Wt>, [<Xn|SP>{, #<pimm>}]
        64-bit (size == 11)                 64-bit (size == 11)                  64-bit (size == 11)
        STR <Xt>, [<Xn|SP>], #<simm>        STR <Xt>, [<Xn|SP>, #<simm>]!        STR <Xt>, [<Xn|SP>{, #<pimm>}]

        # STRB (immediate)
        ## Post-index                       ## Pre-index                         ## Unsigned offset
        STRB <Wt>, [<Xn|SP>], #<simm>       STRB <Wt>, [<Xn|SP>, #<simm>]!       STRB <Wt>, [<Xn|SP>{, #<pimm>}]

        # STRH (immediate)
        ## Post-index                       ## Pre-index                         ## Unsigned offset
        STRH <Wt>, [<Xn|SP>], #<simm>       STRH <Wt>, [<Xn|SP>, #<simm>]!       STRH <Wt>, [<Xn|SP>{, #<pimm>}]
    */

    if(clearGprHigh && arm_la_map[a->rn] >= 0){ /* all reg_n is 64-bit size*/
        clear_gpr_high(a->rn);
    }

    if(clearGprHigh && a->sz == 3 && arm_la_map[a->rt] >= 0 && a->rt != 31){
        clear_gpr_high(a->rt);
    }
    if(!a->w && offset){ // postindex = false
        /* Unsigned offset立即数是uimm12位，
        la_addi_d立即数是imm12，需要将立即数加载到寄存器
        而Post-index，Pre-index立即数是imm9满足la_addi_d的立即数规范
        */
        if(a->imm > 0x7ff){
            IR2_OPND temp = ra_alloc_itemp();
            li_d(temp, a->imm);  
            la_add_d(temp, reg_n, temp);
            assist_trans_STR_i(&reg_t, &temp, 0, a->sz);
            free_alloc_gpr(temp);
        }
        else{
            assist_trans_STR_i(&reg_t, &reg_n, offset, a->sz);
        }
    }
    else{
        if(!a->p && offset)
            la_addi_d(reg_n, reg_n, offset);
        assist_trans_STR_i(&reg_t, &reg_n, 0, a->sz);
        if(a->p && offset)
            la_addi_d(reg_n, reg_n, offset);
        if(a->w)
            store_gpr_dst(a->rn, reg_n);
    }

    free_alloc_gpr(reg_n);
    free_alloc_gpr(reg_t);
    return true;
}

static void assist_trans_LDR_i(IR2_OPND* reg_t, IR2_OPND* reg_n, uint64_t offset, uint64_t sz, uint64_t sign, bool iss_sf){
        switch(sz){
        case 0:
            if(sign){ // LDRSB_i
                la_ld_b(*reg_t, *reg_n, offset);
            }else{ // LDRB_i
                la_ld_bu(*reg_t, *reg_n, offset);
            }
            break;
        case 1:
            if(sign){ // LDRSH_i
                la_ld_h(*reg_t, *reg_n, offset);
            }else{ // LDRH_i
                la_ld_hu(*reg_t, *reg_n, offset);
            }
            break;
        case 2:
            if(sign){ // LDRSW_i
                la_ld_w(*reg_t, *reg_n, offset);
            }else{ // LDR_i
                la_ld_wu(*reg_t, *reg_n, offset);
            }
            break;
        case 3: // LDR_i
            la_ld_d(*reg_t, *reg_n, offset);
            break;
        default:
            assert(0);
        }
}

static bool trans_LDR_i(DisasContext *s)
{
    /*3 classes: Post-index , Pre-index and Unsigned offset
    *     
    *               Post-index      Pre-index       Unsigned offset
    * postindex     true            false           false
    *   wback       true            true            false           
    */

    arg_ldst_imm *a = &(s->arg.f_ldst_imm);
    int64_t offset = a->imm;
    /* iss_sf ? regsize=64 : regsize=32 */ 
    bool iss_sf = ldst_iss_sf(a->sz, a->sign, a->ext); 
    if(a->w && a->rt == a->rn && a->rn !=31 )
    {
        assert(0);
        return true;
    }

    IR2_OPND reg_n = alloc_gpr_src_sp(a->rn);
    IR2_OPND reg_t = alloc_gpr_dst(a->rt);
    
    /*  # LDR (immediate)
        ## Post-index                       ## Pre-index                         ## Unsigned offset
        32-bit (size == 10)                 32-bit (size == 10)                  32-bit (size == 10)
        LDR <Wt>, [<Xn|SP>], #<simm>        LDR <Wt>, [<Xn|SP>, #<simm>]!        LDR <Wt>, [<Xn|SP>{, #<pimm>}]
        64-bit (size == 11)                 64-bit (size == 11)                  64-bit (size == 11)
        LDR <Xt>, [<Xn|SP>], #<simm>        LDR <Xt>, [<Xn|SP>, #<simm>]!        LDR <Xt>, [<Xn|SP>{, #<pimm>}]

        # LDRB (immediate)
        ## Post-index                       ## Pre-index                         ## Unsigned offset
        LDRB <Wt>, [<Xn|SP>], #<simm>       LDRB <Wt>, [<Xn|SP>, #<simm>]!       LDRB <Wt>, [<Xn|SP>{, #<pimm>}]

        # LDRH (immediate)
        ## Post-index                       ## Pre-index                         ## Unsigned offset
        LDRH <Wt>, [<Xn|SP>], #<simm>       LDRH <Wt>, [<Xn|SP>, #<simm>]!       LDRH <Wt>, [<Xn|SP>{, #<pimm>}]
    */

    if(clearGprHigh && arm_la_map[a->rn] >= 0){ /* all reg_n is 64-bit size*/
        clear_gpr_high(a->rn);
    }

    if (a->rn == 31) {
        gen_check_sp_alignment(s);
    }

    if(!a->w && offset){ // wback = false && offset!=0
        /* Unsigned offset立即数是uimm12位，
        la_addi_d立即数是imm12，需要将立即数加载到寄存器
        而Post-index，Pre-index立即数是imm9满足la_addi_d的立即数规范
        */
        if(offset > 0x7ff){
            IR2_OPND temp = ra_alloc_itemp();
            li_d(temp, offset);  
            la_add_d(temp, reg_n, temp);
            assist_trans_LDR_i(&reg_t, &temp,0, a->sz, a->sign,iss_sf);
            free_alloc_gpr(temp);
        }
        else{
            assist_trans_LDR_i(&reg_t, &reg_n, offset, a->sz, a->sign,iss_sf);
        }
    }else{//Post-index
        if(!a->p && offset){
            la_addi_d(reg_n,reg_n,offset); 
        }

        assist_trans_LDR_i(&reg_t, &reg_n, 0, a->sz, a->sign,iss_sf);

        if(a->p && offset){
            la_addi_d(reg_n,reg_n,offset);         
        }
        if(a->w)
            store_gpr_dst(a->rn, reg_n);               
    }

    if (a->rt != 31 && arm_la_map[a->rt] >= 0 && clearGprHigh) {
        set_w_write_flag(a->rt, !(a->sign && !iss_sf));
    } else {
        if(a->sign && !iss_sf){
            la_bstrpick_d(reg_t, reg_t, 31, 0);
        }
    }

    store_gpr_dst(a->rt, reg_t);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(reg_t);
    return true;
}

static void assist_trans_STR_v_i(IR2_OPND* vreg_t, IR2_OPND* reg_n, uint64_t offset, uint64_t sz){
    IR2_OPND temp = ra_alloc_itemp();
    switch(sz){
    case 0: 
        if(offset){
           la_addi_d(temp, *reg_n, offset);
           la_vstelm_b(*vreg_t, temp, 0, 0); 
        }else{
           la_vstelm_b(*vreg_t, *reg_n, 0, 0); 
        }
         break;
    case 1: 
        if(offset){
           la_addi_d(temp, *reg_n, offset);
           la_vstelm_h(*vreg_t, temp, 0, 0); 
        }else{
           la_vstelm_h(*vreg_t, *reg_n, 0, 0);
        }
        break;
    case 2:
        la_fst_s(*vreg_t, *reg_n, offset);
        break;
    case 3:
        la_fst_d(*vreg_t, *reg_n, offset);
        break;
     case 4:
        la_vst(*vreg_t, *reg_n, offset);
        break;
    default:
        assert(0);
    }
    free_alloc_gpr(temp);
}

static bool trans_STR_v_i(DisasContext *s)
{
    /*3 classes: Post-index , Pre-index and Unsigned offset
    *     
    *               Post-index      Pre-index       Unsigned offset
    * postindex     true            false           false
    *   wback       true            true            false           
    */

    arg_ldst_imm *a = &(s->arg.f_ldst_imm);
    int64_t offset = a->imm;

    IR2_OPND reg_n = alloc_gpr_src_sp(a->rn);
    IR2_OPND vreg_t = alloc_fpr_src(a->rt);

    if(clearGprHigh && arm_la_map[a->rn] >= 0){ /* all reg_n is 64-bit size*/
        clear_gpr_high(a->rn);
    }

    if(!a->w && offset){ // postindex = false
        /* Unsigned offset立即数是uimm12位，
        la_addi_d立即数是imm12，需要将立即数加载到寄存器
        而Post-index，Pre-index立即数是imm9满足la_addi_d的立即数规范
        */
        if(a->imm > 0x7ff){
            IR2_OPND temp = ra_alloc_itemp();
            li_d(temp, a->imm);  
            la_add_d(temp, reg_n, temp);
            free_alloc_gpr(temp);
            assist_trans_STR_v_i(&vreg_t,&temp,0,a->sz);
        }
        else{
            // la_addi_d(temp, reg_n, a->imm);
            assist_trans_STR_v_i(&vreg_t,&reg_n,offset,a->sz);
        }
    }
    else{
        if(!a->p && offset)
            la_addi_d(reg_n, reg_n, offset);
            
        assist_trans_STR_v_i(&vreg_t,&reg_n,0,a->sz);

        if(a->p && offset)
            la_addi_d(reg_n, reg_n, offset);
        if(a->w)
            store_gpr_dst(a->rn, reg_n);
    }

    free_alloc_gpr(reg_n);
    free_alloc_fpr(vreg_t);
    return true;
}

static void assist_trans_LDR_v_i(IR2_OPND* vreg_t, IR2_OPND* reg_n, uint64_t offset, uint64_t sz){
    IR2_OPND temp = ra_alloc_itemp();
    switch(sz){
        case 0:
            la_vxor_v(*vreg_t, *vreg_t, *vreg_t);
            la_ld_bu(temp, *reg_n, offset);
            la_vinsgr2vr_b(*vreg_t, temp, 0);
            break;
        case 1:
            la_vxor_v(*vreg_t, *vreg_t, *vreg_t);
            la_ld_hu(temp, *reg_n, offset);
            la_vinsgr2vr_h(*vreg_t, temp, 0);
            break;
        case 2:
            la_fld_s(*vreg_t, *reg_n, offset);
            la_vand_v(*vreg_t,*vreg_t, fsmask_ir2_opnd);
            break;
        case 3:
            la_fld_d(*vreg_t, *reg_n, offset);
            /* 高64位清零 */
            la_vinsgr2vr_d(*vreg_t, zero_ir2_opnd, 1);
            break;
        case 4:
            la_vld(*vreg_t, *reg_n, offset);
            break;
        default:
            assert(0);
        }
    free_alloc_gpr(temp);
}

static bool trans_LDR_v_i(DisasContext *s)
{
    /*3 classes: Post-index , Pre-index and Unsigned offset
    *     
    *               Post-index      Pre-index       Unsigned offset
    * postindex     true            false           false
    *   wback       true            true            false           
    */

    arg_ldst_imm *a = &(s->arg.f_ldst_imm);
    int64_t offset = a->imm;

    IR2_OPND reg_n = alloc_gpr_src_sp(a->rn);
    IR2_OPND vreg_t = alloc_fpr_dst(a->rt);

    if(clearGprHigh && arm_la_map[a->rn] >= 0){ /* all reg_n is 64-bit size*/
        clear_gpr_high(a->rn);
    }

    if(!a->w && offset){ // wback = false && offset!=0
        /* Unsigned offset立即数是uimm12位，
        la_addi_d立即数是imm12，需要将立即数加载到寄存器
        而Post-index，Pre-index立即数是imm9满足la_addi_d的立即数规范
        */
        if(offset > 0x7ff){
            IR2_OPND temp = ra_alloc_itemp();
            li_d(temp, offset);  
            la_add_d(temp, reg_n, temp);
            assist_trans_LDR_v_i(&vreg_t, &temp, 0, a->sz);
            free_alloc_gpr(temp);
        }
        else{
           assist_trans_LDR_v_i(&vreg_t, &reg_n, offset, a->sz);
        }
    }else{//Post-index
        if(!a->p && offset){
            la_addi_d(reg_n,reg_n,offset); 
        }

        assist_trans_LDR_v_i(&vreg_t, &reg_n, 0, a->sz);

        if(a->p && offset){
            la_addi_d(reg_n,reg_n,offset);         
        }
        if(a->w)
            store_gpr_dst(a->rn, reg_n);               
    }

    store_fpr_dst(a->rt, vreg_t);
    free_alloc_gpr(reg_n);
    free_alloc_fpr(vreg_t);
    return true;
}

static bool trans_LDR(DisasContext *s)
{
    arg_ldst *a = &(s->arg.f_ldst);
    IR2_OPND reg_t = alloc_gpr_dst(a->rt);
    IR2_OPND reg_n = alloc_gpr_src_sp(a->rn);
    IR2_OPND reg_m = alloc_gpr_src(a->rm);
    IR2_OPND temp = ra_alloc_itemp();

    /*  LDR (register)
        32-bit (size == 10)
        LDR <Wt>, [<Xn|SP>, (<Wm>|<Xm>){, <extend> {<amount>}}]
        64-bit (size == 11)
        LDR <Xt>, [<Xn|SP>, (<Wm>|<Xm>){, <extend> {<amount>}}]

        LDRB (register)
        Extended register (option != 011)
        LDRB <Wt>, [<Xn|SP>, (<Wm>|<Xm>), <extend> {<amount>}]
        Shifted register (option == 011)
        LDRB <Wt>, [<Xn|SP>, <Xm>{, LSL <amount>}]

        LDRH (register)
        LDRH <Wt>, [<Xn|SP>, (<Wm>|<Xm>){, <extend> {<amount>}}]
    */
    if(clearGprHigh && arm_la_map[a->rn] >= 0){ /* all reg_n is 64-bit size*/
        clear_gpr_high(a->rn);
    }

    if(clearGprHigh && a->sz == 3 && arm_la_map[a->rm] >= 0 && a->rm != 31){
        clear_gpr_high(a->rm);
    }

    /* iss_sf ? regsize=64 : regsize=32 */ 
    bool iss_sf = ldst_iss_sf(a->sz, a->sign, a->ext); 
    if (extract32(a->opt, 1, 1) == 0) {
        return false;
    }

    int extsize = extract32(a->opt, 0, 2);
    bool is_signed = extract32(a->opt, 2, 1);
    unsigned int shift = a->s ? a->sz : 0;

    if (is_signed) {
        switch (extsize) {
        case 0:
            la_ext_w_b(temp, reg_m);
            break;
        case 1:
            la_ext_w_h(temp, reg_m);
            break;
        case 2:
            la_bstrpick_w(temp,reg_m,31,0);
            break;
        case 3:
            //to reduce instructions inflation,handle separately
            break;
        }
    } else {
        switch (extsize) {
        case 0:
            la_bstrpick_d(temp,reg_m,7,0);
            break;
        case 1:
            la_bstrpick_d(temp,reg_m,15,0);
            break;
        case 2:
            la_bstrpick_d(temp,reg_m,31,0);
            break;
        case 3:
            //to reduce instructions inflation,handle separately
            break;
        }
    }
    
    /* LDR(register), */
    if(extsize == 3 && !shift){
        switch (a->sz)
        {
        case 0:
            if(a->sign){ // LDRSB
                la_ldx_b(reg_t, reg_n, reg_m);
            }else{ // LDRB
                la_ldx_bu(reg_t, reg_n, reg_m);
            }
            break;
        case 1:
            if(a->sign){ // LDRSH
                la_ldx_h(reg_t, reg_n, reg_m);
            }else{ // LDRH
                la_ldx_hu(reg_t, reg_n, reg_m);
            }
            break;
        case 2:
            if(a->sign){ // LDRSW
                la_ldx_w(reg_t, reg_n, reg_m);
            }else{ // LDR
                la_ldx_wu(reg_t, reg_n, reg_m);
            }
            break;
        case 3: // LDR
            la_ldx_d(reg_t, reg_n, reg_m);
            break;
        default:
            break;
        }
    }else{
        if (shift) {
            if(extsize == 3){
                la_slli_d(temp,reg_m,shift);
            }else{
                la_slli_d(temp,temp,shift);
            }        
        }

        switch (a->sz)
        {
        case 0:
            if(a->sign){ // LDRSB
                la_ldx_b(reg_t, reg_n, temp);
            }else{ // LDRB
                la_ldx_bu(reg_t, reg_n, temp);
            }
            break;
        case 1:
            if(a->sign){ // LDRSH
                la_ldx_h(reg_t, reg_n, temp);
            }else{ // LDRH
                la_ldx_hu(reg_t, reg_n, temp);
            }
            break;
        case 2:
            if(a->sign){ // LDRSW
                la_ldx_w(reg_t, reg_n, temp);
            }else{ // LDR
                la_ldx_wu(reg_t, reg_n, temp);
            }
            break;
        case 3: // LDR
            la_ldx_d(reg_t, reg_n, temp);
            break;
        default:
            break;
        }
    }

    if (a->rt != 31 && arm_la_map[a->rt] >= 0 && clearGprHigh) {
        set_w_write_flag(a->rt, !(a->sign && !iss_sf));
    } else {
        if (a->sign && !iss_sf) {
            la_bstrpick_d(reg_t, reg_t, 31, 0);
        }
    }

    store_gpr_dst(a->rt, reg_t);
    free_alloc_gpr(reg_t);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(reg_m);
    free_alloc_gpr(temp);
    return true;
}

static bool trans_STR(DisasContext *s)
{
    arg_ldst *a = &(s->arg.f_ldst);
    IR2_OPND reg_t = alloc_gpr_src(a->rt);
    IR2_OPND reg_n = alloc_gpr_src_sp(a->rn);
    IR2_OPND reg_m = alloc_gpr_src(a->rm);
    IR2_OPND temp = ra_alloc_itemp();
    int extsize = extract32(a->opt, 0, 2);
    bool is_signed = extract32(a->opt, 2, 1);
    unsigned int shift = a->s ? a->sz : 0;

    /*  STR (register)
        32-bit (size == 10)
        STR <Wt>, [<Xn|SP>, (<Wm>|<Xm>){, <extend> {<amount>}}]
        64-bit (size == 11)
        STR <Xt>, [<Xn|SP>, (<Wm>|<Xm>){, <extend> {<amount>}}]

        STRB (register)
        Extended register (option != 011)
        STRB <Wt>, [<Xn|SP>, (<Wm>|<Xm>), <extend> {<amount>}]
        Shifted register (option == 011)
        STRB <Wt>, [<Xn|SP>, <Xm>{, LSL <amount>}]

        STRH (register)
        STRH <Wt>, [<Xn|SP>, (<Wm>|<Xm>){, <extend> {<amount>}}]
    */
    if(clearGprHigh && arm_la_map[a->rn] >= 0){ /* all reg_n is 64-bit size*/
        clear_gpr_high(a->rn);
    }

    if(clearGprHigh && a->sz == 3 && arm_la_map[a->rt] >= 0 && a->rt != 31){
        clear_gpr_high(a->rt);
    }

    if(clearGprHigh && a->sz == 3 && arm_la_map[a->rm] >= 0 && a->rm != 31){
        clear_gpr_high(a->rm);
    }

    if (extract32(a->opt, 1, 1) == 0) {
        return false;
    }

    if (is_signed) {
        switch (extsize) {
        case 0:
            la_ext_w_b(temp, reg_m);
            break;
        case 1:
            la_ext_w_h(temp, reg_m);
            break;
        case 2:
            la_bstrpick_w(temp,reg_m,31,0);
            break;
        case 3:
            //to reduce instructions inflation,handle separately
            break;
        }
    } else {
        switch (extsize) {
        case 0:
            la_bstrpick_d(temp,reg_m,7,0);
            break;
        case 1:
            la_bstrpick_d(temp,reg_m,15,0);
            break;
        case 2:
            la_bstrpick_d(temp,reg_m,31,0);
            break;
        case 3:
            //to reduce instructions inflation,handle separately
            break;
        }
    }
    
    if(extsize == 3 && !shift){
        switch (a->sz)
        {
        case 0:
            la_stx_b(reg_t, reg_n, reg_m);
            break;
        case 1:
            la_stx_h(reg_t, reg_n, reg_m);
            break;
        case 2:
            la_stx_w(reg_t, reg_n, reg_m);
            break;
        case 3:
            la_stx_d(reg_t, reg_n, reg_m);
            break;
        default:
            break;
        }
    }else{
        if (shift) {
            if(extsize == 3){
                la_slli_d(temp,reg_m,shift);
            }else{
                la_slli_d(temp,temp,shift);
            }        
        }

        switch (a->sz)
        {
        case 0:
            la_stx_b(reg_t, reg_n, temp);
            break;
        case 1:
            la_stx_h(reg_t, reg_n, temp);
            break;
        case 2:
            la_stx_w(reg_t, reg_n, temp);
            break;
        case 3:
            la_stx_d(reg_t, reg_n, temp);
            break;
        default:
            break;
        }
    }

    free_alloc_gpr(reg_t);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(reg_m);
    free_alloc_gpr(temp);
    return true;
}

static bool trans_LDR_v(DisasContext *s)
{
    arg_ldst *a = &(s->arg.f_ldst);
    IR2_OPND vreg_t = alloc_fpr_dst(a->rt);
    IR2_OPND reg_n = alloc_gpr_src_sp(a->rn);
    IR2_OPND reg_m = alloc_gpr_src(a->rm);
    IR2_OPND temp = ra_alloc_itemp();

    if(clearGprHigh && arm_la_map[a->rn] >= 0){ /* all reg_n is 64-bit size*/
        clear_gpr_high(a->rn);
    }

    if(clearGprHigh && extract32(a->opt, 0, 2) == 3 && arm_la_map[a->rm] >= 0 && a->rm != 31){ 
        clear_gpr_high(a->rm);
    }

    if (extract32(a->opt, 1, 1) == 0) {
        return false;
    }

    if (!fp_access_check(s)) {
        return true;
    }

    int extsize = extract32(a->opt, 0, 2);
    bool is_signed = extract32(a->opt, 2, 1);
    unsigned int shift = a->s ? a->sz : 0;

    if (is_signed) {
        switch (extsize) {
        case 0:
            la_ext_w_b(temp, reg_m);
            break;
        case 1:
            la_ext_w_h(temp, reg_m);
            break;
        case 2:
            la_bstrpick_w(temp,reg_m,31,0);
            break;
        case 3:
            //to reduce instructions inflation,handle separately
            break;
        }
    } else {
        switch (extsize) {
        case 0:
            la_bstrpick_d(temp,reg_m,7,0);
            break;
        case 1:
            la_bstrpick_d(temp,reg_m,15,0);
            break;
        case 2:
            la_bstrpick_d(temp,reg_m,31,0);
            break;
        case 3:
            //to reduce instructions inflation,handle separately
            break;
        }
    }
    
    /* LDR(register), */
    if(extsize == 3 && !shift){
        switch (a->sz)
        {
        case 0:
            la_vxor_v(vreg_t, vreg_t, vreg_t);
            la_ldx_bu(temp, reg_n, reg_m);
            la_vinsgr2vr_b(vreg_t, temp, 0);
            break;
        case 1:
            la_vxor_v(vreg_t, vreg_t, vreg_t);
            la_ldx_hu(temp, reg_n, reg_m);
            la_vinsgr2vr_h(vreg_t, temp, 0);
            break;
        case 2:
            la_vxor_v(vreg_t, vreg_t, vreg_t);
            la_ldx_wu(temp, reg_n, reg_m);
            la_vinsgr2vr_w(vreg_t, temp, 0);
            break;
        case 3: 
            la_vldx(vreg_t, reg_n, reg_m);
            la_vinsgr2vr_d(vreg_t, zero_ir2_opnd, 1);
            break;
        case 4:
            la_vldx(vreg_t, reg_n, reg_m);
            break;
        default:
            break;
        }
    }else{
        if (shift) {
            if(extsize == 3){
                la_slli_d(temp,reg_m,shift);
            }else{
                la_slli_d(temp,temp,shift);
            }        
        }

        /* LDR(register), */
        switch (a->sz)
        {
        case 0:
            la_vxor_v(vreg_t, vreg_t, vreg_t);
            la_ldx_bu(temp, reg_n, temp);
            la_vinsgr2vr_b(vreg_t, temp, 0);
            break;
        case 1:
            la_vxor_v(vreg_t, vreg_t, vreg_t);
            la_ldx_hu(temp, reg_n, temp);
            la_vinsgr2vr_h(vreg_t, temp, 0);
            break;
        case 2:
            la_vxor_v(vreg_t, vreg_t, vreg_t);
            la_ldx_wu(temp, reg_n, temp);
            la_vinsgr2vr_w(vreg_t, temp, 0);
            break;
        case 3: 
            la_vldx(vreg_t, reg_n, temp);
            la_vinsgr2vr_d(vreg_t, zero_ir2_opnd, 1);
            break;
        case 4:
            la_vldx(vreg_t, reg_n, temp);
            break;
        default:
            break;
        }
    }

    store_fpr_dst(a->rt, vreg_t);
    free_alloc_fpr(vreg_t);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(reg_m);
    free_alloc_gpr(temp);
    return true;
}

static bool trans_STR_v(DisasContext *s)
{
    arg_ldst *a = &(s->arg.f_ldst);
    IR2_OPND vreg_t = alloc_fpr_src(a->rt);
    IR2_OPND reg_n = alloc_gpr_src_sp(a->rn);
    IR2_OPND reg_m = alloc_gpr_src(a->rm);
    IR2_OPND temp = ra_alloc_itemp();
    IR2_OPND temp1 = ra_alloc_itemp();
    int extsize = extract32(a->opt, 0, 2);
    bool is_signed = extract32(a->opt, 2, 1);
    unsigned int shift = a->s ? a->sz : 0;

    if(clearGprHigh && arm_la_map[a->rn] >= 0){ /* all reg_n is 64-bit size*/
        clear_gpr_high(a->rn);
    }

    if(clearGprHigh && extract32(a->opt, 0, 2) == 3 && arm_la_map[a->rm] >= 0 && a->rm != 31){ 
        clear_gpr_high(a->rm);
    }

    if (extract32(a->opt, 1, 1) == 0) {
        return false;
    }

    if (!fp_access_check(s)) {
        return true;
    }

    if (is_signed) {
        switch (extsize) {
        case 0:
            la_ext_w_b(temp, reg_m);
            break;
        case 1:
            la_ext_w_h(temp, reg_m);
            break;
        case 2:
            la_bstrpick_w(temp,reg_m,31,0);
            break;
        case 3:
            //to reduce instructions inflation,handle separately
            break;
        }
    } else {
        switch (extsize) {
        case 0:
            la_bstrpick_d(temp,reg_m,7,0);
            break;
        case 1:
            la_bstrpick_d(temp,reg_m,15,0);
            break;
        case 2:
            la_bstrpick_d(temp,reg_m,31,0);
            break;
        case 3:
            //to reduce instructions inflation,handle separately
            break;
        }
    }
    
    if(extsize == 3 && !shift){
        switch (a->sz)
        {
        case 0:
            la_vpickve2gr_bu(temp1, vreg_t, 0);
            la_stx_b(temp1, reg_n, reg_m);
            break;
        case 1:
            la_vpickve2gr_hu(temp1, vreg_t, 0);
            la_stx_h(temp1, reg_n, reg_m);
            break;
        case 2:
            la_fstx_s(vreg_t, reg_n, reg_m);
            break;
        case 3: 
            la_fstx_d(vreg_t, reg_n, reg_m);
            break;
        case 4:
            la_vstx(vreg_t, reg_n, reg_m);
            break;
        default:
            break;
        }
    }else{
        if (shift) {
            if(extsize == 3){
                la_slli_d(temp,reg_m,shift);
            }else{
                la_slli_d(temp,temp,shift);
            }        
        }

        switch (a->sz)
        {
        case 0:
            la_vpickve2gr_bu(temp1, vreg_t, 0);
            la_stx_b(temp1, reg_n, temp);
            break;
        case 1:
            la_vpickve2gr_hu(temp1, vreg_t, 0);
            la_stx_h(temp1, reg_n, temp);
            break;
        case 2:
            la_fstx_s(vreg_t, reg_n, temp);
            break;
        case 3: 
            la_fstx_d(vreg_t, reg_n, temp);
            break;
        case 4:
            la_vstx(vreg_t, reg_n, temp);
            break;
        default:
            break;
        }
    }

    free_alloc_fpr(vreg_t);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(reg_m);
    free_alloc_gpr(temp);
    free_alloc_gpr(temp1);
    return true;
}


static bool do_atomic_ld(DisasContext *s, arg_atomic *a, AtomicThreeOpFn *fn,
                         int sign, bool invert)
{
    assert(0);
}

TRANS_FEAT_ATOMIC(LDADD, aa64_atomics, do_atomic_ld, tcg_gen_atomic_fetch_add_i64, 0, false)
TRANS_FEAT_ATOMIC(LDCLR, aa64_atomics, do_atomic_ld, tcg_gen_atomic_fetch_and_i64, 0, true)
TRANS_FEAT_ATOMIC(LDEOR, aa64_atomics, do_atomic_ld, tcg_gen_atomic_fetch_xor_i64, 0, false)
TRANS_FEAT_ATOMIC(LDSET, aa64_atomics, do_atomic_ld, tcg_gen_atomic_fetch_or_i64, 0, false)
TRANS_FEAT_ATOMIC(LDSMAX, aa64_atomics, do_atomic_ld, tcg_gen_atomic_fetch_smax_i64, MO_SIGN, false)
TRANS_FEAT_ATOMIC(LDSMIN, aa64_atomics, do_atomic_ld, tcg_gen_atomic_fetch_smin_i64, MO_SIGN, false)
TRANS_FEAT_ATOMIC(LDUMAX, aa64_atomics, do_atomic_ld, tcg_gen_atomic_fetch_umax_i64, 0, false)
TRANS_FEAT_ATOMIC(LDUMIN, aa64_atomics, do_atomic_ld, tcg_gen_atomic_fetch_umin_i64, 0, false)
TRANS_FEAT_ATOMIC(SWP, aa64_atomics, do_atomic_ld, tcg_gen_atomic_xchg_i64, 0, false)

static bool trans_LDAPR(DisasContext *s)
{
    assert(0);
}

static bool trans_LDRA(DisasContext *s)
{
    assert(0);
}

static bool trans_LDAPR_i(DisasContext *s)
{
    assert(0);
}

static bool trans_STLR_i(DisasContext *s)
{
    assert(0);
}

static bool trans_LD_mult(DisasContext *s)
{
    arg_ldst_mult *a = &(s->arg.f_ldst_mult);
    IR2_OPND vreg_d, reg_n, reg_m;

    int total;    /* total bytes */
    int size = a->sz;

    if (!a->p && a->rm != 0) {
        /* For non-postindexed accesses the Rm field must be 0 */
        return false;
    }
    if (size == 3 && !a->q && a->selem != 1) {
        return false;
    }
    if (!fp_access_check(s)) {
        return true;
    }

    if (a->rn == 31) {
        gen_check_sp_alignment(s);
    }

    reg_n = alloc_gpr_src_sp(a->rn);
    if(clearGprHigh && arm_la_map[a->rn] >= 0){ /* all reg_n is 64-bit size*/
        clear_gpr_high(a->rn);
    }

    if(a->selem == 1){ /* LD1(multiple structures) */
        int vbytes = (a->q ? 16 : 8); /* bytes per vector */
        total = a->rpt * vbytes;

        /*  这里默认是小尾端，
            使用vld读取16个字节和使用16次ld_B的效果相同(H, W, D同理) 
        */
        for (int r = 0; r < a->rpt; r++) {
            int tt = (a->rt + r) % 32;
            int offset = r * vbytes;
            assert(offset <= 2047 && offset >= -2048); /* vld的立即数是imm12 */
            vreg_d = alloc_fpr_dst(tt);
            la_vld(vreg_d, reg_n, offset);
            if(!a->q){
                /* 高64位清零 */
                la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
            }
            store_fpr_dst(tt, vreg_d);
            free_alloc_fpr(vreg_d);
        }
    }else{ /* LD2/LD3/LD4, both are multiple structures */
        int elements = (a->q ? 16 : 8) >> size; /* elements per vector */
        int ebytes = 1 << size; /* bytes per element */
        total = a->selem * elements * ebytes;

        IR2_OPND vtemp = ra_alloc_ftemp();
        for(int r = 0; r < a->selem; r++){
            int tt = (a->rt + r) % 32;
            vreg_d = alloc_fpr_dst(tt);
            for(int e = 0; e < elements; e++){
                int offset = (e * a->selem + r) * ebytes;
                la_vld(vtemp, reg_n, offset);
                switch (ebytes)
                {
                case 1:
                    la_vextrins_b(vreg_d, vtemp, e << 4);
                    break;
                case 2:
                    la_vextrins_h(vreg_d, vtemp, e << 4);
                    break;
                case 4:
                    la_vextrins_w(vreg_d, vtemp, e << 4);
                    break;
                case 8:
                    la_vextrins_d(vreg_d, vtemp, e << 4);
                    break;
                default:
                    break;
                }
            }
            if(!a->q){
                la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
            }

            store_fpr_dst(tt, vreg_d);
            free_alloc_fpr(vreg_d);
        }
        free_alloc_fpr(vtemp);
    }

    if (a->p) {
        if (a->rm == 31) {
            // tcg_gen_addi_i64(tcg_rn, tcg_rn, total);
            assert(total <= 2047 && total >= -2048); /* addi的立即数是imm12 */
            la_addi_d(reg_n, reg_n, total);
        } else {
            reg_m = alloc_gpr_src(a->rm);

            if (clearGprHigh && arm_la_map[a->rm] >= 0 && a->rm != 31) { /* reg_m is 64-bit size*/
                clear_gpr_high(a->rm);
            }

            la_add_d(reg_n, reg_n, reg_m);
            free_alloc_gpr(reg_m);
        }
    }

    store_gpr_dst(a->rn, reg_n);
    free_alloc_gpr(reg_n);
    return true;
}

static bool trans_ST_mult(DisasContext *s)
{
    // assert(0);
    arg_ldst_mult *a = &(s->arg.f_ldst_mult);
    IR2_OPND vreg_d, reg_n, reg_m;

    int total;    /* total bytes */
    int size = a->sz;

    if (!a->p && a->rm != 0) {
        /* For non-postindexed accesses the Rm field must be 0 */
        return false;
    }
    if (size == 3 && !a->q && a->selem != 1) {
        return false;
    }
    if (!fp_access_check(s)) {
        return true;
    }

    if (a->rn == 31) {
        gen_check_sp_alignment(s);
    }

    reg_n = alloc_gpr_src_sp(a->rn);
    if(clearGprHigh && arm_la_map[a->rn] >= 0){ /* all reg_n is 64-bit size*/
        clear_gpr_high(a->rn);
    }

    if(a->selem == 1){ /* ST1(multiple structures) */
        int vbytes = (a->q ? 16 : 8); /* bytes per vector */
        total = a->rpt * vbytes;

        /*  这里默认是小尾端，
            使用vst写入16个字节和使用16次st_B的效果相同(H, W, D同理) 
        */
        for (int r = 0; r < a->rpt; r++) {
            int tt = (a->rt + r) % 32;
            int offset = r * vbytes;
            assert(offset <= 2047 && offset >= -2048); /* vld的立即数是imm12 */
            vreg_d = alloc_fpr_src(tt);
            if(a->q){
                la_vst(vreg_d, reg_n, offset);
            }else{
                /* 这里和LD1有点差别，load多了可以高位清零，但是store不行 */
                la_fst_d(vreg_d, reg_n, offset);
            }
            free_alloc_fpr(vreg_d);
        }
    }else{ /* ST2/ST3/ST4, both are multiple structures */
        int elements = (a->q ? 16 : 8) >> size; /* elements per vector */
        int ebytes = 1 << size; /* bytes per element */
        total = a->selem * elements * ebytes;

        for(int r = 0; r < a->selem; r++){ /* structure elements 等于需要store的寄存器个数 */
            int tt = (a->rt + r) % 32;
            vreg_d = alloc_fpr_src(tt);
            for(int e = 0; e < elements; e++){
                int offset = e * a->selem + r;
                switch (ebytes)
                {
                case 1:
                    la_vstelm_b(vreg_d, reg_n, offset, e);
                    break;
                case 2:
                    la_vstelm_h(vreg_d, reg_n, offset, e);
                    break;
                case 4:
                    la_vstelm_w(vreg_d, reg_n, offset, e);
                    break;
                case 8:
                    la_vstelm_d(vreg_d, reg_n, offset, e);
                    break;
                default:
                    break;
                }
            }

            free_alloc_fpr(vreg_d);
        }
    }

    if (a->p) {
        if (a->rm == 31) {
            // tcg_gen_addi_i64(tcg_rn, tcg_rn, total);
            assert(total <= 2047 && total >= -2048); /* addi的立即数是imm12 */
            la_addi_d(reg_n, reg_n, total);
        } else {
            reg_m = alloc_gpr_src(a->rm);

            if (clearGprHigh && arm_la_map[a->rm] >= 0 && a->rm != 31) { /* reg_m is 64-bit size*/
                clear_gpr_high(a->rm);
            }

            la_add_d(reg_n, reg_n, reg_m);
            free_alloc_gpr(reg_m);
        }
    }

    store_gpr_dst(a->rn, reg_n);
    free_alloc_gpr(reg_n);
    return true;
}

static bool trans_ST_single(DisasContext *s)
{
    arg_ldst_single *a = &(s->arg.f_ldst_single);
    IR2_OPND vreg_d, reg_n, reg_m;
    int total;    /* total bytes */
    int esize;
    int xs, rt;

    if (!a->p && a->rm != 0) {
        return false;
    }
    if (!fp_access_check(s)) {
        return true;
    }

    if (a->rn == 31) {
        gen_check_sp_alignment(s);
    }

    total = a->selem << a->scale;
    esize = 1 << a->scale;

    reg_n = alloc_gpr_src_sp(a->rn);
    if(clearGprHigh && arm_la_map[a->rn] >= 0){ /* all reg_n is 64-bit size*/
        clear_gpr_high(a->rn);
    }

    for (xs = 0, rt = a->rt; xs < a->selem; xs++, rt = (rt + 1) % 32) {
        vreg_d = alloc_fpr_src(rt);
        switch (esize)
        {
        case 1:
            la_vstelm_b(vreg_d, reg_n, xs, a->index);
            break;
        case 2:
            la_vstelm_h(vreg_d, reg_n, xs, a->index);
            break;
        case 4:
            la_vstelm_w(vreg_d, reg_n, xs, a->index);
            break;
        case 8:
            la_vstelm_d(vreg_d, reg_n, xs, a->index);
            break;
        default:
            break;
        }
    }

    if (a->p) {
        if (a->rm == 31) {
            // tcg_gen_addi_i64(tcg_rn, tcg_rn, total);
            assert(total <= 2047 && total >= -2048); /* addi的立即数是imm12 */
            la_addi_d(reg_n, reg_n, total);
        } else {
            reg_m = alloc_gpr_src(a->rm);
            if (clearGprHigh && arm_la_map[a->rm] >= 0 && a->rm != 31) { /* reg_m is 64-bit size*/
                clear_gpr_high(a->rm);
            }

            la_add_d(reg_n, reg_n, reg_m);
            free_alloc_gpr(reg_m);
        }
    }

    store_gpr_dst(a->rn, reg_n);
    free_alloc_gpr(reg_n);
    return true;
}

static bool trans_LD_single(DisasContext *s)
{
    arg_ldst_single *a = &(s->arg.f_ldst_single);
    IR2_OPND vreg_d, reg_n, reg_m, vtemp;
    int total;    /* total bytes */
    int esize;
    int xs, rt;

    if (!a->p && a->rm != 0) {
        return false;
    }
    if (!fp_access_check(s)) {
        return true;
    }

    if (a->rn == 31) {
        gen_check_sp_alignment(s);
    }

    total = a->selem << a->scale;
    esize = 1 << a->scale;
    reg_n = alloc_gpr_src_sp(a->rn);
    if(clearGprHigh && arm_la_map[a->rn] >= 0){ /* all reg_n is 64-bit size*/
        clear_gpr_high(a->rn);
    }

    vtemp = ra_alloc_ftemp();
    for (xs = 0, rt = a->rt; xs < a->selem; xs++, rt = (rt + 1) % 32) {
        // do_vec_ld(s, rt, a->index, clean_addr, mop);
        // tcg_gen_add_i64(clean_addr, clean_addr, tcg_ebytes);
        vreg_d = alloc_fpr_src(rt);
        int offset = xs * esize;
        la_vld(vtemp, reg_n, offset);
        switch (esize)
        {
        case 1:
            la_vextrins_b(vreg_d, vtemp, a->index << 4);
            break;
        case 2:
            la_vextrins_h(vreg_d, vtemp, a->index << 4);
            break;
        case 4:
            la_vextrins_w(vreg_d, vtemp, a->index << 4);
            break;
        case 8:
            la_vextrins_d(vreg_d, vtemp, a->index << 4);
            break;
        default:
            break;
        }
        store_fpr_dst(rt, vreg_d);
    }

    if (a->p) {
        if (a->rm == 31) {
            // tcg_gen_addi_i64(tcg_rn, tcg_rn, total);
            assert(total <= 2047 && total >= -2048); /* addi的立即数是imm12 */
            la_addi_d(reg_n, reg_n, total);
        } else {
            reg_m = alloc_gpr_src(a->rm);
            if (clearGprHigh && arm_la_map[a->rm] >= 0 && a->rm != 31) { /* reg_m is 64-bit size*/
                clear_gpr_high(a->rm);
            }

            la_add_d(reg_n, reg_n, reg_m);
            free_alloc_gpr(reg_m);
        }
    }

    store_gpr_dst(a->rn, reg_n);
    free_alloc_gpr(reg_n);
    free_alloc_fpr(vtemp);
    return true;
}

static bool trans_LD_single_repl(DisasContext *s)
{
    arg_LD_single_repl *a = &(s->arg.f_decode_insn3232);
    IR2_OPND vreg_d, reg_n, reg_m;
    int total;    /* total bytes */
    int esize;

    if (!a->p && a->rm != 0) {
        return false;
    }
    if (!fp_access_check(s)) {
        return true;
    }

    if (a->rn == 31) {
        gen_check_sp_alignment(s);
    }

    total = a->selem << a->scale;
    esize = 1 << a->scale;

    reg_n = alloc_gpr_src_sp(a->rn);
    if(clearGprHigh && arm_la_map[a->rn] >= 0){ /* all reg_n is 64-bit size*/
        clear_gpr_high(a->rn);
    }

    vreg_d = alloc_fpr_dst(a->rt);
    switch (esize)
    {
    case 1:
        la_vldrepl_b(vreg_d, reg_n, 0);
        break;
    case 2:
        la_vldrepl_h(vreg_d, reg_n, 0);
        break;
    case 4:
        la_vldrepl_w(vreg_d, reg_n, 0);
        break;
    case 8:
        la_vldrepl_d(vreg_d, reg_n, 0);
        break;
    default:
        break;
    }

    if(!a->q){
        /* 高64位清零 */
        la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
    }
    store_fpr_dst(a->rt, vreg_d);

    if (a->p) {
        if (a->rm == 31) {
            // tcg_gen_addi_i64(tcg_rn, tcg_rn, total);
            assert(total <= 2047 && total >= -2048); /* addi的立即数是imm12 */
            la_addi_d(reg_n, reg_n, total);
        } else {
            reg_m = alloc_gpr_src(a->rm);
            if (clearGprHigh && arm_la_map[a->rm] >= 0 && a->rm != 31) { /* reg_m is 64-bit size*/
                clear_gpr_high(a->rm);
            }

            la_add_d(reg_n, reg_n, reg_m);
            free_alloc_gpr(reg_m);
        }
    }

    store_gpr_dst(a->rn, reg_n);
    free_alloc_gpr(reg_n);
    return true;
}

static bool trans_STZGM(DisasContext *s)
{
    assert(0);
}

static bool trans_STGM(DisasContext *s)
{
    assert(0);
}

static bool trans_LDGM(DisasContext *s)
{
    assert(0);
}

static bool trans_LDG(DisasContext *s)
{
    assert(0);
}

static bool do_STG(DisasContext *s, arg_ldst_tag *a, bool is_zero, bool is_pair)
{
    assert(0);
}

TRANS_FEAT_LDST_TAG(STG, aa64_mte_insn_reg, do_STG, false, false)
TRANS_FEAT_LDST_TAG(STZG, aa64_mte_insn_reg, do_STG, true, false)
TRANS_FEAT_LDST_TAG(ST2G, aa64_mte_insn_reg, do_STG, false, true)
TRANS_FEAT_LDST_TAG(STZ2G, aa64_mte_insn_reg, do_STG, true, true)

typedef void ArithTwoOp(TCGv_i64, TCGv_i64, TCGv_i64);

/*
 * PC-rel. addressing
 */

static bool trans_ADR(DisasContext *s)
{
    arg_ri *a = &(s->arg.f_ri);
    IR2_OPND reg_d = alloc_gpr_dst(a->rd);
    li_d(reg_d, (int64_t)(a->imm + s->pc_curr) );

    if (a->rd != 31 && arm_la_map[a->rd] >= 0 && clearGprHigh) {
        set_w_write_flag(a->rd, TRUE);
    }

    store_gpr_dst(a->rd,reg_d);
    free_alloc_gpr(reg_d);
    return true;
}

static bool trans_ADRP(DisasContext *s)
{
    arg_ri *a = &(s->arg.f_ri);
    int64_t offset = a->imm << 12;

    /* The page offset is ok for CF_PCREL. */
    offset -= s->pc_curr & 0xfff;
    IR2_OPND reg_d = alloc_gpr_dst(a->rd);
    li_d(reg_d, (int64_t)(offset + s->pc_curr) );

    if (a->rd != 31 && arm_la_map[a->rd] >= 0 && clearGprHigh) {
        set_w_write_flag(a->rd, TRUE);
    }

    store_gpr_dst(a->rd,reg_d);
    free_alloc_gpr(reg_d);
    return true;
}

/*
 * Add/subtract (immediate)
 */

static bool trans_ADD_i(DisasContext *s)
{
    arg_ADD_i *a = &(s->arg.f_rri_sf);
    IR2_OPND reg_d = alloc_gpr_dst_sp(a->rd);   
    IR2_OPND reg_n = alloc_gpr_src_sp(a->rn);   
    IR2_OPND temp = ra_alloc_itemp();

    if(clearGprHigh && a->sf &&  arm_la_map[a->rn] >= 0 && a->rn != 31){
        clear_gpr_high(a->rn);
    }

    uint8_t shift = extract32(s->insn, 22, 1);
    switch(shift){
        case 0:
            if(a->imm <= 0x7ff){
                if(a->sf){
                    la_addi_d(reg_d, reg_n, a->imm);
                }else{
                    la_addi_w(reg_d, reg_n, a->imm);
                }
            }else {
                la_ori(temp, zero_ir2_opnd, a->imm);
                if(a->sf){
                    la_add_d(reg_d, reg_n, temp);
                }else{
                    la_add_w(reg_d, reg_n, temp);
                }               
            }
            break;
        case 1:
            la_lu12i_w(temp, a->imm >> 12);
            if(a->sf){
                la_add_d(reg_d, reg_n, temp);
            }else{
                la_add_w(reg_d, reg_n, temp);
            }
            break;
        default:
            assert(0);
            break;
    }

    if (a->rd != 31 && arm_la_map[a->rd] >= 0 && clearGprHigh) {
        set_w_write_flag(a->rd, a->sf);
    } else {
        if (!a->sf) {
            la_bstrpick_d(reg_d, reg_d, 31, 0);
        }
    }

    store_gpr_dst(a->rd, reg_d);
    free_alloc_gpr(temp);
    free_alloc_gpr(reg_d);
    free_alloc_gpr(reg_n);
    return true;
}

static bool trans_SUB_i(DisasContext *s)
{
    arg_SUB_i *a = &(s->arg.f_rri_sf);
    IR2_OPND reg_d = alloc_gpr_dst_sp(a->rd);   
    IR2_OPND reg_n = alloc_gpr_src_sp(a->rn);   
    IR2_OPND temp = ra_alloc_itemp();


    if(clearGprHigh && a->sf && arm_la_map[a->rn] >= 0 && a->rn != 31){
        clear_gpr_high(a->rn);
    }

    uint8_t shift = extract32(s->insn, 22, 1);
    switch(shift){
        case 0:
            if(a->imm <= 0x7ff){
                if(a->sf){
                    la_addi_d(reg_d, reg_n, -a->imm);
                }else{
                    la_addi_w(reg_d, reg_n, -a->imm);
                }
            }else {
                la_ori(temp, zero_ir2_opnd, a->imm);
                if(a->sf){
                    la_sub_d(reg_d, reg_n, temp);
                }else{
                    la_sub_w(reg_d, reg_n, temp);
                }               
            }
            break;
        case 1:
            la_lu12i_w(temp, a->imm >> 12);
            if(a->sf){
                la_sub_d(reg_d, reg_n, temp);
            }else{
                la_sub_w(reg_d, reg_n, temp);
            }
            break;
        default:
            assert(0);
            break;
    }

    if (a->rd != 31 && arm_la_map[a->rd] >= 0 && clearGprHigh) {
        set_w_write_flag(a->rd, a->sf);
    } else {
        if (!a->sf) {
            la_bstrpick_d(reg_d, reg_d, 31, 0);
        }
    }
    store_gpr_dst(a->rd, reg_d);
    free_alloc_gpr(reg_d);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(temp);
    return true;
}

static bool trans_ADDS_i(DisasContext *s)
{
    arg_ADDS_i *a = &(s->arg.f_rri_sf);
    IR2_OPND reg_d = alloc_gpr_dst(a->rd);   
    IR2_OPND reg_n = alloc_gpr_src_sp(a->rn);   
    IR2_OPND temp = ra_alloc_itemp();

    if(clearGprHigh && a->sf && arm_la_map[a->rn] >= 0 && a->rn != 31){
        clear_gpr_high(a->rn);
    }

    uint8_t shift = extract32(s->insn, 22, 1);
    switch(shift){
        case 0:
            la_ori(temp, zero_ir2_opnd, a->imm);
            break;
        case 1:
            la_lu12i_w(temp, a->imm >> 12);
            break;
        default:
            assert(0);
            break;
    }

    gen_add_CC(&reg_n, &temp, a->sf);
    gen_add(&reg_d, &reg_n, &temp, a->sf);

    if (a->rd != 31 && arm_la_map[a->rd] >= 0 && clearGprHigh) {
        set_w_write_flag(a->rd, a->sf);
    } else {
        if (!a->sf) {
            la_bstrpick_d(reg_d, reg_d, 31, 0);
        }
    }

    store_gpr_dst(a->rd, reg_d);
    free_alloc_gpr(reg_d);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(temp);
    return true;
}

static bool trans_SUBS_i(DisasContext *s)
{
    arg_SUBS_i *a = &(s->arg.f_rri_sf);
    IR2_OPND reg_d = alloc_gpr_dst(a->rd);   
    IR2_OPND reg_n = alloc_gpr_src_sp(a->rn);   
    IR2_OPND temp = ra_alloc_itemp();

    if(clearGprHigh && a->sf && arm_la_map[a->rn] >= 0 && a->rn != 31){
        clear_gpr_high(a->rn);
    }

    uint8_t shift = extract32(s->insn, 22, 1);
    switch(shift){
        case 0:
            la_ori(temp, zero_ir2_opnd, a->imm);
            break;
        case 1:
            la_lu12i_w(temp, a->imm >> 12);
            break;
        default:
            assert(0);
            break;
    }

    gen_sub_CC(&reg_n, &temp, a->sf);
    gen_sub(&reg_d, &reg_n, &temp, a->sf);

    if (a->rd != 31 && arm_la_map[a->rd] >= 0 && clearGprHigh) {
        set_w_write_flag(a->rd, a->sf);
    } else {
        if (!a->sf) {
            la_bstrpick_d(reg_d, reg_d, 31, 0);
        }
    }

    store_gpr_dst(a->rd, reg_d);
    free_alloc_gpr(reg_d);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(temp);
    return true;
}

/*
 * Add/subtract (immediate, with tags)
 */
static bool trans_ADDG_i(DisasContext *s)
{
    assert(0);
}

static bool trans_SUBG_i(DisasContext *s)
{
    assert(0);
}

/* The input should be a value in the bottom e bits (with higher
 * bits zero); returns that value replicated into every element
 * of size e in a 64 bit integer.
 */
static uint64_t bitfield_replicate(uint64_t mask, unsigned int e)
{
    assert(e != 0);
    while (e < 64) {
        mask |= mask << e;
        e *= 2;
    }
    return mask;
}

/*
 * Logical (immediate)
 */

/*
 * Simplified variant of pseudocode DecodeBitMasks() for the case where we
 * only require the wmask. Returns false if the imms/immr/immn are a reserved
 * value (ie should cause a guest UNDEF exception), and true if they are
 * valid, in which case the decoded bit pattern is written to result.
 */
bool logic_imm_decode_wmask(uint64_t *result, unsigned int immn,
                            unsigned int imms, unsigned int immr)
{
    uint64_t mask;
    unsigned e, levels, s, r;
    int len;

    assert(immn < 2 && imms < 64 && immr < 64);

    /* The bit patterns we create here are 64 bit patterns which
     * are vectors of identical elements of size e = 2, 4, 8, 16, 32 or
     * 64 bits each. Each element contains the same value: a run
     * of between 1 and e-1 non-zero bits, rotated within the
     * element by between 0 and e-1 bits.
     *
     * The element size and run length are encoded into immn (1 bit)
     * and imms (6 bits) as follows:
     * 64 bit elements: immn = 1, imms = <length of run - 1>
     * 32 bit elements: immn = 0, imms = 0 : <length of run - 1>
     * 16 bit elements: immn = 0, imms = 10 : <length of run - 1>
     *  8 bit elements: immn = 0, imms = 110 : <length of run - 1>
     *  4 bit elements: immn = 0, imms = 1110 : <length of run - 1>
     *  2 bit elements: immn = 0, imms = 11110 : <length of run - 1>
     * Notice that immn = 0, imms = 11111x is the only combination
     * not covered by one of the above options; this is reserved.
     * Further, <length of run - 1> all-ones is a reserved pattern.
     *
     * In all cases the rotation is by immr % e (and immr is 6 bits).
     */

    /* First determine the element size */
    len = 31 - clz32((immn << 6) | (~imms & 0x3f));
    if (len < 1) {
        /* This is the immn == 0, imms == 0x11111x case */
        return false;
    }
    e = 1 << len;

    levels = e - 1;
    s = imms & levels;
    r = immr & levels;

    if (s == levels) {
        /* <length of run - 1> mustn't be all-ones. */
        return false;
    }

    /* Create the value of one element: s+1 set bits rotated
     * by r within the element (which is e bits wide)...
     */
    mask = MAKE_64BIT_MASK(0, s + 1);
    if (r) {
        mask = (mask >> r) | (mask << (e - r));
        mask &= MAKE_64BIT_MASK(0, e);
    }
    /* ...then replicate the element over the whole 64 bit value */
    mask = bitfield_replicate(mask, e);
    *result = mask;
    return true;
}

static bool trans_AND_i(DisasContext *s){
    arg_AND_i *a = &(s->arg.f_rri_log);
    uint64_t imm;
    /* Some immediate field values are reserved. */
    if (!logic_imm_decode_wmask(&imm, extract32(a->dbm, 12, 1),
                                extract32(a->dbm, 0, 6),
                                extract32(a->dbm, 6, 6))) {
        return false;
    }

    if (!a->sf) 
    {
        imm &= 0xffffffffull;
    }

    IR2_OPND reg_d = alloc_gpr_dst_sp(a->rd);
    IR2_OPND reg_n = alloc_gpr_src(a->rn);

    if(clearGprHigh && a->sf && arm_la_map[a->rn] >= 0 && a->rn != 31){
        clear_gpr_high(a->rn);
    }

    if(imm > 0xfff){
        IR2_OPND temp = ra_alloc_itemp();
        li_d(temp, imm);
        la_and(reg_d, reg_n, temp);  
        free_alloc_gpr(temp);
    }   
    else{
        la_andi(reg_d, reg_n, imm);  
    }

    if (a->rd != 31 && arm_la_map[a->rd] >= 0 && clearGprHigh) {
        set_w_write_flag(a->rd, a->sf);
    } else {
        if (!a->sf) {
            la_bstrpick_d(reg_d, reg_d, 31, 0);
        }
    }

    store_gpr_dst(a->rd, reg_d);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(reg_d);
    return true;
}

static bool trans_ORR_i(DisasContext *s){
    arg_ORR_i *a = &(s->arg.f_rri_log);
    uint64_t imm;
    /* Some immediate field values are reserved. */
    if (!logic_imm_decode_wmask(&imm, extract32(a->dbm, 12, 1),
                                extract32(a->dbm, 0, 6),
                                extract32(a->dbm, 6, 6))) {
        return false;
    }

    if (!a->sf) 
    {
        imm &= 0xffffffffull;
    }

    IR2_OPND reg_d = alloc_gpr_dst_sp(a->rd);
    IR2_OPND reg_n = alloc_gpr_src(a->rn);

    if(clearGprHigh && a->sf && arm_la_map[a->rn] >= 0 && a->rn != 31){
        clear_gpr_high(a->rn);
    }

    if(imm > 0xfff){
        IR2_OPND temp = ra_alloc_itemp();
        li_d(temp, imm);
        la_or(reg_d, reg_n, temp);  
        free_alloc_gpr(temp);
    }   
    else{
        la_ori(reg_d, reg_n, imm);  
    }

    if (a->rd != 31 && arm_la_map[a->rd] >= 0 && clearGprHigh) {
        set_w_write_flag(a->rd, a->sf);
    } else {
        if (!a->sf) {
            la_bstrpick_d(reg_d, reg_d, 31, 0);
        }
    }

    store_gpr_dst(a->rd, reg_d);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(reg_d);
    return true;
}


static bool trans_EOR_i(DisasContext *s) { 
    arg_EOR_i *a = &(s->arg.f_rri_log);
    uint64_t imm;
    /* Some immediate field values are reserved. */
    if (!logic_imm_decode_wmask(&imm, extract32(a->dbm, 12, 1),
                                extract32(a->dbm, 0, 6),
                                extract32(a->dbm, 6, 6))) {
        return false;
    }

    if (!a->sf) 
    {
        imm &= 0xffffffffull;
    }
    
    IR2_OPND reg_d = alloc_gpr_dst_sp(a->rd);
    IR2_OPND reg_n = alloc_gpr_src(a->rn);

    if(clearGprHigh && a->sf && arm_la_map[a->rn] >= 0 && a->rn != 31){
        clear_gpr_high(a->rn);
    }

    if(imm > 0xfff){
        IR2_OPND temp = ra_alloc_itemp();
        li_d(temp, imm);
        la_xor(reg_d, reg_n, temp);  
        free_alloc_gpr(temp);
    }   
    else{
        la_xori(reg_d, reg_n, imm);  
    }

    if (a->rd != 31 && arm_la_map[a->rd] >= 0 && clearGprHigh) {
        set_w_write_flag(a->rd, a->sf);
    } else {
        if (!a->sf) {
            la_bstrpick_d(reg_d, reg_d, 31, 0);
        }
    }

    store_gpr_dst(a->rd, reg_d);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(reg_d);
    return true;
}
    
static bool trans_ANDS_i(DisasContext *s) { 
    arg_ANDS_i *a = &(s->arg.f_rri_log);
    uint64_t imm;
    /* Some immediate field values are reserved. */
    if (!logic_imm_decode_wmask(&imm, extract32(a->dbm, 12, 1),
                                extract32(a->dbm, 0, 6),
                                extract32(a->dbm, 6, 6))) {
        return false;
    }

    if (!a->sf) 
    {
        imm &= 0xffffffffull;
    }

    IR2_OPND reg_d = alloc_gpr_dst(a->rd);
    IR2_OPND reg_n = alloc_gpr_src(a->rn);
    IR2_OPND temp = ra_alloc_itemp();

    if(clearGprHigh && a->sf && arm_la_map[a->rn] >= 0 && a->rn != 31){
        clear_gpr_high(a->rn);
    }    

    li_d(temp, imm);
    la_and(reg_d, reg_n, temp);
    if(!a->sf){
        la_x86and_w(reg_n, temp);
    }else{
        la_x86and_d(reg_n, temp);
    }

    if (a->rd != 31 && arm_la_map[a->rd] >= 0 && clearGprHigh) {
        set_w_write_flag(a->rd, a->sf);
    } else {
        if (!a->sf) {
            la_bstrpick_d(reg_d, reg_d, 31, 0);
        }
    }

    store_gpr_dst(a->rd, reg_d);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(reg_d);
    free_alloc_gpr(temp);
    return true;
}


/*
 * Move wide (immediate)
 */

static bool trans_MOVZ(DisasContext *s)
{
    arg_MOVZ *a = &(s->arg.f_movw);
    int pos = a->hw << 4;
    uint64_t imm =  (uint64_t)a->imm << pos;

    IR2_OPND reg_d = alloc_gpr_dst(a->rd);
    if(imm > 0xfff){
        li_d(reg_d,imm);
    }else if(imm){
        la_ori(reg_d, zero_ir2_opnd, imm);
    }
    if(a->rd != 31 && arm_la_map[a->rd] >= 0 && clearGprHigh){
        set_w_write_flag(a->rd, TRUE);
    }

    if(!imm){
        store_gpr_zero(a->rd);
    }else{
        store_gpr_dst(a->rd, reg_d);
    }

    free_alloc_gpr(reg_d);
    return true;
}

static bool trans_MOVN(DisasContext *s)
{
    arg_movw *a = &(s->arg.f_movw);
    int pos = a->hw << 4;

    IR2_OPND reg_d = alloc_gpr_dst(a->rd);    

    uint64_t imm = a->imm;
    if( ~(imm << pos) > 0xfff){
        li_d(reg_d, ~(imm << pos));
    }else{
        la_ori(reg_d, zero_ir2_opnd,  ~(imm << pos));
    }

    if (a->rd != 31 && arm_la_map[a->rd] >= 0 && clearGprHigh) {
        set_w_write_flag(a->rd, a->sf);
    } else {
        if (!a->sf) {
            la_bstrpick_d(reg_d, reg_d, 31, 0);
        }
    }

    store_gpr_dst(a->rd, reg_d);
    free_alloc_gpr(reg_d);
    return true;
}

static bool trans_MOVK(DisasContext *s)
{
    arg_movw *a = &(s->arg.f_movw);
    int pos = a->hw << 4;
    IR2_OPND reg_d = alloc_gpr_src(a->rd);    
    IR2_OPND temp = ra_alloc_itemp();

    if(clearGprHigh && a->sf && arm_la_map[a->rd] >= 0 && a->rd != 31){
        clear_gpr_high(a->rd);
    }    

    if(a->imm > 0xfff){
        li_d(temp,a->imm);
    }else{
        la_ori(temp, zero_ir2_opnd, a->imm);
    }

    if(a->sf){
        la_bstrins_d(reg_d, temp, pos + 15, pos);
    }else{
        la_bstrins_w(reg_d, temp, pos + 15, pos);
    }

    if (a->rd != 31 && arm_la_map[a->rd] >= 0 && clearGprHigh) {
        set_w_write_flag(a->rd, a->sf);
    } else {
        if (!a->sf) {
            la_bstrpick_d(reg_d, reg_d, 31, 0);
        }
    }

    store_gpr_dst(a->rd, reg_d);
    free_alloc_gpr(reg_d);
    free_alloc_gpr(temp);
    return true;
}

/*
 * Bitfield
 */

static bool trans_SBFM(DisasContext *s)
{
    arg_SBFM *a = &(s->arg.f_bitfield);
    unsigned int bitsize = a->sf ? 64 : 32;
    unsigned int ri = a->immr;
    unsigned int si = a->imms;

    IR2_OPND reg_d = alloc_gpr_dst(a->rd);
    IR2_OPND reg_n = alloc_gpr_src(a->rn);

    if (clearGprHigh && a->sf && arm_la_map[a->rn] >= 0 && a->rn != 31) {
        clear_gpr_high(a->rn);
    }

    if(bitsize == 32 && si == 31){ // 32-bit ASR
        la_srai_w(reg_d, reg_n, ri);
    } else if (bitsize == 64 && si == 63){ // 64-bit ASR
        la_srai_d(reg_d, reg_n, ri);
    } 
    /* Wd<s-r:0> = Wn<s:r> */
    else if(ri == 0 && si == 7){ // SXTB
        la_ext_w_b(reg_d, reg_n);
    }else if(ri == 0 && si == 15){ // SXTH
        la_ext_w_h(reg_d, reg_n);
    }else if(ri == 0 && si == 31){ // SXTW
        la_add_w(reg_d, zero_ir2_opnd, reg_n);
    }else if(si >= ri){ // SBFX           
        if(bitsize == 64){
            la_slli_d(reg_d, reg_n, 63 - si);
            la_srai_d(reg_d, reg_d, 63 - si + ri);
        }else {
            la_slli_w(reg_d, reg_n, 31 - si);
            la_srai_w(reg_d, reg_d, 31 - si + ri);
        }
    } else { // SBFIZ 
        /* Wd<32+s-r,32-r> = Wn<s:0> */
        if(bitsize == 64){
            la_slli_d(reg_d, reg_n, 63 - si);
            la_srai_d(reg_d, reg_d, ri - si -1);
        }else {
            la_slli_w(reg_d, reg_n, 31 - si);
            la_srai_w(reg_d, reg_d, ri - si -1);
        }
    }

    if (a->rd != 31 && arm_la_map[a->rd] >= 0 && clearGprHigh) {
        set_w_write_flag(a->rd, a->sf);
    } else {
        if (!a->sf) {
            la_bstrpick_d(reg_d, reg_d, 31, 0);
        }
    }

    store_gpr_dst(a->rd, reg_d);
    free_alloc_gpr(reg_d);
    free_alloc_gpr(reg_n);
    return true;
}

static bool trans_UBFM(DisasContext *s)
{
    arg_UBFM *a = &(s->arg.f_bitfield);
    unsigned int bitsize = a->sf ? 64 : 32;
    unsigned int ri = a->immr;
    unsigned int si = a->imms;

    IR2_OPND reg_d = alloc_gpr_dst(a->rd);
    IR2_OPND reg_n = alloc_gpr_src(a->rn);
    if (clearGprHigh && a->sf && arm_la_map[a->rn] >= 0 && a->rn != 31) {
        clear_gpr_high(a->rn);
    }

    if (si >= ri) { // LSR, UXTB, UXTH, UBFX
        /* Wd<s-r:0> = Wn<s:r> */
        if(a->sf){
            la_bstrpick_d(reg_d, reg_n, si, ri);
        }else{
            la_bstrpick_w(reg_d, reg_n, si, ri);
        }
    } else {
        /* Wd<32+s-r,32-r> = Wn<s:0> */
        if ((!a->sf) && (si != 31) && (si+1 == ri)) { // 32-bit LSL
            la_slli_w(reg_d, reg_n, bitsize - ri);
        }else if(a->sf && (si != 63) && (si+1 == ri)){ // 64-bit LSL
            la_slli_d(reg_d, reg_n, bitsize - ri);
        }else{ // UBFIZ
            if(a->sf){
                la_bstrpick_d(reg_d, reg_n, si, 0);
                la_slli_d(reg_d, reg_d, bitsize - ri);
            }else{
                la_bstrpick_w(reg_d, reg_n, si, 0);
                la_slli_d(reg_d, reg_d, bitsize - ri);
            }
        }
    }

    if (a->rd != 31 && arm_la_map[a->rd] >= 0 && clearGprHigh) {
        set_w_write_flag(a->rd, a->sf);
    } else {
        if (!a->sf) {
            la_bstrpick_d(reg_d, reg_d, 31, 0);
        }
    }

    store_gpr_dst(a->rd, reg_d);
    free_alloc_gpr(reg_d);
    free_alloc_gpr(reg_n);
    return true;
}

static bool trans_BFM(DisasContext *s)
{
    arg_BFM *a = &(s->arg.f_bitfield);
    unsigned int bitsize = a->sf ? 64 : 32;
    unsigned int ri = a->immr;
    unsigned int si = a->imms;

    IR2_OPND reg_d = alloc_gpr_src(a->rd);
    IR2_OPND reg_n = alloc_gpr_src(a->rn);
    IR2_OPND temp;
    if (clearGprHigh && a->sf && arm_la_map[a->rd] >= 0 && a->rd != 31) {
        clear_gpr_high(a->rd);
    }
    if (clearGprHigh && a->sf && arm_la_map[a->rn] >= 0 && a->rn != 31) {
        clear_gpr_high(a->rn);
    }

    if (si >= ri) { // BFXIL
        /* Wd<s-r:0> = Wn<s:r> */
        temp = ra_alloc_itemp();
        if( a->sf ){
            la_srli_d(temp, reg_n, ri);
            la_bstrins_d(reg_d, temp, si - ri, 0);
        }else {
            la_srli_w(temp, reg_n, ri);
            la_bstrins_w(reg_d, temp, si - ri, 0);
        }
        free_alloc_gpr(temp);
    } else {
        /* Wd<32+s-r,32-r> = Wn<s:0> */
        // if(a->rn == 31){ // BFC
        //     if(a->sf){
        //         la_bstrins_d(reg_d, zero_ir2_opnd, bitsize + si - ri, bitsize - ri);
        //     }else{
        //         la_bstrins_w(reg_d, zero_ir2_opnd, bitsize + si - ri, bitsize - ri);
        //     }
        // }else{ // BFI
        //     reg_n = alloc_gpr_src(a->rn);
        //     if(a->sf){
        //         la_bstrins_d(reg_d, reg_n, bitsize + si - ri, bitsize - ri);
        //     }else{
        //         la_bstrins_w(reg_d, reg_n, bitsize + si - ri, bitsize - ri);
        //     }
        //     free_alloc_gpr(reg_n);
        // }
        if(a->sf){
            la_bstrins_d(reg_d, reg_n, bitsize + si - ri, bitsize - ri);
        }else{
            la_bstrins_w(reg_d, reg_n, bitsize + si - ri, bitsize - ri);
        }
    }

    if (a->rd != 31 && arm_la_map[a->rd] >= 0 && clearGprHigh) {
        set_w_write_flag(a->rd, a->sf);
    } else {
        if (!a->sf) {
            la_bstrpick_d(reg_d, reg_d, 31, 0);
        }
    }

    store_gpr_dst(a->rd, reg_d);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(reg_d);
    return true;
}

static bool trans_EXTR(DisasContext *s)
{
    arg_extract *a = &(s->arg.f_extract);
    IR2_OPND reg_d = alloc_gpr_dst(a->rd);
    IR2_OPND reg_m = alloc_gpr_src(a->rm);
    IR2_OPND reg_n = alloc_gpr_src(a->rn);
    IR2_OPND temp = ra_alloc_itemp();

    if (clearGprHigh && a->sf && arm_la_map[a->rm] >= 0 && a->rm != 31) {
        clear_gpr_high(a->rm);
    }
    if (clearGprHigh && a->sf && arm_la_map[a->rn] >= 0 && a->rn != 31) {
        clear_gpr_high(a->rn);
    }

    if (unlikely(a->imm == 0)) {
        /*
         * tcg shl_i32/shl_i64 is undefined for 32/64 bit shifts,
         * so an extract from bit 0 is a special case.
         */
        la_or(reg_d, reg_m, zero_ir2_opnd);
    } else {
        if(!a->sf){
            la_srli_w(temp, reg_m, a->imm);
            la_bstrins_w(temp, reg_n, 31, 32 - a->imm);
        }else{
            la_srli_d(temp, reg_m, a->imm);
            la_bstrins_d(temp, reg_n, 63, 64 - a->imm);
        }
        la_or(reg_d, temp, zero_ir2_opnd);
    }

    if (a->rd != 31 && arm_la_map[a->rd] >= 0 && clearGprHigh) {
        set_w_write_flag(a->rd, a->sf);
    } else {
        if (!a->sf) {
            la_bstrpick_d(reg_d, reg_d, 31, 0);
        }
    }

    store_gpr_dst(a->rd, reg_d);
    free_alloc_gpr(reg_d);
    free_alloc_gpr(reg_m);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(temp);
    return true;
}

/* Shift a TCGv src by immediate, put result in dst.
 * The shift amount must be in range (this should always be true as the
 * relevant instructions will UNDEF on bad shift immediates).
 */
static void shift_reg_imm(IR2_OPND *dst, IR2_OPND *src, int sf,
                          enum a64_shift_type shift_type, unsigned int shift_i)
{
    assert(shift_i < (sf ? 64 : 32));

    switch (shift_type) {
    case A64_SHIFT_TYPE_LSL:
        la_slli_d(*dst, *src, shift_i);
        break;
    case A64_SHIFT_TYPE_LSR:
        if(!sf){
            la_srli_w(*dst, *src, shift_i);
        }else{
            la_srli_d(*dst, *src, shift_i);
        }
        break;
    case A64_SHIFT_TYPE_ASR:
        if (!sf) {
            la_srai_w(*dst, *src, shift_i);
        }else{
            la_srai_d(*dst, *src, shift_i);
        }
        break;
    case A64_SHIFT_TYPE_ROR:
        if (!sf) {
            assert(shift_i<=31);
            la_rotri_w(*dst, *src, shift_i);
        }else{
            la_rotri_d(*dst, *src, shift_i);
        }
        break;
    default:
        assert(FALSE); /* all shift types should be handled */
        break;
    }

    if (!sf) { /* zero extend final result */
        la_bstrpick_d(*dst, *dst, 31, 0);
    }
}

/* Logical (shifted register)
 *   31  30 29 28       24 23   22 21  20  16 15    10 9    5 4    0
 * +----+-----+-----------+-------+---+------+--------+------+------+
 * | sf | opc | 0 1 0 1 0 | shift | N |  Rm  |  imm6  |  Rn  |  Rd  |
 * +----+-----+-----------+-------+---+------+--------+------+------+
 */
static void disas_logic_reg(DisasContext *s, uint32_t insn)
{
    unsigned int sf, opc, shift_type, invert, rm, shift_amount, rn, rd;

    sf = extract32(insn, 31, 1);
    opc = extract32(insn, 29, 2);
    shift_type = extract32(insn, 22, 2);
    invert = extract32(insn, 21, 1);
    rm = extract32(insn, 16, 5);
    shift_amount = extract32(insn, 10, 6);
    rn = extract32(insn, 5, 5);
    rd = extract32(insn, 0, 5);

    if (!sf && (shift_amount & (1 << 5))) {
        lata_unallocated_encoding(s);
        return;
    }

    IR2_OPND reg_d = alloc_gpr_dst(rd);
    IR2_OPND reg_m = alloc_gpr_src(rm);

    if (clearGprHigh && sf && arm_la_map[rm] >= 0 && rm != 31) {
        clear_gpr_high(rm);
    }

    if (opc == 1 && shift_amount == 0 && shift_type == 0 && rn == 31) {
        /* Unshifted ORR and ORN with WZR/XZR is the standard encoding for
         * register-register MOV and MVN, so it is worth special casing.
         */
        // IR2_OPND reg_d = alloc_gpr_dst(rd);
        // IR2_OPND reg_m = alloc_gpr_src(rm);

        if (invert) {
            la_orn(reg_d, zero_ir2_opnd, reg_m);
        } else {
            la_or(reg_d, zero_ir2_opnd, reg_m);
        }

        if (rd != 31 && arm_la_map[rd] >= 0 && clearGprHigh) {
            set_w_write_flag(rd, sf);
        } else {
            if (!sf) {
                la_bstrpick_d(reg_d, reg_d, 31, 0);
            }
        }

        store_gpr_dst(rd, reg_d);
        free_alloc_gpr(reg_d);
        free_alloc_gpr(reg_m);
        return;
    }

    IR2_OPND reg_n = alloc_gpr_src(rn);
    IR2_OPND temp = ra_alloc_itemp();

    if (clearGprHigh && sf && arm_la_map[rn] >= 0 && rn != 31) {
        clear_gpr_high(rn);
    }

    if (shift_amount) {
        shift_reg_imm(&temp, &reg_m, sf, shift_type, shift_amount);

        switch (opc | (invert << 2)) {
        case 0: /* AND */
        case 3: /* ANDS */
            if(opc==3){
                if(sf){
                    la_x86and_d(reg_n, temp);
                }else{
                    la_x86and_w(reg_n, temp);
                }
            }
            la_and(reg_d, reg_n, temp);
            break;
        case 1: /* ORR */
            la_or(reg_d, reg_n, temp);
            break;
        case 2: /* EOR */
            la_xor(reg_d, reg_n, temp);
            break;
        case 4: /* BIC */
        case 7: /* BICS */
            la_andn(reg_d, reg_n, temp);
            if(opc==3){
                la_nor(temp, zero_ir2_opnd, temp);
                if(sf){
                    la_x86and_d(reg_n, temp);
                }else{
                    la_x86and_w(reg_n, temp);
                }
            }
            break;
        case 5: /* ORN */
            la_orn(reg_d, reg_n, temp);
            break;
        case 6: /* EON */
            la_nor(temp, zero_ir2_opnd, temp);
            la_xor(reg_d, reg_n, temp);
            break;
        default:
            assert(FALSE);
            break;
        }
    }else{
        switch (opc | (invert << 2)) {
        case 0: /* AND */
        case 3: /* ANDS */
            if(opc==3){
                if(sf){
                    la_x86and_d(reg_n, reg_m);
                }else{
                    la_x86and_w(reg_n, reg_m);
                }
            }
            la_and(reg_d, reg_n, reg_m);
            break;
        case 1: /* ORR */
            la_or(reg_d, reg_n, reg_m);
            break;
        case 2: /* EOR */
            la_xor(reg_d, reg_n, reg_m);
            break;
        case 4: /* BIC */
        case 7: /* BICS */
            if(opc==3){
                la_nor(temp, zero_ir2_opnd, reg_m);
                if(sf){
                    la_x86and_d(reg_n, temp);
                }else{
                    la_x86and_w(reg_n, temp);
                }
            }
            la_andn(reg_d, reg_n, reg_m);
            break;
        case 5: /* ORN */
            la_orn(reg_d, reg_n, reg_m);
            break;
        case 6: /* EON */
            la_nor(temp, zero_ir2_opnd, reg_m);
            la_xor(reg_d, reg_n, temp);
            break;
        default:
            assert(FALSE);
            break;
        }
    }

    if (rd != 31 && arm_la_map[rd] >= 0 && clearGprHigh) {
        set_w_write_flag(rd, sf);
    } else {
        if (!sf) {
            la_bstrpick_d(reg_d, reg_d, 31, 0);
        }
    }

    store_gpr_dst(rd, reg_d);
    free_alloc_gpr(reg_d);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(reg_m);
    free_alloc_gpr(temp);
}

static void gen_add_sub_reg_result( bool setflags, bool sub_op, bool sf, 
                                IR2_OPND *reg_d, IR2_OPND *reg_n, IR2_OPND *temp){

    if (setflags) {
        if (sub_op) {
            gen_sub_CC(reg_n, temp, sf);
        } else {
            gen_add_CC(reg_n, temp, sf);
        }
    }

    if (sub_op) {
        gen_sub(reg_d, reg_n, temp, sf);
    } else {
        gen_add(reg_d, reg_n, temp, sf);
    }
}
/*
 * Add/subtract (extended register)
 *
 *  31|30|29|28       24|23 22|21|20   16|15  13|12  10|9  5|4  0|
 * +--+--+--+-----------+-----+--+-------+------+------+----+----+
 * |sf|op| S| 0 1 0 1 1 | opt | 1|  Rm   |option| imm3 | Rn | Rd |
 * +--+--+--+-----------+-----+--+-------+------+------+----+----+
 *
 *  sf: 0 -> 32bit, 1 -> 64bit
 *  op: 0 -> add  , 1 -> sub
 *   S: 1 -> set flags
 * opt: 00
 * option: extension type (see DecodeRegExtend)
 * imm3: optional shift to Rm
 *
 * Rd = Rn + LSL(extend(Rm), amount)
 */
static void disas_add_sub_ext_reg(DisasContext *s, uint32_t insn)
{
    int rd = extract32(insn, 0, 5);
    int rn = extract32(insn, 5, 5);
    int imm3 = extract32(insn, 10, 3);
    int option = extract32(insn, 13, 3);
    int rm = extract32(insn, 16, 5);
    int opt = extract32(insn, 22, 2);
    bool setflags = extract32(insn, 29, 1);
    bool sub_op = extract32(insn, 30, 1);
    bool sf = extract32(insn, 31, 1);
    int extsize = extract32(option, 0, 2);
    bool is_signed = extract32(option, 2, 1);
    if (imm3 > 4 || opt != 0) {
        lata_unallocated_encoding(s);
        return;
    }

    IR2_OPND reg_d;
    IR2_OPND reg_n = alloc_gpr_src_sp(rn);
    IR2_OPND reg_m = alloc_gpr_src(rm);
    IR2_OPND temp = ra_alloc_itemp();

    if (clearGprHigh && sf && arm_la_map[rn] >= 0) {
        clear_gpr_high(rn);
    }
    if (clearGprHigh && sf && arm_la_map[rm] >= 0 && rm != 31) {
        clear_gpr_high(rm);
    }

    /* non-flag setting ops may use SP */
    if (!setflags) {
        reg_d = alloc_gpr_dst_sp(rd);
    } else {
        reg_d = alloc_gpr_dst(rd);
    }

    if (is_signed) {
        switch (extsize) {
        case 0:
            la_ext_w_b(temp, reg_m);
            break;
        case 1:
            la_ext_w_h(temp, reg_m);
            break;
        case 2:
            la_bstrpick_w(temp,reg_m,31,0);
            break;
        case 3:
            // la_or(*dst,*src,zero_ir2_opnd);
            break;
        }
    } else {
        switch (extsize) {
        case 0:
            la_bstrpick_d(temp,reg_m,7,0);
            break;
        case 1:
            la_bstrpick_d(temp,reg_m,15,0);
            break;
        case 2:
            la_bstrpick_d(temp,reg_m,31,0);
            break;
        case 3:
            // la_or(*dst,*src,zero_ir2_opnd);
            break;
        }
    }

    if (imm3) {
        if(extsize == 3)
            la_slli_d(temp,reg_m,imm3);
        else
            la_slli_d(temp,temp,imm3);        
        gen_add_sub_reg_result(setflags, sub_op, sf, &reg_d, &reg_n, &temp);
    }else{
        if(extsize == 3)
            gen_add_sub_reg_result(setflags, sub_op, sf, &reg_d, &reg_n, &reg_m);
        else
            gen_add_sub_reg_result(setflags, sub_op, sf, &reg_d, &reg_n, &temp);
    }


    if ((rd != 31 || !setflags) && arm_la_map[rd] >= 0 && clearGprHigh) {
        set_w_write_flag(rd, sf);
    } else {
        if (!sf) {
            la_bstrpick_d(reg_d, reg_d, 31, 0);
        }
    }

    store_gpr_dst(rd, reg_d);
    free_alloc_gpr(reg_d);
    free_alloc_gpr(reg_m);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(temp);
}

/*
 * Add/subtract (shifted register)
 *
 *  31 30 29 28       24 23 22 21 20   16 15     10 9    5 4    0
 * +--+--+--+-----------+-----+--+-------+---------+------+------+
 * |sf|op| S| 0 1 0 1 1 |shift| 0|  Rm   |  imm6   |  Rn  |  Rd  |
 * +--+--+--+-----------+-----+--+-------+---------+------+------+
 *
 *    sf: 0 -> 32bit, 1 -> 64bit
 *    op: 0 -> add  , 1 -> sub
 *     S: 1 -> set flags
 * shift: 00 -> LSL, 01 -> LSR, 10 -> ASR, 11 -> RESERVED
 *  imm6: Shift amount to apply to Rm before the add/sub
 */
static void disas_add_sub_reg(DisasContext *s, uint32_t insn)
{
    int rd = extract32(insn, 0, 5);
    int rn = extract32(insn, 5, 5);
    int imm6 = extract32(insn, 10, 6);
    int rm = extract32(insn, 16, 5);
    int shift_type = extract32(insn, 22, 2);
    bool setflags = extract32(insn, 29, 1);
    bool sub_op = extract32(insn, 30, 1);
    bool sf = extract32(insn, 31, 1);

    if ((shift_type == 3) || (!sf && (imm6 > 31))) {
        lata_unallocated_encoding(s);
        return;
    }

    IR2_OPND reg_d = alloc_gpr_dst(rd);
    IR2_OPND reg_n = alloc_gpr_src(rn);
    IR2_OPND reg_m = alloc_gpr_src(rm);

    if (clearGprHigh && sf && arm_la_map[rn] >= 0 && rn != 31) {
        clear_gpr_high(rn);
    }
    if (clearGprHigh && sf && arm_la_map[rm] >= 0 && rm != 31) {
        clear_gpr_high(rm);
    }

    IR2_OPND temp = ra_alloc_itemp();
    if(imm6){
        shift_reg_imm(&temp, &reg_m, sf, shift_type, imm6);
        gen_add_sub_reg_result(setflags, sub_op, sf, &reg_d, &reg_n, &temp);
    }else{

        gen_add_sub_reg_result(setflags, sub_op, sf, &reg_d, &reg_n, &reg_m);
    }

    if (rd != 31 && arm_la_map[rd] >= 0 && clearGprHigh) {
        set_w_write_flag(rd, sf);
    } else {
        if (!sf) {
            la_bstrpick_d(reg_d, reg_d, 31, 0);
        }
    }

    store_gpr_dst(rd, reg_d);
    free_alloc_gpr(reg_d);
    free_alloc_gpr(reg_m);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(temp);
}

/* Data-processing (3 source)
 *
 *    31 30  29 28       24 23 21  20  16  15  14  10 9    5 4    0
 *  +--+------+-----------+------+------+----+------+------+------+
 *  |sf| op54 | 1 1 0 1 1 | op31 |  Rm  | o0 |  Ra  |  Rn  |  Rd  |
 *  +--+------+-----------+------+------+----+------+------+------+
 */
static void disas_data_proc_3src(DisasContext *s, uint32_t insn)
{
    int rd = extract32(insn, 0, 5);
    int rn = extract32(insn, 5, 5);
    int ra = extract32(insn, 10, 5);
    int rm = extract32(insn, 16, 5);
    int op_id = (extract32(insn, 29, 3) << 4) |
        (extract32(insn, 21, 3) << 1) |
        extract32(insn, 15, 1);
    bool sf = extract32(insn, 31, 1);
    bool is_sub = extract32(op_id, 0, 1);
    // bool is_high = extract32(op_id, 2, 1);
    // bool is_signed = false;

    IR2_OPND reg_d = alloc_gpr_dst(rd);
    IR2_OPND reg_n = alloc_gpr_src(rn);
    IR2_OPND reg_a, temp;
    IR2_OPND reg_m = alloc_gpr_src(rm);

    if (clearGprHigh && sf && arm_la_map[rn] >= 0 && rn != 31) {
        clear_gpr_high(rn);
    }
    if (clearGprHigh && sf && arm_la_map[rm] >= 0 && rm != 31) {
        clear_gpr_high(rm);
    }

    /* Note that op_id is sf:op54:op31:o0 so it includes the 32/64 size flag */
    switch (op_id) {
    case 0x42: /* SMADDL */
    case 0x43: /* SMSUBL */
        /* 
            SMADDL <Xd>, <Wn>, <Wm>, <Xa> 
            SMSUBL <Xd>, <Wn>, <Wm>, <Xa>
        */
        if (ra == 31 && !is_sub) {
        /* Special-case MADD with rA == XZR; it is the standard MUL alias */
            la_mulw_d_w(reg_d, reg_n, reg_m);
        }else{
            reg_a = alloc_gpr_src(ra);
            if (clearGprHigh && arm_la_map[ra] >= 0 && ra != 31) {
                clear_gpr_high(ra);
            }

            temp = ra_alloc_itemp();
            la_mulw_d_w(temp, reg_m, reg_n);
            if(is_sub){
                la_sub_d(reg_d, reg_a, temp);
            }else {
                la_add_d(reg_d, reg_a, temp);
            }

            free_alloc_gpr(reg_a);
            free_alloc_gpr(temp);
        }
        break;
    case 0x44: /* SMULH */
        la_mulh_d(reg_d, reg_n, reg_m);
        break;
    case 0x0: /* MADD (32bit) */
    case 0x1: /* MSUB (32bit) */
    case 0x40: /* MADD (64bit) */
    case 0x41: /* MSUB (64bit) */
        if (ra == 31 && !is_sub) {
        /* Special-case MADD with rA == XZR; it is the standard MUL alias */
            if(sf){
                la_mul_d(reg_d, reg_n, reg_m);
            }else {
                la_mul_w(reg_d, reg_n, reg_m);
            }
        }else{
            reg_a = alloc_gpr_src(ra);
            if (clearGprHigh && arm_la_map[ra] >= 0 && ra != 31) {
                clear_gpr_high(ra);
            }

            temp = ra_alloc_itemp();
            if(sf){
                la_mul_d(temp, reg_n, reg_m);
            }else {
                la_mul_w(temp, reg_n, reg_m);
            }
            if(is_sub){
                if(sf){
                    la_sub_d(reg_d, reg_a, temp);
                }else {
                    la_sub_w(reg_d, reg_a, temp);
                }
            }else {
                if(sf){
                    la_add_d(reg_d, reg_a, temp);
                }else {
                    la_add_w(reg_d, reg_a, temp);
                }
            }

            free_alloc_gpr(reg_a);
            free_alloc_gpr(temp);
        }
        break;
    case 0x4a: /* UMADDL */
    case 0x4b: /* UMSUBL */
        if (ra == 31 && !is_sub) {
        /* Special-case MADD with rA == XZR; it is the standard MUL alias */
            la_mulw_d_wu(reg_d, reg_n, reg_m);
        }else{
            reg_a = alloc_gpr_src(ra);
            if (clearGprHigh && arm_la_map[ra] >= 0 && ra != 31) {
                clear_gpr_high(ra);
            }

            temp = ra_alloc_itemp();
            la_mulw_d_wu(temp, reg_n, reg_m);
            if(is_sub){
                la_sub_d(reg_d, reg_a, temp);
            }else {
                la_add_d(reg_d, reg_a, temp);
            }

            free_alloc_gpr(reg_a);
            free_alloc_gpr(temp);
        }
        break;
    case 0x4c: /* UMULH */
        la_mulh_du(reg_d, reg_n, reg_m);
        break;
    default:
        lata_unallocated_encoding(s);
        return;
    }

    if (rd != 31 && arm_la_map[rd] >= 0 && clearGprHigh) {
        set_w_write_flag(rd, sf);
    } else {
        if (!sf) {
            la_bstrpick_d(reg_d, reg_d, 31, 0);
        }
    }

    store_gpr_dst(rd, reg_d);
    free_alloc_gpr(reg_d);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(reg_m);
}

/* Add/subtract (with carry)
 *  31 30 29 28 27 26 25 24 23 22 21  20  16  15       10  9    5 4   0
 * +--+--+--+------------------------+------+-------------+------+-----+
 * |sf|op| S| 1  1  0  1  0  0  0  0 |  rm  | 0 0 0 0 0 0 |  Rn  |  Rd |
 * +--+--+--+------------------------+------+-------------+------+-----+
 */

static void disas_adc_sbc(DisasContext *s, uint32_t insn)
{
    unsigned int sf, op, setflags, rm, rn, rd;

    sf = extract32(insn, 31, 1);
    op = extract32(insn, 30, 1);
    setflags = extract32(insn, 29, 1);
    rm = extract32(insn, 16, 5);
    rn = extract32(insn, 5, 5);
    rd = extract32(insn, 0, 5);

    IR2_OPND reg_n = alloc_gpr_src(rn);
    IR2_OPND reg_m = alloc_gpr_src(rm);
    IR2_OPND reg_d = alloc_gpr_dst(rd);
    IR2_OPND temp = ra_alloc_itemp();
    if (clearGprHigh && sf && arm_la_map[rn] >= 0 && rn != 31) {
        clear_gpr_high(rn);
    }
    if (clearGprHigh && sf && arm_la_map[rm] >= 0 && rm != 31) {
        clear_gpr_high(rm);
    }
    
    if (op) {
        la_orn(temp, zero_ir2_opnd, reg_m);
        if (setflags) {
            if (sf) {
                la_x86adc_d(reg_n, temp);
            } else {
                la_x86adc_w(reg_n, temp);
            }
        }
        if (sf) {
                la_adc_d(reg_d, reg_n, temp);
        } else {
                la_adc_w(reg_d, reg_n, temp);
        }
    } else {
        if (setflags) {
            if (sf) {
                la_x86adc_d(reg_n, reg_m);
            } else {
                la_x86adc_w(reg_n, reg_m);
            }
        }
        if (sf) {
                la_adc_d(reg_d, reg_n, reg_m);
        } else {
                la_adc_w(reg_d, reg_n, reg_m);
        }
    }

    if (rd != 31 && arm_la_map[rd] >= 0 && clearGprHigh) {
        set_w_write_flag(rd, sf);
    } else {
        if (!sf) {
            la_bstrpick_d(reg_d, reg_d, 31, 0);
        }
    }

    free_alloc_gpr(reg_d);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(reg_m);
    free_alloc_gpr(temp);
    store_gpr_dst(rd, reg_d);
}

/*
 * Rotate right into flags
 *  31 30 29                21       15          10      5  4      0
 * +--+--+--+-----------------+--------+-----------+------+--+------+
 * |sf|op| S| 1 1 0 1 0 0 0 0 |  imm6  | 0 0 0 0 1 |  Rn  |o2| mask |
 * +--+--+--+-----------------+--------+-----------+------+--+------+
 */
static void disas_rotate_right_into_flags(DisasContext *s, uint32_t insn)
{
    assert(0);
}

/*
 * Evaluate into flags
 *  31 30 29                21        15   14        10      5  4      0
 * +--+--+--+-----------------+---------+----+---------+------+--+------+
 * |sf|op| S| 1 1 0 1 0 0 0 0 | opcode2 | sz | 0 0 1 0 |  Rn  |o3| mask |
 * +--+--+--+-----------------+---------+----+---------+------+--+------+
 */
static void disas_evaluate_into_flags(DisasContext *s, uint32_t insn)
{
    assert(0);
}

/* Conditional compare (immediate / register)
 *  31 30 29 28 27 26 25 24 23 22 21  20    16 15  12  11  10  9   5  4 3   0
 * +--+--+--+------------------------+--------+------+----+--+------+--+-----+
 * |sf|op| S| 1  1  0  1  0  0  1  0 |imm5/rm | cond |i/r |o2|  Rn  |o3|nzcv |
 * +--+--+--+------------------------+--------+------+----+--+------+--+-----+
 *        [1]                             y                [0]       [0]
 */
static void disas_cc(DisasContext *s, uint32_t insn)
{
    unsigned int sf, op, y, cond, rn, nzcv, is_imm;

    if (!extract32(insn, 29, 1)) {
        lata_unallocated_encoding(s);
        return;
    }
    if (insn & (1 << 10 | 1 << 4)) {
        lata_unallocated_encoding(s);
        return;
    }
    sf = extract32(insn, 31, 1);
    op = extract32(insn, 30, 1);
    is_imm = extract32(insn, 11, 1);
    y = extract32(insn, 16, 5); /* y = rm (reg) or imm5 (imm) */
    cond = extract32(insn, 12, 4);
    rn = extract32(insn, 5, 5);
    nzcv = extract32(insn, 0, 4);

    IR2_OPND reg_n = alloc_gpr_src(rn);
    IR2_OPND reg_m; 
    IR2_OPND temp = ra_alloc_itemp();
    IR2_OPND label_false = ir2_opnd_new_type(IR2_OPND_LABEL);
    IR2_OPND label_end = ir2_opnd_new_type(IR2_OPND_LABEL);
    if (clearGprHigh && sf && arm_la_map[rn] >= 0 && rn != 31) {
        clear_gpr_high(rn);
    }

    la_setarmj(temp, cond);
    la_beqz(temp, label_false);

    /* TRUE */
    if (is_imm) {
        if(y){
            reg_m = ra_alloc_itemp();
            la_addi_d(reg_m, zero_ir2_opnd, y);
        }else{
            reg_m = zero_ir2_opnd;
        }
    } else {
        reg_m = alloc_gpr_src(y);
        if (clearGprHigh && sf && arm_la_map[y] >= 0 && y != 31) {
           clear_gpr_high(y);
        }
    }

    if(op){ // CCMP
        gen_sub_CC(&reg_n, &reg_m, sf);
    }else{ // CCMN
        gen_add_CC(&reg_n, &reg_m, sf);
    }
    la_b(label_end);

    la_label(label_false); 
    /* FALSE */
    la_addi_w(temp, zero_ir2_opnd, nzcv);
    la_slli_w(temp, temp, 28);
    la_armmtflag(temp, 0x39);

    la_label(label_end);

    free_alloc_gpr(reg_m);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(temp);
}

/* Conditional select
 *   31   30  29  28             21 20  16 15  12 11 10 9    5 4    0
 * +----+----+---+-----------------+------+------+-----+------+------+
 * | sf | op | S | 1 1 0 1 0 1 0 0 |  Rm  | cond | op2 |  Rn  |  Rd  |
 * +----+----+---+-----------------+------+------+-----+------+------+
 */
static void disas_cond_select(DisasContext *s, uint32_t insn)
{
    unsigned int sf, else_inv, rm, cond, else_inc, rn, rd;

    if (extract32(insn, 29, 1) || extract32(insn, 11, 1)) {
        /* S == 1 or op2<1> == 1 */
        lata_unallocated_encoding(s);
        return;
    }
    sf = extract32(insn, 31, 1);
    else_inv = extract32(insn, 30, 1);
    rm = extract32(insn, 16, 5);
    cond = extract32(insn, 12, 4);
    else_inc = extract32(insn, 10, 1);
    rn = extract32(insn, 5, 5);
    rd = extract32(insn, 0, 5);

    /*
    CSEL                          00
    CSNEG CNEG                    11
    CSINC CINC CSET(rm==rn==xzr)  01
    CSINV CINV CSETM(rm==rn==xzr) 10
    */
    IR2_OPND reg_d = alloc_gpr_dst(rd);

    if (rn == 31 && rm == 31 && (else_inc ^ else_inv)) {
        /* CSET & CSETM.  */
        la_addi_d(reg_d, zero_ir2_opnd, 1);
        la_armmove(reg_d, zero_ir2_opnd, cond); // CSET: reg_d = cond ? 0 : 1
        if (else_inv) { // CSETM
            // tcg_gen_neg_i64(tcg_rd, tcg_rd); 取负
            la_orn(reg_d, zero_ir2_opnd, reg_d);
            la_addi_d(reg_d, reg_d, 1);
        }
    } else {
        IR2_OPND reg_n = alloc_gpr_src(rn);
        IR2_OPND reg_m = alloc_gpr_src(rm);
        IR2_OPND temp_m = ra_alloc_itemp();
        IR2_OPND temp_n = ra_alloc_itemp();

        if (clearGprHigh && sf && arm_la_map[rn] >= 0 && rn != 31) {
            clear_gpr_high(rn);
        }
        if (clearGprHigh && sf && arm_la_map[rm] >= 0 && rm != 31) {
            clear_gpr_high(rm);
        }

        if (else_inv && else_inc) { // CSNEG, CNEG(FALSE)
            la_orn(temp_m, zero_ir2_opnd, reg_m);
            la_addi_d(temp_m, temp_m, 1);
        } else if (else_inv) { // CSINV, CINV(FALSE)
            la_orn(temp_m, zero_ir2_opnd, reg_m);
        } else if (else_inc) { // CSINC, CINC(FALSE)
            la_addi_d(temp_m, reg_m, 1);
        }else{ // CSEL(FALSE)
            la_or(temp_m, zero_ir2_opnd, reg_m);
        }

        if(rd == rn){
            la_or(temp_n, zero_ir2_opnd, reg_n);
        }

        la_or(reg_d, zero_ir2_opnd, temp_m);

        /* CSNEG, CNEG, CSINV, CINV, CSINC, CINC, CSEL(TRUE)*/
        if(rd == rn){
            la_armmove(reg_d, temp_n, cond); 
        }else {
            la_armmove(reg_d, reg_n, cond);
        }

        free_alloc_gpr(reg_n);
        free_alloc_gpr(reg_m);
        free_alloc_gpr(temp_n);
        free_alloc_gpr(temp_m);
    }

    if (rd != 31 && arm_la_map[rd] >= 0 && clearGprHigh) {
        set_w_write_flag(rd, sf);
    } else {
        if (!sf) {
            la_bstrpick_d(reg_d, reg_d, 31, 0);
        }
    }

    store_gpr_dst(rd, reg_d);
    free_alloc_gpr(reg_d);
}

static void handle_clz(DisasContext *s, unsigned int sf,
                       unsigned int rn, unsigned int rd)
{
    IR2_OPND reg_d = alloc_gpr_dst(rd);
    IR2_OPND reg_n = alloc_gpr_src(rn);
    if (clearGprHigh && sf && arm_la_map[rn] >= 0 && rn != 31) {
        clear_gpr_high(rn);
    }

    if (sf) {
        la_clz_d(reg_d, reg_n);
    } else {
        la_clz_w(reg_d, reg_n);
    }

    if (arm_la_map[rd] >= 0 && rd != 31 && clearGprHigh) {
        set_w_write_flag(rd, TRUE);
    }

    store_gpr_dst(rd, reg_d);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(reg_d);
}

static void handle_cls(DisasContext *s, unsigned int sf,
                       unsigned int rn, unsigned int rd)
{
    IR2_OPND reg_d = alloc_gpr_dst(rd);
    IR2_OPND reg_n = alloc_gpr_src(rn);
    IR2_OPND zero_label = ir2_opnd_new_type(IR2_OPND_LABEL);
    IR2_OPND exit_label = ir2_opnd_new_type(IR2_OPND_LABEL);
    if (clearGprHigh && sf && arm_la_map[rn] >= 0 && rn != 31) {
        clear_gpr_high(rn);
    }

    if (sf) {
        la_bstrpick_d(reg_d, reg_n, 63, 63);
        la_beqz(reg_d, zero_label);
        la_clo_d(reg_d, reg_n);
        la_b(exit_label);
        la_label(zero_label);
        la_clz_d(reg_d, reg_n);
        la_label(exit_label);
    } else {
        la_bstrpick_d(reg_d, reg_n, 31, 31);
        la_beqz(reg_d, zero_label);
        la_clo_w(reg_d, reg_n);
        la_b(exit_label);
        la_label(zero_label);
        la_clz_w(reg_d, reg_n);
        la_label(exit_label);
    }
    la_addi_d(reg_d, reg_d, -1);
    
    if (arm_la_map[rd] >= 0 && rd != 31 && clearGprHigh) {
        set_w_write_flag(rd, TRUE);
    }

    store_gpr_dst(rd, reg_d);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(reg_d);
}

static void handle_rbit(DisasContext *s, unsigned int sf,
                        unsigned int rn, unsigned int rd)
{
    IR2_OPND reg_d = alloc_gpr_dst(rd);
    IR2_OPND reg_n = alloc_gpr_src(rn);
    if (clearGprHigh && sf && arm_la_map[rn] >= 0 && rn != 31) {
        clear_gpr_high(rn);
    }

    if (sf) {
        la_bitrev_d(reg_d, reg_n);
    } else {
        la_bitrev_w(reg_d, reg_n);
    }

    if (rd != 31 && arm_la_map[rd] >= 0 && clearGprHigh) {
        set_w_write_flag(rd, sf);
    } else {
        if (!sf) {
            la_bstrpick_d(reg_d, reg_d, 31, 0);
        }
    }
    store_gpr_dst(rd, reg_d);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(reg_d);
}

/* REV with sf==1, opcode==3 ("REV64") */
static void handle_rev64(DisasContext *s, unsigned int sf,
                         unsigned int rn, unsigned int rd)
{
    if (!sf) {
        lata_unallocated_encoding(s);
        return;
    }

    IR2_OPND reg_d = alloc_gpr_dst(rd);
    IR2_OPND reg_n = alloc_gpr_src(rn);
    if (clearGprHigh && sf && arm_la_map[rn] >= 0 && rn != 31) {
        clear_gpr_high(rn);
    }

    la_revb_d(reg_d, reg_n);

    if (arm_la_map[rd] >= 0 && rd != 31 && clearGprHigh) {
        set_w_write_flag(rd, TRUE);
    }

    store_gpr_dst(rd, reg_d);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(reg_d);
}

/* REV with sf==0, opcode==2
 * REV32 (sf==1, opcode==2)
 */
static void handle_rev32(DisasContext *s, unsigned int sf,
                         unsigned int rn, unsigned int rd)
{
    IR2_OPND reg_d = alloc_gpr_dst(rd);
    IR2_OPND reg_n = alloc_gpr_src(rn);
    if (clearGprHigh && sf && arm_la_map[rn] >= 0 && rn != 31) {
        clear_gpr_high(rn);
    }

    la_revb_2w(reg_d, reg_n);

    if (rd != 31 && arm_la_map[rd] >= 0 && clearGprHigh) {
        set_w_write_flag(rd, sf);
    } else {
        if (!sf) {
            la_bstrpick_d(reg_d, reg_d, 31, 0);
        }
    }

    store_gpr_dst(rd, reg_d);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(reg_d);
}

/* REV16 (opcode==1) */
static void handle_rev16(DisasContext *s, unsigned int sf,
                         unsigned int rn, unsigned int rd)
{
    IR2_OPND reg_d = alloc_gpr_dst(rd);
    IR2_OPND reg_n = alloc_gpr_src(rn);
    if (clearGprHigh && sf && arm_la_map[rn] >= 0 && rn != 31) {
        clear_gpr_high(rn);
    }

    if (sf) {
        la_revb_4h(reg_d, reg_n);
    }else{
        la_revb_2h(reg_d, reg_n);
    }

    if (rd != 31 && arm_la_map[rd] >= 0 && clearGprHigh) {
        set_w_write_flag(rd, sf);
    } else {
        if (!sf) {
            la_bstrpick_d(reg_d, reg_d, 31, 0);
        }
    }

    store_gpr_dst(rd, reg_d);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(reg_d);
}

/* Data-processing (1 source)
 *   31  30  29  28             21 20     16 15    10 9    5 4    0
 * +----+---+---+-----------------+---------+--------+------+------+
 * | sf | 1 | S | 1 1 0 1 0 1 1 0 | opcode2 | opcode |  Rn  |  Rd  |
 * +----+---+---+-----------------+---------+--------+------+------+
 */
static void disas_data_proc_1src(DisasContext *s, uint32_t insn)
{
    unsigned int sf, opcode, opcode2, rn, rd;

    if (extract32(insn, 29, 1)) {
        lata_unallocated_encoding(s);
        return;
    }

    sf = extract32(insn, 31, 1);
    opcode = extract32(insn, 10, 6);
    opcode2 = extract32(insn, 16, 5);
    rn = extract32(insn, 5, 5);
    rd = extract32(insn, 0, 5);

#define MAP(SF, O2, O1) ((SF) | (O1 << 1) | (O2 << 7))

    switch (MAP(sf, opcode2, opcode)) {
    case MAP(0, 0x00, 0x00): /* RBIT */
    case MAP(1, 0x00, 0x00):
        handle_rbit(s, sf, rn, rd);
        break;
    case MAP(0, 0x00, 0x01): /* REV16 */
    case MAP(1, 0x00, 0x01):
        handle_rev16(s, sf, rn, rd);
        break;
    case MAP(0, 0x00, 0x02): /* REV/REV32 */
    case MAP(1, 0x00, 0x02):
        handle_rev32(s, sf, rn, rd);
        break;
    case MAP(1, 0x00, 0x03): /* REV64 */
        handle_rev64(s, sf, rn, rd);
        break;
    case MAP(0, 0x00, 0x04): /* CLZ */
    case MAP(1, 0x00, 0x04):
        handle_clz(s, sf, rn, rd);
        break;
    case MAP(0, 0x00, 0x05): /* CLS */
    case MAP(1, 0x00, 0x05):
        handle_cls(s, sf, rn, rd);
        break;
    case MAP(1, 0x01, 0x00): /* PACIA */
    case MAP(1, 0x01, 0x01): /* PACIB */
    case MAP(1, 0x01, 0x02): /* PACDA */
    case MAP(1, 0x01, 0x03): /* PACDB */
    case MAP(1, 0x01, 0x04): /* AUTIA */
    case MAP(1, 0x01, 0x05): /* AUTIB */
    case MAP(1, 0x01, 0x06): /* AUTDA */
    case MAP(1, 0x01, 0x07): /* AUTDB */
    case MAP(1, 0x01, 0x08): /* PACIZA */
    case MAP(1, 0x01, 0x09): /* PACIZB */
    case MAP(1, 0x01, 0x0a): /* PACDZA */
    case MAP(1, 0x01, 0x0b): /* PACDZB */
    case MAP(1, 0x01, 0x0c): /* AUTIZA */
    case MAP(1, 0x01, 0x0d): /* AUTIZB */
    case MAP(1, 0x01, 0x0e): /* AUTDZA */
    case MAP(1, 0x01, 0x0f): /* AUTDZB */
    case MAP(1, 0x01, 0x10): /* XPACI */
    case MAP(1, 0x01, 0x11): /* XPACD */
        assert(0);
        break;
    default:
        lata_unallocated_encoding(s);
        break;
    }

#undef MAP
}

static void handle_div(DisasContext *s, bool is_signed, unsigned int sf,
                       unsigned int rm, unsigned int rn, unsigned int rd)
{
    IR2_OPND reg_d = alloc_gpr_dst(rd);
    IR2_OPND reg_n = alloc_gpr_src(rn);
    IR2_OPND reg_m = alloc_gpr_src(rm);
    IR2_OPND temp_m = ra_alloc_itemp();
    IR2_OPND temp_n = ra_alloc_itemp();
    if (clearGprHigh && sf && arm_la_map[rn] >= 0 && rn != 31) {
        clear_gpr_high(rn);
    }
    if (clearGprHigh && sf && arm_la_map[rm] >= 0 && rm != 31) {
        clear_gpr_high(rm);
    }

    if (is_signed) {
        if(sf){
            la_div_d(reg_d, reg_n, reg_m);
        }else{
            la_bstrpick_w(temp_m, reg_m, 31, 0);
            la_bstrpick_w(temp_n, reg_n, 31, 0);
            la_div_d(reg_d, temp_n, temp_m);
        }
    } else {
        la_div_du(reg_d, reg_n, reg_m);
    }
    
    if (rd != 31 && arm_la_map[rd] >= 0 && clearGprHigh) {
        set_w_write_flag(rd, sf);
    } else {
        if (!sf) {
            la_bstrpick_d(reg_d, reg_d, 31, 0);
        }
    }

    store_gpr_dst(rd, reg_d);
    free_alloc_gpr(reg_d);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(reg_m);
    free_alloc_gpr(temp_m);
    free_alloc_gpr(temp_n);
}

/* LSLV, LSRV, ASRV, RORV */
static void handle_shift_reg(DisasContext *s,
                             enum a64_shift_type shift_type, unsigned int sf,
                             unsigned int rm, unsigned int rn, unsigned int rd)
{
    IR2_OPND reg_d = alloc_gpr_dst(rd);
    IR2_OPND reg_n = alloc_gpr_src(rn);
    IR2_OPND reg_m = alloc_gpr_src(rm);
    if (clearGprHigh && sf && arm_la_map[rn] >= 0 && rn != 31) {
        clear_gpr_high(rn);
    }
    if (clearGprHigh && sf && arm_la_map[rm] >= 0 && rm != 31) {
        clear_gpr_high(rm);
    }

    switch (shift_type) {
    case A64_SHIFT_TYPE_LSL:
        if(sf){
            la_sll_d(reg_d, reg_n, reg_m);
        }else{
            la_sll_w(reg_d, reg_n, reg_m);
        }
        break;
    case A64_SHIFT_TYPE_LSR:
        if(sf){
            la_srl_d(reg_d, reg_n, reg_m);
        }else{
            la_srl_w(reg_d, reg_n, reg_m);
        }
        break;
    case A64_SHIFT_TYPE_ASR:
        if(sf){
            la_sra_d(reg_d, reg_n, reg_m);
        }else{
            la_sra_w(reg_d, reg_n, reg_m);
        }
        break;
    case A64_SHIFT_TYPE_ROR:
        if(sf){
            la_rotr_d(reg_d, reg_n, reg_m);
        }else{
            la_rotr_w(reg_d, reg_n, reg_m);
        }
        break;
    default:
        assert(FALSE); /* all shift types should be handled */
        break;
    }

    if (rd != 31 && arm_la_map[rd] >= 0 && clearGprHigh) {
        set_w_write_flag(rd, sf);
    } else {
        if (!sf) {
            la_bstrpick_d(reg_d, reg_d, 31, 0);
        }
    }

    store_gpr_dst(rd, reg_d);
    free_alloc_gpr(reg_d);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(reg_m);
}

/* Data-processing (2 source)
 *   31   30  29 28             21 20  16 15    10 9    5 4    0
 * +----+---+---+-----------------+------+--------+------+------+
 * | sf | 0 | S | 1 1 0 1 0 1 1 0 |  Rm  | opcode |  Rn  |  Rd  |
 * +----+---+---+-----------------+------+--------+------+------+
 */
static void disas_data_proc_2src(DisasContext *s, uint32_t insn)
{
    unsigned int sf, rm, opcode, rn, rd, setflag;
    sf = extract32(insn, 31, 1);
    setflag = extract32(insn, 29, 1);
    rm = extract32(insn, 16, 5);
    opcode = extract32(insn, 10, 6);
    rn = extract32(insn, 5, 5);
    rd = extract32(insn, 0, 5);

    if (setflag && opcode != 0) {
        lata_unallocated_encoding(s);
        return;
    }

    switch (opcode) {
    case 0: /* SUBP(S) */
        if (sf == 0 || !dc_isar_feature(aa64_mte_insn_reg, s)) {
            goto do_unallocated;
        } else {
            assert(0);
        }
        break;
    case 2: /* UDIV */
        handle_div(s, false, sf, rm, rn, rd);
        break;
    case 3: /* SDIV */
        handle_div(s, true, sf, rm, rn, rd);
        break;
    case 4: /* IRG */
        assert(0);
        break;
    case 5: /* GMI */
        assert(0);
        break;
    case 8: /* LSLV */
        handle_shift_reg(s, A64_SHIFT_TYPE_LSL, sf, rm, rn, rd);
        break;
    case 9: /* LSRV */
        handle_shift_reg(s, A64_SHIFT_TYPE_LSR, sf, rm, rn, rd);
        break;
    case 10: /* ASRV */
        handle_shift_reg(s, A64_SHIFT_TYPE_ASR, sf, rm, rn, rd);
        break;
    case 11: /* RORV */
        assert(0);
        handle_shift_reg(s, A64_SHIFT_TYPE_ROR, sf, rm, rn, rd);
        break;
    case 12: /* PACGA */
        assert(0);
        break;
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23: /* CRC32 */
    {
        assert(0);
        break;
    }
    default:
    do_unallocated:
        lata_unallocated_encoding(s);
        break;
    }
}

/*
 * Data processing - register
 *  31  30 29  28      25    21  20  16      10         0
 * +--+---+--+---+-------+-----+-------+-------+---------+
 * |  |op0|  |op1| 1 0 1 | op2 |       |  op3  |         |
 * +--+---+--+---+-------+-----+-------+-------+---------+
 */
static bool disas_data_proc_reg(DisasContext *s)
{
    uint32_t insn = s->insn;
    int op0 = extract32(insn, 30, 1);
    int op1 = extract32(insn, 28, 1);
    int op2 = extract32(insn, 21, 4);
    int op3 = extract32(insn, 10, 6);

    if (!op1) {
        if (op2 & 8) {
            if (op2 & 1) {
                /* Add/sub (extended register) */
                disas_add_sub_ext_reg(s, insn);
            } else {
                /* Add/sub (shifted register) */
                disas_add_sub_reg(s, insn);
            }
        } else {
            /* Logical (shifted register) */
            disas_logic_reg(s, insn);
        }
        return true;
    }

    switch (op2) {
    case 0x0:
        switch (op3) {
        case 0x00: /* Add/subtract (with carry) */
            disas_adc_sbc(s, insn);
            break;

        case 0x01: /* Rotate right into flags */
        case 0x21:
            assert(0);
            disas_rotate_right_into_flags(s, insn);
            break;

        case 0x02: /* Evaluate into flags */
        case 0x12:
        case 0x22:
        case 0x32:
            assert(0);
            disas_evaluate_into_flags(s, insn);
            break;

        default:
            goto do_unallocated;
        }
        break;

    case 0x2: /* Conditional compare */
        disas_cc(s, insn); /* both imm and reg forms */
        break;

    case 0x4: /* Conditional select */
        disas_cond_select(s, insn);
        break;

    case 0x6: /* Data-processing */
        if (op0) {    /* (1 source) */
            disas_data_proc_1src(s, insn);
        } else {      /* (2 source) */
            disas_data_proc_2src(s, insn);
        }
        break;
    case 0x8 ... 0xf: /* (3 source) */
        disas_data_proc_3src(s, insn);
        break;

    default:
    do_unallocated:
        lata_unallocated_encoding(s);
        break;
    }

    return true;
}

static void handle_fp_compare(DisasContext *s, int size,
                              unsigned int rn, unsigned int rm,
                              bool cmp_with_zero, bool signal_all_nans)
{
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND vreg_m;
    IR2_OPND temp = ra_alloc_itemp();
    IR2_OPND l_equal = ir2_opnd_new_type(IR2_OPND_LABEL); 
    IR2_OPND l_less = ir2_opnd_new_type(IR2_OPND_LABEL); 
    IR2_OPND l_greater = ir2_opnd_new_type(IR2_OPND_LABEL); 
    IR2_OPND l_exit = ir2_opnd_new_type(IR2_OPND_LABEL); 

    if(cmp_with_zero){
        vreg_m = ra_alloc_ftemp();
        la_movgr2fr_d(vreg_m, zero_ir2_opnd);
    }else{
        vreg_m = alloc_fpr_src(rm);
    }

    /*  res     equal   less    greater     unordered
        NZCV    0110    1000    0010        0011
    */
    if (size == MO_64) {
        la_fcmp_cond_d(fcc0_ir2_opnd, vreg_n, vreg_m, 
                        signal_all_nans ? FCMP_COND_SEQ : FCMP_COND_CEQ);
        la_bcnez(fcc0_ir2_opnd, l_equal);
        la_fcmp_cond_d(fcc0_ir2_opnd, vreg_n, vreg_m, 
                        signal_all_nans ? FCMP_COND_SLT : FCMP_COND_CLT);
        la_bcnez(fcc0_ir2_opnd, l_less);
        la_fcmp_cond_d(fcc0_ir2_opnd, vreg_m, vreg_n, 
                        signal_all_nans ? FCMP_COND_SLT : FCMP_COND_CLT);
        la_bcnez(fcc0_ir2_opnd, l_greater);

        la_addi_d(temp, zero_ir2_opnd, 0b0011);
        la_b(l_exit);

        la_label(l_equal);
        la_addi_d(temp, zero_ir2_opnd, 0b0110);
        la_b(l_exit);

        la_label(l_less);
        la_addi_d(temp, zero_ir2_opnd, 0b1000);
        la_b(l_exit);

        la_label(l_greater);
        la_addi_d(temp, zero_ir2_opnd, 0b0010);

        la_label(l_exit);
        la_slli_d(temp, temp, 0x1c);/* 左移28位 */
        la_armmtflag(temp, 0x39);

    } else if(size == MO_32){
        la_fcmp_cond_s(fcc0_ir2_opnd, vreg_n, vreg_m, 
                        signal_all_nans ? FCMP_COND_SEQ : FCMP_COND_CEQ);
        la_bcnez(fcc0_ir2_opnd, l_equal);
        la_fcmp_cond_s(fcc0_ir2_opnd, vreg_n, vreg_m, 
                        signal_all_nans ? FCMP_COND_SLT : FCMP_COND_CLT);
        la_bcnez(fcc0_ir2_opnd, l_less);
        la_fcmp_cond_s(fcc0_ir2_opnd, vreg_m, vreg_n, 
                        signal_all_nans ? FCMP_COND_SLT : FCMP_COND_CLT);
        la_bcnez(fcc0_ir2_opnd, l_greater);

        la_addi_d(temp, zero_ir2_opnd, 0b0011);
        la_b(l_exit);

        la_label(l_equal);
        la_addi_d(temp, zero_ir2_opnd, 0b0110);
        la_b(l_exit);

        la_label(l_less);
        la_addi_d(temp, zero_ir2_opnd, 0b1000);
        la_b(l_exit);

        la_label(l_greater);
        la_addi_d(temp, zero_ir2_opnd, 0b0010);

        la_label(l_exit);
        la_slli_d(temp, temp, 0x1c);/* 左移28位 */
        la_armmtflag(temp, 0x39);
    }else{
        assert(0);
    }

    free_alloc_fpr(vreg_m);
    free_alloc_fpr(vreg_n);
    free_alloc_gpr(temp);
    // gen_set_nzcv(tcg_flags);
}

/* Floating point compare
 *   31  30  29 28       24 23  22  21 20  16 15 14 13  10    9    5 4     0
 * +---+---+---+-----------+------+---+------+-----+---------+------+-------+
 * | M | 0 | S | 1 1 1 1 0 | type | 1 |  Rm  | op  | 1 0 0 0 |  Rn  |  op2  |
 * +---+---+---+-----------+------+---+------+-----+---------+------+-------+
 */
static void disas_fp_compare(DisasContext *s, uint32_t insn)
{
    unsigned int mos, type, rm, op, rn, opc, op2r;
    int size;

    mos = extract32(insn, 29, 3);
    type = extract32(insn, 22, 2);
    rm = extract32(insn, 16, 5);
    op = extract32(insn, 14, 2);
    rn = extract32(insn, 5, 5);
    opc = extract32(insn, 3, 2);
    op2r = extract32(insn, 0, 3);

    if (mos || op || op2r) {
        lata_unallocated_encoding(s);
        return;
    }

    switch (type) {
    case 0:
        size = MO_32;
        break;
    case 1:
        size = MO_64;
        break;
    case 3:
        size = MO_16;
        if (dc_isar_feature(aa64_fp16, s)) {
            break;
        }
        /* fallthru */
    default:
        lata_unallocated_encoding(s);
        return;
    }

    if (!fp_access_check(s)) {
        return;
    }

    handle_fp_compare(s, size, rn, rm, opc & 1, opc & 2);
}

/* Floating point conditional compare
 *   31  30  29 28       24 23  22  21 20  16 15  12 11 10 9    5  4   3    0
 * +---+---+---+-----------+------+---+------+------+-----+------+----+------+
 * | M | 0 | S | 1 1 1 1 0 | type | 1 |  Rm  | cond | 0 1 |  Rn  | op | nzcv |
 * +---+---+---+-----------+------+---+------+------+-----+------+----+------+
 */
static void disas_fp_ccomp(DisasContext *s, uint32_t insn)
{
    unsigned int mos, type, rm, cond, rn, op, nzcv;
    int size;

    mos = extract32(insn, 29, 3);
    type = extract32(insn, 22, 2);
    rm = extract32(insn, 16, 5);
    cond = extract32(insn, 12, 4);
    rn = extract32(insn, 5, 5);
    op = extract32(insn, 4, 1);
    nzcv = extract32(insn, 0, 4);

    if (mos) {
        lata_unallocated_encoding(s);
        return;
    }

    switch (type) {
    case 0:
        size = MO_32;
        break;
    case 1:
        size = MO_64;
        break;
    case 3:
        size = MO_16;
        if (dc_isar_feature(aa64_fp16, s)) {
            break;
        }
        /* fallthru */
    default:
        lata_unallocated_encoding(s);
        return;
    }

    if (!fp_access_check(s)) {
        return;
    }

    IR2_OPND temp = ra_alloc_itemp();
    IR2_OPND label_match = ir2_opnd_new_type(IR2_OPND_LABEL);
    IR2_OPND label_end = ir2_opnd_new_type(IR2_OPND_LABEL);

    if (cond < 0x0e) { /* not always */
        la_setarmj(temp, cond);
        la_bnez(temp, label_match);

        /* nomatch: */
        la_addi_w(temp, zero_ir2_opnd, nzcv);
        la_slli_w(temp, temp, 28);
        la_armmtflag(temp, 0x39);
        la_b(label_end);

        la_label(label_match);
    }

    handle_fp_compare(s, size, rn, rm, false, op);

    if (cond < 0x0e) {
        la_label(label_end);
    }

    free_alloc_gpr(temp);
}

/* Floating point conditional select
 *   31  30  29 28       24 23  22  21 20  16 15  12 11 10 9    5 4    0
 * +---+---+---+-----------+------+---+------+------+-----+------+------+
 * | M | 0 | S | 1 1 1 1 0 | type | 1 |  Rm  | cond | 1 1 |  Rn  |  Rd  |
 * +---+---+---+-----------+------+---+------+------+-----+------+------+
 */
static void disas_fp_csel(DisasContext *s, uint32_t insn)
{
    unsigned int mos, type, rm, cond, rn, rd;

    mos = extract32(insn, 29, 3);
    type = extract32(insn, 22, 2);
    rm = extract32(insn, 16, 5);
    cond = extract32(insn, 12, 4);
    rn = extract32(insn, 5, 5);
    rd = extract32(insn, 0, 5);

    if (mos) {
        lata_unallocated_encoding(s);
        return;
    }


    if (!fp_access_check(s)) {
        return;
    }
    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND vreg_m = alloc_fpr_src(rm);
    IR2_OPND temp = ra_alloc_itemp();
    IR2_OPND l_false = ir2_opnd_new_type(IR2_OPND_LABEL);
    IR2_OPND l_end = ir2_opnd_new_type(IR2_OPND_LABEL);

    la_setarmj(temp, cond);
    la_beqz(temp, l_false);

    /* TRUE */
    switch (type) {
        case 0:
            la_fmov_s(vreg_d, vreg_n);
            la_movgr2frh_w(vreg_d, zero_ir2_opnd);
            break;
        case 1:
            la_fmov_d(vreg_d, vreg_n);
            break;
        case 3:
            assert(0);
            if (dc_isar_feature(aa64_fp16, s)) {
                break;
            }
            /* fallthru */
        default:
            lata_unallocated_encoding(s);
            return;
    }
    la_b(l_end);

    /* FALSE */
    la_label(l_false);
    switch (type) {
        case 0:
            la_fmov_s(vreg_d, vreg_m);
            la_movgr2frh_w(vreg_d, zero_ir2_opnd);
            break;
        case 1:
            la_fmov_d(vreg_d, vreg_m);
            break;
        case 3:
            assert(0);
            if (dc_isar_feature(aa64_fp16, s)) {
                break;
            }
            /* fallthru */
        default:
            lata_unallocated_encoding(s);
            return;
    }
    
    la_label(l_end);

    /* 高64位清零 */
    la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
    free_alloc_fpr(vreg_m);
    free_alloc_gpr(temp);
}

/* Floating-point data-processing (1 source) - half precision */
static void handle_fp_1src_half(DisasContext *s, int opcode, int rd, int rn)
{
    assert(0);
}

/* Floating-point data-processing (1 source) - single precision */
static void handle_fp_1src_single(DisasContext *s, int opcode, int rd, int rn)
{
    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);

    switch (opcode) {
    case 0x0: /* FMOV */
        la_fmov_s(vreg_d, vreg_n);
        goto done;
    case 0x1: /* FABS */
        la_fabs_s(vreg_d, vreg_n);
        goto done;
    case 0x2: /* FNEG */
        la_fneg_s(vreg_d, vreg_n);
        goto done;
    case 0x3: /* FSQRT */
        la_fsqrt_s(vreg_d, vreg_n);
        goto done;
    case 0x6: /* BFCVT */
        // gen_fpst = gen_helper_bfcvt;
        break;
    case 0x8: /* FRINTN */
        break;
    case 0x9: /* FRINTP */
        break;
    case 0xa: /* FRINTM */
        la_vfrintrm_s(vreg_d, vreg_n);
        goto done;
        break;
    case 0xb: /* FRINTZ */
        la_vfrintrz_s(vreg_d, vreg_n);
        goto done;
        break;
    case 0xc: /* FRINTA */
        // rmode = opcode & 7;
        // gen_fpst = gen_helper_rints;
        break;
    case 0xe: /* FRINTX */
        // gen_fpst = gen_helper_rints_exact;
        break;
    case 0xf: /* FRINTI */
        // gen_fpst = gen_helper_rints;
        break;
    case 0x10: /* FRINT32Z */
        // rmode = FPROUNDING_ZERO;
        // gen_fpst = gen_helper_frint32_s;
        break;
    case 0x11: /* FRINT32X */
        // gen_fpst = gen_helper_frint32_s;
        break;
    case 0x12: /* FRINT64Z */
        // rmode = FPROUNDING_ZERO;
        // gen_fpst = gen_helper_frint64_s;
        break;
    case 0x13: /* FRINT64X */
        // gen_fpst = gen_helper_frint64_s;
        break;
    default:
        g_assert_not_reached();
    }

    assert(0);
    
 done:
    la_vand_v(vreg_d,vreg_d, fsmask_ir2_opnd);

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
}

/* Floating-point data-processing (1 source) - double precision */
static void handle_fp_1src_double(DisasContext *s, int opcode, int rd, int rn)
{
    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);

    switch (opcode) {
    case 0x0: /* FMOV */
        la_fmov_d(vreg_d, vreg_n);
        goto done;
    case 0x1: /* FABS */
        la_fabs_d(vreg_d, vreg_n);
        goto done;
    case 0x2: /* FNEG */
        la_fneg_d(vreg_d, vreg_n);
        goto done;
    case 0x3: /* FSQRT */
        la_fsqrt_d(vreg_d, vreg_n);
        goto done;
    case 0x8: /* FRINTN */
        break;
    case 0x9: /* FRINTP */
        la_vfrintrp_d(vreg_d, vreg_n);
        goto done;
        break;
    case 0xa: /* FRINTM */
        la_vfrintrm_d(vreg_d, vreg_n);
        goto done;
        break;
    case 0xb: /* FRINTZ */
        la_vfrintrz_d(vreg_d, vreg_n);
        goto done;
        break;
    case 0xc: /* FRINTA */
        // rmode = opcode & 7;
        // gen_fpst = gen_helper_rintd;
        break;
    case 0xe: /* FRINTX */
        // gen_fpst = gen_helper_rintd_exact;
        break;
    case 0xf: /* FRINTI */
        // gen_fpst = gen_helper_rintd;
        break;
    case 0x10: /* FRINT32Z */
        // rmode = FPROUNDING_ZERO;
        // gen_fpst = gen_helper_frint32_d;
        break;
    case 0x11: /* FRINT32X */
        // gen_fpst = gen_helper_frint32_d;
        break;
    case 0x12: /* FRINT64Z */
        // rmode = FPROUNDING_ZERO;
        // gen_fpst = gen_helper_frint64_d;
        break;
    case 0x13: /* FRINT64X */
        // gen_fpst = gen_helper_frint64_d;
        break;
    default:
        g_assert_not_reached();
    }

    assert(0);

 done:
    /* 高64位清零 */
    la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
}

static void handle_fp_fcvt(DisasContext *s, int opcode,
                           int rd, int rn, int dtype, int ntype)
{
    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);

    switch (ntype) {
    case 0x0:
    {
        if (dtype == 1) {
            /* Single to double */
            la_fcvt_d_s(vreg_d, vreg_n);
        } else {
            assert(0);
            /* Single to half */
        }
        break;
    }
    case 0x1:
    {
        if (dtype == 0) {
            /* Double to single */
            la_fcvt_s_d(vreg_d, vreg_n);
        } else {
            assert(0);
            /* Double to half */
        }
        break;
    }
    case 0x3:
    {
        assert(0);
        if (dtype == 0) {
            /* Half to single */

        } else {
            /* Half to double */

        }
        break;
    }
    default:
        g_assert_not_reached();
    }

    /* 高64位清零 */
    la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
}

/* Floating point data-processing (1 source)
 *   31  30  29 28       24 23  22  21 20    15 14       10 9    5 4    0
 * +---+---+---+-----------+------+---+--------+-----------+------+------+
 * | M | 0 | S | 1 1 1 1 0 | type | 1 | opcode | 1 0 0 0 0 |  Rn  |  Rd  |
 * +---+---+---+-----------+------+---+--------+-----------+------+------+
 */
static void disas_fp_1src(DisasContext *s, uint32_t insn)
{
    int mos = extract32(insn, 29, 3);
    int type = extract32(insn, 22, 2);
    int opcode = extract32(insn, 15, 6);
    int rn = extract32(insn, 5, 5);
    int rd = extract32(insn, 0, 5);

    if (mos) {
        goto do_unallocated;
    }

    switch (opcode) {
    case 0x4: case 0x5: case 0x7:
    {
        /* FCVT between half, single and double precision */
        int dtype = extract32(opcode, 0, 2);
        if (type == 2 || dtype == type) {
            goto do_unallocated;
        }
        if (!fp_access_check(s)) {
            return;
        }

        handle_fp_fcvt(s, opcode, rd, rn, dtype, type);
        break;
    }

    case 0x10 ... 0x13: /* FRINT{32,64}{X,Z} */
        if (type > 1 || !dc_isar_feature(aa64_frint, s)) {
            goto do_unallocated;
        }
        /* fall through */
    case 0x0 ... 0x3:
    case 0x8 ... 0xc:
    case 0xe ... 0xf:
        /* 32-to-32 and 64-to-64 ops */
        switch (type) {
        case 0:
            if (!fp_access_check(s)) {
                return;
            }
            handle_fp_1src_single(s, opcode, rd, rn);
            break;
        case 1:
            if (!fp_access_check(s)) {
                return;
            }
            handle_fp_1src_double(s, opcode, rd, rn);
            break;
        case 3:
            if (!dc_isar_feature(aa64_fp16, s)) {
                goto do_unallocated;
            }

            if (!fp_access_check(s)) {
                return;
            }
            handle_fp_1src_half(s, opcode, rd, rn);
            break;
        default:
            goto do_unallocated;
        }
        break;

    case 0x6:
        switch (type) {
        case 1: /* BFCVT */
            if (!dc_isar_feature(aa64_bf16, s)) {
                goto do_unallocated;
            }
            if (!fp_access_check(s)) {
                return;
            }
            handle_fp_1src_single(s, opcode, rd, rn);
            break;
        default:
            goto do_unallocated;
        }
        break;

    default:
    do_unallocated:
        lata_unallocated_encoding(s);
        break;
    }
}

/* Floating-point data-processing (2 source) - single precision */
static void handle_fp_2src_single(DisasContext *s, int opcode,
                                  int rd, int rn, int rm)
{
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND vreg_m = alloc_fpr_src(rm);
    IR2_OPND vreg_d = alloc_fpr_dst(rd);

    switch (opcode) {
    case 0x0: /* FMUL */
        la_fmul_s(vreg_d, vreg_n, vreg_m);
        break;
    case 0x1: /* FDIV */
        la_fdiv_s(vreg_d, vreg_n, vreg_m);
        break;
    case 0x2: /* FADD */
        la_fadd_s(vreg_d, vreg_n, vreg_m);
        break;
    case 0x3: /* FSUB */
        la_fsub_s(vreg_d, vreg_n, vreg_m);
        break;
    case 0x4: /* FMAX */
        assert(0);
        break;
    case 0x5: /* FMIN */
        assert(0);
        break;
    case 0x6: /* FMAXNM */
        la_fmax_s(vreg_d, vreg_n, vreg_m);
        break;
    case 0x7: /* FMINNM */
        la_fmin_s(vreg_d, vreg_n, vreg_m);
        break;
    case 0x8: /* FNMUL */
        la_fmul_s(vreg_d, vreg_n, vreg_m);
        la_fneg_s(vreg_d, vreg_d);
        break;
    }
    la_vand_v(vreg_d,vreg_d, fsmask_ir2_opnd);

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
    free_alloc_fpr(vreg_m);
}

/* Floating-point data-processing (2 source) - double precision */
static void handle_fp_2src_double(DisasContext *s, int opcode,
                                  int rd, int rn, int rm)
{
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND vreg_m = alloc_fpr_src(rm);
    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    // IR2_OPND vtemp;

    switch (opcode) {
    case 0x0: /* FMUL */
        la_fmul_d(vreg_d, vreg_n, vreg_m);
        break;
    case 0x1: /* FDIV */
        la_fdiv_d(vreg_d, vreg_n, vreg_m);
        break;
    case 0x2: /* FADD */
        la_fadd_d(vreg_d, vreg_n, vreg_m);
        break;
    case 0x3: /* FSUB */
        la_fsub_d(vreg_d, vreg_n, vreg_m);
        break;
    case 0x4: /* FMAX */
        assert(0);
        break;
    case 0x5: /* FMIN */
        assert(0);
        break;
    case 0x6: /* FMAXNM */
        la_fmax_d(vreg_d, vreg_n, vreg_m);
        break;
    case 0x7: /* FMINNM */
        la_fmin_d(vreg_d, vreg_n, vreg_m);
        break;
    case 0x8: /* FNMUL */
        la_fmul_d(vreg_d, vreg_n, vreg_m);
        la_fneg_d(vreg_d, vreg_d);
        break;
    }
    /* 高64位清零 */
    la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
    free_alloc_fpr(vreg_m);
}

/* Floating-point data-processing (2 source) - half precision */
static void handle_fp_2src_half(DisasContext *s, int opcode,
                                int rd, int rn, int rm)
{
    assert(0);
}

/* Floating point data-processing (2 source)
 *   31  30  29 28       24 23  22  21 20  16 15    12 11 10 9    5 4    0
 * +---+---+---+-----------+------+---+------+--------+-----+------+------+
 * | M | 0 | S | 1 1 1 1 0 | type | 1 |  Rm  | opcode | 1 0 |  Rn  |  Rd  |
 * +---+---+---+-----------+------+---+------+--------+-----+------+------+
 */
static void disas_fp_2src(DisasContext *s, uint32_t insn)
{
    int mos = extract32(insn, 29, 3);
    int type = extract32(insn, 22, 2);
    int rd = extract32(insn, 0, 5);
    int rn = extract32(insn, 5, 5);
    int rm = extract32(insn, 16, 5);
    int opcode = extract32(insn, 12, 4);

    if (opcode > 8 || mos) {
        lata_unallocated_encoding(s);
        return;
    }

    switch (type) {
    case 0:
        if (!fp_access_check(s)) {
            return;
        }
        handle_fp_2src_single(s, opcode, rd, rn, rm);
        break;
    case 1:
        if (!fp_access_check(s)) {
            return;
        }
        handle_fp_2src_double(s, opcode, rd, rn, rm);
        break;
    case 3:
        if (!dc_isar_feature(aa64_fp16, s)) {
            lata_unallocated_encoding(s);
            return;
        }
        if (!fp_access_check(s)) {
            return;
        }
        handle_fp_2src_half(s, opcode, rd, rn, rm);
        break;
    default:
        lata_unallocated_encoding(s);
    }
}

/* Floating-point data-processing (3 source) - single precision */
static void handle_fp_3src_single(DisasContext *s, bool o0, bool o1,
                                  int rd, int rn, int rm, int ra)
{
    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND vreg_m = alloc_fpr_src(rm);
    IR2_OPND vreg_a = alloc_fpr_src(ra);
    IR2_OPND vtemp = ra_alloc_ftemp();
    IR2_OPND vtemp1 = ra_alloc_ftemp();

    /* These are fused multiply-add, and must be done as one
     * floating point operation with no rounding between the
     * multiplication and addition steps.
     * NB that doing the negations here as separate steps is
     * correct : an input NaN should come out with its sign bit
     * flipped if it is a negated-input.
     */

    if(o0 != o1){
        la_fneg_s(vtemp, vreg_n);
        if(o1 == true){ /* FNMADD */
            la_fneg_s(vtemp1, vreg_a);
            la_fmadd_s(vreg_d, vtemp, vreg_m, vtemp1);
        }else{ /* FMSUB */
            la_fmadd_s(vreg_d, vtemp, vreg_m, vreg_a);
        }
    }else{
        if(o1 == true){ /* FNMSUB */
            la_fneg_s(vtemp1, vreg_a);
            la_fmadd_s(vreg_d, vreg_n, vreg_m, vtemp1);
        }else{ /* FMADD */
            la_fmadd_s(vreg_d, vreg_n, vreg_m, vreg_a);
        }
    }
    la_vand_v(vreg_d,vreg_d, fsmask_ir2_opnd);

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
    free_alloc_fpr(vreg_m);
    free_alloc_fpr(vreg_a);
    free_alloc_fpr(vtemp);
    free_alloc_fpr(vtemp1);
}

/* Floating-point data-processing (3 source) - double precision */
static void handle_fp_3src_double(DisasContext *s, bool o0, bool o1,
                                  int rd, int rn, int rm, int ra)
{
    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND vreg_m = alloc_fpr_src(rm);
    IR2_OPND vreg_a = alloc_fpr_src(ra);
    IR2_OPND vtemp = ra_alloc_ftemp();
    IR2_OPND vtemp1 = ra_alloc_ftemp();

    /* These are fused multiply-add, and must be done as one
     * floating point operation with no rounding between the
     * multiplication and addition steps.
     * NB that doing the negations here as separate steps is
     * correct : an input NaN should come out with its sign bit
     * flipped if it is a negated-input.
     */

    if(o0 != o1){
        la_fneg_d(vtemp, vreg_n);
        if(o1 == true){ /* FNMADD */
            la_fneg_d(vtemp1, vreg_a);
            la_fmadd_d(vreg_d, vtemp, vreg_m, vtemp1);
        }else{ /* FMSUB */
            la_fmadd_d(vreg_d, vtemp, vreg_m, vreg_a);
        }
    }else{
        if(o1 == true){ /* FNMSUB */
            la_fneg_d(vtemp1, vreg_a);
            la_fmadd_d(vreg_d, vreg_n, vreg_m, vtemp1);
        }else{ /* FMADD */
            la_fmadd_d(vreg_d, vreg_n, vreg_m, vreg_a);
        }
    }
    /* 高64位清零 */
    la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
    free_alloc_fpr(vreg_m);
    free_alloc_fpr(vreg_a);
    free_alloc_fpr(vtemp);
    free_alloc_fpr(vtemp1);
}

/* Floating-point data-processing (3 source) - half precision */
static void handle_fp_3src_half(DisasContext *s, bool o0, bool o1,
                                int rd, int rn, int rm, int ra)
{
    assert(0);
}

/* Floating point data-processing (3 source)
 *   31  30  29 28       24 23  22  21  20  16  15  14  10 9    5 4    0
 * +---+---+---+-----------+------+----+------+----+------+------+------+
 * | M | 0 | S | 1 1 1 1 1 | type | o1 |  Rm  | o0 |  Ra  |  Rn  |  Rd  |
 * +---+---+---+-----------+------+----+------+----+------+------+------+
 */
static void disas_fp_3src(DisasContext *s, uint32_t insn)
{
    int mos = extract32(insn, 29, 3);
    int type = extract32(insn, 22, 2);
    int rd = extract32(insn, 0, 5);
    int rn = extract32(insn, 5, 5);
    int ra = extract32(insn, 10, 5);
    int rm = extract32(insn, 16, 5);
    bool o0 = extract32(insn, 15, 1);
    bool o1 = extract32(insn, 21, 1);

    if (mos) {
        lata_unallocated_encoding(s);
        return;
    }

    switch (type) {
    case 0:
        if (!fp_access_check(s)) {
            return;
        }
        handle_fp_3src_single(s, o0, o1, rd, rn, rm, ra);
        break;
    case 1:
        if (!fp_access_check(s)) {
            return;
        }
        handle_fp_3src_double(s, o0, o1, rd, rn, rm, ra);
        break;
    case 3:
        if (!dc_isar_feature(aa64_fp16, s)) {
            lata_unallocated_encoding(s);
            return;
        }
        if (!fp_access_check(s)) {
            return;
        }
        handle_fp_3src_half(s, o0, o1, rd, rn, rm, ra);
        break;
    default:
        lata_unallocated_encoding(s);
    }
}

/* Floating point immediate
 *   31  30  29 28       24 23  22  21 20        13 12   10 9    5 4    0
 * +---+---+---+-----------+------+---+------------+-------+------+------+
 * | M | 0 | S | 1 1 1 1 0 | type | 1 |    imm8    | 1 0 0 | imm5 |  Rd  |
 * +---+---+---+-----------+------+---+------------+-------+------+------+
 */
static void disas_fp_imm(DisasContext *s, uint32_t insn)
{
    int rd = extract32(insn, 0, 5);
    int imm5 = extract32(insn, 5, 5);
    int imm8 = extract32(insn, 13, 8);
    int type = extract32(insn, 22, 2);
    int mos = extract32(insn, 29, 3);
    uint64_t imm;
    MemOp sz;

    if (mos || imm5) {
        lata_unallocated_encoding(s);
        return;
    }

    switch (type) {
    case 0:
        sz = MO_32;
        break;
    case 1:
        sz = MO_64;
        break;
    case 3:
        sz = MO_16;
        if (dc_isar_feature(aa64_fp16, s)) {
            break;
        }
        /* fallthru */
    default:
        lata_unallocated_encoding(s);
        return;
    }

    if (!fp_access_check(s)) {
        return;
    }

    imm = vfp_expand_imm(sz, imm8);
    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND temp = ra_alloc_itemp();

    li_d(temp, (int64_t)imm);
    la_movgr2fr_d(vreg_d, temp);
    /* 高64位清零 */
    la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_gpr(temp);
}

/* Handle floating point <=> fixed point conversions. Note that we can
 * also deal with fp <=> integer conversions as a special case (scale == 64)
 * OPTME: consider handling that special case specially or at least skipping
 * the call to scalbn in the helpers for zero shifts.
 */
static void handle_fpfpcvt(DisasContext *s, int rd, int rn, int opcode,
                           bool itof, int rmode, int scale, int sf, int type)
{
    bool is_signed = !(opcode & 1);

    if (itof) {
        IR2_OPND vreg_d = alloc_fpr_dst(rd);
        IR2_OPND reg_n = alloc_gpr_src(rn);
        IR2_OPND vtemp = ra_alloc_ftemp();

        switch (type) {
        case 1: /* float64 */
            if(sf){
                la_movgr2fr_d(vtemp, reg_n);
                if(is_signed){
                    la_vffint_d_l(vreg_d, vtemp);
                }else{
                    la_vffint_d_lu(vreg_d, vtemp);
                }
            }else{
                la_movgr2fr_w(vtemp, reg_n);
                if(is_signed){
                    la_ffint_d_w(vreg_d, vtemp);
                }else{
                    la_movgr2frh_w(vtemp, zero_ir2_opnd);
                    la_vffint_d_lu(vreg_d, vtemp);
                }
            }
            break;
        case 0: /* float32 */
            if(sf){
                la_movgr2fr_d(vtemp, reg_n);
                if(is_signed){
                    la_ffint_s_l(vreg_d, vtemp);
                }else{
                    la_vffint_d_lu(vtemp, vtemp);
                    la_fcvt_s_d(vreg_d, vtemp);
                }
            }else{
                la_movgr2fr_w(vtemp, reg_n);
                if(is_signed){
                    la_vffint_s_w(vreg_d, vtemp);
                }else{
                    la_vffint_s_wu(vreg_d, vtemp);
                }
            }
            break;

        case 3: /* float16 */ 
            assert(0);

        default:
            g_assert_not_reached();
        }
        la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);

        store_fpr_dst(rd, vreg_d);
        free_alloc_fpr(vreg_d);
        free_alloc_fpr(vtemp);
        free_alloc_gpr(reg_n);
    } else {

        if (extract32(opcode, 2, 1)) {
            /* There are too many rounding modes to all fit into rmode,
             * so FCVTA[US] is a special case.
             */
            rmode = FPROUNDING_TIEAWAY;
        }
        if(is_signed && extract32(opcode, 2, 1) != 1 && scale == 64){ /* FCVT[NPMZ]S && SHIFT == 0 */
            IR2_OPND reg_d = alloc_gpr_dst(rd);
            IR2_OPND vreg_n = alloc_fpr_src(rn);
            IR2_OPND vtemp = ra_alloc_ftemp();


            switch(rmode){ 
            case 0: /* Round to Nearest (RN) mode. */
                switch (type) {
                case 1: /* float64 */
                    if (sf) {
                        la_ftintrne_l_d(vtemp, vreg_n);
                        la_movfr2gr_d(reg_d, vtemp);
                    } else {
                        la_ftintrne_w_d(vtemp, vreg_n);
                        la_movfr2gr_s(reg_d, vtemp);

                    }
                    break;
                case 0: /* float32 */
                    if (sf) {
                        la_ftintrne_l_s(vtemp, vreg_n);
                        la_movfr2gr_d(reg_d, vtemp);
                    } else {
                        la_ftintrne_w_s(vtemp, vreg_n);
                        la_movfr2gr_s(reg_d, vtemp);

                    }
                    break;
                case 3: /* float16 */
                    assert(0);
                    break;
                default:
                    g_assert_not_reached();
                }
                break;
            case 1: /* Round towards Plus Infinity (RP) mode. */
                switch (type) {
                case 1: /* float64 */
                    if (sf) {
                        la_ftintrp_l_d(vtemp, vreg_n);
                        la_movfr2gr_d(reg_d, vtemp);
                    } else {
                        la_ftintrp_w_d(vtemp, vreg_n);
                        la_movfr2gr_s(reg_d, vtemp);

                    }
                    break;
                case 0: /* float32 */
                    if (sf) {
                        la_ftintrp_l_s(vtemp, vreg_n);
                        la_movfr2gr_d(reg_d, vtemp);
                    } else {
                        la_ftintrp_w_s(vtemp, vreg_n);
                        la_movfr2gr_s(reg_d, vtemp);

                    }
                    break;
                case 3: /* float16 */
                    assert(0);
                    break;
                default:
                    g_assert_not_reached();
                }
                break;
            case 2: /* Round towards Minus Infinity (RM) mode. */
                switch (type) {
                case 1: /* float64 */
                    if (sf) {
                        la_ftintrm_l_d(vtemp, vreg_n);
                        la_movfr2gr_d(reg_d, vtemp);
                    } else {
                        la_ftintrm_w_d(vtemp, vreg_n);
                        la_movfr2gr_s(reg_d, vtemp);

                    }
                    break;
                case 0: /* float32 */
                    if (sf) {
                        la_ftintrm_l_s(vtemp, vreg_n);
                        la_movfr2gr_d(reg_d, vtemp);
                    } else {
                        la_ftintrm_w_s(vtemp, vreg_n);
                        la_movfr2gr_s(reg_d, vtemp);

                    }
                    break;
                case 3: /* float16 */
                    assert(0);
                    break;
                default:
                    g_assert_not_reached();
                }
                break;
            case 3: /* Round towards Zero (RZ) mode. */
                switch (type) {
                case 1: /* float64 */
                    if (sf) {
                        la_ftintrz_l_d(vtemp, vreg_n);
                        la_movfr2gr_d(reg_d, vtemp);
                    } else {
                        la_ftintrz_w_d(vtemp, vreg_n);
                        la_movfr2gr_s(reg_d, vtemp);

                    }
                    break;
                case 0: /* float32 */
                    if (sf) {
                        la_ftintrz_l_s(vtemp, vreg_n);
                        la_movfr2gr_d(reg_d, vtemp);
                    } else {
                        la_ftintrz_w_s(vtemp, vreg_n);
                        la_movfr2gr_s(reg_d, vtemp);

                    }
                    break;
                case 3: /* float16 */
                    assert(0);
                    break;
                default:
                    g_assert_not_reached();
                }
                break;
            default:
                g_assert_not_reached();
            }

            if (rd != 31 && arm_la_map[rd] >= 0 && clearGprHigh) {
                set_w_write_flag(rd, sf);
            } else {
                if (!sf) {
                    la_bstrpick_d(reg_d, reg_d, 31, 0);
                }
            }
            store_gpr_dst(rd, reg_d);
            free_alloc_gpr(reg_d);
            free_alloc_fpr(vreg_n);
            free_alloc_fpr(vtemp);
        }else{  /* FCVT[NPMZ]U || (FCVT[NPMZ]S && SHIFT != 0) || FCVTA[U/S]*/
            /* 龙芯没有对应浮点数转成无符号整数的指令，
            当大于最大的无符号浮点数(0xffffffff或0xffffffffffffffff), arm64会置成最大的无符号整数。
            当小于0的浮点数转成无符号整数时， arm64直接置0。
            */

            IR2_OPND temp = ra_alloc_itemp();
            IR2_OPND reg_d = alloc_gpr_dst(rd);

            li_d(temp, s->base->pc_next);
            la_st_d(temp, env_ir2_opnd, env_offset_pc());
            lata_gen_call_helper_prologue(tcg_ctx);

            /* set rmode */
            li_d(a0_ir2_opnd, env_offset_RMODE());
            li_d(a1_ir2_opnd, arm_rmode_to_sf(rmode));
            la_stx_w(a1_ir2_opnd, env_ir2_opnd, a0_ir2_opnd);

            /* a2: fpstatus */
            li_d(temp, env_offset_fpstatus());
            la_add_d(a2_ir2_opnd, env_ir2_opnd, temp);
            /* a1: shift */
            li_d(a1_ir2_opnd, 64 - scale);
            /* a0: fp */
            li_d(temp, env_offset_fpr(rn));
            la_ldx_d(a0_ir2_opnd, env_ir2_opnd, temp);

            switch (type) {
            case 1: /* float64 */
                if(is_signed){
                    if (sf) {
                        li_d(temp, (uint64_t)helper_vfp_tosqd);
                    } else {
                        li_d(temp, (uint64_t)helper_vfp_tosld);
                    }
                }else{
                    if (sf) {
                        li_d(temp, (uint64_t)helper_vfp_touqd);
                    } else {
                        li_d(temp, (uint64_t)helper_vfp_tould);
                    }
                }
                break;
            case 0: /* float32 */
                if(is_signed){
                    if (sf) {
                        li_d(temp, (uint64_t)helper_vfp_tosqs);
                    } else {
                        li_d(temp, (uint64_t)helper_vfp_tosls);
                    }
                }else{
                    if (sf) {
                        li_d(temp, (uint64_t)helper_vfp_touqs);
                    } else {
                        li_d(temp, (uint64_t)helper_vfp_touls);
                    }
                }
                break;
            case 3: /* float16 */
                assert(0);
                break;

            default:
                g_assert_not_reached();
            }
            la_jirl(ra_ir2_opnd, temp, 0);
            
            /* set rmode */
            li_d(a6_ir2_opnd, env_offset_FPSCR());
            la_ldx_d(a7_ir2_opnd, env_ir2_opnd, a6_ir2_opnd);
            la_bstrpick_d(a7_ir2_opnd, a7_ir2_opnd, 23, 22);
            li_d(a6_ir2_opnd, env_offset_RMODE());
            la_stx_w(a7_ir2_opnd, env_ir2_opnd, a6_ir2_opnd);

            lata_gen_call_helper_epilogue(tcg_ctx);
            la_mov64(reg_d, a0_ir2_opnd);

            if (rd != 31 && arm_la_map[rd] >= 0 && clearGprHigh) {
                set_w_write_flag(rd, sf);
            } else {
                if (!sf) {
                    la_bstrpick_d(reg_d, reg_d, 31, 0);
                }
            }
            store_gpr_dst(rd, reg_d);
            free_alloc_gpr(reg_d);
            free_alloc_gpr(temp);
        }   
    }
}

/* Floating point <-> fixed point conversions
 *   31   30  29 28       24 23  22  21 20   19 18    16 15   10 9    5 4    0
 * +----+---+---+-----------+------+---+-------+--------+-------+------+------+
 * | sf | 0 | S | 1 1 1 1 0 | type | 0 | rmode | opcode | scale |  Rn  |  Rd  |
 * +----+---+---+-----------+------+---+-------+--------+-------+------+------+
 */
static void disas_fp_fixed_conv(DisasContext *s, uint32_t insn)
{
    int rd = extract32(insn, 0, 5);
    int rn = extract32(insn, 5, 5);
    int scale = extract32(insn, 10, 6);
    int opcode = extract32(insn, 16, 3);
    int rmode = extract32(insn, 19, 2);
    int type = extract32(insn, 22, 2);
    bool sbit = extract32(insn, 29, 1);
    bool sf = extract32(insn, 31, 1);
    bool itof;

    if (sbit || (!sf && scale < 32)) {
        lata_unallocated_encoding(s);
        return;
    }

    switch (type) {
    case 0: /* float32 */
    case 1: /* float64 */
        break;
    case 3: /* float16 */
        if (dc_isar_feature(aa64_fp16, s)) {
            break;
        }
        /* fallthru */
    default:
        lata_unallocated_encoding(s);
        return;
    }

    switch ((rmode << 3) | opcode) {
    case 0x2: /* SCVTF */
    case 0x3: /* UCVTF */
        itof = true;
        break;
    case 0x18: /* FCVTZS */
    case 0x19: /* FCVTZU */
        itof = false;
        break;
    default:
        lata_unallocated_encoding(s);
        return;
    }

    if (!fp_access_check(s)) {
        return;
    }

    handle_fpfpcvt(s, rd, rn, opcode, itof, FPROUNDING_ZERO, scale, sf, type);
}

static void handle_fmov(DisasContext *s, int rd, int rn, int type, bool itof)
{
    /* FMOV: gpr to or from float, double, or top half of quad fp reg,
     * without conversion.
     */

    if (itof) {
        IR2_OPND reg_n = alloc_gpr_src(rn);
        IR2_OPND vreg_d;
        IR2_OPND temp = ra_alloc_itemp();

        if(type == 2){
            vreg_d = alloc_fpr_src(rd);
        }else{
            vreg_d = alloc_fpr_dst(rd);
        }
        switch (type) {
        case 0:
            /* 32 bit */
            la_bstrpick_d(temp, reg_n, 31, 0);
            la_movgr2fr_w(vreg_d, temp);
            break;
        case 1:
            /* 64 bit */
            if (clearGprHigh && arm_la_map[rn] >= 0 && rn != 31) {
                clear_gpr_high(rn);
            }
            la_movgr2fr_d(vreg_d, reg_n);
            break;
        case 2:
            /* 64 bit to top half. */
            if (clearGprHigh && arm_la_map[rn] >= 0 && rn != 31) {
                clear_gpr_high(rn);
            }
            la_vinsgr2vr_d(vreg_d, reg_n, 1);
            break;
        case 3:
            /* 16 bit */
            la_bstrpick_d(temp, reg_n, 16, 0);
            la_movgr2fr_d(vreg_d, temp);
            break;
        default:
            g_assert_not_reached();
        }
        if(type != 2){
            la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
        }

        store_fpr_dst(rd, vreg_d);
        free_alloc_fpr(vreg_d);
        free_alloc_gpr(reg_n);
        free_alloc_gpr(temp);
    } else {
        IR2_OPND reg_d = alloc_gpr_dst(rd);
        IR2_OPND vreg_n = alloc_fpr_src(rn);
        IR2_OPND temp = ra_alloc_itemp();

        switch (type) {
        case 0:
            /* 32 bit */
            la_movfr2gr_d(temp, vreg_n);
            la_bstrpick_d(reg_d, temp, 31, 0);
            break;
        case 1:
            /* 64 bit */
            la_movfr2gr_d(reg_d, vreg_n);
            break;
        case 2:
            /* 64 bits from top half */
            // tcg_gen_ld_i64(tcg_rd, cpu_env, fp_reg_hi_offset(s, rn));
            la_vpickve2gr_du(reg_d, vreg_n, 1);
            break;
        case 3:
            /* 16 bit */
            la_movfr2gr_d(temp, vreg_n);
            la_bstrpick_d(reg_d, temp, 16, 0);
            break;
        default:
            g_assert_not_reached();
        }

        if (rd != 31 && arm_la_map[rd] >= 0 && clearGprHigh) {
            set_w_write_flag(rd, TRUE);
        }
        store_gpr_dst(rd, reg_d);
        free_alloc_gpr(reg_d);
        free_alloc_fpr(vreg_n);
        free_alloc_gpr(temp);
    }
}

static void handle_fjcvtzs(DisasContext *s, int rd, int rn)
{
    assert(0);
}

/* Floating point <-> integer conversions
 *   31   30  29 28       24 23  22  21 20   19 18 16 15         10 9  5 4  0
 * +----+---+---+-----------+------+---+-------+-----+-------------+----+----+
 * | sf | 0 | S | 1 1 1 1 0 | type | 1 | rmode | opc | 0 0 0 0 0 0 | Rn | Rd |
 * +----+---+---+-----------+------+---+-------+-----+-------------+----+----+
 */
static void disas_fp_int_conv(DisasContext *s, uint32_t insn)
{
    int rd = extract32(insn, 0, 5);
    int rn = extract32(insn, 5, 5);
    int opcode = extract32(insn, 16, 3);
    int rmode = extract32(insn, 19, 2);
    int type = extract32(insn, 22, 2);
    bool sbit = extract32(insn, 29, 1);
    bool sf = extract32(insn, 31, 1);
    bool itof = false;

    if (sbit) {
        goto do_unallocated;
    }

    switch (opcode) {
    case 2: /* SCVTF */
    case 3: /* UCVTF */
        itof = true;
        /* fallthru */
    case 4: /* FCVTAS */
    case 5: /* FCVTAU */
        if (rmode != 0) {
            goto do_unallocated;
        }
        /* fallthru */
    case 0: /* FCVT[NPMZ]S */
    case 1: /* FCVT[NPMZ]U */
        switch (type) {
        case 0: /* float32 */
        case 1: /* float64 */
            break;
        case 3: /* float16 */
            if (!dc_isar_feature(aa64_fp16, s)) {
                goto do_unallocated;
            }
            break;
        default:
            goto do_unallocated;
        }
        if (!fp_access_check(s)) {
            return;
        }
        handle_fpfpcvt(s, rd, rn, opcode, itof, rmode, 64, sf, type);
        break;

    default:
        switch (sf << 7 | type << 5 | rmode << 3 | opcode) {
        case 0b01100110: /* FMOV half <-> 32-bit int */
        case 0b01100111:
        case 0b11100110: /* FMOV half <-> 64-bit int */
        case 0b11100111:
            if (!dc_isar_feature(aa64_fp16, s)) {
                goto do_unallocated;
            }
            /* fallthru */
        case 0b00000110: /* FMOV 32-bit */
        case 0b00000111:
        case 0b10100110: /* FMOV 64-bit */
        case 0b10100111:
        case 0b11001110: /* FMOV top half of 128-bit */
        case 0b11001111:
            if (!fp_access_check(s)) {
                return;
            }
            itof = opcode & 1;
            handle_fmov(s, rd, rn, type, itof);
            break;

        case 0b00111110: /* FJCVTZS */
            if (!dc_isar_feature(aa64_jscvt, s)) {
                goto do_unallocated;
            } else if (fp_access_check(s)) {
                handle_fjcvtzs(s, rd, rn);
            }
            break;

        default:
        do_unallocated:
            lata_unallocated_encoding(s);
            return;
        }
        break;
    }
}

/* FP-specific subcases of table C3-6 (SIMD and FP data processing)
 *   31  30  29 28     25 24                          0
 * +---+---+---+---------+-----------------------------+
 * |   | 0 |   | 1 1 1 1 |                             |
 * +---+---+---+---------+-----------------------------+
 */
static void disas_data_proc_fp(DisasContext *s, uint32_t insn)
{
    if (extract32(insn, 24, 1)) {
        /* Floating point data-processing (3 source) */
        disas_fp_3src(s, insn);
    } else if (extract32(insn, 21, 1) == 0) {
        /* Floating point to fixed point conversions */
        disas_fp_fixed_conv(s, insn);
    } else {
        switch (extract32(insn, 10, 2)) {
        case 1:
            /* Floating point conditional compare */
            disas_fp_ccomp(s, insn);
            break;
        case 2:
            /* Floating point data-processing (2 source) */
            disas_fp_2src(s, insn);
            break;
        case 3:
            /* Floating point conditional select */
            disas_fp_csel(s, insn);
            break;
        case 0:
            switch (ctz32(extract32(insn, 12, 4))) {
            case 0: /* [15:12] == xxx1 */
                /* Floating point immediate */
                disas_fp_imm(s, insn);
                break;
            case 1: /* [15:12] == xx10 */
                /* Floating point compare */
                disas_fp_compare(s, insn);
                break;
            case 2: /* [15:12] == x100 */
                /* Floating point data-processing (1 source) */
                disas_fp_1src(s, insn);
                break;
            case 3: /* [15:12] == 1000 */
                lata_unallocated_encoding(s);
                break;
            default: /* [15:12] == 0000 */
                /* Floating point <-> integer conversions */
                disas_fp_int_conv(s, insn);
                break;
            }
            break;
        }
    }
}

/* EXT
 *   31  30 29         24 23 22  21 20  16 15  14  11 10  9    5 4    0
 * +---+---+-------------+-----+---+------+---+------+---+------+------+
 * | 0 | Q | 1 0 1 1 1 0 | op2 | 0 |  Rm  | 0 | imm4 | 0 |  Rn  |  Rd  |
 * +---+---+-------------+-----+---+------+---+------+---+------+------+
 */
static void disas_simd_ext(DisasContext *s, uint32_t insn)
{
    int is_q = extract32(insn, 30, 1);
    int op2 = extract32(insn, 22, 2);
    int imm4 = extract32(insn, 11, 4);
    int rm = extract32(insn, 16, 5);
    int rn = extract32(insn, 5, 5);
    int rd = extract32(insn, 0, 5);

    if (op2 != 0 || (!is_q && extract32(imm4, 3, 1))) {
        lata_unallocated_encoding(s);
        return;
    }

    if (!fp_access_check(s)) {
        return;
    }

    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND vreg_m = alloc_fpr_src(rm);
    IR2_OPND vtemp = ra_alloc_ftemp();
    IR2_OPND vtemp1 = ra_alloc_ftemp();

    /* Vd gets bits starting at pos bits into Vm:Vn. This is
     * either extracting 128 bits from a 128:128 concatenation, or
     * extracting 64 bits from a 64:64 concatenation.
     */
    if(!is_q){
        if(imm4 == 0){
            la_vori_b(vreg_d, vreg_n, 0);
        }else{
            la_vori_b(vtemp, vreg_n, 0);
            /* 高64位清零 */
            la_vinsgr2vr_d(vtemp, zero_ir2_opnd, 1);
            la_vbsrl_v(vtemp, vtemp, imm4);
            la_vbsll_v(vtemp1, vreg_m, 0x8 - imm4);
            la_vor_v(vreg_d, vtemp, vtemp1);
        }
        /* 高64位清零 */
        la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
    }else{
        if(imm4 == 0){
            la_vori_b(vreg_d, vreg_n, 0);
        }else{
            la_vbsrl_v(vtemp, vreg_n, imm4);
            la_vbsll_v(vtemp1, vreg_m, 0x10 - imm4);
            la_vor_v(vreg_d, vtemp, vtemp1);
        }
    }
    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_m);
    free_alloc_fpr(vreg_n);
    free_alloc_fpr(vtemp);
    free_alloc_fpr(vtemp1);
}

/* TBL/TBX
 *   31  30 29         24 23 22  21 20  16 15  14 13  12  11 10 9    5 4    0
 * +---+---+-------------+-----+---+------+---+-----+----+-----+------+------+
 * | 0 | Q | 0 0 1 1 1 0 | op2 | 0 |  Rm  | 0 | len | op | 0 0 |  Rn  |  Rd  |
 * +---+---+-------------+-----+---+------+---+-----+----+-----+------+------+
 */
static void disas_simd_tb(DisasContext *s, uint32_t insn)
{
    int op2 = extract32(insn, 22, 2);
    int is_q = extract32(insn, 30, 1);
    int rm = extract32(insn, 16, 5);
    int rn = extract32(insn, 5, 5);
    int rd = extract32(insn, 0, 5);
    int is_tbx = extract32(insn, 12, 1);
    int len = (extract32(insn, 13, 2) + 1) * 16;

    if (op2 != 0) {
        lata_unallocated_encoding(s);
        return;
    }

    if (!fp_access_check(s)) {
        return;
    }
    IR2_OPND temp = ra_alloc_itemp();

    li_d(temp, s->base->pc_next);
    la_st_d(temp, env_ir2_opnd, env_offset_pc());

    lata_gen_call_helper_prologue(tcg_ctx);
    /* void HELPER(simd_tblx)(void *vd, void *vm, void *venv, uint32_t desc) */
    la_mov64(a2_ir2_opnd, env_ir2_opnd);
    li_d(a0_ir2_opnd, vec_full_reg_offset(s, rd));
    li_d(a1_ir2_opnd, vec_full_reg_offset(s, rm));
    la_add_d(a0_ir2_opnd, a0_ir2_opnd, a2_ir2_opnd);
    la_add_d(a1_ir2_opnd, a1_ir2_opnd, a2_ir2_opnd);
    li_d(a3_ir2_opnd, simd_desc(is_q ? 16 : 8, 
                                vec_full_reg_size(s), 
                                (len << 6) | (is_tbx << 5) | rn));
    li_d(temp, (int64_t)helper_simd_tblx);
    la_jirl(ra_ir2_opnd, temp, 0);
    lata_gen_call_helper_epilogue(tcg_ctx);

    free_alloc_gpr(temp);
}

/* ZIP/UZP/TRN
 *   31  30 29         24 23  22  21 20   16 15 14 12 11 10 9    5 4    0
 * +---+---+-------------+------+---+------+---+------------------+------+
 * | 0 | Q | 0 0 1 1 1 0 | size | 0 |  Rm  | 0 | opc | 1 0 |  Rn  |  Rd  |
 * +---+---+-------------+------+---+------+---+------------------+------+
 */
static void disas_simd_zip_trn(DisasContext *s, uint32_t insn)
{
    int rd = extract32(insn, 0, 5);
    int rn = extract32(insn, 5, 5);
    int rm = extract32(insn, 16, 5);
    int size = extract32(insn, 22, 2);
    /* opc field bits [1:0] indicate ZIP/UZP/TRN;
     * bit 2 indicates 1 vs 2 variant of the insn.
     */
    int opcode = extract32(insn, 12, 2);
    bool part = extract32(insn, 14, 1);
    bool is_q = extract32(insn, 30, 1);

    if (opcode == 0 || (size == 3 && !is_q)){
        lata_unallocated_encoding(s);
        return;
    }

    if (!fp_access_check(s)){
        return;
    }

    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND vreg_m = alloc_fpr_src(rm);
    IR2_OPND vtemp = ra_alloc_ftemp();
    IR2_OPND vtemp1 = ra_alloc_ftemp();

    switch (opcode){
    case 1:        /* UZP1/2 */
        if (!part) /* UZP1 */{
            if (!is_q){
                switch (size){
                case 0:
                    la_vpickev_b(vtemp, vreg_m, vreg_n);
                    la_vpickev_b(vtemp1, vreg_m, vreg_n);
                    la_vbsrl_v(vtemp1, vtemp1, 8);
                    la_vpackev_w(vreg_d, vtemp1, vtemp);

                    break;
                case 1:
                    la_vpickev_h(vtemp, vreg_m, vreg_n);
                    la_vpickev_h(vtemp1, vreg_m, vreg_n);
                    la_vbsrl_v(vtemp1, vtemp1, 8);
                    la_vpackev_w(vreg_d, vtemp1, vtemp);

                    break;
                case 2:
                    la_vpickev_w(vtemp, vreg_m, vreg_n);
                    la_vpickev_w(vtemp1, vreg_m, vreg_n);
                    la_vbsrl_v(vtemp1, vtemp1, 8);
                    la_vpackev_w(vreg_d, vtemp1, vtemp);
                    break;
                case 3:
                    la_vpickev_d(vtemp, vreg_m, vreg_n);
                    la_vpickev_d(vtemp1, vreg_m, vreg_n);
                    la_vbsrl_v(vtemp1, vtemp1, 8);
                    la_vpackev_w(vreg_d, vtemp1, vtemp);
                    break;
                default:
                    assert(0);
                }
            }
            else{
                switch (size){
                case 0:
                    la_vpickev_b(vreg_d, vreg_m, vreg_n);
                    break;
                case 1:
                    la_vpickev_h(vreg_d, vreg_m, vreg_n);
                    break;
                case 2:
                    la_vpickev_w(vreg_d, vreg_m, vreg_n);
                    break;
                case 3:
                    la_vpickev_d(vreg_d, vreg_m, vreg_n);
                    break;
                default:
                    assert(0);
                }
            }
        }
        else{ /* UZP2 */
            if (!is_q){
                switch (size){
                case 0:
                    la_vpickod_b(vtemp, vreg_m, vreg_n);
                    la_vpickod_b(vtemp1, vreg_m, vreg_n);
                    la_vbsrl_v(vtemp1, vtemp1, 8);
                    la_vpackev_w(vreg_d, vtemp1, vtemp);

                    break;
                case 1:
                    la_vpickod_h(vtemp, vreg_m, vreg_n);
                    la_vpickod_h(vtemp1, vreg_m, vreg_n);
                    la_vbsrl_v(vtemp1, vtemp1, 8);
                    la_vpackev_w(vreg_d, vtemp1, vtemp);

                    break;
                case 2:
                    la_vpickod_w(vtemp, vreg_m, vreg_n);
                    la_vpickod_w(vtemp1, vreg_m, vreg_n);
                    la_vbsrl_v(vtemp1, vtemp1, 8);
                    la_vpackev_w(vreg_d, vtemp1, vtemp);
                    break;
                case 3:
                    la_vpickod_d(vtemp, vreg_m, vreg_n);
                    la_vpickod_d(vtemp1, vreg_m, vreg_n);
                    la_vbsrl_v(vtemp1, vtemp1, 8);
                    la_vpackev_w(vreg_d, vtemp1, vtemp);
                    break;
                default:
                    assert(0);
                }
            }
            else{
                switch (size){
                case 0:
                    la_vpickod_b(vreg_d, vreg_m, vreg_n);
                    break;
                case 1:
                    la_vpickod_h(vreg_d, vreg_m, vreg_n);
                    break;
                case 2:
                    la_vpickod_w(vreg_d, vreg_m, vreg_n);
                    break;
                case 3:
                    la_vpickod_d(vreg_d, vreg_m, vreg_n);
                    break;
                default:
                    assert(0);
                }
            }
        }
        break;
    case 2: /* TRN1/2 */
        if (!part){
            switch (size){
            case 0:
                la_vpackev_b(vreg_d, vreg_m, vreg_n);
                break;
            case 1:
                la_vpackev_h(vreg_d, vreg_m, vreg_n);
                break;
            case 2:
                la_vpackev_w(vreg_d, vreg_m, vreg_n);
                break;
            case 3:
                la_vpackev_d(vreg_d, vreg_m, vreg_n);
                break;
            default:
                assert(0);
            }
        }
        else{
            switch (size){
            case 0:
                la_vpackod_b(vreg_d, vreg_m, vreg_n);
                break;
            case 1:
                la_vpackod_h(vreg_d, vreg_m, vreg_n);
                break;
            case 2:
                la_vpackod_w(vreg_d, vreg_m, vreg_n);
                break;
            case 3:
                la_vpackod_d(vreg_d, vreg_m, vreg_n);
                break;
            default:
                assert(0);
            }
        }

        break;
    case 3: /* ZIP1/2 */
        if (!part){
            switch (size){
            case 0:
                la_vilvl_b(vreg_d, vreg_m, vreg_n);
                break;
            case 1:
                la_vilvl_h(vreg_d, vreg_m, vreg_n);
                break;
            case 2:
                la_vilvl_w(vreg_d, vreg_m, vreg_n);
                break;
            case 3:
                la_vilvl_d(vreg_d, vreg_m, vreg_n);
                break;
            default:
                assert(0);
            }
        }
        else
        {
            if (!is_q){
                switch (size){
                case 0:
                    la_vbsll_v(vtemp, vreg_n, 8);
                    la_vbsll_v(vtemp1, vreg_m, 8);
                    la_vilvh_b(vtemp, vtemp1, vtemp);
                    la_vbsrl_v(vreg_d, vtemp, 8);
                    break;
                case 1:
                    la_vbsll_v(vtemp, vreg_n, 8);
                    la_vbsll_v(vtemp1, vreg_m, 8);
                    la_vilvh_h(vtemp, vtemp1, vtemp);
                    la_vbsrl_v(vreg_d, vtemp, 8);
                    break;
                case 2:
                    la_vbsll_v(vtemp, vreg_n, 8);
                    la_vbsll_v(vtemp1, vreg_m, 8);
                    la_vilvh_w(vtemp, vtemp1, vtemp);
                    la_vbsrl_v(vreg_d, vtemp, 8);
                    break;
                case 3:
                    la_vbsll_v(vtemp, vreg_n, 8);
                    la_vbsll_v(vtemp1, vreg_m, 8);
                    la_vilvh_d(vtemp, vtemp1, vtemp);
                    la_vbsrl_v(vreg_d, vtemp, 8);
                    break;
                default:
                    assert(0);
                }
            }
            else{
                switch (size){
                case 0:
                    la_vilvh_b(vreg_d, vreg_m, vreg_n);
                    break;
                case 1:
                    la_vilvh_h(vreg_d, vreg_m, vreg_n);
                    break;
                case 2:
                    la_vilvh_w(vreg_d, vreg_m, vreg_n);
                    break;
                case 3:
                    la_vilvh_d(vreg_d, vreg_m, vreg_n);
                    break;
                default:
                    assert(0);
                }
            }
        }

        break;
    default:
        g_assert_not_reached();
    }

    /* 高64位清零 */
    if (!is_q){
        la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
    }
    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
    free_alloc_fpr(vreg_m);
    free_alloc_fpr(vtemp);
    free_alloc_fpr(vtemp1);
}

/* AdvSIMD across lanes
 *   31  30  29 28       24 23  22 21       17 16    12 11 10 9    5 4    0
 * +---+---+---+-----------+------+-----------+--------+-----+------+------+
 * | 0 | Q | U | 0 1 1 1 0 | size | 1 1 0 0 0 | opcode | 1 0 |  Rn  |  Rd  |
 * +---+---+---+-----------+------+-----------+--------+-----+------+------+
 */
static void disas_simd_across_lanes(DisasContext *s, uint32_t insn)
{
    int rd = extract32(insn, 0, 5);
    int rn = extract32(insn, 5, 5);
    int size = extract32(insn, 22, 2);
    int opcode = extract32(insn, 12, 5);
    bool is_q = extract32(insn, 30, 1);
    bool is_u = extract32(insn, 29, 1);
    bool is_min = extract32(insn, 16, 1);;
    int esize = 8 << size;
    int elements = (is_q ? 128 : 64) / esize;
    int i;

    if (!fp_access_check(s)) {
        return;
    }

    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND vtemp = ra_alloc_ftemp();
    IR2_OPND vtemp2 = ra_alloc_ftemp();
    

    switch (opcode) {
    case 0x1b: /* ADDV */
        if (is_u || size == 3 || (size == 2 && !is_q)) {
            lata_unallocated_encoding(s);
            return;
        }

        la_vslli_d(vtemp2, vreg_n, 0);
        for (i = 1; i < elements; i++) {
            if(is_q && i >= elements/2){
                la_vpickod_d(vtemp, vreg_n, vreg_n);
                la_vsrli_d(vtemp, vtemp, esize * (i - elements/2) );
            }
            else{
                la_vpickev_d(vtemp, vreg_n, vreg_n);
                la_vsrli_d(vtemp, vtemp, esize * i);
            }
            switch (size) {
            case 0: 
                la_vadd_b(vtemp2, vtemp2, vtemp);
                break;
            case 1: 
                la_vadd_h(vtemp2, vtemp2, vtemp);
                break;
            case 2: 
                la_vadd_w(vtemp2, vtemp2, vtemp);
                break;
            default:
                assert(0);
            }
        }

        la_vslli_d(vtemp2, vtemp2, 64-esize);
        la_vsrli_d(vreg_d, vtemp2, 64-esize);
        la_vinsgr2vr_d(vreg_d,zero_ir2_opnd,1);

        break;
    case 0x3: /* SADDLV, UADDLV */
    case 0xa: /* SMAXV, UMAXV */
    case 0x1a: /* SMINV, UMINV */
        if (size == 3 || (size == 2 && !is_q)) {
            lata_unallocated_encoding(s);
            return;
        }
        la_vslli_d(vtemp2, vreg_n, 64 - esize);
        la_vsrli_d(vtemp2, vtemp2, 64 - esize);
        for (i = 1; i < elements; i++) {
            if(is_q && i >= elements/2){
                la_vpickod_d(vtemp, vreg_n, vreg_n);
                la_vsrli_d(vtemp, vtemp, esize * (i - elements/2) );
            }
            else{
                la_vpickev_d(vtemp, vreg_n, vreg_n);
                la_vsrli_d(vtemp, vtemp, esize * i);
            }
            /* SMINV, UMINV */
            if(is_min){
                switch (size) {
                case 0: 
                    if(is_u){
                        la_vmin_bu(vtemp2, vtemp2, vtemp); 
                    }
                    else{
                        la_vmin_b(vtemp2, vtemp2, vtemp); 
                    }
                    break;
                case 1: 
                    if(is_u){
                        la_vmin_hu(vtemp2, vtemp2, vtemp); 
                    }
                    else{
                        la_vmin_h(vtemp2, vtemp2, vtemp); 
                    }
                    break;
                case 2: 
                    if(is_u){
                        la_vmin_wu(vtemp2, vtemp2, vtemp); 
                    }
                    else{
                        la_vmin_w(vtemp2, vtemp2, vtemp); 
                    }
                    break;
                default:
                    assert(0);
                }
            }
            /* SMAXV, UMAXV */
            else{
                switch (size) {
                case 0: 
                    if(is_u){
                        la_vmax_bu(vtemp2, vtemp2, vtemp); 
                    }
                    else{
                        la_vmax_b(vtemp2, vtemp2, vtemp); 
                    }
                    break;
                case 1: 
                    if(is_u){
                        la_vmax_hu(vtemp2, vtemp2, vtemp); 
                    }
                    else{
                        la_vmax_h(vtemp2, vtemp2, vtemp); 
                    }
                    break;
                case 2: 
                    if(is_u){
                        la_vmax_wu(vtemp2, vtemp2, vtemp); 
                    }
                    else{
                        la_vmax_w(vtemp2, vtemp2, vtemp); 
                    }
                    break;
                default:
                    assert(0);
                }                
            }
        }

        la_vslli_d(vtemp2, vtemp2, 64-esize);
        la_vsrli_d(vreg_d, vtemp2, 64-esize);
        la_vinsgr2vr_d(vreg_d,zero_ir2_opnd,1);
        break;
    case 0xc: /* FMAXNMV, FMINNMV */
    case 0xf: /* FMAXV, FMINV */
        is_min = extract32(size, 1, 1);
        if (!is_u && dc_isar_feature(aa64_fp16, s)) {
            size = 1;
        } else if (!is_u || !is_q || extract32(size, 0, 1)) {
            lata_unallocated_encoding(s);
            return;
        } else {
            size = 2;
        }   
        assert(opcode == 0xc && size == 2);

        la_vori_b(vtemp2, vreg_n, 0);
        la_vreplvei_w(vreg_d, vreg_n, 0);
        for(i = 1; i < (is_q ? 4 : 2); ++i){
            la_vreplvei_w(vtemp, vtemp2, i);
            if(is_min){
                la_fmin_s(vreg_d, vreg_d, vtemp);
            }else{
                la_fmax_s(vreg_d, vreg_d, vtemp);
            }
        }
        la_vand_v(vreg_d,vreg_d, fsmask_ir2_opnd);
        break;
    default:
        lata_unallocated_encoding(s);
        return;
    }

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
    free_alloc_fpr(vtemp);
    free_alloc_fpr(vtemp2);
}

/* DUP (Element, Vector)
 *
 *  31  30   29              21 20    16 15        10  9    5 4    0
 * +---+---+-------------------+--------+-------------+------+------+
 * | 0 | Q | 0 0 1 1 1 0 0 0 0 |  imm5  | 0 0 0 0 0 1 |  Rn  |  Rd  |
 * +---+---+-------------------+--------+-------------+------+------+
 *
 * size: encoded in imm5 (see ARM ARM LowestSetBit())
 */
static void handle_simd_dupe(DisasContext *s, int is_q, int rd, int rn,
                             int imm5)
{
    int size = ctz32(imm5);
    int index;

    if (size > 3 || (size == 3 && !is_q)) {
        lata_unallocated_encoding(s);
        return;
    }

    if (!fp_access_check(s)) {
        return;
    }

    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);

    index = imm5 >> (size + 1);
    switch (size)
    {
    case 0:
        la_vreplvei_b(vreg_d, vreg_n, index);
        break;
    case 1:
        la_vreplvei_h(vreg_d, vreg_n, index);
        break;
    case 2:
        la_vreplvei_w(vreg_d, vreg_n, index);
        break;
    case 3:
        la_vreplvei_d(vreg_d, vreg_n, index);
        break;
    default:
        assert(0);
        break;
    }
    
    if(!is_q){
        /* 高64位清零 */
        la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
    }

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
}

/* DUP (element, scalar)
 *  31                   21 20    16 15        10  9    5 4    0
 * +-----------------------+--------+-------------+------+------+
 * | 0 1 0 1 1 1 1 0 0 0 0 |  imm5  | 0 0 0 0 0 1 |  Rn  |  Rd  |
 * +-----------------------+--------+-------------+------+------+
 */
static void handle_simd_dupes(DisasContext *s, int rd, int rn,
                              int imm5)
{
    int size = ctz32(imm5);
    int index;
    // TCGv_i64 tmp;

    if (size > 3) {
        lata_unallocated_encoding(s);
        return;
    }

    if (!fp_access_check(s)) {
        return;
    }

    index = imm5 >> (size + 1);

    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND vtemp;

    if(rd != rn){
        la_vxor_v(vreg_d, vreg_d, vreg_d);
        switch (size)
        {
        case 0:
            la_vextrins_b(vreg_d, vreg_n, index);
            break;
        case 1:
            la_vextrins_h(vreg_d, vreg_n, index);
            break;
        case 2:
            la_vextrins_w(vreg_d, vreg_n, index);
            break;
        case 3:
            la_vextrins_d(vreg_d, vreg_n, index);
            break;
        default:
            assert(0);
            break;
        }
    }else{
        vtemp = ra_alloc_ftemp();
        
        la_vori_b(vtemp, vreg_n, 0);
        la_vxor_v(vreg_d, vreg_d, vreg_d);
        switch (size)
        {
        case 0:
            la_vextrins_b(vreg_d, vtemp, index);
            break;
        case 1:
            la_vextrins_h(vreg_d, vtemp, index);
            break;
        case 2:
            la_vextrins_w(vreg_d, vtemp, index);
            break;
        case 3:
            la_vextrins_d(vreg_d, vtemp, index);
            break;
        default:
            assert(0);
            break;
        }
    }

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
    if(rd == rn){
        free_alloc_fpr(vtemp);
    }
}

/* DUP (General)
 *
 *  31  30   29              21 20    16 15        10  9    5 4    0
 * +---+---+-------------------+--------+-------------+------+------+
 * | 0 | Q | 0 0 1 1 1 0 0 0 0 |  imm5  | 0 0 0 0 1 1 |  Rn  |  Rd  |
 * +---+---+-------------------+--------+-------------+------+------+
 *
 * size: encoded in imm5 (see ARM ARM LowestSetBit())
 */
static void handle_simd_dupg(DisasContext *s, int is_q, int rd, int rn,
                             int imm5)
{
    int size = ctz32(imm5);

    if (size > 3 || ((size == 3) && !is_q)) {
        lata_unallocated_encoding(s);
        return;
    }

    if (!fp_access_check(s)) {
        return;
    }

    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND reg_n = alloc_gpr_src(rn);

    switch (size)
    {
    case 0:
        la_vreplgr2vr_b(vreg_d, reg_n);
        break;
    case 1:
        la_vreplgr2vr_h(vreg_d, reg_n);
        break;
    case 2:
        la_vreplgr2vr_w(vreg_d, reg_n);
        break;
    case 3:
        la_vreplgr2vr_d(vreg_d, reg_n);
        break;
    default:
        assert(0);
        break;
    }
    
    if(!is_q){
        /* 高64位清零 */
        la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
    }

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_gpr(reg_n);
}

/* INS (Element)
 *
 *  31                   21 20    16 15  14    11  10 9    5 4    0
 * +-----------------------+--------+------------+---+------+------+
 * | 0 1 1 0 1 1 1 0 0 0 0 |  imm5  | 0 |  imm4  | 1 |  Rn  |  Rd  |
 * +-----------------------+--------+------------+---+------+------+
 *
 * size: encoded in imm5 (see ARM ARM LowestSetBit())
 * index: encoded in imm5<4:size+1>
 */
static void handle_simd_inse(DisasContext *s, int rd, int rn,
                             int imm4, int imm5)
{
    int size = ctz32(imm5);
    int src_index, dst_index;

    if (size > 3) {
        lata_unallocated_encoding(s);
        return;
    }

    if (!fp_access_check(s)) {
        return;
    }

    IR2_OPND vreg_d = alloc_fpr_src(rd); /* 其他bit不变，需要源寄存器 */
    IR2_OPND vreg_n = alloc_fpr_src(rn);

    dst_index = extract32(imm5, 1+size, 5);
    src_index = extract32(imm4, size, 4);
    int index = (dst_index << 4) + src_index;
    switch (size)
    {
    case 0:
        la_vextrins_b(vreg_d, vreg_n, index);
        break;
    case 1:
        la_vextrins_h(vreg_d, vreg_n, index);
        break;
    case 2:
        la_vextrins_w(vreg_d, vreg_n, index);
        break;
    case 3:
        la_vextrins_d(vreg_d, vreg_n, index);
        break;
    default:
        assert(0);
        break;
    }

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
}


/* INS (General)
 *
 *  31                   21 20    16 15        10  9    5 4    0
 * +-----------------------+--------+-------------+------+------+
 * | 0 1 0 0 1 1 1 0 0 0 0 |  imm5  | 0 0 0 1 1 1 |  Rn  |  Rd  |
 * +-----------------------+--------+-------------+------+------+
 *
 * size: encoded in imm5 (see ARM ARM LowestSetBit())
 * index: encoded in imm5<4:size+1>
 */
static void handle_simd_insg(DisasContext *s, int rd, int rn, int imm5)
{
    int size = ctz32(imm5);
    int idx;

    if (size > 3) {
        lata_unallocated_encoding(s);
        return;
    }

    if (!fp_access_check(s)) {
        return;
    }

    IR2_OPND vreg_d = alloc_fpr_src(rd); /* 其他bit不变，需要源寄存器 */
    IR2_OPND reg_n = alloc_gpr_src(rn);

    idx = extract32(imm5, 1 + size, 4 - size);
    switch (size)
    {
    case 0:
        la_vinsgr2vr_b(vreg_d, reg_n, idx);
        break;
    case 1:
        la_vinsgr2vr_h(vreg_d, reg_n, idx);
        break;
    case 2:
        la_vinsgr2vr_w(vreg_d, reg_n, idx);
        break;
    case 3:
        la_vinsgr2vr_d(vreg_d, reg_n, idx);
        break;
    default:
        assert(0);
        break;
    }

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_gpr(reg_n);
}

/*
 * UMOV (General)
 * SMOV (General)
 *
 *  31  30   29              21 20    16 15    12   10 9    5 4    0
 * +---+---+-------------------+--------+-------------+------+------+
 * | 0 | Q | 0 0 1 1 1 0 0 0 0 |  imm5  | 0 0 1 U 1 1 |  Rn  |  Rd  |
 * +---+---+-------------------+--------+-------------+------+------+
 *
 * U: unsigned when set
 * size: encoded in imm5 (see ARM ARM LowestSetBit())
 */
static void handle_simd_umov_smov(DisasContext *s, int is_q, int is_signed,
                                  int rn, int rd, int imm5)
{
    int size = ctz32(imm5);
    int element;

    /* Check for UnallocatedEncodings */
    if (is_signed) {
        if (size > 2 || (size == 2 && !is_q)) {
            lata_unallocated_encoding(s);
            return;
        }
    } else {
        if (size > 3
            || (size < 3 && is_q)
            || (size == 3 && !is_q)) {
            lata_unallocated_encoding(s);
            return;
        }
    }

    if (!fp_access_check(s)) {
        return;
    }

    element = extract32(imm5, 1+size, 4);
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND reg_d = alloc_gpr_dst(rd);

    switch (size)
    {
    case 0:
        if(is_signed){
            la_vpickve2gr_b(reg_d, vreg_n, element);
        }else{
            la_vpickve2gr_bu(reg_d, vreg_n, element);
        }
        break;
    case 1:
        if(is_signed){
            la_vpickve2gr_h(reg_d, vreg_n, element);
        }else{
            la_vpickve2gr_hu(reg_d, vreg_n, element);
        }
        break;
    case 2:
        if(is_signed){
            la_vpickve2gr_w(reg_d, vreg_n, element);
        }else{
            la_vpickve2gr_wu(reg_d, vreg_n, element);
        }
        break;
    case 3:
        if(is_signed){
            la_vpickve2gr_d(reg_d, vreg_n, element);
        }else{
            la_vpickve2gr_du(reg_d, vreg_n, element);
        }
        break;;
    default:
        assert(0);
    }

    if (rd != 31 && arm_la_map[rd] >= 0 && clearGprHigh) {
        set_w_write_flag(rd, !(is_signed && !is_q));
    } else {
        if(is_signed && !is_q){
            la_bstrpick_d(reg_d, reg_d, 31, 0);
        }
    }

    store_gpr_dst(rd, reg_d);
    free_alloc_fpr(vreg_n);
    free_alloc_gpr(reg_d);
}

/* AdvSIMD copy
 *   31  30  29  28             21 20  16 15  14  11 10  9    5 4    0
 * +---+---+----+-----------------+------+---+------+---+------+------+
 * | 0 | Q | op | 0 1 1 1 0 0 0 0 | imm5 | 0 | imm4 | 1 |  Rn  |  Rd  |
 * +---+---+----+-----------------+------+---+------+---+------+------+
 */
static void disas_simd_copy(DisasContext *s, uint32_t insn)
{
    int rd = extract32(insn, 0, 5);
    int rn = extract32(insn, 5, 5);
    int imm4 = extract32(insn, 11, 4);
    int op = extract32(insn, 29, 1);
    int is_q = extract32(insn, 30, 1);
    int imm5 = extract32(insn, 16, 5);

    if (op) {
        if (is_q) {
            /* INS (element) */
            handle_simd_inse(s, rd, rn, imm4, imm5);
        } else {
            lata_unallocated_encoding(s);
        }
    } else {
        switch (imm4) {
        case 0:
            /* DUP (element - vector) */
            handle_simd_dupe(s, is_q, rd, rn, imm5);
            break;
        case 1:
            /* DUP (general) */
            handle_simd_dupg(s, is_q, rd, rn, imm5);
            break;
        case 3:
            if (is_q) {
                /* INS (general) */
                handle_simd_insg(s, rd, rn, imm5);
            } else {
                lata_unallocated_encoding(s);
            }
            break;
        case 5:
        case 7:
            /* UMOV/SMOV (is_q indicates 32/64; imm4 indicates signedness) */
            handle_simd_umov_smov(s, is_q, (imm4 == 5), rn, rd, imm5);
            break;
        default:
            lata_unallocated_encoding(s);
            break;
        }
    }
}

/* AdvSIMD modified immediate
 *  31  30   29  28                 19 18 16 15   12  11  10  9     5 4    0
 * +---+---+----+---------------------+-----+-------+----+---+-------+------+
 * | 0 | Q | op | 0 1 1 1 1 0 0 0 0 0 | abc | cmode | o2 | 1 | defgh |  Rd  |
 * +---+---+----+---------------------+-----+-------+----+---+-------+------+
 *
 * There are a number of operations that can be carried out here:
 *   MOVI - move (shifted) imm into register
 *   MVNI - move inverted (shifted) imm into register
 *   ORR  - bitwise OR of (shifted) imm with register
 *   BIC  - bitwise clear of (shifted) imm with register
 * With ARMv8.2 we also have:
 *   FMOV half-precision
 */
static void disas_simd_mod_imm(DisasContext *s, uint32_t insn)
{
    int rd = extract32(insn, 0, 5);
    int cmode = extract32(insn, 12, 4);
    int o2 = extract32(insn, 11, 1);
    uint64_t abcdefgh = extract32(insn, 5, 5) | (extract32(insn, 16, 3) << 5);
    bool is_neg = extract32(insn, 29, 1);
    bool is_q = extract32(insn, 30, 1);
    uint64_t imm = 0;

    if (o2 != 0 || ((cmode == 0xf) && is_neg && !is_q)) {
        /* Check for FMOV (vector, immediate) - half-precision */
        if (!(dc_isar_feature(aa64_fp16, s) && o2 && cmode == 0xf)) {
            lata_unallocated_encoding(s);
            return;
        }
    }

    if (!fp_access_check(s)) {
        return;
    }

    if (cmode == 15 && o2 && !is_neg) {
        /* FMOV (vector, immediate) - half-precision */
        imm = vfp_expand_imm(MO_16, abcdefgh);
        /* now duplicate across the lanes */
        imm = dup_const(MO_16, imm);
    } else {
        imm = asimd_imm_const(abcdefgh, cmode, is_neg);
    }
    IR2_OPND vreg_d;
    IR2_OPND vtemp = ra_alloc_ftemp();
    IR2_OPND temp = ra_alloc_itemp();
    li_d(temp, (int64_t)imm);
    if (!((cmode & 0x9) == 0x1 || (cmode & 0xd) == 0x9)) {
        /* MOVI or MVNI, with MVNI negation handled above.  */
        vreg_d = alloc_fpr_dst(rd);
        la_vreplgr2vr_d(vreg_d, temp);
    } else {
        // assert(0);
        /* ORR or BIC, with BIC negation to AND handled above.  */
        vreg_d = alloc_fpr_src(rd);
        la_vreplgr2vr_d(vtemp, temp);
        if (is_neg) {
            la_vand_v(vreg_d, vreg_d, vtemp);
        } else {
            la_vor_v(vreg_d, vreg_d, vtemp);
        }
    }

    if(!is_q){
        /* 高64位清零 */
        la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
    }
    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vtemp);
    free_alloc_gpr(temp);
}

/* AdvSIMD scalar copy
 *  31 30  29  28             21 20  16 15  14  11 10  9    5 4    0
 * +-----+----+-----------------+------+---+------+---+------+------+
 * | 0 1 | op | 1 1 1 1 0 0 0 0 | imm5 | 0 | imm4 | 1 |  Rn  |  Rd  |
 * +-----+----+-----------------+------+---+------+---+------+------+
 */
static void disas_simd_scalar_copy(DisasContext *s, uint32_t insn)
{
    int rd = extract32(insn, 0, 5);
    int rn = extract32(insn, 5, 5);
    int imm4 = extract32(insn, 11, 4);
    int imm5 = extract32(insn, 16, 5);
    int op = extract32(insn, 29, 1);

    if (op != 0 || imm4 != 0) {
        lata_unallocated_encoding(s);
        return;
    }

    /* DUP (element, scalar) */
    handle_simd_dupes(s, rd, rn, imm5);
}

/* AdvSIMD scalar pairwise
 *  31 30  29 28       24 23  22 21       17 16    12 11 10 9    5 4    0
 * +-----+---+-----------+------+-----------+--------+-----+------+------+
 * | 0 1 | U | 1 1 1 1 0 | size | 1 1 0 0 0 | opcode | 1 0 |  Rn  |  Rd  |
 * +-----+---+-----------+------+-----------+--------+-----+------+------+
 */
static void disas_simd_scalar_pairwise(DisasContext *s, uint32_t insn)
{
    int u = extract32(insn, 29, 1);
    int size = extract32(insn, 22, 2);
    int opcode = extract32(insn, 12, 5);
    int rn = extract32(insn, 5, 5);
    int rd = extract32(insn, 0, 5);

    /* For some ops (the FP ones), size[1] is part of the encoding.
     * For ADDP strictly it is not but size[1] is always 1 for valid
     * encodings.
     */
    opcode |= (extract32(size, 1, 1) << 5);

    switch (opcode) {
    case 0x3b: /* ADDP */
        if (u || size != 3) {
            lata_unallocated_encoding(s);
            return;
        }
        if (!fp_access_check(s)) {
            return;
        }

        break;
    case 0xc: /* FMAXNMP */
    case 0xd: /* FADDP */
    case 0xf: /* FMAXP */
    case 0x2c: /* FMINNMP */
    case 0x2f: /* FMINP */
        /* FP op, size[0] is 32 or 64 bit*/
        if (!u) {
            if (!dc_isar_feature(aa64_fp16, s)) {
                lata_unallocated_encoding(s);
                return;
            } else {
                size = MO_16;
            }
        } else {
            size = extract32(size, 0, 1) ? MO_64 : MO_32;
        }

        if (!fp_access_check(s)) {
            return;
        }

        break;
    default:
        lata_unallocated_encoding(s);
        return;
    }

    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND vtemp = ra_alloc_ftemp();
    if (size == MO_64) {
        la_vreplvei_d(vtemp, vreg_n, 1);

        switch (opcode) {
        case 0x3b: /* ADDP */
            // tcg_gen_add_i64(tcg_res, tcg_op1, tcg_op2);
            la_vadd_d(vreg_d, vtemp, vreg_n);
            break;
        case 0xc: /* FMAXNMP */
            // gen_helper_vfp_maxnumd(tcg_res, tcg_op1, tcg_op2, fpst);
            la_vfmax_d(vreg_d, vreg_n, vtemp);
            break;
        case 0xd: /* FADDP */
            // gen_helper_vfp_addd(tcg_res, tcg_op1, tcg_op2, fpst);
            la_vfadd_d(vreg_d, vtemp, vreg_n);
            break;
        case 0xf: /* FMAXP */
            assert(0);
            // gen_helper_vfp_maxd(tcg_res, tcg_op1, tcg_op2, fpst);
            break;
        case 0x2c: /* FMINNMP */
            // gen_helper_vfp_minnumd(tcg_res, tcg_op1, tcg_op2, fpst);
            la_vfmin_d(vreg_d, vreg_n, vtemp);
            break;
        case 0x2f: /* FMINP */
            assert(0);
            // gen_helper_vfp_mind(tcg_res, tcg_op1, tcg_op2, fpst);
            break;
        default:
            g_assert_not_reached();
        }

    } else {
        assert(0);
        if (size == MO_16) {
            switch (opcode) {
            case 0xc: /* FMAXNMP */
                break;
            case 0xd: /* FADDP */
                break;
            case 0xf: /* FMAXP */
                break;
            case 0x2c: /* FMINNMP */
                break;
            case 0x2f: /* FMINP */
                break;
            default:
                g_assert_not_reached();
            }
        } else {
            switch (opcode) {
            case 0xc: /* FMAXNMP */
                break;
            case 0xd: /* FADDP */
                break;
            case 0xf: /* FMAXP */
                break;
            case 0x2c: /* FMINNMP */
                break;
            case 0x2f: /* FMINP */
                break;
            default:
                g_assert_not_reached();
            }
        }

        la_vand_v(vreg_d,vreg_d, fsmask_ir2_opnd);
    }

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
    free_alloc_fpr(vtemp);
}

/* SSHR[RA]/USHR[RA] - Scalar shift right (optional rounding/accumulate) */
static void handle_scalar_simd_shri(DisasContext *s,
                                    bool is_u, int immh, int immb,
                                    int opcode, int rn, int rd)
{
    int size = 32 - clz32(immh) - 1;
    int immhb = immh << 3 | immb;
    int shift = 2 * (8 << size)- immhb;
    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND vtemp = ra_alloc_ftemp();

    if (extract32(immh, 3, 1)) {
        lata_unallocated_encoding(s);
        return;
    }

    if (!fp_access_check(s)) {
        return;
    }

    la_vbsrl_v(vtemp, vreg_n, 0);

    switch (opcode) {
    case 0x02: /* SSRA / USRA (accumulate) */
        assert(0);
        break;

    case 0x08: /* SRI */
        assert(0);
        break;

    case 0x00: /* SSHR / USHR */
        if (is_u) {
            /* Shift count the same size as element size produces zero.  */
            if(shift == 8 << size){
                la_vandi_b(vreg_d, vtemp, 0);
                break;
            }
            la_vsrli_d(vreg_d,vtemp,shift);
        } else {
            /* Shift count the same size as element size produces all sign.  */
            if(shift == 8 << size){
                shift -= 1;
            }
            la_vsrai_d(vreg_d,vtemp,shift);
        }
        assert(0);
        break;

    case 0x04: /* SRSHR / URSHR (rounding) */
        assert(0);
        break;

    case 0x06: /* SRSRA / URSRA (accum + rounding) */
        assert(0);
        break;

    default:
        g_assert_not_reached();
    }

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
    free_alloc_fpr(vtemp);
}

/* SHL/SLI - Scalar shift left */
static void handle_scalar_simd_shli(DisasContext *s, bool insert,
                                    int immh, int immb, int opcode,
                                    int rn, int rd)
{
    assert(!insert);

    int size = 32 - clz32(immh) - 1;
    int immhb = immh << 3 | immb;
    int shift = immhb - (8 << size);
    // TCGv_i64 tcg_rn;
    // TCGv_i64 tcg_rd;

    if (!extract32(immh, 3, 1)) {
        lata_unallocated_encoding(s);
        return;
    }

    if (!fp_access_check(s)) {
        return;
    }

    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);

    la_vslli_d(vreg_d, vreg_n, shift);
    /* 高64位清零 */
    la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
}

/* SQSHRN/SQSHRUN - Saturating (signed/unsigned) shift right with
 * (signed/unsigned) narrowing */
static void handle_vec_simd_sqshrn(DisasContext *s, bool is_scalar, bool is_q,
                                   bool is_u_shift, bool is_u_narrow,
                                   int immh, int immb, int opcode,
                                   int rn, int rd)
{
    assert(0);
}

/* SQSHLU, UQSHL, SQSHL: saturating left shifts */
static void handle_simd_qshl(DisasContext *s, bool scalar, bool is_q,
                             bool src_unsigned, bool dst_unsigned,
                             int immh, int immb, int rn, int rd)
{
    assert(0);
}

/* Common vector code for handling integer to FP conversion */
static void handle_simd_intfp_conv(DisasContext *s, int rd, int rn,
                                   int elements, int is_signed,
                                   int fracbits, int size)
{
    assert(!fracbits);
    
    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);

    if (size == MO_64) {

        if (is_signed) {
            la_vffint_d_l(vreg_d, vreg_n);
        } else {
            la_vffint_d_lu(vreg_d, vreg_n);
        }

    } else {

        switch (size) {
        case MO_32:
            if (is_signed) {
                la_vffint_s_w(vreg_d, vreg_n);
            } else {
                la_vffint_s_wu(vreg_d, vreg_n);
            }
            break;
        case MO_16:
            assert(0);
            break;
        default:
            g_assert_not_reached();
        }

    }

    if(elements * (8 << size) < 128){
        /* 高64位清零 */
        la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
    }
    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
}

/* UCVTF/SCVTF - Integer to FP conversion */
static void handle_simd_shift_intfp_conv(DisasContext *s, bool is_scalar,
                                         bool is_q, bool is_u,
                                         int immh, int immb, int opcode,
                                         int rn, int rd)
{
    int size, elements, fracbits;
    int immhb = immh << 3 | immb;

    if (immh & 8) {
        size = MO_64;
        if (!is_scalar && !is_q) {
            lata_unallocated_encoding(s);
            return;
        }
    } else if (immh & 4) {
        size = MO_32;
    } else if (immh & 2) {
        size = MO_16;
        if (!dc_isar_feature(aa64_fp16, s)) {
            lata_unallocated_encoding(s);
            return;
        }
    } else {
        /* immh == 0 would be a failure of the decode logic */
        g_assert(immh == 1);
        lata_unallocated_encoding(s);
        return;
    }

    if (is_scalar) {
        elements = 1;
    } else {
        elements = (8 << is_q) >> size;
    }
    fracbits = (16 << size) - immhb;

    if (!fp_access_check(s)) {
        return;
    }

    handle_simd_intfp_conv(s, rd, rn, elements, !is_u, fracbits, size);
}

/* FCVTZS, FVCVTZU - FP to fixedpoint conversion */
static void handle_simd_shift_fpint_conv(DisasContext *s, bool is_scalar,
                                         bool is_q, bool is_u,
                                         int immh, int immb, int rn, int rd)
{
    assert(0);
}

/* AdvSIMD scalar shift by immediate
 *  31 30  29 28         23 22  19 18  16 15    11  10 9    5 4    0
 * +-----+---+-------------+------+------+--------+---+------+------+
 * | 0 1 | U | 1 1 1 1 1 0 | immh | immb | opcode | 1 |  Rn  |  Rd  |
 * +-----+---+-------------+------+------+--------+---+------+------+
 *
 * This is the scalar version so it works on a fixed sized registers
 */
static void disas_simd_scalar_shift_imm(DisasContext *s, uint32_t insn)
{
    int rd = extract32(insn, 0, 5);
    int rn = extract32(insn, 5, 5);
    int opcode = extract32(insn, 11, 5);
    int immb = extract32(insn, 16, 3);
    int immh = extract32(insn, 19, 4);
    bool is_u = extract32(insn, 29, 1);

    if (immh == 0) {
        lata_unallocated_encoding(s);
        return;
    }

    switch (opcode) {
    case 0x08: /* SRI */
        if (!is_u) {
            lata_unallocated_encoding(s);
            return;
        }
        /* fall through */
    case 0x00: /* SSHR / USHR */
    case 0x02: /* SSRA / USRA */
    case 0x04: /* SRSHR / URSHR */
    case 0x06: /* SRSRA / URSRA */
        handle_scalar_simd_shri(s, is_u, immh, immb, opcode, rn, rd);
        break;
    case 0x0a: /* SHL / SLI */
        handle_scalar_simd_shli(s, is_u, immh, immb, opcode, rn, rd);
        break;
    case 0x1c: /* SCVTF, UCVTF */
        handle_simd_shift_intfp_conv(s, true, false, is_u, immh, immb,
                                     opcode, rn, rd);
        break;
    case 0x10: /* SQSHRUN, SQSHRUN2 */
    case 0x11: /* SQRSHRUN, SQRSHRUN2 */
        if (!is_u) {
            lata_unallocated_encoding(s);
            return;
        }
        handle_vec_simd_sqshrn(s, true, false, false, true,
                               immh, immb, opcode, rn, rd);
        break;
    case 0x12: /* SQSHRN, SQSHRN2, UQSHRN */
    case 0x13: /* SQRSHRN, SQRSHRN2, UQRSHRN, UQRSHRN2 */
        handle_vec_simd_sqshrn(s, true, false, is_u, is_u,
                               immh, immb, opcode, rn, rd);
        break;
    case 0xc: /* SQSHLU */
        if (!is_u) {
            lata_unallocated_encoding(s);
            return;
        }
        handle_simd_qshl(s, true, false, false, true, immh, immb, rn, rd);
        break;
    case 0xe: /* SQSHL, UQSHL */
        handle_simd_qshl(s, true, false, is_u, is_u, immh, immb, rn, rd);
        break;
    case 0x1f: /* FCVTZS, FCVTZU */
        handle_simd_shift_fpint_conv(s, true, false, is_u, immh, immb, rn, rd);
        break;
    default:
        lata_unallocated_encoding(s);
        break;
    }
}

/* AdvSIMD scalar three different
 *  31 30  29 28       24 23  22  21 20  16 15    12 11 10 9    5 4    0
 * +-----+---+-----------+------+---+------+--------+-----+------+------+
 * | 0 1 | U | 1 1 1 1 0 | size | 1 |  Rm  | opcode | 0 0 |  Rn  |  Rd  |
 * +-----+---+-----------+------+---+------+--------+-----+------+------+
 */
static void disas_simd_scalar_three_reg_diff(DisasContext *s, uint32_t insn)
{
    assert(0);
}

static void handle_3same_64(DisasContext *s, int opcode, bool u,
                            int rd, int rn, int rm)
{
    /* Handle 64x64->64 opcodes which are shared between the scalar
     * and vector 3-same groups. We cover every opcode where size == 3
     * is valid in either the three-reg-same (integer, not pairwise)
     * or scalar-three-reg-same groups.
     */
    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND vreg_m = alloc_fpr_src(rm);

    switch (opcode) {
    case 0x1: /* SQADD */
        assert(0);
        break;
    case 0x5: /* SQSUB */
        assert(0);
        break;
    case 0x6: /* CMGT, CMHI */
        assert(0);
        break;
    case 0x7: /* CMGE, CMHS */
        assert(0);
        break;
    case 0x11: /* CMTST, CMEQ */
        assert(0);
        break;
    case 0x8: /* SSHL, USHL */
        assert(0);
        break;
    case 0x9: /* SQSHL, UQSHL */
        assert(0);
        break;
    case 0xa: /* SRSHL, URSHL */
        assert(0);
        break;
    case 0xb: /* SQRSHL, UQRSHL */
        assert(0);
        break;
    case 0x10: /* ADD, SUB */
        if (u) {
            la_vsub_d(vreg_d, vreg_n, vreg_m);
        } else {
            la_vadd_d(vreg_d, vreg_n, vreg_m);
        }
        la_vinsgr2vr_d(vreg_d,zero_ir2_opnd,1);
        break;
    default:
        g_assert_not_reached();
    }

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
    free_alloc_fpr(vreg_m);
}

static void handle_3same_float(DisasContext *s, int size, int elements,
                               int fpopcode, int rd, int rn, int rm)
{
    // IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND vreg_m = alloc_fpr_src(rm);
    IR2_OPND vtemp = ra_alloc_ftemp();
    IR2_OPND vtemp1 = ra_alloc_ftemp();

    IR2_OPND vreg_d;
    if(fpopcode == 0x39 || fpopcode == 0x19){
        vreg_d = alloc_fpr_src(rd);
    }else{
        vreg_d = alloc_fpr_dst(rd);
    }
    if (size) {
        /* Double */

        switch (fpopcode) {
        case 0x39: /* FMLS */
            la_vxor_v(vtemp, vtemp, vtemp);
            la_vfsub_d(vtemp, vtemp, vreg_n);
            la_vfmadd_d(vreg_d, vtemp, vreg_m, vreg_d);
            break;
        case 0x19: /* FMLA */
            la_vfmadd_d(vreg_d, vreg_n, vreg_m, vreg_d);
            break;
        case 0x18: /* FMAXNM */
            la_vfmax_d(vreg_d, vreg_n, vreg_m);
            break;
        case 0x1a: /* FADD */
            la_vfadd_d(vreg_d, vreg_n, vreg_m);
            break;
        case 0x1b: /* FMULX */
            assert(0);
            break;
        case 0x1c: /* FCMEQ */
            la_vfcmp_cond_d(vreg_d, vreg_n, vreg_m, FCMP_COND_SEQ);
            break;
        case 0x1e: /* FMAX */
            assert(0);
            break;
        case 0x1f: /* FRECPS */
            assert(0);
            break;
        case 0x38: /* FMINNM */
            la_vfmin_d(vreg_d, vreg_n, vreg_m);
            break;
        case 0x3a: /* FSUB */
            la_vfsub_d(vreg_d, vreg_n, vreg_m);
            break;
        case 0x3e: /* FMIN */
            assert(0);
            break;
        case 0x3f: /* FRSQRTS */
            assert(0);
            break;
        case 0x5b: /* FMUL */
            la_vfmul_d(vreg_d, vreg_n, vreg_m);
            break;
        case 0x5c: /* FCMGE */
            la_vfcmp_cond_d(vreg_d, vreg_m, vreg_n, FCMP_COND_SLE);
            break;
        case 0x5d: /* FACGE */
            assert(0);
            break;
        case 0x5f: /* FDIV */
            la_vfdiv_d(vreg_d, vreg_n, vreg_m);
            break;
        case 0x7a: /* FABD */
            la_vfsub_d(vreg_d, vreg_n, vreg_m);
            la_vreplvei_d(vtemp, vreg_d, 1);
            la_fabs_d(vreg_d, vreg_d);
            la_fabs_d(vtemp, vtemp);
            la_vextrins_d(vreg_d, vtemp, 1 << 4);
            break;
        case 0x7c: /* FCMGT */
            la_vfcmp_cond_d(vreg_d, vreg_m, vreg_n, FCMP_COND_SLT);
            break;
        case 0x7d: /* FACGT */
            assert(0);
            break;
        default:
            g_assert_not_reached();
        }

    } else {
        /* Single */

        switch (fpopcode) {
        case 0x39: /* FMLS */
            la_vxor_v(vtemp, vtemp, vtemp);
            la_vfsub_s(vtemp, vtemp, vreg_n);
            la_vfmadd_s(vreg_d, vtemp, vreg_m, vreg_d);
            break;
        case 0x19: /* FMLA */
            la_vfmadd_s(vreg_d, vreg_n, vreg_m, vreg_d);
            break;
        case 0x1a: /* FADD */
            la_vfadd_s(vreg_d, vreg_n, vreg_m);
            break;
        case 0x1b: /* FMULX */
            assert(0);
            break;
        case 0x1c: /* FCMEQ */
            la_vfcmp_cond_s(vreg_d, vreg_n, vreg_m, FCMP_COND_SEQ);
            break;
        case 0x1e: /* FMAX */
            assert(0);
            break;
        case 0x1f: /* FRECPS */
            assert(0);
            break;
        case 0x18: /* FMAXNM */
            la_vfmax_s(vreg_d, vreg_n, vreg_m);
            break;
        case 0x38: /* FMINNM */
            la_vfmin_s(vreg_d, vreg_n, vreg_m);
            break;
        case 0x3a: /* FSUB */
            la_vfsub_s(vreg_d, vreg_n, vreg_m);
            break;
        case 0x3e: /* FMIN */
            assert(0);
            break;
        case 0x3f: /* FRSQRTS */
            assert(0);
            break;
        case 0x5b: /* FMUL */
            la_vfmul_s(vreg_d, vreg_n, vreg_m);
            break;
        case 0x5c: /* FCMGE */
            la_vfcmp_cond_s(vreg_d, vreg_m, vreg_n, FCMP_COND_SLE);
            break;
        case 0x5d: /* FACGE */
            assert(0);
            break;
        case 0x5f: /* FDIV */
            la_vfdiv_s(vreg_d, vreg_n, vreg_m);
            break;
        case 0x7a: /* FABD */
            la_vfsub_s(vtemp1, vreg_n, vreg_m);
            for(int i = 0; i < elements; ++i){
                la_vreplvei_w(vtemp, vtemp1, i);
                la_fabs_s(vtemp, vtemp);
                la_vextrins_w(vreg_d, vtemp, i << 4);
            }
            break;
        case 0x7c: /* FCMGT */
            la_vfcmp_cond_s(vreg_d, vreg_m, vreg_n, FCMP_COND_SLT);
            break;
        case 0x7d: /* FACGT */
            assert(0);
            break;
        default:
            g_assert_not_reached();
        }

        if(elements == 2){
            /* 高64位清零 */
            la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
        }
    }

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
    free_alloc_fpr(vreg_m);
    free_alloc_fpr(vtemp);
    free_alloc_fpr(vtemp1);
}

static void handle_3same_float_scalar(DisasContext *s, int size, int fpopcode, 
                                                        int rd, int rn, int rm)
{
    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND vreg_m = alloc_fpr_src(rm);

    if (size) {
        /* Double */

        switch (fpopcode) {
        case 0x1b: /* FMULX */
            assert(0);
            break;
        case 0x1c: /* FCMEQ */
            assert(0);
            break;
        case 0x1f: /* FRECPS */
            assert(0);
            break;
        case 0x3f: /* FRSQRTS */
            assert(0);
            break;
        case 0x5c: /* FCMGE */
            assert(0);
            break;
        case 0x5d: /* FACGE */
            assert(0);
            break;
        case 0x7a: /* FABD */
            la_fsub_d(vreg_d, vreg_n, vreg_m);
            la_fabs_d(vreg_d, vreg_d);
            break;
        case 0x7c: /* FCMGT */
            assert(0);
            break;
        case 0x7d: /* FACGT */
            assert(0);
            break;
        default:
            g_assert_not_reached();
        }
        /* 高64位清零 */
        la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
    } else {
        /* Single */

        switch (fpopcode) {
        case 0x1b: /* FMULX */
            assert(0);
            break;
        case 0x1c: /* FCMEQ */
            assert(0);
            break;
        case 0x1f: /* FRECPS */
            assert(0);
            break;
        case 0x3f: /* FRSQRTS */
            assert(0);
            break;
        case 0x5c: /* FCMGE */
            assert(0);
            break;
        case 0x5d: /* FACGE */
            assert(0);
            break;
        case 0x7a: /* FABD */
            la_fsub_s(vreg_d, vreg_n, vreg_m);
            la_fabs_s(vreg_d, vreg_d);
            break;
        case 0x7c: /* FCMGT */
            assert(0);
            break;
        case 0x7d: /* FACGT */
            assert(0);
            break;
        default:
            g_assert_not_reached();
        }

        la_vand_v(vreg_d,vreg_d, fsmask_ir2_opnd);
    }

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
    free_alloc_fpr(vreg_m);
}

/* AdvSIMD scalar three same
 *  31 30  29 28       24 23  22  21 20  16 15    11  10 9    5 4    0
 * +-----+---+-----------+------+---+------+--------+---+------+------+
 * | 0 1 | U | 1 1 1 1 0 | size | 1 |  Rm  | opcode | 1 |  Rn  |  Rd  |
 * +-----+---+-----------+------+---+------+--------+---+------+------+
 */
static void disas_simd_scalar_three_reg_same(DisasContext *s, uint32_t insn)
{
    int rd = extract32(insn, 0, 5);
    int rn = extract32(insn, 5, 5);
    int opcode = extract32(insn, 11, 5);
    int rm = extract32(insn, 16, 5);
    int size = extract32(insn, 22, 2);
    bool u = extract32(insn, 29, 1);

    if (opcode >= 0x18) {
        /* Floating point: U, size[1] and opcode indicate operation */
        int fpopcode = opcode | (extract32(size, 1, 1) << 5) | (u << 6);
        switch (fpopcode) {
        case 0x1b: /* FMULX */
        case 0x1f: /* FRECPS */
        case 0x3f: /* FRSQRTS */
        case 0x5d: /* FACGE */
        case 0x7d: /* FACGT */
        case 0x1c: /* FCMEQ */
        case 0x5c: /* FCMGE */
        case 0x7c: /* FCMGT */
        case 0x7a: /* FABD */
            break;
        default:
            lata_unallocated_encoding(s);
            return;
        }

        if (!fp_access_check(s)) {
            return;
        }

        handle_3same_float_scalar(s, extract32(size, 0, 1), fpopcode, rd, rn, rm);
        return;
    }

    switch (opcode) {
    case 0x1: /* SQADD, UQADD */
    case 0x5: /* SQSUB, UQSUB */
    case 0x9: /* SQSHL, UQSHL */
    case 0xb: /* SQRSHL, UQRSHL */
        break;
    case 0x8: /* SSHL, USHL */
    case 0xa: /* SRSHL, URSHL */
    case 0x6: /* CMGT, CMHI */
    case 0x7: /* CMGE, CMHS */
    case 0x11: /* CMTST, CMEQ */
    case 0x10: /* ADD, SUB (vector) */
        if (size != 3) {
            lata_unallocated_encoding(s);
            return;
        }
        break;
    case 0x16: /* SQDMULH, SQRDMULH (vector) */
        if (size != 1 && size != 2) {
            lata_unallocated_encoding(s);
            return;
        }
        break;
    default:
        lata_unallocated_encoding(s);
        return;
    }

    if (!fp_access_check(s)) {
        return;
    }

    if (size == 3) {
        handle_3same_64(s, opcode, u, rd, rn, rm);
    } else {
        assert(0);
    }

}

/* AdvSIMD scalar three same FP16
 *  31 30  29 28       24 23  22 21 20  16 15 14 13    11 10  9  5 4  0
 * +-----+---+-----------+---+-----+------+-----+--------+---+----+----+
 * | 0 1 | U | 1 1 1 1 0 | a | 1 0 |  Rm  | 0 0 | opcode | 1 | Rn | Rd |
 * +-----+---+-----------+---+-----+------+-----+--------+---+----+----+
 * v: 0101 1110 0100 0000 0000 0100 0000 0000 => 5e400400
 * m: 1101 1111 0110 0000 1100 0100 0000 0000 => df60c400
 */
static void disas_simd_scalar_three_reg_same_fp16(DisasContext *s,
                                                  uint32_t insn)
{
    assert(0);
}

/* AdvSIMD scalar three same extra
 *  31 30  29 28       24 23  22  21 20  16  15 14    11  10 9  5 4  0
 * +-----+---+-----------+------+---+------+---+--------+---+----+----+
 * | 0 1 | U | 1 1 1 1 0 | size | 0 |  Rm  | 1 | opcode | 1 | Rn | Rd |
 * +-----+---+-----------+------+---+------+---+--------+---+----+----+
 */
static void disas_simd_scalar_three_reg_same_extra(DisasContext *s,
                                                   uint32_t insn)
{
    assert(0);
}

static void handle_2misc_64(DisasContext *s, int opcode, bool u, 
                            IR2_OPND *vreg_d, IR2_OPND *vreg_n)
{
    /* Handle 64->64 opcodes which are shared between the scalar and
     * vector 2-reg-misc groups. We cover every integer opcode where size == 3
     * is valid in either group and also the double-precision fp ops.
     * The caller only need provide tcg_rmode and tcg_fpstatus if the op
     * requires them.
     */
    // TCGCond cond;
    IR2_OPND vtemp = ra_alloc_ftemp();

    switch (opcode) {
    case 0x4: /* CLS, CLZ */
        break;
    case 0x5: /* NOT */
        break;
    case 0x7: /* SQABS, SQNEG */
        break;
    case 0xa: /* CMLT */
    do_cmop:
        break;
    case 0x8: /* CMGT, CMGE */
        goto do_cmop;
    case 0x9: /* CMEQ, CMLE */
        goto do_cmop;
    case 0xb: /* ABS, NEG */
        break;
    case 0x2f: /* FABS */
        la_vreplvei_d(vtemp, *vreg_n, 1);
        la_fabs_d(*vreg_d, *vreg_n);
        la_fabs_d(vtemp, vtemp);
        la_vextrins_d(*vreg_d, vtemp, 1 << 4);
        break;
    case 0x6f: /* FNEG */
        la_vreplvei_d(vtemp, *vreg_n, 1);
        la_fneg_d(*vreg_d, *vreg_n);
        la_fneg_d(vtemp, vtemp);
        la_vextrins_d(*vreg_d, vtemp, 1 << 4);
        break;
    case 0x7f: /* FSQRT */
        la_vfsqrt_d(*vreg_d, *vreg_n);
        break;
    case 0x1a: /* FCVTNS */
        la_vftintrne_l_d(*vreg_d, *vreg_n);
        break;
    case 0x1b: /* FCVTMS */
        la_vftintrm_l_d(*vreg_d, *vreg_n);
        break;
    case 0x1c: /* FCVTAS */
        assert(0);
        break;
    case 0x3a: /* FCVTPS */
        la_vftintrp_l_d(*vreg_d, *vreg_n);
        break;;
    case 0x3b: /* FCVTZS */
        la_vftintrz_l_d(*vreg_d, *vreg_n);
        break;
    case 0x5a: /* FCVTNU */
    case 0x5b: /* FCVTMU */
    case 0x5c: /* FCVTAU */
    case 0x7a: /* FCVTPU */
        assert(0);
    case 0x7b: /* FCVTZU */
        la_vftintrz_lu_d(*vreg_d, *vreg_n);
        break;
    case 0x18: /* FRINTN */
        break;
    case 0x19: /* FRINTM */
        la_vfrintrm_d(*vreg_d, *vreg_n);
        break;
    case 0x38: /* FRINTP */
        break;
    case 0x39: /* FRINTZ */
        la_vfrintrz_d(*vreg_d, *vreg_n);
        break;
    case 0x58: /* FRINTA */
        break;
    case 0x79: /* FRINTI */
        break;
    case 0x59: /* FRINTX */
        break;
    case 0x1e: /* FRINT32Z */
    case 0x5e: /* FRINT32X */
        break;
    case 0x1f: /* FRINT64Z */
    case 0x5f: /* FRINT64X */
        break;
    default:
        g_assert_not_reached();
    }
    
    free_alloc_fpr(vtemp);
}

static void handle_2misc_fcmp_zero(DisasContext *s, int opcode,
                                   bool is_scalar, bool is_u, bool is_q,
                                   int size, int rn, int rd)
{
    bool is_double = (size == MO_64);

    if (!fp_access_check(s)) {
        return;
    }

    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND vtemp = ra_alloc_ftemp();
    la_vxor_v(vtemp, vtemp, vtemp);
    if (is_double) {

        switch (opcode) {
        case 0x2e: /* FCMLT (zero) */
            la_vfcmp_cond_d(vreg_d, vreg_n, vtemp, FCMP_COND_SLT);
            break;
        case 0x2c: /* FCMGT (zero) */
            la_vfcmp_cond_d(vreg_d, vtemp, vreg_n, FCMP_COND_SLT);
            break;
        case 0x2d: /* FCMEQ (zero) */
            la_vfcmp_cond_d(vreg_d, vreg_n, vtemp, FCMP_COND_SEQ);
            break;
        case 0x6d: /* FCMLE (zero) */
            la_vfcmp_cond_d(vreg_d, vreg_n, vtemp, FCMP_COND_SLE);
            break;
        case 0x6c: /* FCMGE (zero) */
            la_vfcmp_cond_d(vreg_d, vtemp, vreg_n, FCMP_COND_SLE);
            break;
        default:
            g_assert_not_reached();
        }

    } else {

        if (size == MO_16) {
            assert(0);
            switch (opcode) {
            case 0x2e: /* FCMLT (zero) */
                break;
            case 0x2c: /* FCMGT (zero) */
                break;
            case 0x2d: /* FCMEQ (zero) */
                break;
            case 0x6d: /* FCMLE (zero) */
                break;
            case 0x6c: /* FCMGE (zero) */
                break;
            default:
                g_assert_not_reached();
            }
        } else {
        switch (opcode) {
            case 0x2e: /* FCMLT (zero) */
                la_vfcmp_cond_s(vreg_d, vreg_n, vtemp, FCMP_COND_SLT);
                break;
            case 0x2c: /* FCMGT (zero) */
                la_vfcmp_cond_s(vreg_d, vtemp, vreg_n, FCMP_COND_SLT);
                break;
            case 0x2d: /* FCMEQ (zero) */
                la_vfcmp_cond_s(vreg_d, vreg_n, vtemp, FCMP_COND_SEQ);
                break;
            case 0x6d: /* FCMLE (zero) */
                la_vfcmp_cond_s(vreg_d, vreg_n, vtemp, FCMP_COND_SLE);
                break;
            case 0x6c: /* FCMGE (zero) */
                la_vfcmp_cond_s(vreg_d, vtemp, vreg_n, FCMP_COND_SLE);
                break;
            default:
                g_assert_not_reached();
            }

        }

    }
    /* 高64位清零 */
    if(!is_q){
        la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
    }

    if(is_scalar){
        la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
        if(!is_double){
            la_vinsgr2vr_w(vreg_d, zero_ir2_opnd, 1);
            if(size == MO_16){
                la_vinsgr2vr_h(vreg_d, zero_ir2_opnd, 1);
            }
        }
    }

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
    free_alloc_fpr(vtemp);
}

static void handle_2misc_reciprocal(DisasContext *s, int opcode,
                                    bool is_scalar, bool is_u, bool is_q,
                                    int size, int rn, int rd)
{
    assert(0);
}

static void handle_2misc_narrow(DisasContext *s, bool scalar,
                                int opcode, bool u, bool is_q,
                                int size, int rn, int rd)
{
    /* Handle 2-reg-misc ops which are narrowing (so each 2*size element
     * in the source becomes a size element in the destination).
     */
    IR2_OPND vreg_d = alloc_fpr_src(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND vtemp = ra_alloc_ftemp();
    la_vandi_b(vtemp, vtemp, 0);

    switch (opcode) {
    case 0x12: /* XTN,XTN2, SQXTUN */
    {    
        if(!is_q){
            switch (size){
            case 0:
                la_vpickev_b(vreg_d, vtemp, vreg_n);
                break;
            case 1:
                la_vpickev_h(vreg_d, vtemp, vreg_n);
                break;
            case 2:
                la_vpickev_w(vreg_d, vtemp, vreg_n);
                break;
            case 3:
                la_vpickev_d(vreg_d, vtemp, vreg_n);
                break;
            default:
                assert(0);
            }
        }
        else{
            switch (size){
            case 0:
                la_vpickev_b(vtemp, vtemp, vreg_n);
                la_vpickev_d(vreg_d, vtemp, vreg_d);
                break;
            case 1:
                la_vpickev_h(vtemp, vtemp, vreg_n);
                la_vpickev_d(vreg_d, vtemp, vreg_d);
                break;
            case 2:
                la_vpickev_w(vtemp, vtemp, vreg_n);
                la_vpickev_d(vreg_d, vtemp, vreg_d);
                break;
            case 3:
                la_vpickev_d(vtemp, vtemp, vreg_n);
                la_vpickev_d(vreg_d, vtemp, vreg_d);
                break;
            default:
                assert(0);
            }
        }
        break;
    }
    case 0x14: /* SQXTN, UQXTN */
    {
        assert(0);
        break;
    }
    case 0x16: /* FCVTN, FCVTN2 */
        /* 32 bit to 16 bit or 64 bit to 32 bit float conversion */
        if (size == 2) {
            if(is_q){
                la_vori_b(vtemp, vreg_d, 0);
                la_vfcvt_s_d(vreg_d, vreg_n, vreg_n);     
                la_vextrins_d(vreg_d, vtemp, 0);
            }else{
                la_vfcvt_s_d(vreg_d, vreg_n, vreg_n);
                la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
            }

        } else {
            if(is_q){
                la_vori_b(vtemp, vreg_d, 0);
                la_vfcvt_h_s(vreg_d, vreg_n, vreg_n);     
                la_vextrins_d(vreg_d, vtemp, 0);
            }else{
                la_vfcvt_h_s(vreg_d, vreg_n, vreg_n);
                la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
            }
        }
        break;
    case 0x36: /* BFCVTN, BFCVTN2 */
        {
            assert(0);
        }
        break;
    case 0x56:  /* FCVTXN, FCVTXN2 */
        /* 64 bit to 32 bit float conversion
            * with von Neumann rounding (round to odd)
            */
        assert(0);
        break;
    default:
        g_assert_not_reached();
    }

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vtemp);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
}

/* Remaining saturating accumulating ops */
static void handle_2misc_satacc(DisasContext *s, bool is_scalar, bool is_u,
                                bool is_q, int size, int rn, int rd)
{
    assert(0);
}

/* AdvSIMD scalar two reg misc
 *  31 30  29 28       24 23  22 21       17 16    12 11 10 9    5 4    0
 * +-----+---+-----------+------+-----------+--------+-----+------+------+
 * | 0 1 | U | 1 1 1 1 0 | size | 1 0 0 0 0 | opcode | 1 0 |  Rn  |  Rd  |
 * +-----+---+-----------+------+-----------+--------+-----+------+------+
 */
static void disas_simd_scalar_two_reg_misc(DisasContext *s, uint32_t insn)
{
    int rd = extract32(insn, 0, 5);
    int rn = extract32(insn, 5, 5);
    int opcode = extract32(insn, 12, 5);
    int size = extract32(insn, 22, 2);
    bool u = extract32(insn, 29, 1);

    switch (opcode) {
    case 0x3: /* USQADD / SUQADD*/
        if (!fp_access_check(s)) {
            return;
        }
        handle_2misc_satacc(s, true, u, false, size, rn, rd);
        return;
    case 0x7: /* SQABS / SQNEG */
        break;
    case 0xa: /* CMLT */
        if (u) {
            lata_unallocated_encoding(s);
            return;
        }
        /* fall through */
    case 0x8: /* CMGT, CMGE */
    case 0x9: /* CMEQ, CMLE */
    case 0xb: /* ABS, NEG */
        if (size != 3) {
            lata_unallocated_encoding(s);
            return;
        }
        break;
    case 0x12: /* SQXTUN */
        if (!u) {
            lata_unallocated_encoding(s);
            return;
        }
        /* fall through */
    case 0x14: /* SQXTN, UQXTN */
        if (size == 3) {
            lata_unallocated_encoding(s);
            return;
        }
        if (!fp_access_check(s)) {
            return;
        }
        handle_2misc_narrow(s, true, opcode, u, false, size, rn, rd);
        return;
    case 0xc ... 0xf:
    case 0x16 ... 0x1d:
    case 0x1f:
        /* Floating point: U, size[1] and opcode indicate operation;
         * size[0] indicates single or double precision.
         */
        opcode |= (extract32(size, 1, 1) << 5) | (u << 6);
        size = extract32(size, 0, 1) ? 3 : 2;
        switch (opcode) {
        case 0x2c: /* FCMGT (zero) */
        case 0x2d: /* FCMEQ (zero) */
        case 0x2e: /* FCMLT (zero) */
        case 0x6c: /* FCMGE (zero) */
        case 0x6d: /* FCMLE (zero) */
            handle_2misc_fcmp_zero(s, opcode, true, u, true, size, rn, rd);
            return;
        case 0x1d: /* SCVTF */
        case 0x5d: /* UCVTF */
        {
            bool is_signed = (opcode == 0x1d);
            if (!fp_access_check(s)) {
                return;
            }
            handle_simd_intfp_conv(s, rd, rn, 1, is_signed, 0, size);
            return;
        }
        case 0x3d: /* FRECPE */
        case 0x3f: /* FRECPX */
        case 0x7d: /* FRSQRTE */
            if (!fp_access_check(s)) {
                return;
            }
            handle_2misc_reciprocal(s, opcode, true, u, true, size, rn, rd);
            return;
        case 0x1a: /* FCVTNS */
        case 0x1b: /* FCVTMS */
        case 0x3a: /* FCVTPS */
        case 0x3b: /* FCVTZS */
        case 0x5a: /* FCVTNU */
        case 0x5b: /* FCVTMU */
        case 0x7a: /* FCVTPU */
        case 0x7b: /* FCVTZU */
        case 0x1c: /* FCVTAS */
        case 0x5c: /* FCVTAU */
            /* TIEAWAY doesn't fit in the usual rounding mode encoding */
            assert(0);
            break;
        case 0x56: /* FCVTXN, FCVTXN2 */
            if (size == 2) {
                lata_unallocated_encoding(s);
                return;
            }
            if (!fp_access_check(s)) {
                return;
            }
            handle_2misc_narrow(s, true, opcode, u, false, size - 1, rn, rd);
            return;
        default:
            lata_unallocated_encoding(s);
            return;
        }
        break;
    default:
        lata_unallocated_encoding(s);
        return;
    }

    if (!fp_access_check(s)) {
        return;
    }

}

/* SSHR[RA]/USHR[RA] - Vector shift right (optional rounding/accumulate) */
static void handle_vec_simd_shri(DisasContext *s, bool is_q, bool is_u,
                                 int immh, int immb, int opcode, int rn, int rd)
{
    int size = 32 - clz32(immh) - 1;
    int immhb = immh << 3 | immb;
    int shift = 2 * (8 << size)- immhb;
    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND vtemp = ra_alloc_ftemp();

    if (extract32(immh, 3, 1) && !is_q) {
        lata_unallocated_encoding(s);
        return;
    }

    if (!fp_access_check(s)) {
        return;
    }

    la_vbsrl_v(vtemp, vreg_n, 0);
    /* 高位清零 */
    if(!is_q){
        la_vinsgr2vr_d(vtemp,zero_ir2_opnd,1);
    }

    switch (opcode) {
    case 0x02: /* SSRA / USRA (accumulate) */
        assert(0);
        break;

    case 0x08: /* SRI */
        assert(0);
        break;

    case 0x00: /* SSHR / USHR */
        if (is_u) {
            /* Shift count the same size as element size produces zero.  */
            if(shift == 8 << size){
                la_vandi_b(vreg_d, vtemp, 0);
                break;
            }
            switch (size){
            case 0:
                la_vsrli_b(vreg_d,vtemp,shift);
                break;
            case 1:
                la_vsrli_h(vreg_d,vtemp,shift);
                break;
            case 2:
                la_vsrli_w(vreg_d,vtemp,shift);
                break;
            case 3:
                la_vsrli_d(vreg_d,vtemp,shift);
                break;
            default:
                assert(0);
            } 
        } else {
            /* Shift count the same size as element size produces all sign.  */
            if(shift == 8 << size){
                shift -= 1;
            }
            switch (size){
            case 0:
                la_vsrai_b(vreg_d,vtemp,shift);
                break;
            case 1:
                la_vsrai_h(vreg_d,vtemp,shift);
                break;
            case 2:
                la_vsrai_w(vreg_d,vtemp,shift);
                break;
            case 3:
                la_vsrai_d(vreg_d,vtemp,shift);
                break;
            default:
                assert(0);
            } 
        }
        break;

    case 0x04: /* SRSHR / URSHR (rounding) */
        assert(0);
        break;

    case 0x06: /* SRSRA / URSRA (accum + rounding) */
        assert(0);
        break;

    default:
        g_assert_not_reached();
    }

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
    free_alloc_fpr(vtemp);
}

/* SHL/SLI - Vector shift left */
static void handle_vec_simd_shli(DisasContext *s, bool is_q, bool insert,
                                 int immh, int immb, int opcode, int rn, int rd)
{
    assert(!insert);

    int size = 32 - clz32(immh) - 1;
    int immhb = immh << 3 | immb;
    int shift = immhb - (8 << size);

    /* Range of size is limited by decode: immh is a non-zero 4 bit field */
    assert(size >= 0 && size <= 3);

    if (extract32(immh, 3, 1) && !is_q) {
        lata_unallocated_encoding(s);
        return;
    }

    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);

    switch (size)
    {
    case 0:
        la_vslli_b(vreg_d, vreg_n, shift);
        break;
    case 1:
        la_vslli_h(vreg_d, vreg_n, shift);
        break;
    case 2:
        la_vslli_w(vreg_d, vreg_n, shift);
        break;
    case 3:
        la_vslli_d(vreg_d, vreg_n, shift);
        break;
    default:
        break;
    }

    if(!is_q){
        /* 高64位清零 */
        la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
    }
    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
}

/* USHLL/SSHLL - Vector shift left with widening */
static void handle_vec_simd_wshli(DisasContext *s, bool is_q, bool is_u,
                                 int immh, int immb, int opcode, int rn, int rd)
{
    int size = 32 - clz32(immh) - 1;
    int immhb = immh << 3 | immb;
    int shift = immhb - (8 << size);
    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND vtemp = ra_alloc_ftemp();

    if (size >= 3) {
        lata_unallocated_encoding(s);
        return;
    }

    if (!fp_access_check(s)) {
        return;
    }

    /* 统一移至低位处理 */
    if (!is_q){
        la_vbsrl_v(vtemp, vreg_n, 0);
    }
    else{
        la_vbsrl_v(vtemp, vreg_n, 8);
    }

    /* USHLL/USHLL2*/
    if(is_u){
        switch (size){
        case 0:
            la_vsllwil_hu_bu(vreg_d, vtemp, shift);
            break;
        case 1:
            la_vsllwil_wu_hu(vreg_d, vtemp, shift);
            break;
        case 2:
            la_vsllwil_du_wu(vreg_d, vtemp, shift);
            break;
        default:
            assert(0);
        }       
    }
    else{
        switch (size){
        case 0:
            la_vsllwil_h_b(vreg_d, vtemp, shift);
            break;
        case 1:
            la_vsllwil_w_h(vreg_d, vtemp, shift);
            break;
        case 2:
            la_vsllwil_d_w(vreg_d, vtemp, shift);
            break;
        default:
            assert(0);
        }          
    }
    


    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
    free_alloc_fpr(vtemp);
}

/* SHRN/RSHRN - Shift right with narrowing (and potential rounding) */
static void handle_vec_simd_shrn(DisasContext *s, bool is_q,
                                 int immh, int immb, int opcode, int rn, int rd)
{
    int immhb = immh << 3 | immb;
    int size = 32 - clz32(immh) - 1;
    int esize = 8 << size;
    int shift = (2 * esize) - immhb;
    bool round = extract32(opcode, 0, 1);
    IR2_OPND vreg_d = alloc_fpr_src(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND vtemp = ra_alloc_ftemp();
    IR2_OPND vtemp2 = ra_alloc_ftemp();

    if (extract32(immh, 3, 1)) {
        lata_unallocated_encoding(s);
        return;
    }

    if (!fp_access_check(s)) {
        return;
    }

    la_vandi_b(vtemp2, vtemp2, 0);

    if (is_q) {//SHRN2/RSHRN2 (writes the upper, keep the lower)
        switch (size){
        case 0:
            if(round) {
                la_vsrlri_h(vtemp, vreg_n, shift);
            } else {
                la_vsrli_h(vtemp, vreg_n, shift);
            }       
            la_vsrln_b_h(vtemp, vtemp, vtemp2);  
            break;
        case 1:
            if(round) {
                la_vsrlri_w(vtemp, vreg_n, shift);
            } else {
                la_vsrli_w(vtemp, vreg_n, shift);
            }  
            la_vsrln_h_w(vtemp, vtemp, vtemp2);
            break;
        case 2:
            if(round) {
                la_vsrlri_d(vtemp, vreg_n, shift);
            } else {
                la_vsrli_d(vtemp, vreg_n, shift);
            }  
            la_vsrln_w_d(vtemp, vtemp, vtemp2);
            break;
        default:
            assert(0);
        }  
        la_vpickev_d(vreg_d, vtemp, vreg_d);
    } else {//SHRN/RSHRN (writes the lower, clears the upper)
        switch (size){
        case 0:
            if(round) {
                la_vsrlri_h(vtemp, vreg_n, shift);
            } else {
                la_vsrli_h(vtemp, vreg_n, shift);
            }  
            la_vsrln_b_h(vreg_d, vtemp, vtemp2);
            break;            
        case 1:
            if(round) {
                la_vsrlri_w(vtemp, vreg_n, shift);
            } else {
                la_vsrli_w(vtemp, vreg_n, shift);
            }  
            la_vsrln_h_w(vreg_d, vtemp, vtemp2);
            break;
        case 2:
            if(round) {
                la_vsrlri_d(vtemp, vreg_n, shift);
            } else {
                la_vsrli_d(vtemp, vreg_n, shift);
            }  
            la_vsrln_w_d(vreg_d, vtemp, vtemp2);
            break;
        default:
            assert(0);
        }          
    }

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
    free_alloc_fpr(vtemp);
    free_alloc_fpr(vtemp2);
}


/* AdvSIMD shift by immediate
 *  31  30   29 28         23 22  19 18  16 15    11  10 9    5 4    0
 * +---+---+---+-------------+------+------+--------+---+------+------+
 * | 0 | Q | U | 0 1 1 1 1 0 | immh | immb | opcode | 1 |  Rn  |  Rd  |
 * +---+---+---+-------------+------+------+--------+---+------+------+
 */
static void disas_simd_shift_imm(DisasContext *s, uint32_t insn)
{
    int rd = extract32(insn, 0, 5);
    int rn = extract32(insn, 5, 5);
    int opcode = extract32(insn, 11, 5);
    int immb = extract32(insn, 16, 3);
    int immh = extract32(insn, 19, 4);
    bool is_u = extract32(insn, 29, 1);
    bool is_q = extract32(insn, 30, 1);

    /* data_proc_simd[] has sent immh == 0 to disas_simd_mod_imm. */
    assert(immh != 0);

    switch (opcode) {
    case 0x08: /* SRI */
        if (!is_u) {
            lata_unallocated_encoding(s);
            return;
        }
        /* fall through */
    case 0x00: /* SSHR / USHR */
    case 0x02: /* SSRA / USRA (accumulate) */
    case 0x04: /* SRSHR / URSHR (rounding) */
    case 0x06: /* SRSRA / URSRA (accum + rounding) */
        handle_vec_simd_shri(s, is_q, is_u, immh, immb, opcode, rn, rd);
        break;
    case 0x0a: /* SHL / SLI */
        handle_vec_simd_shli(s, is_q, is_u, immh, immb, opcode, rn, rd);
        break;
    case 0x10: /* SHRN */
    case 0x11: /* RSHRN / SQRSHRUN */
        if (is_u) {
            handle_vec_simd_sqshrn(s, false, is_q, false, true, immh, immb,
                                   opcode, rn, rd);
        } else {
            handle_vec_simd_shrn(s, is_q, immh, immb, opcode, rn, rd);
        }
        break;
    case 0x12: /* SQSHRN / UQSHRN */
    case 0x13: /* SQRSHRN / UQRSHRN */
        handle_vec_simd_sqshrn(s, false, is_q, is_u, is_u, immh, immb,
                               opcode, rn, rd);
        break;
    case 0x14: /* SSHLL / USHLL */
        handle_vec_simd_wshli(s, is_q, is_u, immh, immb, opcode, rn, rd);
        break;
    case 0x1c: /* SCVTF / UCVTF */
        handle_simd_shift_intfp_conv(s, false, is_q, is_u, immh, immb,
                                     opcode, rn, rd);
        break;
    case 0xc: /* SQSHLU */
        if (!is_u) {
            lata_unallocated_encoding(s);
            return;
        }
        handle_simd_qshl(s, false, is_q, false, true, immh, immb, rn, rd);
        break;
    case 0xe: /* SQSHL, UQSHL */
        handle_simd_qshl(s, false, is_q, is_u, is_u, immh, immb, rn, rd);
        break;
    case 0x1f: /* FCVTZS/ FCVTZU */
        handle_simd_shift_fpint_conv(s, false, is_q, is_u, immh, immb, rn, rd);
        return;
    default:
        lata_unallocated_encoding(s);
        return;
    }
}

static void handle_3rd_widening(DisasContext *s, int is_q, int is_u, int size,
                                int opcode, int rd, int rn, int rm)
{
    /* 3-reg-different widening insns: 64 x 64 -> 128 */
    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND vreg_m = alloc_fpr_src(rm);
    IR2_OPND vtemp = ra_alloc_ftemp();
    IR2_OPND vtemp1 = ra_alloc_ftemp();

    /* 0向量寄存器 */
    IR2_OPND vzero = ra_alloc_ftemp();
    la_vandi_b(vzero, vzero, 0);

    int op_id = (is_u << 4) | opcode;
    int is_sub = extract32(op_id, 1, 1);

    switch (op_id){
    case 0x0: /* SADDL, SADDL2 */
        assert(0);
        break;
    case 0x2: /* SSUBL, SSUBL2 */
        assert(0);
        break;
    case 0x5: /* SABAL, SABAL2 */
        assert(0);
        break;
    case 0x7: /* SABDL, SABDL2 */
        assert(0);
        break;
    case 0x8: /* SMLAL, SMLAL2 */
        assert(0);
        break;
    case 0xa: /* SMLSL, SMLSL2 */
        assert(0);
        break;
    case 0xc: /* SMULL, SMULL2 */
        if (!is_q){
            /* 低位统一移至偶数位后做偶数位的相乘并拓2倍宽 */
            switch (size){
            case 0:
                la_vilvl_b(vtemp, vzero, vreg_n);
                la_vilvl_b(vtemp1, vzero, vreg_m);
                la_vmulwev_h_b(vreg_d, vtemp, vtemp1);
                break;
            case 1:
                la_vilvl_h(vtemp, vzero, vreg_n);
                la_vilvl_h(vtemp1, vzero, vreg_m);
                la_vmulwev_w_h(vreg_d, vtemp, vtemp1);
                break;
            case 2:
                la_vilvl_w(vtemp, vzero, vreg_n);
                la_vilvl_w(vtemp1, vzero, vreg_m);
                la_vmulwev_d_w(vreg_d, vtemp, vtemp1);
                break;
            case 3:
                la_vilvl_d(vtemp, vzero, vreg_n);
                la_vilvl_d(vtemp1, vzero, vreg_m);
                la_vmulwev_q_d(vreg_d, vtemp, vtemp1);
                break;
            default:
                assert(0);
            }
        }
        else{
            /* 高位统一移至偶数位后做偶数位的相乘并拓2倍宽 */
            switch (size){
            case 0:
                la_vilvh_b(vtemp, vzero, vreg_n);
                la_vilvh_b(vtemp1, vzero, vreg_m);
                la_vmulwev_h_b(vreg_d, vtemp, vtemp1);
                break;
            case 1:
                la_vilvh_h(vtemp, vzero, vreg_n);
                la_vilvh_h(vtemp1, vzero, vreg_m);
                la_vmulwev_w_h(vreg_d, vtemp, vtemp1);
                break;
            case 2:
                la_vilvh_w(vtemp, vzero, vreg_n);
                la_vilvh_w(vtemp1, vzero, vreg_m);
                la_vmulwev_d_w(vreg_d, vtemp, vtemp1);
                break;
            case 3:
                la_vilvh_d(vtemp, vzero, vreg_n);
                la_vilvh_d(vtemp1, vzero, vreg_m);
                la_vmulwev_q_d(vreg_d, vtemp, vtemp1);
                break;
            default:
                assert(0);
            }
        }
        break;
    case 0x9: /* SQDMLAL, SQDMLAL2 */
        assert(0);
        break;
    case 0xb: /* SQDMLSL, SQDMLSL2 */
        assert(0);
        break;
    case 0xd: /* SQDMULL, SQDMULL2 */
        assert(0);
        break;
    case 0x10: /* UADDL, UADDL2 */
    case 0x12: /* USUBL, USUBL2 */
        if (!is_q){ /* 统一移到高位处理 */
            la_vbsll_v(vtemp, vreg_n, 8);
            la_vbsll_v(vtemp1, vreg_m, 8);
            switch (size){
            case 0:
                /* 高位扩展 */
                la_vexth_hu_bu(vtemp, vtemp);
                la_vexth_hu_bu(vtemp1, vtemp1);
                if (is_sub)
                    la_vsub_h(vreg_d, vtemp, vtemp1);
                else
                    la_vadd_h(vreg_d, vtemp, vtemp1);
                break;
            case 1:
                la_vexth_wu_hu(vtemp, vtemp);
                la_vexth_wu_hu(vtemp1, vtemp1);
                if (is_sub)
                    la_vsub_w(vreg_d, vtemp, vtemp1);
                else
                    la_vadd_w(vreg_d, vtemp, vtemp1);
                break;
            case 2:
                la_vexth_du_wu(vtemp, vtemp);
                la_vexth_du_wu(vtemp1, vtemp1);
                if (is_sub)
                    la_vsub_d(vreg_d, vtemp, vtemp1);
                else
                    la_vadd_d(vreg_d, vtemp, vtemp1);
                break;
            case 3:
                la_vexth_qu_du(vtemp, vtemp);
                la_vexth_qu_du(vtemp1, vtemp1);
                if (is_sub)
                    la_vsub_q(vreg_d, vtemp, vtemp1);
                else
                    la_vadd_q(vreg_d, vtemp, vtemp1);
                break;
            default:
                assert(0);
            }
        }
        else{
            switch (size){
            case 0:
                /* 高位扩展 */
                la_vexth_hu_bu(vtemp, vreg_n);
                la_vexth_hu_bu(vtemp1, vreg_m);
                if (is_sub)
                    la_vsub_h(vreg_d, vtemp, vtemp1);
                else
                    la_vadd_h(vreg_d, vtemp, vtemp1);
                break;
            case 1:
                la_vexth_wu_hu(vtemp, vreg_n);
                la_vexth_wu_hu(vtemp1, vreg_m);
                if (is_sub)
                    la_vsub_w(vreg_d, vtemp, vtemp1);
                else
                    la_vadd_w(vreg_d, vtemp, vtemp1);
                break;
            case 2:
                la_vexth_du_wu(vtemp, vreg_n);
                la_vexth_du_wu(vtemp1, vreg_m);
                if (is_sub)
                    la_vsub_d(vreg_d, vtemp, vtemp1);
                else
                    la_vadd_d(vreg_d, vtemp, vtemp1);
                break;
            case 3:
                la_vexth_qu_du(vtemp, vreg_n);
                la_vexth_qu_du(vtemp1, vreg_m);
                if (is_sub)
                    la_vsub_q(vreg_d, vtemp, vtemp1);
                else
                    la_vadd_q(vreg_d, vtemp, vtemp1);
                break;
            default:
                assert(0);
            }
        }

        break;
    case 0x15: /* UABAL, UABAL2 */
        assert(0);
        break;
    case 0x17: /* UABDL, UABDL2 */
        assert(0);
        break;
    case 0x18: /* UMLAL, UMLAL2 */
        assert(0);
        break;
    case 0x1a: /* UMLSL, UMLSL2 */
        assert(0);
        break;
    case 0x1c: /* UMULL, UMULL2 */
        if (!is_q){
            /* 低位统一移至偶数位后做偶数位的相乘并拓2倍宽 */
            switch (size){
            case 0:
                la_vilvl_b(vtemp, vzero, vreg_n);
                la_vilvl_b(vtemp1, vzero, vreg_m);
                la_vmulwev_h_bu(vreg_d, vtemp, vtemp1);
                break;
            case 1:
                la_vilvl_h(vtemp, vzero, vreg_n);
                la_vilvl_h(vtemp1, vzero, vreg_m);
                la_vmulwev_w_hu(vreg_d, vtemp, vtemp1);
                break;
            case 2:
                la_vilvl_w(vtemp, vzero, vreg_n);
                la_vilvl_w(vtemp1, vzero, vreg_m);
                la_vmulwev_d_wu(vreg_d, vtemp, vtemp1);
                break;
            case 3:
                la_vilvl_d(vtemp, vzero, vreg_n);
                la_vilvl_d(vtemp1, vzero, vreg_m);
                la_vmulwev_q_du(vreg_d, vtemp, vtemp1);
                break;
            default:
                assert(0);
            }
        }
        else{
            /* 高位统一移至偶数位后做偶数位的相乘并拓2倍宽 */
            switch (size){
            case 0:
                la_vilvh_b(vtemp, vzero, vreg_n);
                la_vilvh_b(vtemp1, vzero, vreg_m);
                la_vmulwev_h_bu(vreg_d, vtemp, vtemp1);
                break;
            case 1:
                la_vilvh_h(vtemp, vzero, vreg_n);
                la_vilvh_h(vtemp1, vzero, vreg_m);
                la_vmulwev_w_hu(vreg_d, vtemp, vtemp1);
                break;
            case 2:
                la_vilvh_w(vtemp, vzero, vreg_n);
                la_vilvh_w(vtemp1, vzero, vreg_m);
                la_vmulwev_d_wu(vreg_d, vtemp, vtemp1);
                break;
            case 3:
                la_vilvh_d(vtemp, vzero, vreg_n);
                la_vilvh_d(vtemp1, vzero, vreg_m);
                la_vmulwev_q_du(vreg_d, vtemp, vtemp1);
                break;
            default:
                assert(0);
            }
        }

        break;
    default:
        g_assert_not_reached();
    }

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
    free_alloc_fpr(vreg_m);
    free_alloc_fpr(vtemp);
    free_alloc_fpr(vtemp1);
    free_alloc_fpr(vzero);
}

static void handle_3rd_wide(DisasContext *s, int is_q, int is_u, int size,
                            int opcode, int rd, int rn, int rm)
{
    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND vreg_m = alloc_fpr_src(rm);
    IR2_OPND vtemp = ra_alloc_ftemp();
    int is_sub = extract32(opcode, 1, 1);
    
    /* 统一移到高位处理*/
    if(!is_q){
        la_vbsll_v(vtemp, vreg_m, 8);
    }
    else{
        la_vbsll_v(vtemp, vreg_m, 0);
    }

    /* SSUBW/SSUBW2 */
    if(!is_u){
        switch (size){
        case 0:
            /* 高位扩展 */
            la_vexth_h_b(vtemp, vtemp);
            if (is_sub)
                la_vsub_h(vreg_d, vreg_n, vtemp);
            else
                la_vadd_h(vreg_d, vreg_n, vtemp);
            break;
        case 1:
            la_vexth_w_h(vtemp, vtemp);
            if (is_sub)
                la_vsub_w(vreg_d, vreg_n, vtemp);
            else
                la_vadd_w(vreg_d, vreg_n, vtemp);
            break;
        case 2:
            la_vexth_d_w(vtemp, vtemp);
            if (is_sub)
                la_vsub_d(vreg_d, vreg_n, vtemp);
            else
                la_vadd_d(vreg_d, vreg_n, vtemp);
            break;
        case 3:
            la_vexth_q_d(vtemp, vtemp);
            if (is_sub)
                la_vsub_q(vreg_d, vreg_n, vtemp);
            else
                la_vadd_q(vreg_d, vreg_n, vtemp);
            break;
        default:
            assert(0);
        }
    }
    /* USUBW/USUBW2 */
    else{
        switch (size){
        case 0:
            /* 高位扩展 */
            la_vexth_hu_bu(vtemp, vtemp);
            if (is_sub)
                la_vsub_h(vreg_d, vreg_n, vtemp);
            else
                la_vadd_h(vreg_d, vreg_n, vtemp);
            break;
        case 1:
            la_vexth_wu_hu(vtemp, vtemp);
            if (is_sub)
                la_vsub_w(vreg_d, vreg_n, vtemp);
            else
                la_vadd_w(vreg_d, vreg_n, vtemp);
            break;
        case 2:
            la_vexth_du_wu(vtemp, vtemp);
            if (is_sub)
                la_vsub_d(vreg_d, vreg_n, vtemp);
            else
                la_vadd_d(vreg_d, vreg_n, vtemp);
            break;
        case 3:
            la_vexth_qu_du(vtemp, vtemp);
            if (is_sub)
                la_vsub_q(vreg_d, vreg_n, vtemp);
            else
                la_vadd_q(vreg_d, vreg_n, vtemp);
            break;
        default:
            assert(0);
        }
    }

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
    free_alloc_fpr(vreg_m);
    free_alloc_fpr(vtemp);
}


static void handle_3rd_narrowing(DisasContext *s, int is_q, int is_u, int size,
                                 int opcode, int rd, int rn, int rm)
{
    assert(0);
}

/* AdvSIMD three different
 *   31  30  29 28       24 23  22  21 20  16 15    12 11 10 9    5 4    0
 * +---+---+---+-----------+------+---+------+--------+-----+------+------+
 * | 0 | Q | U | 0 1 1 1 0 | size | 1 |  Rm  | opcode | 0 0 |  Rn  |  Rd  |
 * +---+---+---+-----------+------+---+------+--------+-----+------+------+
 */
static void disas_simd_three_reg_diff(DisasContext *s, uint32_t insn)
{
    /* Instructions in this group fall into three basic classes
     * (in each case with the operation working on each element in
     * the input vectors):
     * (1) widening 64 x 64 -> 128 (with possibly Vd as an extra
     *     128 bit input)
     * (2) wide 64 x 128 -> 128
     * (3) narrowing 128 x 128 -> 64
     * Here we do initial decode, catch unallocated cases and
     * dispatch to separate functions for each class.
     */
    int is_q = extract32(insn, 30, 1);
    int is_u = extract32(insn, 29, 1);
    int size = extract32(insn, 22, 2);
    int opcode = extract32(insn, 12, 4);
    int rm = extract32(insn, 16, 5);
    int rn = extract32(insn, 5, 5);
    int rd = extract32(insn, 0, 5);

    switch (opcode) {
    case 1: /* SADDW, SADDW2, UADDW, UADDW2 */
    case 3: /* SSUBW, SSUBW2, USUBW, USUBW2 */
        /* 64 x 128 -> 128 */
        if (size == 3) {
            lata_unallocated_encoding(s);
            return;
        }
        if (!fp_access_check(s)) {
            return;
        }
        handle_3rd_wide(s, is_q, is_u, size, opcode, rd, rn, rm);
        break;
    case 4: /* ADDHN, ADDHN2, RADDHN, RADDHN2 */
    case 6: /* SUBHN, SUBHN2, RSUBHN, RSUBHN2 */
        /* 128 x 128 -> 64 */
        if (size == 3) {
            lata_unallocated_encoding(s);
            return;
        }
        if (!fp_access_check(s)) {
            return;
        }
        handle_3rd_narrowing(s, is_q, is_u, size, opcode, rd, rn, rm);
        break;
    case 14: /* PMULL, PMULL2 */
        if (is_u) {
            lata_unallocated_encoding(s);
            return;
        }
        switch (size) {
        case 0: /* PMULL.P8 */
            if (!fp_access_check(s)) {
                return;
            }
            assert(0);
            break;

        case 3: /* PMULL.P64 */
            if (!dc_isar_feature(aa64_pmull, s)) {
                lata_unallocated_encoding(s);
                return;
            }
            if (!fp_access_check(s)) {
                return;
            }
            assert(0);
            break;

        default:
            lata_unallocated_encoding(s);
            break;
        }
        return;
    case 9: /* SQDMLAL, SQDMLAL2 */
    case 11: /* SQDMLSL, SQDMLSL2 */
    case 13: /* SQDMULL, SQDMULL2 */
        if (is_u || size == 0) {
            lata_unallocated_encoding(s);
            return;
        }
        /* fall through */
    case 0: /* SADDL, SADDL2, UADDL, UADDL2 */
    case 2: /* SSUBL, SSUBL2, USUBL, USUBL2 */
    case 5: /* SABAL, SABAL2, UABAL, UABAL2 */
    case 7: /* SABDL, SABDL2, UABDL, UABDL2 */
    case 8: /* SMLAL, SMLAL2, UMLAL, UMLAL2 */
    case 10: /* SMLSL, SMLSL2, UMLSL, UMLSL2 */
    case 12: /* SMULL, SMULL2, UMULL, UMULL2 */
        /* 64 x 64 -> 128 */
        if (size == 3) {
            lata_unallocated_encoding(s);
            return;
        }
        if (!fp_access_check(s)) {
            return;
        }

        handle_3rd_widening(s, is_q, is_u, size, opcode, rd, rn, rm);
        break;
    default:
        /* opcode 15 not allocated */
        lata_unallocated_encoding(s);
        break;
    }
}

/* Logic op (opcode == 3) subgroup of C3.6.16. */
static void disas_simd_3same_logic(DisasContext *s, uint32_t insn)
{
    int rd = extract32(insn, 0, 5);
    int rn = extract32(insn, 5, 5);
    int rm = extract32(insn, 16, 5);
    int size = extract32(insn, 22, 2);
    bool is_u = extract32(insn, 29, 1);
    bool is_q = extract32(insn, 30, 1);

    if (!fp_access_check(s)) {
        return;
    }

    IR2_OPND vreg_d = alloc_fpr_src(rd); // BSL, BIT, BIF结果受到目的寄存器影响
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND vreg_m = alloc_fpr_src(rm);
    IR2_OPND vtemp = ra_alloc_ftemp();

    switch (size + 4 * is_u) {
    case 0: /* AND */
        la_vand_v(vreg_d, vreg_n, vreg_m);
        break;
    case 1: /* BIC */
        /*  vandn.v vd, vj, vk : 
            vr[vd] = ~vr[vj] & vr[vk]
            LA先对第一个寄存器取反再相与，ARM先对第二个寄存器取反再相与;
        */
        la_vandn_v(vreg_d, vreg_m, vreg_n);
        break;
    case 2: /* ORR */
        la_vor_v(vreg_d, vreg_n, vreg_m);
        break;
    case 3: /* ORN */
        la_vorn_v(vreg_d, vreg_n, vreg_m);
        break;
    case 4: /* EOR */
        la_vxor_v(vreg_d, vreg_n, vreg_m);
        break;

    case 5: /* BSL bitwise select */
        la_vxor_v(vtemp, vreg_m, vreg_n);
        la_vand_v(vtemp, vtemp, vreg_d);
        la_vxor_v(vreg_d, vreg_m, vtemp);
        break;
    case 6: /* BIT, bitwise insert if true */
        la_vxor_v(vtemp, vreg_d, vreg_n);
        la_vand_v(vtemp, vtemp, vreg_m);
        la_vxor_v(vreg_d, vreg_d, vtemp);
        break;
    case 7: /* BIF, bitwise insert if false */
        la_vxor_v(vtemp, vreg_d, vreg_n);
        la_vandn_v(vtemp, vreg_m, vtemp);
        la_vxor_v(vreg_d, vreg_d, vtemp);
        break;

    default:
        g_assert_not_reached();
    }

    if(!is_q){
        /* 高64位清零 */
        la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
    }
    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
    free_alloc_fpr(vreg_m);
    free_alloc_fpr(vtemp);
}

/* Pairwise op subgroup of C3.6.16.
 *
 * This is called directly or via the handle_3same_float for float pairwise
 * operations where the opcode and size are calculated differently.
 */
static void handle_simd_3same_pair(DisasContext *s, int is_q, int u, int opcode,
                                   int size, int rn, int rm, int rd)
{
    // if(!is_q){
    //     assert(0);
    // }

    if (!fp_access_check(s)) {
        return;
    }

    /* These operations work on the concatenated rm:rn, with each pair of
     * adjacent elements being operated on to produce an element in the result.
     */
    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND vreg_m = alloc_fpr_src(rm);
    IR2_OPND vtemp = ra_alloc_ftemp();
    IR2_OPND vtemp1 = ra_alloc_ftemp();

    switch (opcode) {
    case 0x17: /* ADDP */
    {
        switch (size)
        {
        case 0:
            la_vpickev_b(vtemp, vreg_m, vreg_n);
            la_vpickod_b(vtemp1, vreg_m, vreg_n);
            la_vadd_b(vreg_d, vtemp, vtemp1);
            break;
        case 1:
            la_vpickev_h(vtemp, vreg_m, vreg_n);
            la_vpickod_h(vtemp1, vreg_m, vreg_n);
            la_vadd_h(vreg_d, vtemp, vtemp1);
            break;
        case 2:
            la_vpickev_w(vtemp, vreg_m, vreg_n);
            la_vpickod_w(vtemp1, vreg_m, vreg_n);
            la_vadd_w(vreg_d, vtemp, vtemp1);
            break;
        case 3:
            la_vpickev_d(vtemp, vreg_m, vreg_n);
            la_vpickod_d(vtemp1, vreg_m, vreg_n);
            la_vadd_d(vreg_d, vtemp, vtemp1);
            break;
        default:
            assert(0);
        }

        if(!is_q){
            la_vpickev_w(vreg_d,vreg_d,vreg_d);
        }

        break;
    }
    case 0x14: /* SMAXP, UMAXP */
    {
        switch (size)
        {
        case 0:
            /* pack vector pair*/
            la_vpickev_b(vtemp, vreg_m, vreg_n);
            la_vpickod_b(vtemp1, vreg_m, vreg_n);
            
            /* compare*/
            if(u) {//UMAXP
                la_vmax_bu(vreg_d, vtemp, vtemp1);
            } else {//SMAXP
                la_vmax_b(vreg_d, vtemp, vtemp1);
            }
            break;
        case 1:
            la_vpickev_h(vtemp, vreg_m, vreg_n);
            la_vpickod_h(vtemp1, vreg_m, vreg_n);

            if(u) {
                la_vmax_hu(vreg_d, vtemp, vtemp1);
            } else {
                la_vmax_h(vreg_d, vtemp, vtemp1);
            }
            break;
        case 2:
            la_vpickev_w(vtemp, vreg_m, vreg_n);
            la_vpickod_w(vtemp1, vreg_m, vreg_n);

            if(u) {
                la_vmax_wu(vreg_d, vtemp, vtemp1);
            } else {
                la_vmax_w(vreg_d, vtemp, vtemp1);
            }
            break;
        default:
            assert(0);
        }

        /* move vector to the lower*/
        if(!is_q){
            la_vpickev_w(vreg_d,vreg_d,vreg_d);
        }
        break;
    }
    case 0x15: /* SMINP, UMINP */
    {
        switch (size)
        {
        case 0:
            /* pack vector pair*/
            la_vpickev_b(vtemp, vreg_m, vreg_n);
            la_vpickod_b(vtemp1, vreg_m, vreg_n);
            
            /* compare*/
            if(u) {//UMINP
                la_vmin_bu(vreg_d, vtemp, vtemp1);
            } else {//SMINP
                la_vmin_b(vreg_d, vtemp, vtemp1);
            }
            break;
        case 1:
            la_vpickev_h(vtemp, vreg_m, vreg_n);
            la_vpickod_h(vtemp1, vreg_m, vreg_n);

            if(u) {
                la_vmin_hu(vreg_d, vtemp, vtemp1);
            } else {
                la_vmin_h(vreg_d, vtemp, vtemp1);
            }
            break;
        case 2:
            la_vpickev_w(vtemp, vreg_m, vreg_n);
            la_vpickod_w(vtemp1, vreg_m, vreg_n);

            if(u) {
                la_vmin_wu(vreg_d, vtemp, vtemp1);
            } else {
                la_vmin_w(vreg_d, vtemp, vtemp1);
            }
            break;
        default:
            assert(0);
        }

        /* move vector to the lower*/
        if(!is_q){
            la_vpickev_w(vreg_d,vreg_d,vreg_d);
        }
        break;
    }
    /* The FP operations are all on single floats (32 bit) */
    case 0x58: /* FMAXNMP */
        assert(0);
        break;
    case 0x5a: /* FADDP */
        switch (size)
        {
        case 2:
            la_vpickev_w(vtemp, vreg_m, vreg_n);
            la_vpickod_w(vtemp1, vreg_m, vreg_n);
            la_vfadd_s(vreg_d, vtemp, vtemp1);
            break;
        case 3:
            la_vpickev_d(vtemp, vreg_m, vreg_n);
            la_vpickod_d(vtemp1, vreg_m, vreg_n);
            la_vfadd_d(vreg_d, vtemp, vtemp1);
            break;
        default:
            assert(0);
        }

        if(!is_q){
            la_vpickev_w(vreg_d, vreg_d, vreg_d);
        }
        
        break;
    case 0x5e: /* FMAXP */
        assert(0);
        break;
    case 0x78: /* FMINNMP */
        assert(0);
        break;
    case 0x7e: /* FMINP */
        assert(0);
        break;
    default:
        g_assert_not_reached();
    }

    if(!is_q){
        /* 高64位清零 */
        la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
    }

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
    free_alloc_fpr(vreg_m);
    free_alloc_fpr(vtemp);
    free_alloc_fpr(vtemp1);
}

/* Floating point op subgroup of C3.6.16. */
static void disas_simd_3same_float(DisasContext *s, uint32_t insn)
{
    /* For floating point ops, the U, size[1] and opcode bits
     * together indicate the operation. size[0] indicates single
     * or double.
     */
    int fpopcode = extract32(insn, 11, 5)
        | (extract32(insn, 23, 1) << 5)
        | (extract32(insn, 29, 1) << 6);
    int is_q = extract32(insn, 30, 1);
    int size = extract32(insn, 22, 1);
    int rm = extract32(insn, 16, 5);
    int rn = extract32(insn, 5, 5);
    int rd = extract32(insn, 0, 5);

    int datasize = is_q ? 128 : 64;
    int esize = 32 << size;
    int elements = datasize / esize;

    if (size == 1 && !is_q) {
        lata_unallocated_encoding(s);
        return;
    }

    switch (fpopcode) {
    case 0x58: /* FMAXNMP */
    case 0x5a: /* FADDP */
    case 0x5e: /* FMAXP */
    case 0x78: /* FMINNMP */
    case 0x7e: /* FMINP */
        if (size && !is_q) {
            lata_unallocated_encoding(s);
            return;
        }
        handle_simd_3same_pair(s, is_q, 0, fpopcode, size ? MO_64 : MO_32,
                               rn, rm, rd);
        return;
    case 0x1b: /* FMULX */
    case 0x1f: /* FRECPS */
    case 0x3f: /* FRSQRTS */
    case 0x5d: /* FACGE */
    case 0x7d: /* FACGT */
    case 0x19: /* FMLA */
    case 0x39: /* FMLS */
    case 0x18: /* FMAXNM */
    case 0x1a: /* FADD */
    case 0x1c: /* FCMEQ */
    case 0x1e: /* FMAX */
    case 0x38: /* FMINNM */
    case 0x3a: /* FSUB */
    case 0x3e: /* FMIN */
    case 0x5b: /* FMUL */
    case 0x5c: /* FCMGE */
    case 0x5f: /* FDIV */
    case 0x7a: /* FABD */
    case 0x7c: /* FCMGT */
        if (!fp_access_check(s)) {
            return;
        }
        handle_3same_float(s, size, elements, fpopcode, rd, rn, rm);
        return;

    case 0x1d: /* FMLAL  */
    case 0x3d: /* FMLSL  */
    case 0x59: /* FMLAL2 */
    case 0x79: /* FMLSL2 */
        if (size & 1 || !dc_isar_feature(aa64_fhm, s)) {
            lata_unallocated_encoding(s);
            return;
        }
        if (fp_access_check(s)) {
            assert(0);
        }
        return;

    default:
        lata_unallocated_encoding(s);
        return;
    }
}

/* Integer op subgroup of C3.6.16. */
static void disas_simd_3same_int(DisasContext *s, uint32_t insn)
{
    int is_q = extract32(insn, 30, 1);
    int u = extract32(insn, 29, 1);
    int size = extract32(insn, 22, 2);
    int opcode = extract32(insn, 11, 5);
    int rm = extract32(insn, 16, 5);
    int rn = extract32(insn, 5, 5);
    int rd = extract32(insn, 0, 5);

    switch (opcode) {
    case 0x13: /* MUL, PMUL */
        if (u && size != 0) {
            lata_unallocated_encoding(s);
            return;
        }
        /* fall through */
    case 0x0: /* SHADD, UHADD */
    case 0x2: /* SRHADD, URHADD */
    case 0x4: /* SHSUB, UHSUB */
    case 0xc: /* SMAX, UMAX */
    case 0xd: /* SMIN, UMIN */
    case 0xe: /* SABD, UABD */
    case 0xf: /* SABA, UABA */
    case 0x12: /* MLA, MLS */
        if (size == 3) {
            lata_unallocated_encoding(s);
            return;
        }
        break;
    case 0x16: /* SQDMULH, SQRDMULH */
        if (size == 0 || size == 3) {
            lata_unallocated_encoding(s);
            return;
        }
        break;
    default:
        if (size == 3 && !is_q) {
            lata_unallocated_encoding(s);
            return;
        }
        break;
    }

    if (!fp_access_check(s)) {
        return;
    }

    IR2_OPND vreg_d;
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND vreg_m = alloc_fpr_src(rm);
    if(opcode == 0x12){
        vreg_d = alloc_fpr_src(rd);
    }else{
        vreg_d = alloc_fpr_dst(rd);
    }
    /* 分别执行左移和右移 */
    IR2_OPND vtemp = ra_alloc_ftemp();
    IR2_OPND vleft = ra_alloc_ftemp();
    IR2_OPND vright = ra_alloc_ftemp();

    switch (opcode) {
    case 0x01: /* SQADD, UQADD */
        assert(0);
        return;
    case 0x05: /* SQSUB, UQSUB */
        assert(0);
        return;
    case 0x08: /* SSHL, USHL */
        switch (size){
            case 0:
                /* 取元素中shift */
                la_vslli_b(vleft, vreg_m, 0);
                la_vsrli_b(vleft, vleft, 0);

                /* 正数放vleft左移，负数取反后放vright右移，vtemp存放符号位*/
                la_vslti_b(vtemp, vleft, 0);
                la_vand_v(vright, vtemp, vleft);
                la_vsub_b(vleft, vleft, vright);
                la_vsigncov_b(vright, vtemp, vright);

                /* ARM 移位量不做模运算，这里需要特殊处理，左移大于等于元素长度对应清零，右移大于等于元素长度shift-1*/
                la_vsll_b(vreg_d, vreg_n, vleft);
                if(!u){
                    la_vsra_b(vreg_d, vreg_d, vright);
                }
                else{
                    la_vsrl_b(vreg_d, vreg_d, vright);
                }
                la_vslei_bu(vleft, vleft, 7);
                la_vslei_bu(vright, vright, 7);                
                la_vand_v(vtemp, vreg_d, vleft);

                /* 正常右移 */
                
                la_vand_v(vtemp, vtemp, vright);
                /* 满位右移 */
                la_vnori_b(vright, vright, 0);
                if(!u){
                    la_vsrai_b(vleft, vreg_n, 7); 
                }
                else{
                    la_vandi_b(vleft, vreg_n, 0);
                }
                la_vand_v(vleft, vright, vleft);
                la_vadd_b(vreg_d, vtemp, vleft);

                break;
            case 1:
                /* 取元素中shift */
                la_vslli_h(vleft, vreg_m, 8);
                la_vsrli_h(vleft, vleft, 8);

                /* 正数放vleft左移，负数取反后放vright右移，vtemp存放符号位*/
                la_vslti_b(vtemp, vleft, 0);
                la_vand_v(vright, vtemp, vleft);
                la_vsub_b(vleft, vleft, vright);
                la_vsigncov_b(vright, vtemp, vright);

                /* ARM 移位量不做模运算，这里需要特殊处理，左移大于等于元素长度对应清零，右移大于等于元素长度shift-1*/
                la_vsll_h(vreg_d, vreg_n, vleft);
                if(!u){
                    la_vsra_h(vreg_d, vreg_d, vright);
                }
                else{
                    la_vsrl_h(vreg_d, vreg_d, vright);
                }
                la_vslei_hu(vleft, vleft, 15);
                la_vslei_hu(vright, vright, 15);                
                la_vand_v(vtemp, vreg_d, vleft);

                /* 正常右移 */
                
                la_vand_v(vtemp, vtemp, vright);
                /* 满位右移 */
                la_vnori_b(vright, vright, 0);
                if(!u){
                    la_vsrai_h(vleft, vreg_n, 15); 
                }
                else{
                    la_vandi_b(vleft, vreg_n, 0);
                }
                la_vand_v(vleft, vright, vleft);
                la_vadd_h(vreg_d, vtemp, vleft);

                break;
            case 2:
                /* 取元素中shift */
                la_vslli_w(vleft, vreg_m, 24);
                la_vsrli_w(vleft, vleft, 24);

                /* 正数放vleft左移，负数取反后放vright右移，vtemp存放符号位*/
                la_vslti_b(vtemp, vleft, 0);
                la_vand_v(vright, vtemp, vleft);
                la_vsub_b(vleft, vleft, vright);
                la_vsigncov_b(vright, vtemp, vright);

                /* ARM 移位量不做模运算，这里需要特殊处理，左移大于等于元素长度对应清零，右移大于等于元素长度shift-1*/
                la_vsll_w(vreg_d, vreg_n, vleft);
                if(!u){
                    la_vsra_w(vreg_d, vreg_d, vright);
                }
                else{
                    la_vsrl_w(vreg_d, vreg_d, vright);
                }
                la_vslei_wu(vleft, vleft, 31);
                la_vslei_wu(vright, vright, 31);                
                la_vand_v(vtemp, vreg_d, vleft);

                /* 正常右移 */
                
                la_vand_v(vtemp, vtemp, vright);
                /* 满位右移 */
                la_vnori_b(vright, vright, 0);
                if(!u){
                    la_vsrai_w(vleft, vreg_n, 31); 
                }
                else{
                    la_vandi_b(vleft, vreg_n, 0);
                }
                la_vand_v(vleft, vright, vleft);
                la_vadd_w(vreg_d, vtemp, vleft);

                break;
            case 3:
                /* 取元素中shift */
                la_vslli_d(vleft, vreg_m, 56);
                la_vsrli_d(vleft, vleft, 56);

                /* 正数放vleft左移，负数取反后放vright右移，vtemp存放符号位*/
                la_vslti_b(vtemp, vleft, 0);
                la_vand_v(vright, vtemp, vleft);
                la_vsub_b(vleft, vleft, vright);
                la_vsigncov_b(vright, vtemp, vright);

                /* ARM 移位量不做模运算，这里需要特殊处理，左移大于等于元素长度对应清零，右移大于等于元素长度shift-1*/
                /* vslti的立即数限制5位，进行特殊处理，右移缩小2倍与16比较 */
                la_vsll_d(vreg_d, vreg_n, vleft);
                if(!u){
                    la_vsra_d(vreg_d, vreg_d, vright);
                }
                else{
                    la_vsrl_d(vreg_d, vreg_d, vright);
                }
                la_vsrli_b(vtemp, vleft, 2);
                la_vslti_du(vleft, vtemp, 16);
                la_vsrli_b(vright, vright, 2);
                la_vslti_du(vright, vright, 16);

                // la_vsll_d(vtemp, vreg_n, vleft);
                la_vand_v(vtemp, vreg_d, vleft);

                /* 正常右移 */
                la_vand_v(vtemp, vtemp, vright);
                /* 满位右移 */
                la_vnori_b(vright, vright, 0);
                if(!u){
                    la_vsrai_d(vleft, vreg_n, 63); 
                }
                else{
                    la_vandi_b(vleft, vreg_n, 0);
                }
                la_vand_v(vleft, vright, vleft);
                la_vadd_d(vreg_d, vtemp, vleft);

                break;
            default:
                assert(0);
            }    

        goto do_gvec_end;
    case 0x0c: /* SMAX, UMAX */
        switch(size){
            case 0:
                if(u){
                    la_vmax_bu(vreg_d, vreg_n, vreg_m);
                }else{
                    la_vmax_b(vreg_d, vreg_n, vreg_m);
                }
                break;
            case 1:
                if(u){
                    la_vmax_hu(vreg_d, vreg_n, vreg_m);
                }else{
                    la_vmax_h(vreg_d, vreg_n, vreg_m);
                }
                break;
            case 2:
                if(u){
                    la_vmax_wu(vreg_d, vreg_n, vreg_m);
                }else{
                    la_vmax_w(vreg_d, vreg_n, vreg_m);
        }
                break;
            default:
                assert(0);
        }
        goto do_gvec_end;
    case 0x0d: /* SMIN, UMIN */
        switch(size){
            case 0:
                if(u){
                    la_vmin_bu(vreg_d, vreg_n, vreg_m);
                }else{
                    la_vmin_b(vreg_d, vreg_n, vreg_m);
                }
                break;
            case 1:
                if(u){
                    la_vmin_hu(vreg_d, vreg_n, vreg_m);
                }else{
                    la_vmin_h(vreg_d, vreg_n, vreg_m);
                }
                break;
            case 2:
                if(u){
                    la_vmin_wu(vreg_d, vreg_n, vreg_m);
                }else{
                    la_vmin_w(vreg_d, vreg_n, vreg_m);
        }
                break;
            default:
                assert(0);
        }
        goto do_gvec_end;
    case 0xe: /* SABD, UABD */
        assert(0);
        return;
    case 0xf: /* SABA, UABA */
        assert(0);
        return;
    case 0x10: /* ADD, SUB */
        switch(size){
            case 0:
                if(u){
                    la_vsub_b(vreg_d, vreg_n, vreg_m);
                }else{
                    la_vadd_b(vreg_d, vreg_n, vreg_m);
                }
                break;
            case 1:
                if(u){
                    la_vsub_h(vreg_d, vreg_n, vreg_m);
                }else{
                    la_vadd_h(vreg_d, vreg_n, vreg_m);
                }
                break;
            case 2:
                if(u){
                    la_vsub_w(vreg_d, vreg_n, vreg_m);
                }else{
                    la_vadd_w(vreg_d, vreg_n, vreg_m);
                }
                break;
            case 3:
                assert(is_q);
                if(u){
                    la_vsub_d(vreg_d, vreg_n, vreg_m);
                }else{
                    la_vadd_d(vreg_d, vreg_n, vreg_m);
                }
                break;
            default:
                assert(0);
        }
        goto do_gvec_end;
    case 0x13: /* MUL, PMUL */
        if (!u) { /* MUL */
            // gen_gvec_fn3(s, is_q, rd, rn, rm, tcg_gen_gvec_mul, size);
            switch(size){
                case 0:
                    la_vmul_b(vreg_d, vreg_n, vreg_m);
                    break;
                case 1:
                    la_vmul_h(vreg_d, vreg_n, vreg_m);
                    break;
                case 2:
                    la_vmul_w(vreg_d, vreg_n, vreg_m);
                    break;
                case 3:
                    la_vmul_d(vreg_d, vreg_n, vreg_m);
                    break;
                default:
                    assert(0);
            }
        } else {  /* PMUL */
            assert(0);
            
        }
        goto do_gvec_end;
    case 0x12: /* MLA, MLS */
        switch(size){
            case 0:
                if(u){
                    la_vmsub_b(vreg_d, vreg_n, vreg_m);
                }else{
                    la_vmadd_b(vreg_d, vreg_n, vreg_m);
                }
                break;
            case 1:
                if(u){
                    la_vmsub_h(vreg_d, vreg_n, vreg_m);
                }else{
                    la_vmadd_h(vreg_d, vreg_n, vreg_m);
                }
                break;
            case 2:
                if(u){
                    la_vmsub_w(vreg_d, vreg_n, vreg_m);
                }else{
                    la_vmadd_w(vreg_d, vreg_n, vreg_m);
                }
                break;
            case 3:
                assert(is_q);
                if(u){
                    la_vmsub_d(vreg_d, vreg_n, vreg_m);
                }else{
                    la_vmadd_d(vreg_d, vreg_n, vreg_m);
                }
                break;
            default:
                assert(0);
        }
        goto do_gvec_end;
    case 0x16: /* SQDMULH, SQRDMULH */
        assert(0);
        return;
    case 0x11: /* CMEQ, CMTST */
        if(u){ /* CMEQ */
            switch (size)
            {
            case 0:
                la_vseq_b(vreg_d, vreg_n, vreg_m);
                break;
            case 1:
                la_vseq_h(vreg_d, vreg_n, vreg_m);
                break;
            case 2:
                la_vseq_w(vreg_d, vreg_n, vreg_m);
                break;
            case 3:
                la_vseq_d(vreg_d, vreg_n, vreg_m);
                break;
            default:
                assert(0);
            }
        }else{ /* CMTST */
            la_vand_v(vreg_d, vreg_n, vreg_m);
            switch (size)
            {
            case 0:
                la_vseqi_b(vreg_d, vreg_d, 0);
                break;
            case 1:
                la_vseqi_h(vreg_d, vreg_d, 0);
                break;
            case 2:
                la_vseqi_w(vreg_d, vreg_d, 0);
                break;
            case 3:
                la_vseqi_d(vreg_d, vreg_d, 0);
                break;
            default:
                assert(0);
            }
            la_vnori_b(vreg_d, vreg_d, 0);
        }
        goto do_gvec_end;
    case 0x06: /* CMGT, CMHI */
        if(u){ /* CMHI */
            switch (size)
            {
            case 0:
                la_vslt_bu(vreg_d, vreg_m, vreg_n);
                break;
            case 1:
                la_vslt_hu(vreg_d, vreg_m, vreg_n);
                break;
            case 2:
                la_vslt_wu(vreg_d, vreg_m, vreg_n);
                break;
            case 3:
                la_vslt_du(vreg_d, vreg_m, vreg_n);
                break;
            default:
                assert(0);
            }
        }else{ /* CMGT */
            switch (size)
            {
            case 0:
                la_vslt_b(vreg_d, vreg_m, vreg_n);
                break;
            case 1:
                la_vslt_h(vreg_d, vreg_m, vreg_n);
                break;
            case 2:
                la_vslt_w(vreg_d, vreg_m, vreg_n);
                break;
            case 3:
                la_vslt_d(vreg_d, vreg_m, vreg_n);
                break;
            default:
                assert(0);
            }
        }
        goto do_gvec_end;
    case 0x07: /* CMGE, CMHS */
        if(u){ /* CMHS */
            switch (size)
            {
            case 0:
                la_vsle_bu(vreg_d, vreg_m, vreg_n);
                break;
            case 1:
                la_vsle_hu(vreg_d, vreg_m, vreg_n);
                break;
            case 2:
                la_vsle_wu(vreg_d, vreg_m, vreg_n);
                break;
            case 3:
                la_vsle_du(vreg_d, vreg_m, vreg_n);
                break;
            default:
                assert(0);
            }
        }else{ /* CMGE */
            switch (size)
            {
            case 0:
                la_vsle_b(vreg_d, vreg_m, vreg_n);
                break;
            case 1:
                la_vsle_h(vreg_d, vreg_m, vreg_n);
                break;
            case 2:
                la_vsle_w(vreg_d, vreg_m, vreg_n);
                break;
            case 3:
                la_vsle_d(vreg_d, vreg_m, vreg_n);
                break;
            default:
                assert(0);
            }
        }
    do_gvec_end:
        if(!is_q){
            /* 高64位清零 */
            la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
        }
        store_fpr_dst(rd, vreg_d);
        free_alloc_fpr(vreg_d);
        free_alloc_fpr(vreg_n);
        free_alloc_fpr(vreg_m);
        free_alloc_fpr(vtemp);
        free_alloc_fpr(vleft);
        free_alloc_fpr(vright);
        return;
    }

    if (size == 3) {
        assert(is_q);
    } else {
        assert(0);
    }
}

/* AdvSIMD three same
 *  31  30  29  28       24 23  22  21 20  16 15    11  10 9    5 4    0
 * +---+---+---+-----------+------+---+------+--------+---+------+------+
 * | 0 | Q | U | 0 1 1 1 0 | size | 1 |  Rm  | opcode | 1 |  Rn  |  Rd  |
 * +---+---+---+-----------+------+---+------+--------+---+------+------+
 */
static void disas_simd_three_reg_same(DisasContext *s, uint32_t insn)
{
    int opcode = extract32(insn, 11, 5);

    switch (opcode) {
    case 0x3: /* logic ops */
        disas_simd_3same_logic(s, insn);
        break;
    case 0x17: /* ADDP */
    case 0x14: /* SMAXP, UMAXP */
    case 0x15: /* SMINP, UMINP */
    {
        /* Pairwise operations */
        int is_q = extract32(insn, 30, 1);
        int u = extract32(insn, 29, 1);
        int size = extract32(insn, 22, 2);
        int rm = extract32(insn, 16, 5);
        int rn = extract32(insn, 5, 5);
        int rd = extract32(insn, 0, 5);
        if (opcode == 0x17) {
            if (u || (size == 3 && !is_q)) {
                lata_unallocated_encoding(s);
                return;
            }
        } else {
            if (size == 3) {
                lata_unallocated_encoding(s);
                return;
            }
        }
        handle_simd_3same_pair(s, is_q, u, opcode, size, rn, rm, rd);
        break;
    }
    case 0x18 ... 0x31:
        /* floating point ops, sz[1] and U are part of opcode */
        disas_simd_3same_float(s, insn);
        break;
    default:
        disas_simd_3same_int(s, insn);
        break;
    }
}

/*
 * Advanced SIMD three same (ARMv8.2 FP16 variants)
 *
 *  31  30  29  28       24 23  22 21 20  16 15 14 13    11 10  9    5 4    0
 * +---+---+---+-----------+---------+------+-----+--------+---+------+------+
 * | 0 | Q | U | 0 1 1 1 0 | a | 1 0 |  Rm  | 0 0 | opcode | 1 |  Rn  |  Rd  |
 * +---+---+---+-----------+---------+------+-----+--------+---+------+------+
 *
 * This includes FMULX, FCMEQ (register), FRECPS, FRSQRTS, FCMGE
 * (register), FACGE, FABD, FCMGT (register) and FACGT.
 *
 */
static void disas_simd_three_reg_same_fp16(DisasContext *s, uint32_t insn)
{
    assert(0);
}

/* AdvSIMD three same extra
 *  31   30  29 28       24 23  22  21 20  16  15 14    11  10 9  5 4  0
 * +---+---+---+-----------+------+---+------+---+--------+---+----+----+
 * | 0 | Q | U | 0 1 1 1 0 | size | 0 |  Rm  | 1 | opcode | 1 | Rn | Rd |
 * +---+---+---+-----------+------+---+------+---+--------+---+----+----+
 */
static void disas_simd_three_reg_same_extra(DisasContext *s, uint32_t insn)
{
    assert(0);
}

static void handle_2misc_widening(DisasContext *s, int opcode, bool is_q,
                                  int size, int rn, int rd)
{
    /* Handle 2-reg-misc ops which are widening (so each size element
     * in the source becomes a 2*size element in the destination.
     * The only instruction like this is FCVTL.
     */
    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    if (size == 3) {
        /* 32 -> 64 bit fp conversion */
        if(is_q){
            la_vfcvth_d_s(vreg_d, vreg_n);
        }else{
            la_vfcvtl_d_s(vreg_d, vreg_n);
        }
    } else {
        /* 16 -> 32 bit fp conversion */
        assert(0);
    }

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
}

static void handle_rev(DisasContext *s, int opcode, bool u,
                       bool is_q, int size, int rn, int rd)
{
    int op = (opcode << 1) | u;
    int opsz = op + size;
    // int grp_size = 3 - opsz;
    // int dsize = is_q ? 128 : 64;
    int i;

    if (opsz >= 3) {
        lata_unallocated_encoding(s);
        return;
    }

    if (!fp_access_check(s)) {
        return;
    }
    
    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND temp = ra_alloc_itemp();

    switch(op){
        case 0: /* REV64 */
            for(i = 0; i < (is_q ? 2 : 1); ++i){
                la_vpickve2gr_d(temp, vreg_n, i);
                switch (size)
                {
                case 0:
                    la_revb_d(temp, temp);
                    break;
                case 1:
                    la_revh_d(temp, temp);
                    break;
                case 2:
                    la_rotri_d(temp, temp, 32);
                    break;
                default:
                    break;
                }
                la_vinsgr2vr_d(vreg_d, temp, i);
            }
            break;
        case 1: /* REV32 */
            for(i = 0; i < (is_q ? 2 : 1); ++i){
                la_vpickve2gr_d(temp, vreg_n, i);
                switch (size)
                {
                case 0:
                    la_revb_2w(temp, temp);
                    break;
                case 1:
                    la_revh_2w(temp, temp);
                    break;
                default:
                    break;
                }
                la_vinsgr2vr_d(vreg_d, temp, i);
            }
            break;
        case 2: /* REV16 */
            assert(0);
            break;
    }

    if(!is_q){
        la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
    }
    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
    free_alloc_gpr(temp);
}

static void handle_2misc_pairwise(DisasContext *s, int opcode, bool u,
                                  bool is_q, int size, int rn, int rd)
{
    /* Implement the pairwise operations from 2-misc:
     * SADDLP, UADDLP, SADALP, UADALP.
     * These all add pairs of elements in the input to produce a
     * double-width result element in the output (possibly accumulating).
     */
    bool accum = (opcode == 0x6);
    IR2_OPND vreg_d;
    IR2_OPND vreg_n = alloc_fpr_src(rn);

    if(accum) {
        vreg_d = alloc_fpr_src(rd);
        IR2_OPND vtemp = ra_alloc_ftemp();
        if(u) { // UADALP
            switch(size){
            case 0:
                la_vhaddw_hu_bu(vtemp, vreg_n, vreg_n);
                la_vadd_h(vreg_d, vreg_d, vtemp);
                break;
            case 1:
                la_vhaddw_wu_hu(vtemp, vreg_n, vreg_n);
                la_vadd_w(vreg_d, vreg_d, vtemp);
                break;
            case 2:
                la_vhaddw_du_wu(vtemp, vreg_n, vreg_n);
                la_vadd_d(vreg_d, vreg_d, vtemp);
                break;
            default:
                assert(0);
            }
        } else { // SADALP
            switch(size){
            case 0:
                la_vhaddw_h_b(vtemp, vreg_n, vreg_n);
                la_vadd_h(vreg_d, vreg_d, vtemp);
                break;
            case 1:
                la_vhaddw_w_h(vtemp, vreg_n, vreg_n);
                la_vadd_w(vreg_d, vreg_d, vtemp);
                break;
            case 2:
                la_vhaddw_d_w(vtemp, vreg_n, vreg_n);
                la_vadd_d(vreg_d, vreg_d, vtemp);
                break;
            default:
                assert(0);
            }
        }
        free_alloc_gpr(vtemp);
    } else {
        vreg_d = alloc_fpr_dst(rd);
        if(u) { // UADDLP
            switch(size){
            case 0:
                la_vhaddw_hu_bu(vreg_d, vreg_n, vreg_n);
                break;
            case 1:
                la_vhaddw_wu_hu(vreg_d, vreg_n, vreg_n);
                break;
            case 2:
                la_vhaddw_du_wu(vreg_d, vreg_n, vreg_n);
                break;
            default:
                assert(0);
            }
        } else { // SADDLP
            switch(size){
            case 0:
                la_vhaddw_h_b(vreg_d, vreg_n, vreg_n);
                break;
            case 1:
                la_vhaddw_w_h(vreg_d, vreg_n, vreg_n);
                break;
            case 2:
                la_vhaddw_d_w(vreg_d, vreg_n, vreg_n);
                break;
            default:
                assert(0);
            }
        }
    }

    if (!is_q) {
        la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
    }

    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
}

static void handle_shll(DisasContext *s, bool is_q, int size, int rn, int rd)
{
    assert(0);
}

/* AdvSIMD two reg misc
 *   31  30  29 28       24 23  22 21       17 16    12 11 10 9    5 4    0
 * +---+---+---+-----------+------+-----------+--------+-----+------+------+
 * | 0 | Q | U | 0 1 1 1 0 | size | 1 0 0 0 0 | opcode | 1 0 |  Rn  |  Rd  |
 * +---+---+---+-----------+------+-----------+--------+-----+------+------+
 */
static void disas_simd_two_reg_misc(DisasContext *s, uint32_t insn)
{
    int size = extract32(insn, 22, 2);
    int opcode = extract32(insn, 12, 5);
    bool u = extract32(insn, 29, 1);
    bool is_q = extract32(insn, 30, 1);
    int rn = extract32(insn, 5, 5);
    int rd = extract32(insn, 0, 5);

    switch (opcode) {
    case 0x0: /* REV64, REV32 */
    case 0x1: /* REV16 */
        handle_rev(s, opcode, u, is_q, size, rn, rd);
        return;
    case 0x5: /* CNT, NOT, RBIT */
        if (u && size == 0) {
            /* NOT */
            break;
        } else if (u && size == 1) {
            /* RBIT */
            break;
        } else if (!u && size == 0) {
            /* CNT */
            break;
        }
        lata_unallocated_encoding(s);
        return;
    case 0x12: /* XTN, XTN2, SQXTUN, SQXTUN2 */
    case 0x14: /* SQXTN, SQXTN2, UQXTN, UQXTN2 */
        if (size == 3) {
            lata_unallocated_encoding(s);
            return;
        }
        if (!fp_access_check(s)) {
            return;
        }

        handle_2misc_narrow(s, false, opcode, u, is_q, size, rn, rd);
        return;
    case 0x4: /* CLS, CLZ */
        if (size == 3) {
            lata_unallocated_encoding(s);
            return;
        }
        break;
    case 0x2: /* SADDLP, UADDLP */
    case 0x6: /* SADALP, UADALP */
        if (size == 3) {
            lata_unallocated_encoding(s);
            return;
        }
        if (!fp_access_check(s)) {
            return;
        }
        handle_2misc_pairwise(s, opcode, u, is_q, size, rn, rd);
        return;
    case 0x13: /* SHLL, SHLL2 */
        if (u == 0 || size == 3) {
            lata_unallocated_encoding(s);
            return;
        }
        if (!fp_access_check(s)) {
            return;
        }
        handle_shll(s, is_q, size, rn, rd);
        return;
    case 0xa: /* CMLT */
        if (u == 1) {
            lata_unallocated_encoding(s);
            return;
        }
        /* fall through */
    case 0x8: /* CMGT, CMGE */
    case 0x9: /* CMEQ, CMLE */
    case 0xb: /* ABS, NEG */
        if (size == 3 && !is_q) {
            lata_unallocated_encoding(s);
            return;
        }
        break;
    case 0x3: /* SUQADD, USQADD */
        if (size == 3 && !is_q) {
            lata_unallocated_encoding(s);
            return;
        }
        if (!fp_access_check(s)) {
            return;
        }
        handle_2misc_satacc(s, false, u, is_q, size, rn, rd);
        return;
    case 0x7: /* SQABS, SQNEG */
        if (size == 3 && !is_q) {
            lata_unallocated_encoding(s);
            return;
        }
        break;
    case 0xc ... 0xf:
    case 0x16 ... 0x1f:
    {
        /* Floating point: U, size[1] and opcode indicate operation;
         * size[0] indicates single or double precision.
         */
        int is_double = extract32(size, 0, 1);
        opcode |= (extract32(size, 1, 1) << 5) | (u << 6);
        size = is_double ? 3 : 2;
        switch (opcode) {
        case 0x2f: /* FABS */
        case 0x6f: /* FNEG */
            if (size == 3 && !is_q) {
                lata_unallocated_encoding(s);
                return;
            }
            break;
        case 0x1d: /* SCVTF */
        case 0x5d: /* UCVTF */
        {
            bool is_signed = (opcode == 0x1d) ? true : false;
            int elements = is_double ? 2 : is_q ? 4 : 2;
            if (is_double && !is_q) {
                lata_unallocated_encoding(s);
                return;
            }
            if (!fp_access_check(s)) {
                return;
            }
            handle_simd_intfp_conv(s, rd, rn, elements, is_signed, 0, size);
            return;
        }
        case 0x2c: /* FCMGT (zero) */
        case 0x2d: /* FCMEQ (zero) */
        case 0x2e: /* FCMLT (zero) */
        case 0x6c: /* FCMGE (zero) */
        case 0x6d: /* FCMLE (zero) */
            if (size == 3 && !is_q) {
                lata_unallocated_encoding(s);
                return;
            }
            handle_2misc_fcmp_zero(s, opcode, false, u, is_q, size, rn, rd);
            return;
        case 0x7f: /* FSQRT */
            if (size == 3 && !is_q) {
                lata_unallocated_encoding(s);
                return;
            }
            break;
        case 0x1a: /* FCVTNS */
        case 0x1b: /* FCVTMS */
        case 0x3a: /* FCVTPS */
        case 0x3b: /* FCVTZS */
        case 0x5a: /* FCVTNU */
        case 0x5b: /* FCVTMU */
        case 0x7a: /* FCVTPU */
        case 0x7b: /* FCVTZU */
            // need_fpstatus = true;
            // rmode = extract32(opcode, 5, 1) | (extract32(opcode, 0, 1) << 1);
            if (size == 3 && !is_q) {
                lata_unallocated_encoding(s);
                return;
            }
            break;
        case 0x5c: /* FCVTAU */
        case 0x1c: /* FCVTAS */
            // need_fpstatus = true;
            // rmode = FPROUNDING_TIEAWAY;
            if (size == 3 && !is_q) {
                lata_unallocated_encoding(s);
                return;
            }
            break;
        case 0x3c: /* URECPE */
            if (size == 3) {
                lata_unallocated_encoding(s);
                return;
            }
            /* fall through */
        case 0x3d: /* FRECPE */
        case 0x7d: /* FRSQRTE */
            if (size == 3 && !is_q) {
                lata_unallocated_encoding(s);
                return;
            }
            if (!fp_access_check(s)) {
                return;
            }
            handle_2misc_reciprocal(s, opcode, false, u, is_q, size, rn, rd);
            return;
        case 0x56: /* FCVTXN, FCVTXN2 */
            if (size == 2) {
                lata_unallocated_encoding(s);
                return;
            }
            /* fall through */
        case 0x16: /* FCVTN, FCVTN2 */
            /* handle_2misc_narrow does a 2*size -> size operation, but these
             * instructions encode the source size rather than dest size.
             */
            if (!fp_access_check(s)) {
                return;
            }
            handle_2misc_narrow(s, false, opcode, 0, is_q, size - 1, rn, rd);
            return;
        case 0x36: /* BFCVTN, BFCVTN2 */
            if (!dc_isar_feature(aa64_bf16, s) || size != 2) {
                lata_unallocated_encoding(s);
                return;
            }
            if (!fp_access_check(s)) {
                return;
            }
            handle_2misc_narrow(s, false, opcode, 0, is_q, size - 1, rn, rd);
            return;
        case 0x17: /* FCVTL, FCVTL2 */
            if (!fp_access_check(s)) {
                return;
            }
            handle_2misc_widening(s, opcode, is_q, size, rn, rd);
            return;
        case 0x18: /* FRINTN */
        case 0x19: /* FRINTM */
        case 0x38: /* FRINTP */
        case 0x39: /* FRINTZ */
            // rmode = extract32(opcode, 5, 1) | (extract32(opcode, 0, 1) << 1);
            /* fall through */
        case 0x59: /* FRINTX */
        case 0x79: /* FRINTI */
            // need_fpstatus = true;
            if (size == 3 && !is_q) {
                lata_unallocated_encoding(s);
                return;
            }
            break;
        case 0x58: /* FRINTA */
            // rmode = FPROUNDING_TIEAWAY;
            // need_fpstatus = true;
            if (size == 3 && !is_q) {
                lata_unallocated_encoding(s);
                return;
            }
            break;
        case 0x7c: /* URSQRTE */
            if (size == 3) {
                lata_unallocated_encoding(s);
                return;
            }
            break;
        case 0x1e: /* FRINT32Z */
        case 0x1f: /* FRINT64Z */
            // rmode = FPROUNDING_ZERO;
            /* fall through */
        case 0x5e: /* FRINT32X */
        case 0x5f: /* FRINT64X */
            // need_fpstatus = true;
            if ((size == 3 && !is_q) || !dc_isar_feature(aa64_frint, s)) {
                lata_unallocated_encoding(s);
                return;
            }
            break;
        default:
            lata_unallocated_encoding(s);
            return;
        }
        break;
    }
    default:
        lata_unallocated_encoding(s);
        return;
    }

    if (!fp_access_check(s)) {
        return;
    }

    IR2_OPND vreg_d = alloc_fpr_dst(rd);
    IR2_OPND vreg_n = alloc_fpr_src(rn);

    switch (opcode) {
    case 0x5:
        if (u && size == 0) { /* NOT */
            la_vnori_b(vreg_d, vreg_n, 0);
            if(!is_q){
                /* 高64位清零 */
                la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
            }
            store_fpr_dst(rd, vreg_d);
            free_alloc_fpr(vreg_d);
            free_alloc_fpr(vreg_n);
            return;            
        }
        break;
    case 0x8: /* CMGT, CMGE */
        switch (size)
        {
        case 0:
            if(u){
                la_vslti_b(vreg_d, vreg_n, 0);
            }else{
                la_vslei_b(vreg_d, vreg_n, 0);
            }
            break;
        case 1:
            if(u){
                la_vslti_h(vreg_d, vreg_n, 0);
            }else{
                la_vslei_h(vreg_d, vreg_n, 0);
            }
            break;
        case 2:
            if(u){
                la_vslti_w(vreg_d, vreg_n, 0);
            }else{
                la_vslei_w(vreg_d, vreg_n, 0);
            }
            break;
        case 3:
            if(u){
                la_vslti_d(vreg_d, vreg_n, 0);
            }else{
                la_vslei_d(vreg_d, vreg_n, 0);
            }
            break;
        default:
            break;
        }
        la_vnori_b(vreg_d, vreg_d, 0);
        if(!is_q){
            /* 高64位清零 */
            la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
        }
        store_fpr_dst(rd, vreg_d);
        free_alloc_fpr(vreg_d);
        free_alloc_fpr(vreg_n);
        return;
    case 0x9: /* CMEQ, CMLE */
        switch (size)
        {
        case 0:
            if(u){
                la_vslei_b(vreg_d, vreg_n, 0);
            }else{
                la_vseqi_b(vreg_d, vreg_n, 0);
            }
            break;
        case 1:
            if(u){
                la_vslei_h(vreg_d, vreg_n, 0);
            }else{
                la_vseqi_h(vreg_d, vreg_n, 0);
            }
            break;
        case 2:
            if(u){
                la_vslei_w(vreg_d, vreg_n, 0);
            }else{
                la_vseqi_w(vreg_d, vreg_n, 0);
            }
            break;
        case 3:
            if(u){
                la_vslei_d(vreg_d, vreg_n, 0);
            }else{
                la_vseqi_d(vreg_d, vreg_n, 0);
            }
            break;
        default:
            break;
        }
        if(!is_q){
            /* 高64位清零 */
            la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
        }
        store_fpr_dst(rd, vreg_d);
        free_alloc_fpr(vreg_d);
        free_alloc_fpr(vreg_n);
        return;
    case 0xa: /* CMLT */
        switch (size)
        {
        case 0:
            la_vslti_b(vreg_d, vreg_n, 0);
            break;
        case 1:
            la_vslti_h(vreg_d, vreg_n, 0);
            break;
        case 2:
            la_vslti_w(vreg_d, vreg_n, 0);
            break;
        case 3:
            la_vslti_d(vreg_d, vreg_n, 0);
            break;
        default:
            break;
        }
        if(!is_q){
            /* 高64位清零 */
            la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
        }
        store_fpr_dst(rd, vreg_d);
        free_alloc_fpr(vreg_d);
        free_alloc_fpr(vreg_n);
        return;
    case 0xb:/* ABS */
        la_vsub_d(vreg_d, vreg_n, vreg_n);
        switch (size)
        {
        case 0:
            la_vabsd_b(vreg_d, vreg_n, vreg_d);
            break;
        case 1:
            la_vabsd_h(vreg_d, vreg_n, vreg_d);
            break;
        case 2:
            la_vabsd_w(vreg_d, vreg_n, vreg_d);
            break;
        case 3:
            la_vabsd_d(vreg_d, vreg_n, vreg_d);
            break;
        default:
            break;
        }
        if(!is_q){
            /* 高64位清零 */
            la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
        }
        store_fpr_dst(rd, vreg_d);
        free_alloc_fpr(vreg_d);
        free_alloc_fpr(vreg_n);
        return;
    }

    if (size == 3) {
        /* All 64-bit element operations can be shared with scalar 2misc */

        /* Coverity claims (size == 3 && !is_q) has been eliminated
         * from all paths leading to here.
         */
        tcg_debug_assert(is_q);

        handle_2misc_64(s, opcode, u, &vreg_d, &vreg_n);

    } else {

        IR2_OPND vtemp = ra_alloc_ftemp();
        if (size == 2) {
            /* Special cases for 32 bit elements */
            switch (opcode) {
            case 0x4: /* CLS */
                break;
            case 0x7: /* SQABS, SQNEG */
                break;
            case 0x2f: /* FABS */
                for(int i = 0; i < (is_q ? 4 : 2); ++i){
                    la_vreplvei_w(vtemp, vreg_n, i);
                    la_fabs_s(vtemp, vtemp);
                    la_vextrins_w(vreg_d, vtemp, i << 4);
                }
                break;
            case 0x6f: /* FNEG */
                for(int i = 0; i < (is_q ? 4 : 2); ++i){
                    la_vreplvei_w(vtemp, vreg_n, i);
                    la_fneg_s(vtemp, vtemp);
                    la_vextrins_w(vreg_d, vtemp, i << 4);
                }
                break;
            case 0x7f: /* FSQRT */
                la_vfsqrt_s(vreg_d, vreg_n);
                // gen_helper_vfp_sqrts(tcg_res, tcg_op, cpu_env);
                break;
            case 0x1a: /* FCVTNS */
                la_vftintrne_w_s(vreg_d, vreg_n);
                break;
            case 0x1b: /* FCVTMS */
                la_vftintrm_w_s(vreg_d, vreg_n);
                break;
            case 0x1c: /* FCVTAS */
                assert(0);
                break;
            case 0x3a: /* FCVTPS */
                la_vftintrp_w_s(vreg_d, vreg_n);
                break;;
            case 0x3b: /* FCVTZS */
                la_vftintrz_w_s(vreg_d, vreg_n);
                break;
            case 0x5a: /* FCVTNU */
            case 0x5b: /* FCVTMU */
            case 0x5c: /* FCVTAU */
            case 0x7a: /* FCVTPU */
                assert(0);
            case 0x7b: /* FCVTZU */
                la_vftintrz_wu_s(vreg_d, vreg_n);
                break;
            case 0x18: /* FRINTN */
                break;
            case 0x19: /* FRINTM */
                la_vfrintrm_s(vreg_d, vreg_n);
                break;
            case 0x38: /* FRINTP */
                break;
            case 0x39: /* FRINTZ */
                la_vfrintrz_s(vreg_d, vreg_n);
                break;
            case 0x58: /* FRINTA */
                break;
            case 0x79: /* FRINTI */
                // gen_helper_rints(tcg_res, tcg_op, tcg_fpstatus);
                break;
            case 0x59: /* FRINTX */
                // gen_helper_rints_exact(tcg_res, tcg_op, tcg_fpstatus);
                break;
            case 0x7c: /* URSQRTE */
                // gen_helper_rsqrte_u32(tcg_res, tcg_op);
                break;
            case 0x1e: /* FRINT32Z */
            case 0x5e: /* FRINT32X */
                // gen_helper_frint32_s(tcg_res, tcg_op, tcg_fpstatus);
                break;
            case 0x1f: /* FRINT64Z */
            case 0x5f: /* FRINT64X */
                // gen_helper_frint64_s(tcg_res, tcg_op, tcg_fpstatus);
                break;
            default:
                g_assert_not_reached();
            }
            free_alloc_fpr(vtemp);
            
        } else {
            /* Use helpers for 8 and 16 bit elements */
            switch (opcode) {
            case 0x5: /* CNT, RBIT */
                if (u) {//RBIT
                    assert(0);
                } else {//CNT
                    la_vpcnt_b(vreg_d, vreg_n);
                }
                break;
            case 0x7: /* SQABS, SQNEG */
                assert(0);
            case 0x4: /* CLS, CLZ */
                assert(0);
            default:
                g_assert_not_reached();
            }
        }
    }

    if(!is_q){
        /* 高64位清零 */
        la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
    }
    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
}

/* AdvSIMD [scalar] two register miscellaneous (FP16)
 *
 *   31  30  29 28  27     24  23 22 21       17 16    12 11 10 9    5 4    0
 * +---+---+---+---+---------+---+-------------+--------+-----+------+------+
 * | 0 | Q | U | S | 1 1 1 0 | a | 1 1 1 1 0 0 | opcode | 1 0 |  Rn  |  Rd  |
 * +---+---+---+---+---------+---+-------------+--------+-----+------+------+
 *   mask: 1000 1111 0111 1110 0000 1100 0000 0000 0x8f7e 0c00
 *   val:  0000 1110 0111 1000 0000 1000 0000 0000 0x0e78 0800
 *
 * This actually covers two groups where scalar access is governed by
 * bit 28. A bunch of the instructions (float to integral) only exist
 * in the vector form and are un-allocated for the scalar decode. Also
 * in the scalar decode Q is always 1.
 */
static void disas_simd_two_reg_misc_fp16(DisasContext *s, uint32_t insn)
{
    assert(0);
}

/* AdvSIMD scalar x indexed element
 *  31 30  29 28       24 23  22 21  20  19  16 15 12  11  10 9    5 4    0
 * +-----+---+-----------+------+---+---+------+-----+---+---+------+------+
 * | 0 1 | U | 1 1 1 1 1 | size | L | M |  Rm  | opc | H | 0 |  Rn  |  Rd  |
 * +-----+---+-----------+------+---+---+------+-----+---+---+------+------+
 * AdvSIMD vector x indexed element
 *   31  30  29 28       24 23  22 21  20  19  16 15 12  11  10 9    5 4    0
 * +---+---+---+-----------+------+---+---+------+-----+---+---+------+------+
 * | 0 | Q | U | 0 1 1 1 1 | size | L | M |  Rm  | opc | H | 0 |  Rn  |  Rd  |
 * +---+---+---+-----------+------+---+---+------+-----+---+---+------+------+
 */
static void disas_simd_indexed(DisasContext *s, uint32_t insn)
{
    /* This encoding has two kinds of instruction:
     *  normal, where we perform elt x idxelt => elt for each
     *     element in the vector
     *  long, where we perform elt x idxelt and generate a result of
     *     double the width of the input element
     * The long ops have a 'part' specifier (ie come in INSN, INSN2 pairs).
     */
    bool is_scalar = extract32(insn, 28, 1);
    bool is_q = extract32(insn, 30, 1);
    bool u = extract32(insn, 29, 1);
    int size = extract32(insn, 22, 2);
    int l = extract32(insn, 21, 1);
    int m = extract32(insn, 20, 1);
    /* Note that the Rm field here is only 4 bits, not 5 as it usually is */
    int rm = extract32(insn, 16, 4);
    int opcode = extract32(insn, 12, 4);
    int h = extract32(insn, 11, 1);
    int rn = extract32(insn, 5, 5);
    int rd = extract32(insn, 0, 5);
    bool is_long = false;
    int is_fp = 0;
    bool is_fp16 = false;
    int index;

    switch (16 * u + opcode) {
    case 0x08: /* MUL */
    case 0x10: /* MLA */
    case 0x14: /* MLS */
        if (is_scalar) {
            lata_unallocated_encoding(s);
            return;
        }
        break;
    case 0x02: /* SMLAL, SMLAL2 */
    case 0x12: /* UMLAL, UMLAL2 */
    case 0x06: /* SMLSL, SMLSL2 */
    case 0x16: /* UMLSL, UMLSL2 */
    case 0x0a: /* SMULL, SMULL2 */
    case 0x1a: /* UMULL, UMULL2 */
        if (is_scalar) {
            lata_unallocated_encoding(s);
            return;
        }
        is_long = true;
        break;
    case 0x03: /* SQDMLAL, SQDMLAL2 */
    case 0x07: /* SQDMLSL, SQDMLSL2 */
    case 0x0b: /* SQDMULL, SQDMULL2 */
        is_long = true;
        break;
    case 0x0c: /* SQDMULH */
    case 0x0d: /* SQRDMULH */
        break;
    case 0x01: /* FMLA */
    case 0x05: /* FMLS */
    case 0x09: /* FMUL */
    case 0x19: /* FMULX */
        is_fp = 1;
        break;
    case 0x1d: /* SQRDMLAH */
    case 0x1f: /* SQRDMLSH */
        if (!dc_isar_feature(aa64_rdm, s)) {
            lata_unallocated_encoding(s);
            return;
        }
        break;
    case 0x0e: /* SDOT */
    case 0x1e: /* UDOT */
        if (is_scalar || size != MO_32 || !dc_isar_feature(aa64_dp, s)) {
            lata_unallocated_encoding(s);
            return;
        }
        break;
    case 0x0f:
        switch (size) {
        case 0: /* SUDOT */
        case 2: /* USDOT */
            if (is_scalar || !dc_isar_feature(aa64_i8mm, s)) {
                lata_unallocated_encoding(s);
                return;
            }
            size = MO_32;
            break;
        case 1: /* BFDOT */
            if (is_scalar || !dc_isar_feature(aa64_bf16, s)) {
                lata_unallocated_encoding(s);
                return;
            }
            size = MO_32;
            break;
        case 3: /* BFMLAL{B,T} */
            if (is_scalar || !dc_isar_feature(aa64_bf16, s)) {
                lata_unallocated_encoding(s);
                return;
            }
            /* can't set is_fp without other incorrect size checks */
            size = MO_16;
            break;
        default:
            lata_unallocated_encoding(s);
            return;
        }
        break;
    case 0x11: /* FCMLA #0 */
    case 0x13: /* FCMLA #90 */
    case 0x15: /* FCMLA #180 */
    case 0x17: /* FCMLA #270 */
        if (is_scalar || !dc_isar_feature(aa64_fcma, s)) {
            lata_unallocated_encoding(s);
            return;
        }
        is_fp = 2;
        break;
    case 0x00: /* FMLAL */
    case 0x04: /* FMLSL */
    case 0x18: /* FMLAL2 */
    case 0x1c: /* FMLSL2 */
        if (is_scalar || size != MO_32 || !dc_isar_feature(aa64_fhm, s)) {
            lata_unallocated_encoding(s);
            return;
        }
        size = MO_16;
        /* is_fp, but we pass cpu_env not fp_status.  */
        break;
    default:
        lata_unallocated_encoding(s);
        return;
    }

    switch (is_fp) {
    case 1: /* normal fp */
        /* convert insn encoded size to MemOp size */
        switch (size) {
        case 0: /* half-precision */
            size = MO_16;
            is_fp16 = true;
            break;
        case MO_32: /* single precision */
        case MO_64: /* double precision */
            break;
        default:
            lata_unallocated_encoding(s);
            return;
        }
        break;

    case 2: /* complex fp */
        /* Each indexable element is a complex pair.  */
        size += 1;
        switch (size) {
        case MO_32:
            if (h && !is_q) {
                lata_unallocated_encoding(s);
                return;
            }
            is_fp16 = true;
            break;
        case MO_64:
            break;
        default:
            lata_unallocated_encoding(s);
            return;
        }
        break;

    default: /* integer */
        switch (size) {
        case MO_8:
        case MO_64:
            lata_unallocated_encoding(s);
            return;
        }
        break;
    }
    if (is_fp16 && !dc_isar_feature(aa64_fp16, s)) {
        lata_unallocated_encoding(s);
        return;
    }

    /* Given MemOp size, adjust register and indexing.  */
    switch (size) {
    case MO_16:
        index = h << 2 | l << 1 | m;
        break;
    case MO_32:
        index = h << 1 | l;
        rm |= m << 4;
        break;
    case MO_64:
        if (l || !is_q) {
            lata_unallocated_encoding(s);
            return;
        }
        index = h;
        rm |= m << 4;
        break;
    default:
        g_assert_not_reached();
    }

    if (!fp_access_check(s)) {
        return;
    }

    IR2_OPND vreg_d;
    IR2_OPND vreg_n = alloc_fpr_src(rn);
    IR2_OPND vreg_m = alloc_fpr_src(rm);
    IR2_OPND vtemp = ra_alloc_ftemp();
    IR2_OPND vtemp1 = ra_alloc_ftemp();

    switch (16 * u + opcode) {
    case 0x0e: /* SDOT */
    case 0x1e: /* UDOT */
        assert(0);
        return;
    case 0x0f:
        assert(0);
        switch (extract32(insn, 22, 2)) {
        case 0: /* SUDOT */
            return;
        case 1: /* BFDOT */
            return;
        case 2: /* USDOT */
            return;
        case 3: /* BFMLAL{B,T} */
            return;
        }
        g_assert_not_reached();
    case 0x11: /* FCMLA #0 */
    case 0x13: /* FCMLA #90 */
    case 0x15: /* FCMLA #180 */
    case 0x17: /* FCMLA #270 */
        {
            assert(0);
        }
        return;

    case 0x00: /* FMLAL */
    case 0x04: /* FMLSL */
    case 0x18: /* FMLAL2 */
    case 0x1c: /* FMLSL2 */
        {
            assert(0);
        }
        return;

    case 0x08: /* MUL */
        if (!is_long && !is_scalar) {
            vreg_d = alloc_fpr_dst(rd);
            switch (size) {
            case 1:
                la_vreplvei_h(vtemp1, vreg_m, index);
                la_vmul_h(vreg_d, vreg_n, vtemp1);
                break;
            case 2:
                la_vreplvei_w(vtemp1, vreg_m, index);
                la_vmul_w(vreg_d, vreg_n, vtemp1);
                break;
            case 3:
                la_vreplvei_d(vtemp1, vreg_m, index);
                la_vmul_d(vreg_d, vreg_n, vtemp1);
                break;
            default:
                g_assert_not_reached();
            }
        }
        goto do_gvec_end;
        break;

    case 0x10: /* MLA */
        if (!is_long && !is_scalar) {
            assert(0);
            return;
        }
        break;

    case 0x14: /* MLS */
        if (!is_long && !is_scalar) {
            assert(0);
            return;
        }
        break;
    }

    if (size == 3) {

        assert(is_fp && is_q && !is_long);

        switch (16 * u + opcode) {
        case 0x05: /* FMLS */
            vreg_d = alloc_fpr_src(rd);
            la_vxor_v(vtemp, vtemp, vtemp);
            la_vfsub_d(vtemp, vtemp, vreg_n);
            la_vreplvei_d(vtemp1, vreg_m, index);
            la_vfmadd_d(vreg_d, vtemp, vtemp1, vreg_d);
            break;
        case 0x01: /* FMLA */
            vreg_d = alloc_fpr_src(rd);
            la_vreplvei_d(vtemp1, vreg_m, index);
            la_vfmadd_d(vreg_d, vreg_n, vtemp1, vreg_d);
            break;
        case 0x09: /* FMUL */
            vreg_d = alloc_fpr_dst(rd);
            la_vreplvei_d(vtemp1, vreg_m, index);
            la_vfmul_d(vreg_d, vreg_n, vtemp1);
            break;
        case 0x19: /* FMULX */
            assert(0);
            break;
        default:
            g_assert_not_reached();
        }


    } else if (!is_long) {
        /* 32 bit floating point, or 16 or 32 bit integer.
         * For the 16 bit scalar case we use the usual Neon helpers and
         * rely on the fact that 0 op 0 == 0 with no side effects.
         */

        if (size == 1 && !is_scalar) {
            /* The simplest way to handle the 16x16 indexed ops is to duplicate
             * the index into both halves of the 32 bit tcg_idx and then use
             * the usual Neon helpers.
             */
            // tcg_gen_deposit_i32(tcg_idx, tcg_idx, tcg_idx, 16, 16);
        }

        switch (16 * u + opcode) {
        case 0x08: /* MUL */
        case 0x10: /* MLA */
        case 0x14: /* MLS */
        {
            assert(0);
            break;
        }
        case 0x05: /* FMLS */
            switch (size) {
            case 1:
                assert(0);
                break;
            case 2:
                vreg_d = alloc_fpr_src(rd);
                la_vxor_v(vtemp, vtemp, vtemp);
                la_vfsub_s(vtemp, vtemp, vreg_n);
                la_vreplvei_w(vtemp1, vreg_m, index);
                la_vfmadd_s(vreg_d, vtemp, vtemp1, vreg_d);
                break;
            default:
                g_assert_not_reached();
            }
            break;
        case 0x01: /* FMLA */
            switch (size) {
            case 1:
                assert(0);
                break;
            case 2:
                vreg_d = alloc_fpr_src(rd);
                la_vreplvei_w(vtemp1, vreg_m, index);
                la_vfmadd_s(vreg_d, vreg_n, vtemp1, vreg_d);
                break;
            default:
                g_assert_not_reached();
            }
            break;
        case 0x09: /* FMUL */
            vreg_d = alloc_fpr_dst(rd);
            la_vreplvei_w(vtemp1, vreg_m, index);
            la_vfmul_s(vreg_d, vreg_n, vtemp1);
            break;
        case 0x19: /* FMULX */
            assert(0);
            break;
        case 0x0c: /* SQDMULH */
            assert(0);
            break;
        case 0x0d: /* SQRDMULH */
            assert(0);
            break;
        case 0x1d: /* SQRDMLAH */
            assert(0);
            break;
        case 0x1f: /* SQRDMLSH */
            assert(0);
            break;
        default:
            g_assert_not_reached();
        }
        
        if(!is_q){
            /* 高64位清零 */
            la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
        }
    } else {
        /* long ops: 16x16->32 or 32x32->64 */
        assert(0);
    }
do_gvec_end:
    if(!is_q){
        /* 高64位清零 */
        la_vinsgr2vr_d(vreg_d, zero_ir2_opnd, 1);
    }
    store_fpr_dst(rd, vreg_d);
    free_alloc_fpr(vreg_d);
    free_alloc_fpr(vreg_n);
    free_alloc_fpr(vreg_m);
    free_alloc_fpr(vtemp);
    free_alloc_fpr(vtemp1);
}

/* Crypto AES
 *  31             24 23  22 21       17 16    12 11 10 9    5 4    0
 * +-----------------+------+-----------+--------+-----+------+------+
 * | 0 1 0 0 1 1 1 0 | size | 1 0 1 0 0 | opcode | 1 0 |  Rn  |  Rd  |
 * +-----------------+------+-----------+--------+-----+------+------+
 */
static void disas_crypto_aes(DisasContext *s, uint32_t insn)
{
    assert(0);
}

/* Crypto three-reg SHA
 *  31             24 23  22  21 20  16  15 14    12 11 10 9    5 4    0
 * +-----------------+------+---+------+---+--------+-----+------+------+
 * | 0 1 0 1 1 1 1 0 | size | 0 |  Rm  | 0 | opcode | 0 0 |  Rn  |  Rd  |
 * +-----------------+------+---+------+---+--------+-----+------+------+
 */
static void disas_crypto_three_reg_sha(DisasContext *s, uint32_t insn)
{
    assert(0);
}

/* Crypto two-reg SHA
 *  31             24 23  22 21       17 16    12 11 10 9    5 4    0
 * +-----------------+------+-----------+--------+-----+------+------+
 * | 0 1 0 1 1 1 1 0 | size | 1 0 1 0 0 | opcode | 1 0 |  Rn  |  Rd  |
 * +-----------------+------+-----------+--------+-----+------+------+
 */
static void disas_crypto_two_reg_sha(DisasContext *s, uint32_t insn)
{
    assert(0);
}

/* Crypto three-reg SHA512
 *  31                   21 20  16 15  14  13 12  11  10  9    5 4    0
 * +-----------------------+------+---+---+-----+--------+------+------+
 * | 1 1 0 0 1 1 1 0 0 1 1 |  Rm  | 1 | O | 0 0 | opcode |  Rn  |  Rd  |
 * +-----------------------+------+---+---+-----+--------+------+------+
 */
static void disas_crypto_three_reg_sha512(DisasContext *s, uint32_t insn)
{
    assert(0);
}

/* Crypto two-reg SHA512
 *  31                                     12  11  10  9    5 4    0
 * +-----------------------------------------+--------+------+------+
 * | 1 1 0 0 1 1 1 0 1 1 0 0 0 0 0 0 1 0 0 0 | opcode |  Rn  |  Rd  |
 * +-----------------------------------------+--------+------+------+
 */
static void disas_crypto_two_reg_sha512(DisasContext *s, uint32_t insn)
{
    assert(0);
}

/* Crypto four-register
 *  31               23 22 21 20  16 15  14  10 9    5 4    0
 * +-------------------+-----+------+---+------+------+------+
 * | 1 1 0 0 1 1 1 0 0 | Op0 |  Rm  | 0 |  Ra  |  Rn  |  Rd  |
 * +-------------------+-----+------+---+------+------+------+
 */
static void disas_crypto_four_reg(DisasContext *s, uint32_t insn)
{
    assert(0);
}

/* Crypto XAR
 *  31                   21 20  16 15    10 9    5 4    0
 * +-----------------------+------+--------+------+------+
 * | 1 1 0 0 1 1 1 0 1 0 0 |  Rm  |  imm6  |  Rn  |  Rd  |
 * +-----------------------+------+--------+------+------+
 */
static void disas_crypto_xar(DisasContext *s, uint32_t insn)
{
    assert(0);
}

/* Crypto three-reg imm2
 *  31                   21 20  16 15  14 13 12  11  10  9    5 4    0
 * +-----------------------+------+-----+------+--------+------+------+
 * | 1 1 0 0 1 1 1 0 0 1 0 |  Rm  | 1 0 | imm2 | opcode |  Rn  |  Rd  |
 * +-----------------------+------+-----+------+--------+------+------+
 */
static void disas_crypto_three_reg_imm2(DisasContext *s, uint32_t insn)
{
    assert(0);
}

/* C3.6 Data processing - SIMD, inc Crypto
 *
 * As the decode gets a little complex we are using a table based
 * approach for this part of the decode.
 */
static const AArch64DecodeTable data_proc_simd[] = {
    /* pattern  ,  mask     ,  fn                        */
    { 0x0e200400, 0x9f200400, disas_simd_three_reg_same },
    { 0x0e008400, 0x9f208400, disas_simd_three_reg_same_extra },
    { 0x0e200000, 0x9f200c00, disas_simd_three_reg_diff },
    { 0x0e200800, 0x9f3e0c00, disas_simd_two_reg_misc },
    { 0x0e300800, 0x9f3e0c00, disas_simd_across_lanes },
    { 0x0e000400, 0x9fe08400, disas_simd_copy },
    { 0x0f000000, 0x9f000400, disas_simd_indexed }, /* vector indexed */
    /* simd_mod_imm decode is a subset of simd_shift_imm, so must precede it */
    { 0x0f000400, 0x9ff80400, disas_simd_mod_imm },
    { 0x0f000400, 0x9f800400, disas_simd_shift_imm },
    { 0x0e000000, 0xbf208c00, disas_simd_tb },
    { 0x0e000800, 0xbf208c00, disas_simd_zip_trn },
    { 0x2e000000, 0xbf208400, disas_simd_ext },
    { 0x5e200400, 0xdf200400, disas_simd_scalar_three_reg_same },
    { 0x5e008400, 0xdf208400, disas_simd_scalar_three_reg_same_extra },
    { 0x5e200000, 0xdf200c00, disas_simd_scalar_three_reg_diff },
    { 0x5e200800, 0xdf3e0c00, disas_simd_scalar_two_reg_misc },
    { 0x5e300800, 0xdf3e0c00, disas_simd_scalar_pairwise },
    { 0x5e000400, 0xdfe08400, disas_simd_scalar_copy },
    { 0x5f000000, 0xdf000400, disas_simd_indexed }, /* scalar indexed */
    { 0x5f000400, 0xdf800400, disas_simd_scalar_shift_imm },
    { 0x4e280800, 0xff3e0c00, disas_crypto_aes },
    { 0x5e000000, 0xff208c00, disas_crypto_three_reg_sha },
    { 0x5e280800, 0xff3e0c00, disas_crypto_two_reg_sha },
    { 0xce608000, 0xffe0b000, disas_crypto_three_reg_sha512 },
    { 0xcec08000, 0xfffff000, disas_crypto_two_reg_sha512 },
    { 0xce000000, 0xff808000, disas_crypto_four_reg },
    { 0xce800000, 0xffe00000, disas_crypto_xar },
    { 0xce408000, 0xffe0c000, disas_crypto_three_reg_imm2 },
    { 0x0e400400, 0x9f60c400, disas_simd_three_reg_same_fp16 },
    { 0x0e780800, 0x8f7e0c00, disas_simd_two_reg_misc_fp16 },
    { 0x5e400400, 0xdf60c400, disas_simd_scalar_three_reg_same_fp16 },
    { 0x00000000, 0x00000000, NULL }
};

static void disas_data_proc_simd(DisasContext *s, uint32_t insn)
{
    /* Note that this is called with all non-FP cases from
     * table C3-6 so it must UNDEF for entries not specifically
     * allocated to instructions in that table.
     */
    AArch64DecodeFn *fn = lookup_disas_fn(&data_proc_simd[0], insn);
    if (fn) {
        fn(s, insn);
    } else {
        lata_unallocated_encoding(s);
    }
}

/* C3.6 Data processing - SIMD and floating point */
static bool disas_data_proc_simd_fp(DisasContext *s)
{
    uint32_t insn = s->insn;
    if (extract32(insn, 28, 1) == 1 && extract32(insn, 30, 1) == 0) {
        disas_data_proc_fp(s, insn);
    } else {
        /* SIMD, including crypto */
        disas_data_proc_simd(s, insn);
    }
    return true;
}

// /* C3.1 A64 instruction index by encoding */
static bool (*translate_functions[])(DisasContext *) = {
    TRANS_FUNC_GEN_REAL(AARCH64_A64_NULL, (void *)lata_unallocated_encoding),
    TRANS_FUNC_GEN(AARCH64_A64_ADDG_i, ADDG_i),
    TRANS_FUNC_GEN(AARCH64_A64_ADDS_i, ADDS_i),
    TRANS_FUNC_GEN(AARCH64_A64_ADD_i, ADD_i),
    TRANS_FUNC_GEN(AARCH64_A64_ADR, ADR),
    TRANS_FUNC_GEN(AARCH64_A64_ADRP, ADRP),
    TRANS_FUNC_GEN(AARCH64_A64_ANDS_i, ANDS_i),
    TRANS_FUNC_GEN(AARCH64_A64_AND_i, AND_i),
    TRANS_FUNC_GEN(AARCH64_A64_AUTIA1716, AUTIA1716),
    TRANS_FUNC_GEN(AARCH64_A64_AUTIASP, AUTIASP),
    TRANS_FUNC_GEN(AARCH64_A64_AUTIAZ, AUTIAZ),
    TRANS_FUNC_GEN(AARCH64_A64_AUTIB1716, AUTIB1716),
    TRANS_FUNC_GEN(AARCH64_A64_AUTIBSP, AUTIBSP),
    TRANS_FUNC_GEN(AARCH64_A64_AUTIBZ, AUTIBZ),
    TRANS_FUNC_GEN(AARCH64_A64_AXFLAG, AXFLAG),
    TRANS_FUNC_GEN(AARCH64_A64_B, B),
    TRANS_FUNC_GEN(AARCH64_A64_BFM, BFM),
    TRANS_FUNC_GEN(AARCH64_A64_BL, BL),
    TRANS_FUNC_GEN(AARCH64_A64_BLR, BLR),
    TRANS_FUNC_GEN(AARCH64_A64_BLRA, BLRA),
    TRANS_FUNC_GEN(AARCH64_A64_BLRAZ, BLRAZ),
    TRANS_FUNC_GEN(AARCH64_A64_BR, BR),
    TRANS_FUNC_GEN(AARCH64_A64_BRA, BRA),
    TRANS_FUNC_GEN(AARCH64_A64_BRAZ, BRAZ),
    TRANS_FUNC_GEN(AARCH64_A64_BRK, BRK),
    TRANS_FUNC_GEN(AARCH64_A64_B_cond, B_cond),
    TRANS_FUNC_GEN(AARCH64_A64_CAS, CAS),
    TRANS_FUNC_GEN(AARCH64_A64_CASP, CASP),
    TRANS_FUNC_GEN(AARCH64_A64_CBZ, CBZ),
    TRANS_FUNC_GEN(AARCH64_A64_CFINV, CFINV),
    TRANS_FUNC_GEN(AARCH64_A64_CLREX, CLREX),
    TRANS_FUNC_GEN(AARCH64_A64_DSB_DMB, DSB_DMB),
    TRANS_FUNC_GEN(AARCH64_A64_EOR_i, EOR_i),
    TRANS_FUNC_GEN(AARCH64_A64_ERET, ERET),
    TRANS_FUNC_GEN(AARCH64_A64_ERETA, ERETA),
    TRANS_FUNC_GEN(AARCH64_A64_ESB, ESB),
    TRANS_FUNC_GEN(AARCH64_A64_EXTR, EXTR),
    TRANS_FUNC_GEN(AARCH64_A64_HLT, HLT),
    TRANS_FUNC_GEN(AARCH64_A64_HVC, HVC),
    TRANS_FUNC_GEN(AARCH64_A64_ISB, ISB),
    TRANS_FUNC_GEN(AARCH64_A64_LDADD, LDADD),
    TRANS_FUNC_GEN(AARCH64_A64_LDAPR, LDAPR),
    TRANS_FUNC_GEN(AARCH64_A64_LDAPR_i, LDAPR_i),
    TRANS_FUNC_GEN(AARCH64_A64_LDAR, LDAR),
    TRANS_FUNC_GEN(AARCH64_A64_LDCLR, LDCLR),
    TRANS_FUNC_GEN(AARCH64_A64_LDEOR, LDEOR),
    TRANS_FUNC_GEN(AARCH64_A64_LDG, LDG),
    TRANS_FUNC_GEN(AARCH64_A64_LDGM, LDGM),
    TRANS_FUNC_GEN(AARCH64_A64_LDP, LDP),
    TRANS_FUNC_GEN(AARCH64_A64_LDP_v, LDP_v),
    TRANS_FUNC_GEN(AARCH64_A64_LDR, LDR),
    TRANS_FUNC_GEN(AARCH64_A64_LDRA, LDRA),
    TRANS_FUNC_GEN(AARCH64_A64_LDR_i, LDR_i),
    TRANS_FUNC_GEN(AARCH64_A64_LDR_v, LDR_v),
    TRANS_FUNC_GEN(AARCH64_A64_LDR_v_i, LDR_v_i),
    TRANS_FUNC_GEN(AARCH64_A64_LDSET, LDSET),
    TRANS_FUNC_GEN(AARCH64_A64_LDSMAX, LDSMAX),
    TRANS_FUNC_GEN(AARCH64_A64_LDSMIN, LDSMIN),
    TRANS_FUNC_GEN(AARCH64_A64_LDUMAX, LDUMAX),
    TRANS_FUNC_GEN(AARCH64_A64_LDUMIN, LDUMIN),
    TRANS_FUNC_GEN(AARCH64_A64_LDXP, LDXP),
    TRANS_FUNC_GEN(AARCH64_A64_LDXR, LDXR),
    TRANS_FUNC_GEN(AARCH64_A64_LD_lit, LD_lit),
    TRANS_FUNC_GEN(AARCH64_A64_LD_lit_v, LD_lit_v),
    TRANS_FUNC_GEN(AARCH64_A64_LD_mult, LD_mult),
    TRANS_FUNC_GEN(AARCH64_A64_LD_single, LD_single),
    TRANS_FUNC_GEN(AARCH64_A64_LD_single_repl, LD_single_repl),
    TRANS_FUNC_GEN(AARCH64_A64_MOVK, MOVK),
    TRANS_FUNC_GEN(AARCH64_A64_MOVN, MOVN),
    TRANS_FUNC_GEN(AARCH64_A64_MOVZ, MOVZ),
    TRANS_FUNC_GEN(AARCH64_A64_MSR_i_DAIFCLEAR, MSR_i_DAIFCLEAR),
    TRANS_FUNC_GEN(AARCH64_A64_MSR_i_DAIFSET, MSR_i_DAIFSET),
    TRANS_FUNC_GEN(AARCH64_A64_MSR_i_DIT, MSR_i_DIT),
    TRANS_FUNC_GEN(AARCH64_A64_MSR_i_PAN, MSR_i_PAN),
    TRANS_FUNC_GEN(AARCH64_A64_MSR_i_SBSS, MSR_i_SBSS),
    TRANS_FUNC_GEN(AARCH64_A64_MSR_i_SPSEL, MSR_i_SPSEL),
    TRANS_FUNC_GEN(AARCH64_A64_MSR_i_SVCR, MSR_i_SVCR),
    TRANS_FUNC_GEN(AARCH64_A64_MSR_i_TCO, MSR_i_TCO),
    TRANS_FUNC_GEN(AARCH64_A64_MSR_i_UAO, MSR_i_UAO),
    TRANS_FUNC_GEN(AARCH64_A64_NOP, NOP),
    TRANS_FUNC_GEN(AARCH64_A64_ORR_i, ORR_i),
    TRANS_FUNC_GEN(AARCH64_A64_PACIA1716, PACIA1716),
    TRANS_FUNC_GEN(AARCH64_A64_PACIASP, PACIASP),
    TRANS_FUNC_GEN(AARCH64_A64_PACIAZ, PACIAZ),
    TRANS_FUNC_GEN(AARCH64_A64_PACIB1716, PACIB1716),
    TRANS_FUNC_GEN(AARCH64_A64_PACIBSP, PACIBSP),
    TRANS_FUNC_GEN(AARCH64_A64_PACIBZ, PACIBZ),
    TRANS_FUNC_GEN(AARCH64_A64_RET, RET),
    TRANS_FUNC_GEN(AARCH64_A64_RETA, RETA),
    TRANS_FUNC_GEN(AARCH64_A64_SB, SB),
    TRANS_FUNC_GEN(AARCH64_A64_SBFM, SBFM),
    TRANS_FUNC_GEN(AARCH64_A64_SMC, SMC),
    TRANS_FUNC_GEN(AARCH64_A64_ST2G, ST2G),
    TRANS_FUNC_GEN(AARCH64_A64_STG, STG),
    TRANS_FUNC_GEN(AARCH64_A64_STGM, STGM),
    TRANS_FUNC_GEN(AARCH64_A64_STGP, STGP),
    TRANS_FUNC_GEN(AARCH64_A64_STLR, STLR),
    TRANS_FUNC_GEN(AARCH64_A64_STLR_i, STLR_i),
    TRANS_FUNC_GEN(AARCH64_A64_STP, STP),
    TRANS_FUNC_GEN(AARCH64_A64_STP_v, STP_v),
    TRANS_FUNC_GEN(AARCH64_A64_STP_v, STP_v),
    TRANS_FUNC_GEN(AARCH64_A64_STR, STR),
    TRANS_FUNC_GEN(AARCH64_A64_STR_i, STR_i),
    TRANS_FUNC_GEN(AARCH64_A64_STR_v, STR_v),
    TRANS_FUNC_GEN(AARCH64_A64_STR_v_i, STR_v_i),
    TRANS_FUNC_GEN(AARCH64_A64_STXP, STXP),
    TRANS_FUNC_GEN(AARCH64_A64_STXR, STXR),
    TRANS_FUNC_GEN(AARCH64_A64_STZ2G, STZ2G),
    TRANS_FUNC_GEN(AARCH64_A64_STZG, STZG),
    TRANS_FUNC_GEN(AARCH64_A64_STZGM, STZGM),
    TRANS_FUNC_GEN(AARCH64_A64_ST_mult, ST_mult),
    TRANS_FUNC_GEN(AARCH64_A64_ST_single, ST_single),
    TRANS_FUNC_GEN(AARCH64_A64_SUBG_i, SUBG_i),
    TRANS_FUNC_GEN(AARCH64_A64_SUBS_i, SUBS_i),
    TRANS_FUNC_GEN(AARCH64_A64_SUB_i, SUB_i),
    TRANS_FUNC_GEN(AARCH64_A64_SVC, SVC),
    TRANS_FUNC_GEN(AARCH64_A64_SWP, SWP),
    TRANS_FUNC_GEN(AARCH64_A64_SYS, SYS),
    TRANS_FUNC_GEN(AARCH64_A64_TBZ, TBZ),
    TRANS_FUNC_GEN(AARCH64_A64_UBFM, UBFM),
    TRANS_FUNC_GEN(AARCH64_A64_WFE, WFE),
    TRANS_FUNC_GEN(AARCH64_A64_WFI, WFI),
    TRANS_FUNC_GEN(AARCH64_A64_XAFLAG, XAFLAG),
    TRANS_FUNC_GEN(AARCH64_A64_XPACLRI, XPACLRI),
    TRANS_FUNC_GEN(AARCH64_A64_YIELD, YIELD),
    TRANS_DISAS_FUNC_GEN(AARCH64_A64_DATA_PROC_REG, reg),
    TRANS_DISAS_FUNC_GEN(AARCH64_A64_DATA_PROC_SIMD_FD, simd_fp),
};

static void aarch64_tr_init_disas_context(DisasContext*dc,
                                          CPUState *cpu)
{
    CPUARMState *env = cpu->env_ptr;
    ARMCPU *arm_cpu = env_archcpu(env);
    CPUARMTBFlags tb_flags = arm_tbflags_from_tb(dc->base->tb);
    int bound, core_mmu_idx;

    dc->isar = &arm_cpu->isar;
    dc->condjmp = 0;
    dc->pc_save = dc->base->pc_first;
    dc->aarch64 = true;
    dc->thumb = false;
    dc->sctlr_b = 0;
    dc->be_data = EX_TBFLAG_ANY(tb_flags, BE_DATA) ? MO_BE : MO_LE;
    dc->condexec_mask = 0;
    dc->condexec_cond = 0;
    core_mmu_idx = EX_TBFLAG_ANY(tb_flags, MMUIDX);
    dc->mmu_idx = core_to_aa64_mmu_idx(core_mmu_idx);
    dc->tbii = EX_TBFLAG_A64(tb_flags, TBII);
    dc->tbid = EX_TBFLAG_A64(tb_flags, TBID);
    dc->tcma = EX_TBFLAG_A64(tb_flags, TCMA);
    dc->current_el = arm_mmu_idx_to_el(dc->mmu_idx);
#if !defined(CONFIG_USER_ONLY)
    dc->user = (dc->current_el == 0);
#endif
    dc->fp_excp_el = EX_TBFLAG_ANY(tb_flags, FPEXC_EL);
    dc->align_mem = EX_TBFLAG_ANY(tb_flags, ALIGN_MEM);
    dc->pstate_il = EX_TBFLAG_ANY(tb_flags, PSTATE__IL);
    dc->fgt_active = EX_TBFLAG_ANY(tb_flags, FGT_ACTIVE);
    dc->fgt_svc = EX_TBFLAG_ANY(tb_flags, FGT_SVC);
    dc->fgt_eret = EX_TBFLAG_A64(tb_flags, FGT_ERET);
    dc->sve_excp_el = EX_TBFLAG_A64(tb_flags, SVEEXC_EL);
    dc->sme_excp_el = EX_TBFLAG_A64(tb_flags, SMEEXC_EL);
    dc->vl = (EX_TBFLAG_A64(tb_flags, VL) + 1) * 16;
    dc->svl = (EX_TBFLAG_A64(tb_flags, SVL) + 1) * 16;
    dc->pauth_active = EX_TBFLAG_A64(tb_flags, PAUTH_ACTIVE);
    dc->bt = EX_TBFLAG_A64(tb_flags, BT);
    dc->btype = EX_TBFLAG_A64(tb_flags, BTYPE);
    dc->unpriv = EX_TBFLAG_A64(tb_flags, UNPRIV);
    dc->ata = EX_TBFLAG_A64(tb_flags, ATA);
    dc->mte_active[0] = EX_TBFLAG_A64(tb_flags, MTE_ACTIVE);
    dc->mte_active[1] = EX_TBFLAG_A64(tb_flags, MTE0_ACTIVE);
    dc->pstate_sm = EX_TBFLAG_A64(tb_flags, PSTATE_SM);
    dc->pstate_za = EX_TBFLAG_A64(tb_flags, PSTATE_ZA);
    dc->sme_trap_nonstreaming = EX_TBFLAG_A64(tb_flags, SME_TRAP_NONSTREAMING);
    dc->naa = EX_TBFLAG_A64(tb_flags, NAA);
    dc->vec_len = 0;
    dc->vec_stride = 0;
    dc->cp_regs = arm_cpu->cp_regs;
    dc->features = env->features;
    dc->dcz_blocksize = arm_cpu->dcz_blocksize;

#ifdef CONFIG_USER_ONLY
    /* In sve_probe_page, we assume TBI is enabled. */
    tcg_debug_assert(dc->tbid & 1);
#endif

    dc->lse2 = dc_isar_feature(aa64_lse2, dc);

    /* Single step state. The code-generation logic here is:
     *  SS_ACTIVE == 0:
     *   generate code with no special handling for single-stepping (except
     *   that anything that can make us go to SS_ACTIVE == 1 must end the TB;
     *   this happens anyway because those changes are all system register or
     *   PSTATE writes).
     *  SS_ACTIVE == 1, PSTATE.SS == 1: (active-not-pending)
     *   emit code for one insn
     *   emit code to clear PSTATE.SS
     *   emit code to generate software step exception for completed step
     *   end TB (as usual for having generated an exception)
     *  SS_ACTIVE == 1, PSTATE.SS == 0: (active-pending)
     *   emit code to generate a software step exception
     *   end the TB
     */
    dc->ss_active = EX_TBFLAG_ANY(tb_flags, SS_ACTIVE);
    dc->pstate_ss = EX_TBFLAG_ANY(tb_flags, PSTATE__SS);
    dc->is_ldex = false;

    /* Bound the number of insns to execute to those left on the page.  */
    bound = -(dc->base->pc_first | TARGET_PAGE_MASK) / 4;

    /* If architectural single step active, limit to 1.  */
    if (dc->ss_active) {
        bound = 1;
    }
    dc->base->max_insns = MIN(dc->base->max_insns, bound);
}

static void aarch64_tr_tb_start(DisasContextBase *db, CPUState *cpu)
{
}

static void aarch64_tr_insn_start(DisasContextBase *dcbase, CPUState *cpu)
{
    // DisasContext *dc = container_of(dcbase, DisasContext, base);
    target_ulong pc_arg = dcbase->pc_next;

    if (tb_cflags(dcbase->tb) & CF_PCREL) {
        pc_arg &= ~TARGET_PAGE_MASK;
    }
    tcg_gen_insn_start(pc_arg, 0, 0);
    // dc->insn_start = tcg_last_op();
}

static void disas_a64_legacy(DisasContext *s, uint32_t insn)
{
    switch (extract32(insn, 25, 4)) {
    case 0x5:
    case 0xd:      /* Data processing - register */
        s->insn_type = AARCH64_A64_DATA_PROC_REG;
        break;
    case 0x7:
    case 0xf:      /* Data processing - SIMD and floating point */
        s->insn_type = AARCH64_A64_DATA_PROC_SIMD_FD;
        break;
    default:
        break;
    }
}

#ifdef CONFIG_LATA_TU
void get_last_info(TranslationBlock *tb, DisasContext *s)
{
    switch (s->insn_type) {
    case AARCH64_A64_B:
        tb->last_ir1_type = IR1_TYPE_JMP;
        tb->target_pc = s->pc_curr + ((arg_i)(s->arg.f_i)).imm;
        break;
    case AARCH64_A64_BR:
        tb->last_ir1_type = IR1_TYPE_JMPIN;
        break;        
    case AARCH64_A64_BL:
        tb->last_ir1_type = IR1_TYPE_CALL;
        tb->target_pc = s->pc_curr + ((arg_i)(s->arg.f_i)).imm;
        break;
    case AARCH64_A64_BLR:
        tb->last_ir1_type = IR1_TYPE_CALLIN;
        break;
    case AARCH64_A64_B_cond:
        tb->last_ir1_type = IR1_TYPE_BRANCH;
        tb->target_pc = s->pc_curr + ((arg_B_cond)(s->arg.f_decode_insn3211)).imm;
        break;
    case AARCH64_A64_TBZ:
        tb->last_ir1_type = IR1_TYPE_BRANCH;
        tb->target_pc = s->pc_curr + ((arg_tbz)(s->arg.f_tbz)).imm;
        break;
    case AARCH64_A64_CBZ:
        tb->last_ir1_type = IR1_TYPE_BRANCH;
        tb->target_pc = s->pc_curr + ((arg_cbz)(s->arg.f_cbz)).imm;
        break;
    case AARCH64_A64_RET:
        tb->last_ir1_type = IR1_TYPE_RET;
        break;
    case AARCH64_A64_SVC:
        tb->last_ir1_type = IR1_TYPE_SYSCALL;
        break;
    default:
        tb->last_ir1_type = IR1_TYPE_NORMAL;
        break;
    }
}
#endif

static void set_base_isjump(DisasContext *s)
{
    if(ir1_is_branch(s) || ir1_is_jmp(s) || ir1_is_call(s) || ir1_is_ret(s) || ir1_is_syscall(s)){
        s->base->is_jmp = DISAS_NORETURN;
        return;
    }
    if(s->insn_type == AARCH64_A64_NULL){
        s->base->is_jmp = DISAS_NORETURN;
        return;        
    }

}

/*
    generate ir1_list
*/
DisasContext *get_ir1_list(CPUState *cpu, TranslationBlock *tb, vaddr pc, int max_insns)
{
    DisasContext *ir1_list = (DisasContext *)mm_calloc(max_insns, sizeof(DisasContext));
    DisasContext *pir1 = NULL;

    uint32_t cflags = tb_cflags(tb);

    /* Initialize DisasContextBase */
    DisasContextBase *db = (DisasContextBase *)malloc(sizeof(DisasContextBase));
    db->tb = tb;
    db->pc_first = pc;
    db->pc_next = pc;
    db->is_jmp = DISAS_NEXT;
    db->num_insns = 0;
    db->max_insns = max_insns;
    db->singlestep_enabled = cflags & CF_SINGLE_STEP;
    db->host_addr[0] = (void *)pc;
    db->host_addr[1] = NULL;

    tcg_debug_assert(db->is_jmp == DISAS_NEXT);  /* no early exit */

    while (true) {
        max_insns = db->num_insns;
        pir1 = &ir1_list[max_insns];
        pir1->base = db;
        pir1->insn_type = AARCH64_A64_NULL;
        aarch64_tr_init_disas_context(pir1, cpu);
        tcg_debug_assert(db->is_jmp == DISAS_NEXT);  /* no early exit */

        /* Disassemble one instruction.  The translate_insn hook should
           update db->pc_next and db->is_jmp to indicate what should be
           done next -- either exiting this loop or locate the start of
           the next instruction.  */
        if (db->num_insns == db->max_insns && (cflags & CF_LAST_IO)) {
            translate_aarch64_insn(pir1, cpu);
        } else {
            /* we should only see CF_MEMI_ONLY for io_recompile */
            tcg_debug_assert(!(cflags & CF_MEMI_ONLY));
            translate_aarch64_insn(pir1, cpu);
        }

#ifdef CONFIG_LATA_TU
        if(pir1->insn_type == AARCH64_A64_NULL){
            tb->s_data->tu_tb_mode = TU_TB_MODE_BROKEN;
            tb->next_pc = tb->pc;
            db->pc_next = tb->pc;
            break;            
        }            
#endif

        db->num_insns++;

        /* Stop translation if translate_insn so indicated.  */
        if (db->is_jmp != DISAS_NEXT) {
            break;
        }

        /* Stop translation if the output buffer is full,
           or we have executed all of the allowed instructions.  */
        if (tcg_op_buf_full() || db->num_insns >= db->max_insns) {
            db->is_jmp = DISAS_TOO_MANY;
            break;
        }
    }

    tb->size = db->pc_next - db->pc_first;
    tb->icount = db->num_insns;

#ifndef CONFIG_LATA_TU
    if (qemu_loglevel_mask(CPU_LOG_TB_IN_ASM)
        && qemu_log_in_addr_range(db->pc_first)) {
        FILE *logfile = qemu_log_trylock();
        if (logfile && tb->icount) {
            fprintf(logfile, "IN: %s\n", lookup_symbol(db->pc_first));
            fprintf(logfile, "-----------------------------------------\n");
          
            target_disas(logfile, cpu, db->pc_first, db->tb->size);
            fprintf(logfile, "-----------------------------------------\n");
            qemu_log_unlock(logfile);
        }
    }
#endif     

#ifdef CONFIG_LATA
    if(clearGprHigh){
        uint32_t old = lsenv->tr_data->w_write_flag;
        if(old != 0){
            printf("0x%lx :old:%x\n", db->pc_first, old);
            assert(old == 0);
        }
    }
#endif

#ifdef CONFIG_LATA_TU 
    tb->next_pc = db->pc_next;
    get_last_info(tb, pir1);
#endif

    return ir1_list;
}

/*
    TB translate:replace original translate_insn funtion to store insn in ir1_list
    a) store insns information in ir1_list,tb->ir1 points to the first element of ir_list
*/
void target_disasm(struct TranslationBlock *tb, int *max_insns, CPUState *cpu)
{
    uint64_t pc = tb->pc;
    tb->ir1 = get_ir1_list(cpu, tb, pc, *max_insns);
}

/* Decode one aarch64 instruction(just generate insns information,don't translate)
  a) use DisasContext to store information of each instruction
  b) use decode_ir1_a64.c.inc to generate insn_type and arg using translate_funtions
  c) determine whether it is jmp ins and mark DISAS_NORETURN if it is
*/
void translate_aarch64_insn(DisasContext *s, CPUState *cpu)
{
    CPUARMState *env = cpu->env_ptr;
    uint64_t pc = s->base->pc_next;
    uint32_t insn;

    /* Singlestep exceptions have the highest priority. */
    if (s->ss_active && !s->pstate_ss) {
        /* Singlestep state is Active-pending.
         * If we're in this state at the start of a TB then either
         *  a) we just took an exception to an EL which is being debugged
         *     and this is the first insn in the exception handler
         *  b) debug exceptions were masked and we just unmasked them
         *     without changing EL (eg by clearing PSTATE.D)
         * In either case we're going to take a swstep exception in the
         * "did not step an insn" case, and so the syndrome ISV and EX
         * bits should be zero.
         */
        assert(0);
        s->base->is_jmp = DISAS_NORETURN;
        s->base->pc_next = pc + 4;
        return;
    }

    if (pc & 3) {
        /*
         * PC alignment fault.  This has priority over the instruction abort
         * that we would receive from a translation fault via arm_ldl_code.
         * This should only be possible after an indirect branch, at the
         * start of the TB.
         */
        assert(0);
        s->base->is_jmp = DISAS_NORETURN;
        s->base->pc_next = QEMU_ALIGN_UP(pc, 4);
        return;
    }

    s->pc_curr = pc;
    insn = arm_ldl_code(env, s->base, pc, s->sctlr_b);
    s->insn = insn;
    s->base->pc_next = pc + 4;

    s->fp_access_checked = false;
    s->sve_access_checked = false;

    s->is_nonstreaming = false;

    //don't support sme and sve
    if (!decode_insn32(s, insn) ) {
        disas_a64_legacy(s, insn);
    }

#ifdef CONFIG_LATA_INSTS_PATTERN
    nzcv_use(s->base->tb, insn);
#endif

    //Determine whether the instruction is a jump type
    set_base_isjump(s);
}

/*ir1 -> ir2*/
bool tr_ir2_generate(struct TranslationBlock *tb)
{
    int i;
    int start = 0;
#ifdef CONFIG_LATA_TU
    start = tu_data->curr_insns;
#endif

    int ir1_nr = tb->icount;
    DisasContext *pir1 = (DisasContext *)tb->ir1;
    for (i = start; i < ir1_nr + start; ++i) {
#ifdef CONFIG_LATA_INSTS_PATTERN
        /*Only check if the last two elements are CMP+B.COND*/
        if(i == ir1_nr + start - 2 && insts_pattern(pir1,pir1 + 1)) {
            tcg_ctx->gen_insn_end_off[i] = (lsenv->tr_data->real_ir2_inst_num)<<2;
            tcg_ctx->gen_insn_data[i * TARGET_INSN_START_WORDS]= pir1->pc_curr;  
            pir1 += 2; //update true pir1 for tb stop check 
            break;          
        }
#endif
        bool trans_success = translate_functions[pir1->insn_type](pir1);
        if (!trans_success) {
            lsassertm(0, "ir1_translate fail");
        }
#ifndef CONFIG_LATA_TU
        tcg_ctx->gen_insn_end_off[i] = (lsenv->tr_data->real_ir2_inst_num)<<2;
        tcg_ctx->gen_insn_data[i * TARGET_INSN_START_WORDS]= pir1->pc_curr;
#endif
        pir1++;
    }

#ifdef CONFIG_LATA_TU
    if(tb->isplit){
        tb->last_ir1_type = IR1_TYPE_NORMAL;
        IR2_OPND goto_label = ir2_opnd_new_type(IR2_OPND_LABEL);
        IR2_OPND ir2_opnd_addr;
        la_label(goto_label);
        tb->jmp_reset_offset[0] = ir2_opnd_label_id(&goto_label);
        ir2_opnd_build(&ir2_opnd_addr, IR2_OPND_IMM, 1);
        la_b(ir2_opnd_addr); // nop
    }
#endif

    /*tb stop function*/
    pir1--;
    switch (pir1->base->is_jmp) {
    case DISAS_NEXT:
    case DISAS_TOO_MANY:
        if (clearGprHigh) {
            for (int i = 0; i < 32; ++i) {
                if (arm_la_map[i] >= 0) {
                    clear_gpr_high(i);
                }
            }
        }  
        gen_goto_tb(pir1, 1, 4);
        break;
    default:
    case DISAS_UPDATE_EXIT:
        lata_gen_a64_update_pc(pir1, 4);
        /* fall through */
    case DISAS_EXIT:
        lata_gen_exit_tb_ret_0(pir1);
        break;
    case DISAS_NORETURN:
        break;
    }

    return true;
}

static void aarch64_tr_disas_log(const DisasContextBase *dcbase,
                                 CPUState *cpu, FILE *logfile)
{
    fprintf(logfile, "IN: %s\n", lookup_symbol(dcbase->pc_first));
    target_disas(logfile, cpu, dcbase->pc_first, dcbase->tb->size);
}

/* initialize TCG globals.  */
void arm_translate_init(void)
{
}

const TranslatorOps aarch64_translator_ops = {
    .init_disas_context = NULL,
    .tb_start           = aarch64_tr_tb_start,
    .insn_start         = aarch64_tr_insn_start,
    .translate_insn     = NULL,
    .tb_stop            = NULL,
    .disas_log          = aarch64_tr_disas_log,
};