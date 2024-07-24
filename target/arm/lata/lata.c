#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "lata.h"

#include "exec/exec-all.h"
#include "qemu/log.h"
#include "env.h"

//sp
//tp
//zero---unused
//a0-----goto_tb
//s8-----env 
//t0-t5--itemp
//FLAG---pstate
const int arm_la_map[] = {
    [armv8_x0] = la_ra,
    [armv8_x1] = la_a1,
    [armv8_x2] = la_a2,
    [armv8_x3] = la_a3,
    [armv8_x4] = la_a4,
    [armv8_x5] = la_a5,
    [armv8_x6] = la_a6,
    [armv8_x7] = la_a7,
    [armv8_x8] = la_fp,
    [armv8_x9] = la_t8,
    [armv8_x10] = la_t6,
    [armv8_x11] = -1,
    [armv8_x12] = -1,
    [armv8_x13] = -1,
    [armv8_x14] = -1,
    [armv8_x15] = -1,
    [armv8_x16] = -1,
    [armv8_x17] = -1,
    [armv8_x18] = -1,
    [armv8_x19] = la_s6,
    [armv8_x20] = la_s0,
    [armv8_x21] = la_s1,
    [armv8_x22] = la_s2,
    [armv8_x23] = la_s3,
    [armv8_x24] = la_s4,
    [armv8_x25] = -1,
    [armv8_x26] = -1,
    [armv8_x27] = -1,
    [armv8_x28] = la_s7,
    [armv8_x29] = la_t7,
    [armv8_x30] = la_s5,
    [armv8_xzr] = la_r21
};

const int arm_la_reverse_map[] = {
    [la_zero] = -1,
    [la_ra]  = armv8_x0,
    [la_sp]  = -1,
    [la_t7]  = armv8_x29,
    [la_s0]  = armv8_x20,
    [la_s1]  = armv8_x21,
    [la_s8]  = -1,
    [la_t8]  = armv8_x9,
    [la_a2]  = armv8_x2,
    [la_a3]  = armv8_x3,
    [la_a4]  = armv8_x4,
    [la_a5]  = armv8_x5,
    [la_a6]  = armv8_x6,
    [la_a7]  = armv8_x7,
    [la_s2]  = armv8_x22,
    [la_s3]  = armv8_x23,
    [la_s4]  = armv8_x24,
    [la_s5]  = armv8_x30,
    [la_s6]  = armv8_x19,
    [la_s7]  = armv8_x28,
    [la_t4]  = -1,
    [la_t5]  = -1,
    [la_t6]  = armv8_x10,
    [la_r21] = armv8_xzr,
    [la_tp] = -1,
    [la_a0] = -1,
    [la_a1] = armv8_x1,
    [la_t0] = -1,
    [la_t1] = -1,
    [la_t2] = -1,
    [la_t3] = -1,
    [la_fp] = armv8_x8 
};

const int arm_la_fmap[] = {
    [armv8_v0] = 0,
    [armv8_v1] = 1,
    [armv8_v2] = 2,
    [armv8_v3] = 3,
    [armv8_v4] = 4,
    [armv8_v5] = 5,
    [armv8_v6] = 6,
    [armv8_v7] = 7,
    [armv8_v8] = 8,
    [armv8_v9] = -1,
    [armv8_v10] = -1,
    [armv8_v11] = -1,
    [armv8_v12] = -1,
    [armv8_v13] = -1,
    [armv8_v14] = -1,
    [armv8_v15] = 15,
    [armv8_v16] = 16,
    [armv8_v17] = 17,
    [armv8_v18] = 18,
    [armv8_v19] = 19,
    [armv8_v20] = 20,
    [armv8_v21] = 21,
    [armv8_v22] = 22,
    [armv8_v23] = 23,
    [armv8_v24] = 24,
    [armv8_v25] = 25,
    [armv8_v26] = 26,
    [armv8_v27] = 27,
    [armv8_v28] = 28,
    [armv8_v29] = 29,
    [armv8_v30] = 30,
    [armv8_v31] = 31
};

const int arm_la_reverse_fmap[] = {
    [0] = 0,
    [1] = 1,
    [2] = 2,
    [3] = 3,
    [4] = 4,
    [5] = 5,
    [6] = 6,
    [7] = 7,
    [8] = 8,
    [9] = -1,
    [10] = -1,
    [11] = -1,
    [12] = -1,
    [13] = -1,
    [14] = -1,
    [15] = 15,
    [16] = 16,
    [17] = 17,
    [18] = 18,
    [19] = 19,
    [20] = 20,
    [21] = 21,
    [22] = 22,
    [23] = 23,
    [24] = 24,
    [25] = 25,
    [26] = 26,
    [27] = 27,
    [28] = 28,
    [29] = 29,
    [30] = 30,
    [31] = 31

};

void lata(void);
__attribute__((constructor))
void lata(void){
    // printf("lata enabled\n");
}

__thread ENV *lsenv;
static __thread ENV lsenv_real;


static __thread TRANSLATION_DATA tr_data_real;
__thread TRANSLATION_DATA *tr_data;

bool lata_dump = 0;
uint64_t context_switch_bt_to_native;
uint64_t context_switch_native_to_bt_ret_0;
uint64_t context_switch_native_to_bt;

static void global_register_init(void)
{
    env_ir2_opnd = INIT_RA(IR2_OPND_GPR, reg_statics_map[S_ENV]);

    /* All LA reg */
    zero_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_zero);
    ra_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_ra);
    tp_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_tp);
    sp_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_sp);
    a0_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_a0);
    a1_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_a1);
    a2_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_a2);
    a3_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_a3);
    a4_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_a4);
    a5_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_a5);
    a6_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_a6);
    a7_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_a7);
    t0_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_t0);
    t1_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_t1);
    t2_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_t2);
    t3_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_t3);
    t4_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_t4);
    t5_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_t5);
    t6_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_t6);
    t7_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_t7);
    t8_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_t8);
    r21_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_r21);
    fp_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_fp);
    s0_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_s0);
    s1_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_s1);
    s2_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_s2);
    s3_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_s3);
    s4_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_s4);
    s5_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_s5);
    s6_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_s6);
    s7_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_s7);
    s8_ir2_opnd = INIT_RA(IR2_OPND_GPR, la_s8);
    fcsr_ir2_opnd = INIT_RA(IR2_OPND_FCSR, 0);
    fcsr1_ir2_opnd = INIT_RA(IR2_OPND_FCSR, 1);
    fcsr2_ir2_opnd = INIT_RA(IR2_OPND_FCSR, 2);
    fcsr3_ir2_opnd = INIT_RA(IR2_OPND_FCSR, 3);
    fcc0_ir2_opnd = INIT_RA(IR2_OPND_CC, 0);
    fcc1_ir2_opnd = INIT_RA(IR2_OPND_CC, 1);
    fcc2_ir2_opnd = INIT_RA(IR2_OPND_CC, 2);
    fcc3_ir2_opnd = INIT_RA(IR2_OPND_CC, 3);
    fcc4_ir2_opnd = INIT_RA(IR2_OPND_CC, 4);
    fcc5_ir2_opnd = INIT_RA(IR2_OPND_CC, 5);
    fcc6_ir2_opnd = INIT_RA(IR2_OPND_CC, 6);
    fcc7_ir2_opnd = INIT_RA(IR2_OPND_CC, 7);

    scr0_ir2_opnd = INIT_RA(IR2_OPND_SCR, 0);
    scr1_ir2_opnd = INIT_RA(IR2_OPND_SCR, 1);
    scr2_ir2_opnd = INIT_RA(IR2_OPND_SCR, 2);
    scr3_ir2_opnd = INIT_RA(IR2_OPND_SCR, 3);
}

void lata_tr_data_init(void)
{
    lsenv = &lsenv_real;
    tr_data = &tr_data_real;
    lsenv->tr_data = tr_data;
    global_register_init();
}

void tr_init(void* tb)
{
    TRANSLATION_DATA *t = tr_data;
    //int i = 0;

    t->curr_tb = tb;
    /* register allocation init */
    ra_free_all();

    /* reset ir2 array */
    if (t->ir2_inst_array == NULL) {
        lsassert(t->ir2_inst_num_max == 0);
        t->ir2_inst_array = (IR2_INST *)mm_calloc(400, sizeof(IR2_INST));
        t->ir2_inst_num_max = 400;
    }
    t->ir2_inst_num_current = 0;
    //uint64 k = &(t->ir2_inst_num_current);
    t->real_ir2_inst_num = 0;

    /* reset ir2 first/last/num */
    t->first_ir2 = NULL;
    t->last_ir2 = NULL;

    /* label number */
    t->label_num = 0;

}

void tr_fini(void)
{
    TRANSLATION_DATA *t = tr_data;
    /* set current tb and ir1 */

    /* reset ir2 array */
    t->ir2_inst_num_current = 0;
    t->real_ir2_inst_num = 0;

    /* reset ir2 first/last/num */
    t->first_ir2 = NULL;
    t->last_ir2 = NULL;

    /* label number */
    t->label_num = 0;
}

#define EXTRA_SPACE             40
#define REG_LEN                 8
#define S0_EXTRA_SPACE          EXTRA_SPACE
#define S1_EXTRA_SPACE          (S0_EXTRA_SPACE + REG_LEN)
#define S2_EXTRA_SPACE          (S1_EXTRA_SPACE + REG_LEN)
#define S3_EXTRA_SPACE          (S2_EXTRA_SPACE + REG_LEN)
#define S4_EXTRA_SPACE          (S3_EXTRA_SPACE + REG_LEN)
#define S5_EXTRA_SPACE          (S4_EXTRA_SPACE + REG_LEN)
#define S6_EXTRA_SPACE          (S5_EXTRA_SPACE + REG_LEN)
#define S7_EXTRA_SPACE          (S6_EXTRA_SPACE + REG_LEN)
#define S8_EXTRA_SPACE          (S7_EXTRA_SPACE + REG_LEN)
#define FP_EXTRA_SPACE          (S8_EXTRA_SPACE + REG_LEN)
#define RA_EXTRA_SPACE          (FP_EXTRA_SPACE + REG_LEN)
#define FCSR_EXTRA_SPACE        (RA_EXTRA_SPACE + REG_LEN)


static void generate_context_switch_bt_to_native(CPUState *cs)
{
    la_addi_d(sp_ir2_opnd, sp_ir2_opnd, -256);

    /* save callee-saved LA registers. s0-s8 */
    la_st_d(s0_ir2_opnd, sp_ir2_opnd, S0_EXTRA_SPACE);
    la_st_d(s1_ir2_opnd, sp_ir2_opnd, S1_EXTRA_SPACE);
    la_st_d(s2_ir2_opnd, sp_ir2_opnd, S2_EXTRA_SPACE);
    la_st_d(s3_ir2_opnd, sp_ir2_opnd, S3_EXTRA_SPACE);
    la_st_d(s4_ir2_opnd, sp_ir2_opnd, S4_EXTRA_SPACE);
    la_st_d(s5_ir2_opnd, sp_ir2_opnd, S5_EXTRA_SPACE);
    la_st_d(s6_ir2_opnd, sp_ir2_opnd, S6_EXTRA_SPACE);
    la_st_d(s7_ir2_opnd, sp_ir2_opnd, S7_EXTRA_SPACE);
    la_st_d(s8_ir2_opnd, sp_ir2_opnd, S8_EXTRA_SPACE);

    /* save fp and ra */
    la_st_d(fp_ir2_opnd, sp_ir2_opnd, FP_EXTRA_SPACE);
    la_st_d(ra_ir2_opnd, sp_ir2_opnd, RA_EXTRA_SPACE);

    /* set env_opnd */
    la_mov64(env_ir2_opnd, a0_ir2_opnd);
    la_mov64(a0_ir2_opnd, a1_ir2_opnd);

    /* set pstate reg */
    la_ld_w(a7_ir2_opnd, env_ir2_opnd, env_offset_PSTATE());
    la_armmtflag(a7_ir2_opnd, 0x39);

    /* set fcsr, rm set by FPSCR[23:22], others was cleared */
    li_d(a5_ir2_opnd, env_offset_FPSCR());
    la_ldx_d(a7_ir2_opnd, env_ir2_opnd, a5_ir2_opnd);
    la_bstrpick_d(a7_ir2_opnd, a7_ir2_opnd, 23, 22);
    li_d(a6_ir2_opnd, 2 << 6 | 1 << 4 | 3 << 2 | 0);
    la_slli_d(a7_ir2_opnd, a7_ir2_opnd, 1);
    la_srl_d(a7_ir2_opnd, a6_ir2_opnd, a7_ir2_opnd);
    la_andi(a7_ir2_opnd, a7_ir2_opnd, 0x3);
    la_slli_d(a7_ir2_opnd, a7_ir2_opnd, 8);
    la_movgr2fcsr(fcsr_ir2_opnd, a7_ir2_opnd);


    /* load gpr and fpr */
    for(int i = 0; i <= 31; ++i) {
        if(arm_la_fmap[i] >= 0) {
            li_d(a7_ir2_opnd, env_offset_fpr(i));
            la_vldx(ir2_opnd_new(IR2_OPND_FPR, arm_la_fmap[i]), env_ir2_opnd, a7_ir2_opnd);
        }
    }
    for(int i = 0; i <= 31; ++i) {
        if(arm_la_map[i] > 0) {
            la_ld_d(ir2_opnd_new(IR2_OPND_GPR, arm_la_map[i]), env_ir2_opnd, env_offset_gpr(i));
        }
    }

    /* jmp to tb */
    la_jirl(zero_ir2_opnd, a0_ir2_opnd, 0);

}

static void generate_context_switch_native_to_bt(CPUState *cs)
{
    la_mov64(a0_ir2_opnd, zero_ir2_opnd);
    for(int i = 0; i <= 31; ++i) {
        if(arm_la_map[i] > 0) {
            la_st_d(ir2_opnd_new(IR2_OPND_GPR, arm_la_map[i]), env_ir2_opnd, env_offset_gpr(i));
        }
    }
    for(int i = 0; i <= 31; ++i) {
        if(arm_la_fmap[i] >= 0) {
            li_d(t0_ir2_opnd, env_offset_fpr(i));
            la_vstx(ir2_opnd_new(IR2_OPND_FPR, arm_la_fmap[i]), env_ir2_opnd, t0_ir2_opnd);
        }
    }

    /* store pstate reg */
    la_armmfflag(t0_ir2_opnd, 0x39);
    la_st_w(t0_ir2_opnd, env_ir2_opnd, env_offset_PSTATE());
    
    /* store fcsr(rouding mode) to vfp.xregs[ARM_VFP_FPSCR] */
    la_movfcsr2gr(t0_ir2_opnd, fcsr3_ir2_opnd);
    li_d(t1_ir2_opnd, 2 << 6 | 1 << 4 | 3 << 2 | 0);
    la_srli_d(t0_ir2_opnd, t0_ir2_opnd, 7);
    la_srl_d(t0_ir2_opnd, t1_ir2_opnd, t0_ir2_opnd);
    la_andi(t0_ir2_opnd, t0_ir2_opnd, 0x3);
    li_d(t2_ir2_opnd, env_offset_FPSCR());
    la_ldx_d(t1_ir2_opnd, env_ir2_opnd, t2_ir2_opnd);
    la_bstrins_d(t1_ir2_opnd, t0_ir2_opnd, 23, 22);
    la_stx_d(t1_ir2_opnd, env_ir2_opnd, t2_ir2_opnd);

    /* load callee-saved LA registers. s0-s8 */
    la_ld_d(s0_ir2_opnd, sp_ir2_opnd, S0_EXTRA_SPACE);
    la_ld_d(s1_ir2_opnd, sp_ir2_opnd, S1_EXTRA_SPACE);
    la_ld_d(s2_ir2_opnd, sp_ir2_opnd, S2_EXTRA_SPACE);
    la_ld_d(s3_ir2_opnd, sp_ir2_opnd, S3_EXTRA_SPACE);
    la_ld_d(s4_ir2_opnd, sp_ir2_opnd, S4_EXTRA_SPACE);
    la_ld_d(s5_ir2_opnd, sp_ir2_opnd, S5_EXTRA_SPACE);
    la_ld_d(s6_ir2_opnd, sp_ir2_opnd, S6_EXTRA_SPACE);
    la_ld_d(s7_ir2_opnd, sp_ir2_opnd, S7_EXTRA_SPACE);
    la_ld_d(s8_ir2_opnd, sp_ir2_opnd, S8_EXTRA_SPACE);

    /* load fp and ra */
    la_ld_d(fp_ir2_opnd, sp_ir2_opnd, FP_EXTRA_SPACE);
    la_ld_d(ra_ir2_opnd, sp_ir2_opnd, RA_EXTRA_SPACE);

    la_addi_d(sp_ir2_opnd, sp_ir2_opnd, 256);

    /* TODO: clear fcsr*/
    la_movgr2fcsr(fcsr_ir2_opnd, zero_ir2_opnd);

    la_jirl(zero_ir2_opnd, ra_ir2_opnd, 0);
}



/* bt -> native */
int lata_gen_prologue(CPUState *cs, TCGContext *tcg_ctx)
{
    lsassert(context_switch_bt_to_native == 0);
    void *code_buf_rw = tcg_ctx->code_ptr;
    const void *code_buf_rx = tcg_splitwx_to_rx(code_buf_rw);
    context_switch_bt_to_native = (uint64_t)code_buf_rx;

    if (lata_dump)
        qemu_log("[LATA] context_switch_bt_to_native = %p\n",
                (void *)context_switch_bt_to_native);

    tr_init(NULL);
    generate_context_switch_bt_to_native(cs);
//    int ins_nr = la_encode(tcg_ctx, code_buf_rw);
    int ins_nr = tr_ir2_assemble(code_buf_rw);
    tr_fini();

    return ins_nr;
}

/* native -> bt */
int lata_gen_epilogue(CPUState *cs, TCGContext *tcg_ctx)
{
    lsassert(context_switch_native_to_bt == 0);
    void *code_buf_rw = tcg_ctx->code_ptr;
    const void *code_buf_rx = tcg_splitwx_to_rx(code_buf_rw);
    context_switch_native_to_bt_ret_0 = (uint64_t)code_buf_rx;
    context_switch_native_to_bt = (uint64_t)code_buf_rx + 4;

    if (lata_dump)
        qemu_log("[LATA] context_switch_native_to_bt = %p\n",
                (void *)context_switch_native_to_bt);

    tr_init(NULL);
    generate_context_switch_native_to_bt(cs);
//    int ins_nr = la_encode(tcg_ctx, code_buf_rw);
    int ins_nr = tr_ir2_assemble(code_buf_rw);
    tr_fini();

    return ins_nr;
}


static void label_dispose(void)
{
    /* 1. record the positions of label */
    int *label_num_to_ir2_num =
        (int *)alloca(lsenv->tr_data->label_num * 4 + 20);
    memset(label_num_to_ir2_num, -1, lsenv->tr_data->label_num * 4 + 20);
    int ir2_num = 0;
    IR2_INST *ir2_current = lsenv->tr_data->first_ir2;
    while (ir2_current != NULL) {
        if (ir2_current->_opcode == LISA_LABEL) {
            int label_num = ir2_opnd_label_id(&ir2_current->_opnd[0]);
            lsassertm(label_num_to_ir2_num[label_num] == -1,
                      "label %d is in multiple positions\n", label_num);
            label_num_to_ir2_num[label_num] = ir2_num;
        } else if (ir2_current->_opcode == LISA_X86_INST) {
            /* will not be assembled */
        } else {
            ir2_num++;
        }
        ir2_current = ir2_next(ir2_current);
    }

    /* 2. resolve the offset of successor linkage code */
    /* @jmp_target_arg recoed the jmp  inst position */
    /* @jmp_reset_offset record the successor inst of jmp(exclude delay slot).
     */
    /*                   when the tb is removed from buffer. the jmp inst use
     */
    /*                   this position to revert the original "fall through".
     */
    {
        TranslationBlock *tb = lsenv->tr_data->curr_tb;

        /* prologue/epilogue has no tb */
        if (tb) {
            /* ctx->jmp_insn_offset point to tb->jmp_target_arg */
            int label_id_0 = tb->jmp_reset_offset[0];
            if (label_id_0 != TB_JMP_OFFSET_INVALID) {
                tb->jmp_insn_offset[0] =
                    (label_num_to_ir2_num[label_id_0] << 2);
                tb->jmp_reset_offset[0] =
                    (label_num_to_ir2_num[label_id_0] << 2) + B_STUB_SIZE;
                // tb->jmp_target_addr[0] = (label_num_to_ir2_num[label_id_0] << 2);
            }

            int label_id_1 = tb->jmp_reset_offset[1];
            if (label_id_1 != TB_JMP_OFFSET_INVALID) {
                tb->jmp_insn_offset[1] =
                    (label_num_to_ir2_num[label_id_1] << 2);
                tb->jmp_reset_offset[1] =
                    (label_num_to_ir2_num[label_id_1] << 2) + B_STUB_SIZE;
                // tb->jmp_target_addr[1] = (label_num_to_ir2_num[label_id_1] << 2);
            }
        }
    }

    /* 3. resolve the branch instructions */
    ir2_num = 0;
    ir2_current = lsenv->tr_data->first_ir2;
    while (ir2_current != NULL) {
        IR2_OPCODE opcode = ir2_current->_opcode;
        if (ir2_opcode_is_branch(opcode) || ir2_opcode_is_f_branch(opcode)) {
            IR2_OPND *label_opnd = NULL;
            if (ir2_current->_opcode == LISA_B ||
                ir2_current->_opcode == LISA_BL) {
                label_opnd = &ir2_current->_opnd[0];
            } else if (ir2_opcode_is_branch_with_3opnds(opcode)) {
                label_opnd = &ir2_current->_opnd[2];
            } else if (ir2_opcode_is_f_branch(opcode) ||
                       ir2_opcode_is_branch_with_2opnds(opcode)) {
                label_opnd = &ir2_current->_opnd[1];
            }
            if(label_opnd && ir2_opnd_is_label(label_opnd)) {
                int label_num = label_opnd->_label_id;
                lsassert(label_num > 0 && label_num <= lsenv->tr_data->label_num);
                int target_ir2_num = label_num_to_ir2_num[label_num];
                lsassertm(target_ir2_num != -1, "label %d is not inserted\n",
                          label_num);
                ir2_opnd_convert_label_to_imm(label_opnd,
                                              target_ir2_num - ir2_num);
            }
        }

        if (ir2_current->_opcode != LISA_LABEL &&
            ir2_current->_opcode != LISA_X86_INST) {
            ir2_num++;
        }
        ir2_current = ir2_next(ir2_current);
    }
}

int tr_ir2_assemble(const void *code_start_addr)
{
    if (lata_dump) {
        qemu_log("[LATA] Assemble IR2.\n");
    }

    /* 1. assign temp register to physical register */
    /* now, temp register is physical register */

    /* 2. label dispose */
    label_dispose();

    /* 3. assemble */
    IR2_INST *pir2 = lsenv->tr_data->first_ir2;

    void *code_addr = (void *) code_start_addr;
    int code_nr = 0;

    while (pir2 != NULL) {
        if (ir2_opcode(pir2) != LISA_LABEL &&
            ir2_opcode(pir2) != LISA_X86_INST) {
            uint32 result = ir2_assemble(pir2);

            if (lata_dump) {
                qemu_log("IR2[%03d] at %p 0x%08x \n", pir2->_id, code_addr,
                        result);
                ir2_dump(pir2);
            }

            *(uint32 *)code_addr = result;
            code_addr = code_addr + 4;
            code_nr += 1;
        }
        pir2 = ir2_next(pir2);
    }

    return code_nr;
}

void lata_gen_call_helper_prologue(TCGContext *tcg_ctx)
{
    for(int i = 0; i <= 31; ++i) {
        if(arm_la_map[i] > 0) {
            la_st_d(ir2_opnd_new(IR2_OPND_GPR, arm_la_map[i]), env_ir2_opnd, env_offset_gpr(i));
        }
    }

    for(int i = 0; i <= 31; ++i) {
        if(arm_la_fmap[i] >= 0) {
            li_d(t0_ir2_opnd, env_offset_fpr(i));
            la_vstx(ir2_opnd_new(IR2_OPND_FPR, arm_la_fmap[i]), env_ir2_opnd, t0_ir2_opnd);
        }
    }

    /* store pstate reg */
    la_armmfflag(t0_ir2_opnd, 0x39);
    la_st_w(t0_ir2_opnd, env_ir2_opnd, env_offset_PSTATE());
    
    /* store fcsr(rouding mode) to vfp.xregs[ARM_VFP_FPSCR] */
    la_movfcsr2gr(t0_ir2_opnd, fcsr3_ir2_opnd);
    li_d(t1_ir2_opnd, 2 << 6 | 1 << 4 | 3 << 2 | 0);
    la_srli_d(t0_ir2_opnd, t0_ir2_opnd, 7);
    la_srl_d(t0_ir2_opnd, t1_ir2_opnd, t0_ir2_opnd);
    la_andi(t0_ir2_opnd, t0_ir2_opnd, 0x3);
    li_d(t2_ir2_opnd, env_offset_FPSCR());
    la_ldx_d(t1_ir2_opnd, env_ir2_opnd, t2_ir2_opnd);
    la_bstrins_d(t1_ir2_opnd, t0_ir2_opnd, 23, 22);
    la_stx_d(t1_ir2_opnd, env_ir2_opnd, t2_ir2_opnd);

    /* clear fcsr */
    la_movgr2fcsr(fcsr_ir2_opnd, zero_ir2_opnd);
}

void lata_gen_call_helper_epilogue(TCGContext *tcg_ctx)
{
    /* set pstate reg */
    la_ld_w(a7_ir2_opnd, env_ir2_opnd, env_offset_PSTATE());
    la_armmtflag(a7_ir2_opnd, 0x39);

    /* set fcsr, rm set by FPSCR[23:22], others was cleared */
    li_d(a5_ir2_opnd, env_offset_FPSCR());
    la_ldx_d(a7_ir2_opnd, env_ir2_opnd, a5_ir2_opnd);
    la_bstrpick_d(a7_ir2_opnd, a7_ir2_opnd, 23, 22);
    li_d(a6_ir2_opnd, 2 << 6 | 1 << 4 | 3 << 2 | 0);
    la_slli_d(a7_ir2_opnd, a7_ir2_opnd, 1);
    la_srl_d(a7_ir2_opnd, a6_ir2_opnd, a7_ir2_opnd);
    la_andi(a7_ir2_opnd, a7_ir2_opnd, 0x3);
    la_slli_d(a7_ir2_opnd, a7_ir2_opnd, 8);
    la_movgr2fcsr(fcsr_ir2_opnd, a7_ir2_opnd);

    for(int i = 0; i <= 31; ++i) {
        if(arm_la_fmap[i] >= 0) {
            li_d(a7_ir2_opnd, env_offset_fpr(i));
            la_vldx(ir2_opnd_new(IR2_OPND_FPR, arm_la_fmap[i]), env_ir2_opnd, a7_ir2_opnd);
        }
    }

    for(int i = 0; i <= 31; ++i) {
        if(arm_la_map[i] > 0) {
            la_ld_d(ir2_opnd_new(IR2_OPND_GPR, arm_la_map[i]), env_ir2_opnd, env_offset_gpr(i));
        }
    }
}

/* gpr同时为src和dst, 使用alloc_gpr_src,并且需要store回去 */
IR2_OPND alloc_gpr_src(int i) {
    if(i==31){
        return ir2_opnd_new(IR2_OPND_GPR, la_zero);
    }
    if (arm_la_map[i] >= 0) {
        return ir2_opnd_new(IR2_OPND_GPR, arm_la_map[i]);
    } else {
        IR2_OPND t = ra_alloc_itemp();
        la_ld_d(t, env_ir2_opnd, env_offset_gpr(i));
        return t;
    }
}

IR2_OPND alloc_gpr_src_sp(int i) {
    if (arm_la_map[i] >= 0) {
        return ir2_opnd_new(IR2_OPND_GPR, arm_la_map[i]);
    } else {
        IR2_OPND t = ra_alloc_itemp();
        la_ld_d(t, env_ir2_opnd, env_offset_gpr(i));
        return t;
    }
}

IR2_OPND alloc_gpr_dst(int i) {
    if(i==31){
        return ir2_opnd_new(IR2_OPND_GPR, la_zero);
    }
    if (arm_la_map[i] >= 0) {
        return ir2_opnd_new(IR2_OPND_GPR, arm_la_map[i]);
    } else {
        return ra_alloc_itemp();
    }
}

IR2_OPND alloc_gpr_dst_sp(int i) {
    if (arm_la_map[i] >= 0) {
        return ir2_opnd_new(IR2_OPND_GPR, arm_la_map[i]);
    } else {
        return ra_alloc_itemp();
    }
}

/* la_zero作为目的寄存器不需要写回 */
void store_gpr_dst(int i, IR2_OPND opnd) {
    if (arm_la_map[i] < 0 && opnd.val != 0) {
        la_st_d(opnd, env_ir2_opnd, env_offset_gpr(i));
    }   
}

/* la_zero不会被映射，但也不是临时寄存器 */
void free_alloc_gpr(IR2_OPND opnd) {
    if (arm_la_reverse_map[opnd.val] == -1 && opnd.val != 0) {
        ra_free_temp(opnd);
    }
}

// fpr
IR2_OPND alloc_fpr_src(int i) {
    if (arm_la_fmap[i] >= 0) {
        return ir2_opnd_new(IR2_OPND_FPR, arm_la_fmap[i]);
    } else {
        IR2_OPND t = ra_alloc_ftemp();
        IR2_OPND offset = ra_alloc_itemp();

        li_d(offset, env_offset_fpr(i));
        la_vldx(t, env_ir2_opnd, offset);

        ra_free_temp(offset);
        return t;
    }
}

IR2_OPND alloc_fpr_dst(int i) {
    if (arm_la_fmap[i] >= 0) {
        return ir2_opnd_new(IR2_OPND_FPR, arm_la_fmap[i]);
    } else {
        return ra_alloc_ftemp();
    }
}

void store_fpr_dst(int i, IR2_OPND opnd) {
    if (arm_la_fmap[i] < 0) {
        IR2_OPND offset = ra_alloc_itemp();

        li_d(offset, env_offset_fpr(i));
        la_vstx(opnd, env_ir2_opnd, offset);

        ra_free_temp(offset);
    }
}

void free_alloc_fpr(IR2_OPND opnd) {
    if (arm_la_reverse_fmap[opnd.val] == -1) {
        ra_free_temp(opnd);
    }
}
