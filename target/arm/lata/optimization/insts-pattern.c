#include "qemu/osdep.h"
#include "qemu/log.h"
#include "target/arm/tcg/translate.h"
#include "target/arm/lata/include/translate.h"
#include "target/arm/tcg/arm_ldst.h"
#include "insts-pattern.h"
#include "ir2.h"
#include "env.h"

#ifdef CONFIG_LATA_INSTS_PATTERN

static const AArch64PatternTable bcond_table[] = {
    /* pattern  ,  mask     ,  dt_aarch64_insn*/
    {0X54000000, 0xff00001f, B_EQ},
    {0X54000001, 0xff00001f, B_NE},
    {0X54000002, 0xff00001f, B_CS},
    {0X54000003, 0xff00001f, B_CC},
    {0X54000008, 0xff00001f, B_HI},
    {0X54000009, 0xff00001f, B_LS},
    {0X5400000a, 0xff00001f, B_GE},
    {0X5400000b, 0xff00001f, B_LT},
    {0X5400000c, 0xff00001f, B_GT},
    {0X5400000d, 0xff00001f, B_LE},
    {0X5400000e, 0xff00001f, B_AL},
    {0X5400000f, 0xff00001f, B_NV},
    {0x00000000, 0x00000000, INVALID}};

static const AArch64PatternTable cmp_table[] = {
    /* pattern  ,  mask     ,  dt_aarch64_insn*/
    {0x7100001f, 0x7f80001f, CMP_IMM},
    {0x6b20001f, 0x7fe0001f, CMP_EREG},
    {0x6b00001f, 0x7f20001f, CMP_SREG},
    {0x00000000, 0x00000000, INVALID}};

static const AArch64PatternTable generate_nzcv_table[] = {
    /* pattern  ,  mask     ,  dt_aarch64_insn*/
    {0X31000000, 0x3f800000, NZCV_GENERATE}, // ADDSI/SUBSI
    {0X2b000000, 0x3f200000, NZCV_GENERATE}, // ADDS/SUBS
    {0X2b200000, 0x3fe00000, NZCV_GENERATE}, // ADDSE/SUBSE
    {0X3a000000, 0x3fe0fc00, NZCV_GENERATE}, // ADCS/SUBCS
    {0X72000000, 0x7f800000, NZCV_GENERATE}, // ANDSI
    {0X6a000000, 0x7f000000, NZCV_GENERATE}, // ANDS/BICS
    {0X1e202000, 0x5f203c00, NZCV_GENERATE}, // Floating-point compare
    {0X1e200400, 0x5f200c00, NZCV_GENERATE}, // Floating-point conditional compare
    {0x00000000, 0x00000000, INVALID}

    // {0X54000000, 0xff000010, NZCV_GENERATE}, // B.cond
    // {0X1a400000, 0x1fe00000, NZCV_GENERATE}, // cc
    // {0X1e200400, 0x5f200c00, NZCV_GENERATE}, // fcc
    // {0X1e200c00, 0x5f200c00, NZCV_GENERATE}, // fcs
    // {0x00000000, 0x00000000, INVALID}
};

static inline dt_aarch64_insn lookup_pattern(const AArch64PatternTable *table,
                                             uint32_t insn)
{
    const AArch64PatternTable *tptr = table;

    while (tptr->mask)
    {
        if ((insn & tptr->mask) == tptr->pattern)
        {
            return tptr->aarch_pattern;
        }
        tptr++;
    }
    return INVALID;
}

static void nzcv_caculate(IR2_OPND temp_n, IR2_OPND temp, IR2_OPND reg_n, int sf, int n)
{
    TranslationBlock *tb = lsenv->tr_data->curr_tb;
    IR2_OPND nzcv = ir2_opnd_new_type(IR2_OPND_LABEL);
    IR2_OPND ir2_opnd_addr;
    la_label(nzcv);
    tb->nzcv_save[n] = ir2_opnd_label_id(&nzcv);
    ir2_opnd_build(&ir2_opnd_addr, IR2_OPND_IMM, 1);
    la_b(ir2_opnd_addr); // nop

    la_lu12i_w(temp_n, 0x20000);
    la_armmtflag(temp_n, 0x39);          // pstate.c = 1
    la_orn(temp_n, zero_ir2_opnd, temp); // NOT(*src2)
    if (sf)
    {
        la_x86adc_d(reg_n, temp_n);
    }
    else
    {
        la_x86adc_w(reg_n, temp_n);
    }
}

static void pattern_goto_tb(DisasContext *s, int n, int64_t diff)
{
    IR2_OPND goto_label = ir2_opnd_new_type(IR2_OPND_LABEL);
    IR2_OPND ir2_opnd_addr;
    TranslationBlock *tb = lsenv->tr_data->curr_tb;

    la_label(goto_label);
    tb->jmp_reset_offset[n] = ir2_opnd_label_id(&goto_label);
    ir2_opnd_build(&ir2_opnd_addr, IR2_OPND_IMM, 1);
    la_b(ir2_opnd_addr); // nop

    li_d(a0_ir2_opnd, s->pc_curr + diff + 4);
    la_st_d(a0_ir2_opnd, env_ir2_opnd, env_offset_pc());

    if (qemu_loglevel_mask(CPU_LOG_TB_NOCHAIN))
    {
        li_d(a0_ir2_opnd, 0); // do not link
    }
    else
    {
        uint32_t hi32 = ((uint64_t)tb | n) >> 32;
        uint32_t lo32 = ((uint64_t)tb | n) & 0xffffffff;

        la_lu12i_w(a0_ir2_opnd, lo32 >> 12);
        la_ori(a0_ir2_opnd, a0_ir2_opnd, lo32 & 0xfff);
        la_lu32i_d(a0_ir2_opnd, hi32 & 0xfffff);
    }

    int64_t curr_ins_pos = (unsigned long)s->base->tb->tc.ptr + (lsenv->tr_data->real_ir2_inst_num << 2);
    int64_t exit_offset = context_switch_native_to_bt - curr_ins_pos;

    ir2_opnd_build(&ir2_opnd_addr, IR2_OPND_IMM, exit_offset >> 2);
    la_b(ir2_opnd_addr);
}

static void pattern_type(dt_aarch64_insn b_type, IR2_OPND *temp_n, IR2_OPND *temp_m, IR2_OPND *label)
{
    switch (b_type)
    {
    case B_EQ:
        la_beq(*temp_n, *temp_m, *label);
        break;
    case B_NE:
        la_bne(*temp_n, *temp_m, *label);
        break;
    case B_CS:
        la_bgeu(*temp_n, *temp_m, *label);
        break;
    case B_CC:
        la_bltu(*temp_n, *temp_m, *label);
        break;
    case B_HI:
        la_bltu(*temp_m, *temp_n, *label);
        break;
    case B_LS:
        la_bgeu(*temp_m, *temp_n, *label);
        break;
    case B_GE:
        la_bge(*temp_n, *temp_m, *label);
        break;
    case B_LT:
        la_blt(*temp_n, *temp_m, *label);
        break;
    case B_GT:
        la_blt(*temp_m, *temp_n, *label);
        break;
    case B_LE:
        la_bge(*temp_m, *temp_n, *label);
        break;
    case B_AL:
        la_b(*label);
        break;
    case B_NV:
        break;
    default:
        assert(0);
        break;
    }
}

static void trans_CMPI_BCOND(DisasContext *s, uint32_t insn, dt_aarch64_insn b_type, int offset)
{
    int imm = extract32(insn, 10, 12);
    uint8_t shift = extract32(insn, 22, 1);
    int sf = extract32(insn, 31, 1);
    int rn = extract32(insn, 5, 5);
    IR2_OPND reg_n = alloc_gpr_src_sp(rn);
    IR2_OPND temp = ra_alloc_itemp();
    IR2_OPND temp_n = ra_alloc_itemp();
    IR2_OPND label = ir2_opnd_new_type(IR2_OPND_LABEL);

    if(clearGprHigh && sf && arm_la_map[rn] >= 0 && rn != 31){
        clear_gpr_high(rn);
    }

    switch (shift)
    {
    case 0:
        la_ori(temp, zero_ir2_opnd, imm);
        break;
    case 1:
        la_lu12i_w(temp, imm);
        break;
    default:
        assert(0);
        break;
    }

    if (sf)
    {
        pattern_type(b_type, &reg_n, &temp, &label);
    }
    else
    {
        la_bstrpick_w(temp_n, reg_n, 31, 0);
        la_bstrpick_w(temp, temp, 31, 0);
        pattern_type(b_type, &temp_n, &temp, &label);
    }

    if (clearGprHigh) {
        for (int i = 0; i < 32; ++i) {
            if (arm_la_map[i] >= 0) {
                clear_gpr_high(i);
            }
        }
    }

    nzcv_caculate(temp_n, temp, reg_n, sf, 0);
    pattern_goto_tb(s, 0, 4);
    la_label(label);
    nzcv_caculate(temp_n, temp, reg_n, sf, 1);
    pattern_goto_tb(s, 1, offset);

    free_alloc_gpr(reg_n);
    free_alloc_gpr(temp);
    free_alloc_gpr(temp_n);
}

static void trans_CMPE_BCOND(DisasContext *s, uint32_t insn, dt_aarch64_insn b_type, int offset)
{
    int imm = extract32(insn, 10, 3);
    int extsize = extract32(insn, 13, 2);
    int sf = extract32(insn, 31, 1);
    bool is_signed = extract32(insn, 15, 1);
    int rm = extract32(insn, 16, 5);
    int rn = extract32(insn, 5, 5);
    IR2_OPND reg_m = alloc_gpr_src(rm);
    IR2_OPND reg_n = alloc_gpr_src(rn);
    IR2_OPND temp = ra_alloc_itemp();
    IR2_OPND temp_n = ra_alloc_itemp();
    IR2_OPND label = ir2_opnd_new_type(IR2_OPND_LABEL);

    if (clearGprHigh && sf && arm_la_map[rn] >= 0) {
        clear_gpr_high(rn);
    }
    if (clearGprHigh && sf && arm_la_map[rm] >= 0 && rm != 31) {
        clear_gpr_high(rm);
    }

    if (is_signed)
    {
        switch (extsize)
        {
        case 0:
            la_ext_w_b(temp, reg_m);
            break;
        case 1:
            la_ext_w_h(temp, reg_m);
            break;
        case 2:
            la_bstrpick_w(temp, reg_m, 31, 0);
            break;
        case 3:
            la_or(temp, reg_m, zero_ir2_opnd);
            break;
        }
    }
    else
    {
        switch (extsize)
        {
        case 0:
            la_bstrpick_d(temp, reg_m, 7, 0);
            break;
        case 1:
            la_bstrpick_d(temp, reg_m, 15, 0);
            break;
        case 2:
            la_bstrpick_d(temp, reg_m, 31, 0);
            break;
        case 3:
            la_or(temp, reg_m, zero_ir2_opnd);
            break;
        }
    }

    if (imm)
        la_slli_d(temp, temp, imm);

    if (sf)
    {
        pattern_type(b_type, &reg_n, &temp, &label);
    }
    else
    {
        la_bstrpick_w(temp_n, reg_n, 31, 0);
        la_bstrpick_w(temp, temp, 31, 0);
        pattern_type(b_type, &reg_n, &temp, &label);
    }

    if (clearGprHigh) {
        for (int i = 0; i < 32; ++i) {
            if (arm_la_map[i] >= 0) {
                clear_gpr_high(i);
            }
        }
    }

    nzcv_caculate(temp_n, temp, reg_n, sf, 0);
    pattern_goto_tb(s, 0, 4);
    la_label(label);
    nzcv_caculate(temp_n, temp, reg_n, sf, 1);
    pattern_goto_tb(s, 1, offset);

    free_alloc_gpr(reg_m);
    free_alloc_gpr(reg_n);
    free_alloc_gpr(temp);
    free_alloc_gpr(temp_n);
}

static void trans_CMPS_BCOND(DisasContext *s, uint32_t insn, dt_aarch64_insn b_type, int offset)
{
    int imm = extract32(insn, 10, 6);
    int shift_type = extract32(insn, 22, 2);
    int sf = extract32(insn, 31, 1);
    int rm = extract32(insn, 16, 5);
    int rn = extract32(insn, 5, 5);
    IR2_OPND reg_m = alloc_gpr_src(rm);
    IR2_OPND reg_n = alloc_gpr_src(rn);
    IR2_OPND temp = ra_alloc_itemp();
    IR2_OPND temp_n = ra_alloc_itemp();
    IR2_OPND label = ir2_opnd_new_type(IR2_OPND_LABEL);

    if (clearGprHigh && sf && arm_la_map[rn] >= 0 && rn != 31) {
        clear_gpr_high(rn);
    }
    if (clearGprHigh && sf && arm_la_map[rm] >= 0 && rm != 31) {
        clear_gpr_high(rm);
    }

    if (imm)
    {
        switch (shift_type)
        {
        case 0:
            la_slli_d(temp, reg_m, imm);
            break;
        case 1:
            if (!sf)
            {
                la_srli_w(temp, reg_m, imm);
            }
            else
            {
                la_srli_d(temp, reg_m, imm);
            }
            break;
        case 2:
            if (!sf)
            {
                la_srai_w(temp, reg_m, imm);
            }
            else
            {
                la_srai_d(temp, reg_m, imm);
            }
            break;
        case 3:
            if (!sf)
            {
                assert(imm <= 31);
                la_rotri_w(temp, reg_m, imm);
            }
            else
            {
                la_rotri_d(temp, reg_m, imm);
            }
            break;
        default:
            assert(0); /* all shift types should be handled */
            break;
        }

        if (sf)
        {
            pattern_type(b_type, &reg_n, &temp, &label);
        }
        else
        {
            la_bstrpick_w(temp_n, reg_n, 31, 0);
            la_bstrpick_w(temp, temp, 31, 0);
            pattern_type(b_type, &temp_n, &temp, &label);
        }
        if (clearGprHigh) {
        for (int i = 0; i < 32; ++i) {
            if (arm_la_map[i] >= 0) {
                clear_gpr_high(i);
            }
        }
    }
        nzcv_caculate(temp_n, temp, reg_n, sf, 0);
        pattern_goto_tb(s, 0, 4);

        la_label(label);
        nzcv_caculate(temp_n, temp, reg_n, sf, 1);
    }
    else
    {
        if (sf)
        {
            pattern_type(b_type, &reg_n, &reg_m, &label);
        }
        else
        {
            la_bstrpick_w(temp_n, reg_n, 31, 0);
            la_bstrpick_w(temp, reg_m, 31, 0);
            pattern_type(b_type, &temp_n, &temp, &label);
        }
        if (clearGprHigh) {
            for (int i = 0; i < 32; ++i) {
                if (arm_la_map[i] >= 0) {
                    clear_gpr_high(i);
                }
            }
        }
        nzcv_caculate(temp_n, reg_m, reg_n, sf, 0);
        pattern_goto_tb(s, 0, 4);

        la_label(label);
        nzcv_caculate(temp_n, reg_m, reg_n, sf, 1);
    }

    pattern_goto_tb(s, 1, offset);

    free_alloc_gpr(reg_n);
    free_alloc_gpr(temp);
    free_alloc_gpr(temp_n);
}

void nzcv_use(TranslationBlock *tb, uint32_t insn)
{
    const AArch64PatternTable *tptr = generate_nzcv_table;

    while (tptr->mask)
    {
        if ((insn & tptr->mask) == tptr->pattern)
        {
            tb->nzcv_use = false;
        }
        tptr++;
    }
}

bool insts_pattern(DisasContext *s, DisasContext *s2)
{
    uint32_t insn2 = s2->insn;
    uint32_t insn = s->insn;
    dt_aarch64_insn cmp_type = lookup_pattern(&cmp_table[0], insn);
    dt_aarch64_insn b_type = lookup_pattern(&bcond_table[0], insn2);
    
    if (b_type && cmp_type)
    {
        int offset = sextract32(insn2, 5, 19) << 2;
        // s->base->tb->nzcv_use = false;
        switch (cmp_type)
        {
        case CMP_IMM:
            trans_CMPI_BCOND(s, insn, b_type, offset);
            break;
        case CMP_EREG:
            trans_CMPE_BCOND(s, insn, b_type, offset);
            break;
        case CMP_SREG:
            trans_CMPS_BCOND(s, insn, b_type, offset);
            break;
        default:
            assert(0);
            break;
        }
        return true;
    }

    return false;
}

#endif