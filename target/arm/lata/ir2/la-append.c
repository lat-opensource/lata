/* this file is produced by convert.py */
#include "la-append.h"
#include "reg-alloc.h"
// #include "latx-options.h"
IR2_INST *la_ill(void) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ILL;
    pir2->op_count = 1;
    IR2_OPND ir2_opnd_none = ir2_opnd_new_none();
    pir2->_opnd[0] = ir2_opnd_none;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_label(IR2_OPND rd) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LABEL;
    pir2->op_count = 1;
    pir2->_opnd[0] = rd;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86_inst(IR2_OPND rd) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86_INST;
    pir2->op_count = 1;
    pir2->_opnd[0] = rd;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_mov64(IR2_OPND dest, IR2_OPND src) {
    if (ir2_opnd_cmp(&dest, &src)) {
        return 0;
    } else {
        return la_or(dest, src, zero_ir2_opnd);
    }
}

IR2_INST *la_mov32_sx(IR2_OPND dest, IR2_OPND src) {
    return la_slli_w(dest, src, 0);
}

IR2_INST *la_mov32_zx(IR2_OPND dest, IR2_OPND src) {
    return la_bstrpick_d(dest, src, 31, 0);
}

IR2_INST *la_clr_h32(IR2_OPND dest, IR2_OPND src) {
    return la_bstrpick_d(dest, src, 31, 0);
}

IR2_INST *la_add(IR2_OPND op0, IR2_OPND op1, IR2_OPND op2) {
#ifndef TARGET_X86_64
    return la_add_w(op0, op1, op2);
#else
    return la_add_d(op0, op1, op2);
#endif
}

IR2_INST *la_sub(IR2_OPND op0, IR2_OPND op1, IR2_OPND op2) {
#ifndef TARGET_X86_64
    return la_sub_w(op0, op1, op2);
#else
    return la_sub_d(op0, op1, op2);
#endif
}

IR2_INST *la_addi_addrx(IR2_OPND op0, IR2_OPND op1, int imm) {
#ifndef TARGET_X86_64
    return la_addi_w(op0, op1, imm);
#else
    return la_addi_d(op0, op1, imm);
#endif
}

IR2_INST *la_load_addrx(IR2_OPND op0, IR2_OPND op1, int imm) {
#ifndef TARGET_X86_64
    return la_ld_wu(op0, op1, imm);
#else
    return la_ld_d(op0, op1, imm);
#endif
}

IR2_INST *la_store_addrx(IR2_OPND op0, IR2_OPND op1, int imm) {
#ifndef TARGET_X86_64
    return la_st_w(op0, op1, imm);
#else
    return la_st_d(op0, op1, imm);
#endif
}

IR2_INST *la_gr2scr(IR2_OPND sd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_GR2SCR;
    pir2->op_count = 2;
    pir2->_opnd[0] = sd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_scr2gr(IR2_OPND rd, IR2_OPND sj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SCR2GR;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = sj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_clo_w(IR2_OPND rd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_CLO_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_clz_w(IR2_OPND rd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_CLZ_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_cto_w(IR2_OPND rd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_CTO_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ctz_w(IR2_OPND rd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_CTZ_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_clo_d(IR2_OPND rd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_CLO_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_clz_d(IR2_OPND rd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_CLZ_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_cto_d(IR2_OPND rd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_CTO_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ctz_d(IR2_OPND rd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_CTZ_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_revb_2h(IR2_OPND rd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_REVB_2H;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_revb_4h(IR2_OPND rd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_REVB_4H;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_revb_2w(IR2_OPND rd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_REVB_2W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_revb_d(IR2_OPND rd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_REVB_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_revh_2w(IR2_OPND rd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_REVH_2W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_revh_d(IR2_OPND rd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_REVH_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_bitrev_4b(IR2_OPND rd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_BITREV_4B;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_bitrev_8b(IR2_OPND rd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_BITREV_8B;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_bitrev_w(IR2_OPND rd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_BITREV_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_bitrev_d(IR2_OPND rd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_BITREV_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ext_w_h(IR2_OPND rd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_EXT_W_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ext_w_b(IR2_OPND rd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_EXT_W_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_rdtimel_w(IR2_OPND rd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_RDTIMEL_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_rdtimeh_w(IR2_OPND rd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_RDTIMEH_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_rdtime_d(IR2_OPND rd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_RDTIME_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_cpucfg(IR2_OPND rd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_CPUCFG;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86mttop(int imm_ptr) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86MTTOP;
    pir2->op_count = 1;
    IR2_OPND op0 = create_immh_opnd(imm_ptr);
    pir2->_opnd[0] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86mftop(IR2_OPND rd) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86MFTOP;
    pir2->op_count = 1;
    pir2->_opnd[0] = rd;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86loope(IR2_OPND rd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86LOOPE;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86loopne(IR2_OPND rd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86LOOPNE;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86inc_b(IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86INC_B;
    pir2->op_count = 1;
    pir2->_opnd[0] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86inc_h(IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86INC_H;
    pir2->op_count = 1;
    pir2->_opnd[0] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86inc_w(IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86INC_W;
    pir2->op_count = 1;
    pir2->_opnd[0] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86inc_d(IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86INC_D;
    pir2->op_count = 1;
    pir2->_opnd[0] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86dec_b(IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86DEC_B;
    pir2->op_count = 1;
    pir2->_opnd[0] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86dec_h(IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86DEC_H;
    pir2->op_count = 1;
    pir2->_opnd[0] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86dec_w(IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86DEC_W;
    pir2->op_count = 1;
    pir2->_opnd[0] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86dec_d(IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86DEC_D;
    pir2->op_count = 1;
    pir2->_opnd[0] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86settm(void) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SETTM;
    pir2->op_count = 1;
    IR2_OPND ir2_opnd_none = ir2_opnd_new_none();
    pir2->_opnd[0] = ir2_opnd_none;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86clrtm(void) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86CLRTM;
    pir2->op_count = 1;
    IR2_OPND ir2_opnd_none = ir2_opnd_new_none();
    pir2->_opnd[0] = ir2_opnd_none;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86inctop(void) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86INCTOP;
    pir2->op_count = 1;
    IR2_OPND ir2_opnd_none = ir2_opnd_new_none();
    pir2->_opnd[0] = ir2_opnd_none;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86dectop(void) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86DECTOP;
    pir2->op_count = 1;
    IR2_OPND ir2_opnd_none = ir2_opnd_new_none();
    pir2->_opnd[0] = ir2_opnd_none;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_asrtle_d(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ASRTLE_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_asrtgt_d(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ASRTGT_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_alsl_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk, int imm_sa2) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ALSL_W;
    pir2->op_count = 4;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    IR2_OPND op0 = create_immh_opnd(imm_sa2);
    pir2->_opnd[3] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_alsl_wu(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk, int imm_sa2) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ALSL_WU;
    pir2->op_count = 4;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    IR2_OPND op0 = create_immh_opnd(imm_sa2);
    pir2->_opnd[3] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_bytepick_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk, int imm_sa2) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_BYTEPICK_W;
    pir2->op_count = 4;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    IR2_OPND op0 = create_immh_opnd(imm_sa2);
    pir2->_opnd[3] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_bytepick_d(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk, int imm_sa3) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_BYTEPICK_D;
    pir2->op_count = 4;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    IR2_OPND op0 = create_immh_opnd(imm_sa3);
    pir2->_opnd[3] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_add_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ADD_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_add_d(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ADD_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_sub_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SUB_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_sub_d(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SUB_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_slt(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SLT;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_sltu(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SLTU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_maskeqz(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MASKEQZ;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_masknez(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MASKNEZ;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_nor(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_NOR;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_and(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AND;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_or(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_OR;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xor(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XOR;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_orn(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ORN;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_andn(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ANDN;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_sll_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SLL_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_srl_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SRL_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_sra_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SRA_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_sll_d(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SLL_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_srl_d(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SRL_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_sra_d(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SRA_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_rotr_b(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ROTR_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_rotr_h(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ROTR_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_rotr_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ROTR_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_rotr_d(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ROTR_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_mul_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MUL_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_mulh_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MULH_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_mulh_wu(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MULH_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_mul_d(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MUL_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_mulh_d(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MULH_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_mulh_du(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MULH_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_mulw_d_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MULW_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_mulw_d_wu(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MULW_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_div_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_DIV_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_mod_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MOD_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_div_wu(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_DIV_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_mod_wu(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MOD_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_div_d(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_DIV_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_mod_d(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MOD_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_div_du(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_DIV_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_mod_du(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MOD_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_crc_w_b_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_CRC_W_B_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_crc_w_h_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_CRC_W_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_crc_w_w_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_CRC_W_W_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_crc_w_d_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_CRC_W_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_crcc_w_b_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_CRCC_W_B_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_crcc_w_h_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_CRCC_W_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_crcc_w_w_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_CRCC_W_W_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_crcc_w_d_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_CRCC_W_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_addu12i_w(IR2_OPND rd, IR2_OPND rj, int imm_si5) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ADDU12I_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_addu12i_d(IR2_OPND rd, IR2_OPND rj, int imm_si5) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ADDU12I_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_break(int imm_code) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_BREAK;
    pir2->op_count = 1;
    IR2_OPND op0 = create_immh_opnd(imm_code);
    pir2->_opnd[0] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_dbgcall(int imm_code) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_DBGCALL;
    pir2->op_count = 1;
    IR2_OPND op0 = create_immh_opnd(imm_code);
    pir2->_opnd[0] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_syscall(int imm_code) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SYSCALL;
    pir2->op_count = 1;
    IR2_OPND op0 = create_immh_opnd(imm_code);
    pir2->_opnd[0] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_hypcall(int imm_code) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_HYPCALL;
    pir2->op_count = 1;
    IR2_OPND op0 = create_immh_opnd(imm_code);
    pir2->_opnd[0] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_alsl_d(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk, int imm_sa2) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ALSL_D;
    pir2->op_count = 4;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    IR2_OPND op0 = create_immh_opnd(imm_sa2);
    pir2->_opnd[3] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_adc_b(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ADC_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_adc_h(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ADC_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_adc_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ADC_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_adc_d(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ADC_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_sbc_b(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SBC_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_sbc_h(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SBC_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_sbc_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SBC_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_sbc_d(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SBC_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_rcr_b(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_RCR_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_rcr_h(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_RCR_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_rcr_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_RCR_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_rcr_d(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_RCR_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_armmove(IR2_OPND rd, IR2_OPND rj, int imm_condh) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ARMMOVE;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_condh);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_setx86j(IR2_OPND rd, int imm_condh) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SETX86J;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    IR2_OPND op0 = create_immh_opnd(imm_condh);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_setarmj(IR2_OPND rd, int imm_condh) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SETARMJ;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    IR2_OPND op0 = create_immh_opnd(imm_condh);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_armadd_w(IR2_OPND rj, IR2_OPND rk, int imm_condl) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ARMADD_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    IR2_OPND op0 = create_immh_opnd(imm_condl);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_armsub_w(IR2_OPND rj, IR2_OPND rk, int imm_condl) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ARMSUB_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    IR2_OPND op0 = create_immh_opnd(imm_condl);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_armadc_w(IR2_OPND rj, IR2_OPND rk, int imm_condl) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ARMADC_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    IR2_OPND op0 = create_immh_opnd(imm_condl);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_armsbc_w(IR2_OPND rj, IR2_OPND rk, int imm_condl) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ARMSBC_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    IR2_OPND op0 = create_immh_opnd(imm_condl);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_armand_w(IR2_OPND rj, IR2_OPND rk, int imm_condl) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ARMAND_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    IR2_OPND op0 = create_immh_opnd(imm_condl);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_armor_w(IR2_OPND rj, IR2_OPND rk, int imm_condl) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ARMOR_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    IR2_OPND op0 = create_immh_opnd(imm_condl);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_armxor_w(IR2_OPND rj, IR2_OPND rk, int imm_condl) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ARMXOR_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    IR2_OPND op0 = create_immh_opnd(imm_condl);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_armsll_w(IR2_OPND rj, IR2_OPND rk, int imm_condl) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ARMSLL_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    IR2_OPND op0 = create_immh_opnd(imm_condl);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_armsrl_w(IR2_OPND rj, IR2_OPND rk, int imm_condl) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ARMSRL_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    IR2_OPND op0 = create_immh_opnd(imm_condl);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_armsra_w(IR2_OPND rj, IR2_OPND rk, int imm_condl) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ARMSRA_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    IR2_OPND op0 = create_immh_opnd(imm_condl);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_armrotr_w(IR2_OPND rj, IR2_OPND rk, int imm_condl) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ARMROTR_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    IR2_OPND op0 = create_immh_opnd(imm_condl);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_armslli_w(IR2_OPND rj, int imm_ui5l, int imm_condl) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ARMSLLI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[1] = op0;
    IR2_OPND op1 = create_immh_opnd(imm_condl);
    pir2->_opnd[2] = op1;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_armsrli_w(IR2_OPND rj, int imm_ui5l, int imm_condl) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ARMSRLI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[1] = op0;
    IR2_OPND op1 = create_immh_opnd(imm_condl);
    pir2->_opnd[2] = op1;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_armsrai_w(IR2_OPND rj, int imm_ui5l, int imm_condl) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ARMSRAI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[1] = op0;
    IR2_OPND op1 = create_immh_opnd(imm_condl);
    pir2->_opnd[2] = op1;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_armrotri_w(IR2_OPND rj, int imm_ui5l, int imm_condl) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ARMROTRI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[1] = op0;
    IR2_OPND op1 = create_immh_opnd(imm_condl);
    pir2->_opnd[2] = op1;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86mul_b(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86MUL_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86mul_h(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86MUL_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86mul_w(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86MUL_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86mul_d(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86MUL_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86mul_bu(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86MUL_BU;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86mul_hu(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86MUL_HU;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86mul_wu(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86MUL_WU;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86mul_du(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86MUL_DU;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86add_wu(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86ADD_WU;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86add_du(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86ADD_DU;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86sub_wu(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SUB_WU;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86sub_du(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SUB_DU;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86add_b(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86ADD_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86add_h(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86ADD_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86add_w(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86ADD_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86add_d(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86ADD_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86sub_b(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SUB_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86sub_h(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SUB_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86sub_w(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SUB_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86sub_d(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SUB_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86adc_b(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86ADC_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86adc_h(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86ADC_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86adc_w(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86ADC_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86adc_d(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86ADC_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86sbc_b(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SBC_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86sbc_h(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SBC_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86sbc_w(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SBC_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86sbc_d(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SBC_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86sll_b(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SLL_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86sll_h(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SLL_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86sll_w(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SLL_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86sll_d(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SLL_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86srl_b(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SRL_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86srl_h(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SRL_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86srl_w(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SRL_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86srl_d(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SRL_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86sra_b(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SRA_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86sra_h(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SRA_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86sra_w(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SRA_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86sra_d(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SRA_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rotr_b(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86ROTR_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rotr_h(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86ROTR_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rotr_d(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86ROTR_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rotr_w(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86ROTR_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rotl_b(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86ROTL_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rotl_h(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86ROTL_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rotl_w(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86ROTL_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rotl_d(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86ROTL_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rcr_b(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86RCR_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rcr_h(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86RCR_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rcr_w(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86RCR_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rcr_d(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86RCR_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rcl_b(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86RCL_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rcl_h(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86RCL_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rcl_w(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86RCL_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rcl_d(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86RCL_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86and_b(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86AND_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86and_h(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86AND_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86and_w(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86AND_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86and_d(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86AND_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86or_b(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86OR_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86or_h(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86OR_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86or_w(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86OR_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86or_d(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86OR_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86xor_b(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86XOR_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86xor_h(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86XOR_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86xor_w(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86XOR_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86xor_d(IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86XOR_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_armnot_w(IR2_OPND rj, int imm_condh) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ARMNOT_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_condh);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_armmov_w(IR2_OPND rj, int imm_condh) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ARMMOV_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_condh);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_armmov_d(IR2_OPND rj, int imm_condh) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ARMMOV_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_condh);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_armrrx_w(IR2_OPND rj, int imm_condh) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ARMRRX_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_condh);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_slli_w(IR2_OPND rd, IR2_OPND rj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SLLI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_slli_d(IR2_OPND rd, IR2_OPND rj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SLLI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_srli_w(IR2_OPND rd, IR2_OPND rj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SRLI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_srli_d(IR2_OPND rd, IR2_OPND rj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SRLI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_srai_w(IR2_OPND rd, IR2_OPND rj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SRAI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_srai_d(IR2_OPND rd, IR2_OPND rj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SRAI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_rotri_b(IR2_OPND rd, IR2_OPND rj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ROTRI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_rotri_h(IR2_OPND rd, IR2_OPND rj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ROTRI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_rotri_w(IR2_OPND rd, IR2_OPND rj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ROTRI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_rotri_d(IR2_OPND rd, IR2_OPND rj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ROTRI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_rcri_b(IR2_OPND rd, IR2_OPND rj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_RCRI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_rcri_h(IR2_OPND rd, IR2_OPND rj, int imm_ui4) {
    assert(imm_ui4 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_RCRI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_rcri_w(IR2_OPND rd, IR2_OPND rj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_RCRI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_rcri_d(IR2_OPND rd, IR2_OPND rj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_RCRI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86slli_b(IR2_OPND rj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SLLI_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86slli_h(IR2_OPND rj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SLLI_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86slli_w(IR2_OPND rj, int imm_ui5l) {
    assert(imm_ui5l <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SLLI_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86slli_d(IR2_OPND rj, int imm_ui6) {
    assert(imm_ui6 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SLLI_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86srli_b(IR2_OPND rj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SRLI_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86srli_h(IR2_OPND rj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SRLI_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86srli_w(IR2_OPND rj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SRLI_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86srli_d(IR2_OPND rj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SRLI_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86srai_b(IR2_OPND rj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SRAI_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86srai_h(IR2_OPND rj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SRAI_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86srai_w(IR2_OPND rj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SRAI_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86srai_d(IR2_OPND rj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SRAI_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rotri_b(IR2_OPND rj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86ROTRI_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rotri_h(IR2_OPND rj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86ROTRI_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rotri_w(IR2_OPND rj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86ROTRI_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rotri_d(IR2_OPND rj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86ROTRI_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rcri_b(IR2_OPND rj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86RCRI_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rcri_h(IR2_OPND rj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86RCRI_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rcri_w(IR2_OPND rj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86RCRI_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rcri_d(IR2_OPND rj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86RCRI_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rotli_b(IR2_OPND rj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86ROTLI_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rotli_h(IR2_OPND rj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86ROTLI_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rotli_w(IR2_OPND rj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86ROTLI_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rotli_d(IR2_OPND rj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86ROTLI_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rcli_b(IR2_OPND rj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86RCLI_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rcli_h(IR2_OPND rj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86RCLI_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rcli_w(IR2_OPND rj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86RCLI_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86rcli_d(IR2_OPND rj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86RCLI_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86settag(IR2_OPND rd, int imm_opx86, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86SETTAG;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    IR2_OPND op0 = create_immh_opnd(imm_opx86);
    pir2->_opnd[1] = op0;
    IR2_OPND op1 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op1;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86mfflag(IR2_OPND rd, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86MFFLAG;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_x86mtflag(IR2_OPND rd, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_X86MTFLAG;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_armmfflag(IR2_OPND rd, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ARMMFFLAG;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_armmtflag(IR2_OPND rd, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ARMMTFLAG;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_bstrins_w(IR2_OPND rd, IR2_OPND rj, int imm_msbw, int imm_lsbw) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_BSTRINS_W;
    pir2->op_count = 4;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_msbw);
    pir2->_opnd[2] = op0;
    IR2_OPND op1 = create_immh_opnd(imm_lsbw);
    pir2->_opnd[3] = op1;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_bstrpick_w(IR2_OPND rd, IR2_OPND rj, int imm_msbw, int imm_lsbw) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_BSTRPICK_W;
    pir2->op_count = 4;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_msbw);
    pir2->_opnd[2] = op0;
    IR2_OPND op1 = create_immh_opnd(imm_lsbw);
    pir2->_opnd[3] = op1;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_bstrins_d(IR2_OPND rd, IR2_OPND rj, int imm_msbd, int imm_lsbd) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_BSTRINS_D;
    pir2->op_count = 4;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_msbd);
    pir2->_opnd[2] = op0;
    IR2_OPND op1 = create_immh_opnd(imm_lsbd);
    pir2->_opnd[3] = op1;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_bstrpick_d(IR2_OPND rd, IR2_OPND rj, int imm_msbd, int imm_lsbd) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_BSTRPICK_D;
    pir2->op_count = 4;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_msbd);
    pir2->_opnd[2] = op0;
    IR2_OPND op1 = create_immh_opnd(imm_lsbd);
    pir2->_opnd[3] = op1;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_slti(IR2_OPND rd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SLTI;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_sltui(IR2_OPND rd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SLTUI;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_addi_w(IR2_OPND rd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ADDI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_addi_d(IR2_OPND rd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ADDI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_lu52i_d(IR2_OPND rd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LU52I_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_andi(IR2_OPND rd, IR2_OPND rj, int imm_ui12) {
    assert(imm_ui12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ANDI;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ori(IR2_OPND rd, IR2_OPND rj, int imm_ui12) {
    assert(imm_ui12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ORI;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xori(IR2_OPND rd, IR2_OPND rj, int imm_ui12) {
    assert(imm_ui12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XORI;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_addu16i_d(IR2_OPND rd, IR2_OPND rj, int imm_si16) {
    assert(imm_si16 <= 0xffff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ADDU16I_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si16);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_lu12i_w(IR2_OPND rd, int imm_si20) {
    assert(imm_si20 <= 0xfffff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LU12I_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    IR2_OPND op0 = create_immh_opnd(imm_si20);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_lu32i_d(IR2_OPND rd, int imm_si20) {
    assert(imm_si20 <= 0xfffff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LU32I_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    IR2_OPND op0 = create_immh_opnd(imm_si20);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_pcaddi(IR2_OPND rd, int imm_si20) {
    assert(imm_si20 <= 0xfffff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_PCADDI;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    IR2_OPND op0 = create_immh_opnd(imm_si20);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_pcalau12i(IR2_OPND rd, int imm_si20) {
    assert(imm_si20 <= 0xfffff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_PCALAU12I;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    IR2_OPND op0 = create_immh_opnd(imm_si20);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_pcaddu12i(IR2_OPND rd, int imm_si20) {
    assert(imm_si20 <= 0xfffff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_PCADDU12I;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    IR2_OPND op0 = create_immh_opnd(imm_si20);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_pcaddu18i(IR2_OPND rd, int imm_si20) {
    assert(imm_si20 <= 0xfffff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_PCADDU18I;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    IR2_OPND op0 = create_immh_opnd(imm_si20);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ll_w(IR2_OPND rd, IR2_OPND rj, int imm_si14) {
    assert(imm_si14 <= 0x3fff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LL_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    imm_si14 = imm_si14 >> 2;
    IR2_OPND op0 = create_immh_opnd(imm_si14);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_sc_w(IR2_OPND rd, IR2_OPND rj, int imm_si14) {
    assert(imm_si14 <= 0x3fff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SC_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    imm_si14 = imm_si14 >> 2;
    IR2_OPND op0 = create_immh_opnd(imm_si14);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ll_d(IR2_OPND rd, IR2_OPND rj, int imm_si14) {
    assert(imm_si14 <= 0x3fff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LL_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    imm_si14 = imm_si14 >> 2;
    IR2_OPND op0 = create_immh_opnd(imm_si14);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_sc_d(IR2_OPND rd, IR2_OPND rj, int imm_si14) {
    assert(imm_si14 <= 0x3fff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SC_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    imm_si14 = imm_si14 >> 2;
    IR2_OPND op0 = create_immh_opnd(imm_si14);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ldptr_w(IR2_OPND rd, IR2_OPND rj, int imm_si14) {
    assert(imm_si14 <= 0x3fff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LDPTR_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    imm_si14 = imm_si14 >> 2;
    IR2_OPND op0 = create_immh_opnd(imm_si14);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_stptr_w(IR2_OPND rd, IR2_OPND rj, int imm_si14) {
    assert(imm_si14 <= 0x3fff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_STPTR_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    imm_si14 = imm_si14 >> 2;
    IR2_OPND op0 = create_immh_opnd(imm_si14);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ldptr_d(IR2_OPND rd, IR2_OPND rj, int imm_si14) {
    assert(imm_si14 <= 0x3fff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LDPTR_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    imm_si14 = imm_si14 >> 2;
    IR2_OPND op0 = create_immh_opnd(imm_si14);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_stptr_d(IR2_OPND rd, IR2_OPND rj, int imm_si14) {
    assert(imm_si14 <= 0x3fff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_STPTR_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    imm_si14 = imm_si14 >> 2;
    IR2_OPND op0 = create_immh_opnd(imm_si14);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ld_b(IR2_OPND rd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LD_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ld_h(IR2_OPND rd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LD_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ld_w(IR2_OPND rd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LD_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ld_d(IR2_OPND rd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LD_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_st_b(IR2_OPND rd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ST_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_st_h(IR2_OPND rd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ST_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_st_w(IR2_OPND rd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ST_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_st_d(IR2_OPND rd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_ST_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ld_bu(IR2_OPND rd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LD_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ld_hu(IR2_OPND rd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LD_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ld_wu(IR2_OPND rd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LD_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_preld(int imm_hints, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_PRELD;
    pir2->op_count = 3;
    IR2_OPND op0 = create_immh_opnd(imm_hints);
    pir2->_opnd[0] = op0;
    pir2->_opnd[1] = rj;
    IR2_OPND op1 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op1;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fld_s(IR2_OPND fd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FLD_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fst_s(IR2_OPND fd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FST_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fld_d(IR2_OPND fd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FLD_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fst_d(IR2_OPND fd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FST_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vld(IR2_OPND vd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VLD;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vst(IR2_OPND vd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VST;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvld(IR2_OPND xd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVLD;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvst(IR2_OPND xd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVST;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ldl_w(IR2_OPND rd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LDL_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ldr_w(IR2_OPND rd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LDR_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ldl_d(IR2_OPND rd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LDL_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ldr_d(IR2_OPND rd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LDR_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_stl_w(IR2_OPND rd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_STL_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_str_w(IR2_OPND rd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_STR_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_stl_d(IR2_OPND rd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_STL_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_str_d(IR2_OPND rd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_STR_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vldrepl_d(IR2_OPND vd, IR2_OPND rj, int imm_si9) {
    assert(imm_si9 <= 0x1ff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VLDREPL_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si9);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vldrepl_w(IR2_OPND vd, IR2_OPND rj, int imm_si10) {
    assert(imm_si10 <= 0x3ff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VLDREPL_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si10);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vldrepl_h(IR2_OPND vd, IR2_OPND rj, int imm_si11) {
    assert(imm_si11 <= 0x7ff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VLDREPL_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si11);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vldrepl_b(IR2_OPND vd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VLDREPL_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vstelm_d(IR2_OPND vd, IR2_OPND rj, int imm_si8, int imm_idxs) {
    assert(imm_si8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSTELM_D;
    pir2->op_count = 4;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si8);
    pir2->_opnd[2] = op0;
    IR2_OPND op1 = create_immh_opnd(imm_idxs);
    pir2->_opnd[3] = op1;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vstelm_w(IR2_OPND vd, IR2_OPND rj, int imm_si8, int imm_idxm) {
    assert(imm_si8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSTELM_W;
    pir2->op_count = 4;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si8);
    pir2->_opnd[2] = op0;
    IR2_OPND op1 = create_immh_opnd(imm_idxm);
    pir2->_opnd[3] = op1;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vstelm_h(IR2_OPND vd, IR2_OPND rj, int imm_si8, int imm_idxl) {
    assert(imm_si8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSTELM_H;
    pir2->op_count = 4;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si8);
    pir2->_opnd[2] = op0;
    IR2_OPND op1 = create_immh_opnd(imm_idxl);
    pir2->_opnd[3] = op1;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vstelm_b(IR2_OPND vd, IR2_OPND rj, int imm_si8, int imm_idxll) {
    assert(imm_si8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSTELM_B;
    pir2->op_count = 4;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si8);
    pir2->_opnd[2] = op0;
    IR2_OPND op1 = create_immh_opnd(imm_idxll);
    pir2->_opnd[3] = op1;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvldrepl_d(IR2_OPND xd, IR2_OPND rj, int imm_si9) {
    assert(imm_si9 <= 0x1ff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVLDREPL_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si9);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvldrepl_w(IR2_OPND xd, IR2_OPND rj, int imm_si10) {
    assert(imm_si10 <= 0x3ff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVLDREPL_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si10);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvldrepl_h(IR2_OPND xd, IR2_OPND rj, int imm_si11) {
    assert(imm_si11 <= 0x7ff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVLDREPL_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si11);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvldrepl_b(IR2_OPND xd, IR2_OPND rj, int imm_si12) {
    assert(imm_si12 <= 0xfff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVLDREPL_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si12);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvstelm_d(IR2_OPND xd, IR2_OPND rj, int imm_si8, int imm_idxs) {
    assert(imm_si8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSTELM_D;
    pir2->op_count = 4;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si8);
    pir2->_opnd[2] = op0;
    IR2_OPND op1 = create_immh_opnd(imm_idxs);
    pir2->_opnd[3] = op1;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvstelm_w(IR2_OPND xd, IR2_OPND rj, int imm_si8, int imm_idxm) {
    assert(imm_si8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSTELM_W;
    pir2->op_count = 4;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si8);
    pir2->_opnd[2] = op0;
    IR2_OPND op1 = create_immh_opnd(imm_idxm);
    pir2->_opnd[3] = op1;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvstelm_h(IR2_OPND xd, IR2_OPND rj, int imm_si8, int imm_idxl) {
    assert(imm_si8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSTELM_H;
    pir2->op_count = 4;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si8);
    pir2->_opnd[2] = op0;
    IR2_OPND op1 = create_immh_opnd(imm_idxl);
    pir2->_opnd[3] = op1;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvstelm_b(IR2_OPND xd, IR2_OPND rj, int imm_si8, int imm_idxll) {
    assert(imm_si8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSTELM_B;
    pir2->op_count = 4;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_si8);
    pir2->_opnd[2] = op0;
    IR2_OPND op1 = create_immh_opnd(imm_idxll);
    pir2->_opnd[3] = op1;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ldx_b(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LDX_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ldx_h(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LDX_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ldx_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LDX_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ldx_d(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LDX_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_stx_b(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_STX_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_stx_h(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_STX_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_stx_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_STX_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_stx_d(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_STX_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ldx_bu(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LDX_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ldx_hu(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LDX_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ldx_wu(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LDX_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_preldx(int imm_hints, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_PRELDX;
    pir2->op_count = 3;
    IR2_OPND op0 = create_immh_opnd(imm_hints);
    pir2->_opnd[0] = op0;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fldx_s(IR2_OPND fd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FLDX_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fldx_d(IR2_OPND fd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FLDX_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fstx_s(IR2_OPND fd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FSTX_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fstx_d(IR2_OPND fd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FSTX_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vldx(IR2_OPND vd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VLDX;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vstx(IR2_OPND vd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSTX;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvldx(IR2_OPND xd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVLDX;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvstx(IR2_OPND xd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSTX;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_amswap_w(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMSWAP_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_amswap_d(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMSWAP_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_amadd_w(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMADD_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_amadd_d(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMADD_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_amand_w(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMAND_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_amand_d(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMAND_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_amor_w(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMOR_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_amor_d(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMOR_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_amxor_w(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMXOR_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_amxor_d(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMXOR_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ammax_w(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMMAX_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ammax_d(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMMAX_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ammin_w(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMMIN_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ammin_d(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMMIN_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ammax_wu(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMMAX_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ammax_du(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMMAX_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ammin_wu(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMMIN_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ammin_du(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMMIN_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_amswap_db_w(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMSWAP_DB_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_amswap_db_d(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMSWAP_DB_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_amadd_db_w(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMADD_DB_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_amadd_db_d(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMADD_DB_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_amand_db_w(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMAND_DB_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_amand_db_d(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMAND_DB_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_amor_db_w(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMOR_DB_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_amor_db_d(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMOR_DB_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_amxor_db_w(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMXOR_DB_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_amxor_db_d(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMXOR_DB_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ammax_db_w(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMMAX_DB_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ammax_db_d(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMMAX_DB_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ammin_db_w(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMMIN_DB_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ammin_db_d(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMMIN_DB_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ammax_db_wu(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMMAX_DB_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ammax_db_du(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMMAX_DB_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ammin_db_wu(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMMIN_DB_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ammin_db_du(IR2_OPND rd, IR2_OPND rk, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AMMIN_DB_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rk;
    pir2->_opnd[2] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_dbar(int imm_hintl) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_DBAR;
    pir2->op_count = 1;
    IR2_OPND op0 = create_immh_opnd(imm_hintl);
    pir2->_opnd[0] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ibar(int imm_hintl) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_IBAR;
    pir2->op_count = 1;
    IR2_OPND op0 = create_immh_opnd(imm_hintl);
    pir2->_opnd[0] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fldgt_s(IR2_OPND fd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FLDGT_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fldgt_d(IR2_OPND fd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FLDGT_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fldle_s(IR2_OPND fd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FLDLE_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fldle_d(IR2_OPND fd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FLDLE_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fstgt_s(IR2_OPND fd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FSTGT_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fstgt_d(IR2_OPND fd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FSTGT_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fstle_s(IR2_OPND fd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FSTLE_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fstle_d(IR2_OPND fd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FSTLE_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ldgt_b(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LDGT_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ldgt_h(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LDGT_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ldgt_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LDGT_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ldgt_d(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LDGT_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ldle_b(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LDLE_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ldle_h(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LDLE_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ldle_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LDLE_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ldle_d(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_LDLE_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_stgt_b(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_STGT_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_stgt_h(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_STGT_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_stgt_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_STGT_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_stgt_d(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_STGT_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_stle_b(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_STLE_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_stle_h(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_STLE_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_stle_w(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_STLE_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_stle_d(IR2_OPND rd, IR2_OPND rj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_STLE_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_beqz(IR2_OPND rj, IR2_OPND offl) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_BEQZ;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = offl;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_bnez(IR2_OPND rj, IR2_OPND offl) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_BNEZ;
    pir2->op_count = 2;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = offl;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_bceqz(IR2_OPND cj, IR2_OPND offl) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_BCEQZ;
    pir2->op_count = 2;
    pir2->_opnd[0] = cj;
    pir2->_opnd[1] = offl;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_bcnez(IR2_OPND cj, IR2_OPND offl) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_BCNEZ;
    pir2->op_count = 2;
    pir2->_opnd[0] = cj;
    pir2->_opnd[1] = offl;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_jiscr0(int imm_offl) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_JISCR0;
    pir2->op_count = 1;
    IR2_OPND op0 = create_immh_opnd(imm_offl);
    pir2->_opnd[0] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_jiscr1(int imm_offl) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_JISCR1;
    pir2->op_count = 1;
    IR2_OPND op0 = create_immh_opnd(imm_offl);
    pir2->_opnd[0] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_jirl(IR2_OPND rd, IR2_OPND rj, int imm_offs) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_JIRL;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_offs);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_b(IR2_OPND offll) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_B;
    pir2->op_count = 1;
    pir2->_opnd[0] = offll;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_bl(IR2_OPND offll) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_BL;
    pir2->op_count = 1;
    pir2->_opnd[0] = offll;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_beq(IR2_OPND rj, IR2_OPND rd, IR2_OPND offs) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_BEQ;
    pir2->op_count = 3;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rd;
    pir2->_opnd[2] = offs;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_bne(IR2_OPND rj, IR2_OPND rd, IR2_OPND offs) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_BNE;
    pir2->op_count = 3;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rd;
    pir2->_opnd[2] = offs;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_blt(IR2_OPND rj, IR2_OPND rd, IR2_OPND offs) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_BLT;
    pir2->op_count = 3;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rd;
    pir2->_opnd[2] = offs;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_bge(IR2_OPND rj, IR2_OPND rd, IR2_OPND offs) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_BGE;
    pir2->op_count = 3;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rd;
    pir2->_opnd[2] = offs;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_bltu(IR2_OPND rj, IR2_OPND rd, IR2_OPND offs) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_BLTU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rd;
    pir2->_opnd[2] = offs;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_bgeu(IR2_OPND rj, IR2_OPND rd, IR2_OPND offs) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_BGEU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rj;
    pir2->_opnd[1] = rd;
    pir2->_opnd[2] = offs;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fadd_s(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FADD_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fadd_d(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FADD_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fsub_s(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FSUB_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fsub_d(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FSUB_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fmul_s(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FMUL_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fmul_d(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FMUL_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fdiv_s(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FDIV_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fdiv_d(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FDIV_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fmax_s(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FMAX_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fmax_d(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FMAX_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fmin_s(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FMIN_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fmin_d(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FMIN_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fmaxa_s(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FMAXA_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fmaxa_d(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FMAXA_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fmina_s(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FMINA_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fmina_d(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FMINA_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fscaleb_s(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FSCALEB_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fscaleb_d(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FSCALEB_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fcopysign_s(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FCOPYSIGN_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fcopysign_d(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FCOPYSIGN_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fabs_s(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FABS_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fabs_d(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FABS_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fneg_s(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FNEG_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fneg_d(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FNEG_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_flogb_s(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FLOGB_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_flogb_d(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FLOGB_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fclass_s(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FCLASS_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fclass_d(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FCLASS_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fsqrt_s(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FSQRT_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fsqrt_d(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FSQRT_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_frecip_s(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FRECIP_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_frecip_d(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FRECIP_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_frsqrt_s(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FRSQRT_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_frsqrt_d(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FRSQRT_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fmov_s(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FMOV_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fmov_d(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FMOV_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_movgr2fr_w(IR2_OPND fd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MOVGR2FR_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_movgr2fr_d(IR2_OPND fd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MOVGR2FR_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_movgr2frh_w(IR2_OPND fd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MOVGR2FRH_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_movfr2gr_s(IR2_OPND rd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MOVFR2GR_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_movfr2gr_d(IR2_OPND rd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MOVFR2GR_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_movfrh2gr_s(IR2_OPND rd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MOVFRH2GR_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_movgr2fcsr(IR2_OPND fcsrl, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MOVGR2FCSR;
    pir2->op_count = 2;
    pir2->_opnd[0] = fcsrl;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_movfcsr2gr(IR2_OPND rd, IR2_OPND fcsrh) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MOVFCSR2GR;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = fcsrh;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_movfr2cf(IR2_OPND cd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MOVFR2CF;
    pir2->op_count = 2;
    pir2->_opnd[0] = cd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_movcf2fr(IR2_OPND fd, IR2_OPND cj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MOVCF2FR;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = cj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_movgr2cf(IR2_OPND cd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MOVGR2CF;
    pir2->op_count = 2;
    pir2->_opnd[0] = cd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_movcf2gr(IR2_OPND rd, IR2_OPND cj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MOVCF2GR;
    pir2->op_count = 2;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = cj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fcvt_ld_d(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FCVT_LD_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fcvt_ud_d(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FCVT_UD_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fcvt_d_ld(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FCVT_D_LD;
    pir2->op_count = 3;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fcvt_s_d(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FCVT_S_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fcvt_d_s(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FCVT_D_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ftintrm_w_s(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FTINTRM_W_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ftintrm_w_d(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FTINTRM_W_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ftintrm_l_s(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FTINTRM_L_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ftintrm_l_d(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FTINTRM_L_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ftintrp_w_s(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FTINTRP_W_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ftintrp_w_d(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FTINTRP_W_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ftintrp_l_s(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FTINTRP_L_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ftintrp_l_d(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FTINTRP_L_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ftintrz_w_s(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FTINTRZ_W_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ftintrz_w_d(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FTINTRZ_W_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ftintrz_l_s(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FTINTRZ_L_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ftintrz_l_d(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FTINTRZ_L_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ftintrne_w_s(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FTINTRNE_W_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ftintrne_w_d(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FTINTRNE_W_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ftintrne_l_s(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FTINTRNE_L_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ftintrne_l_d(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FTINTRNE_L_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ftint_w_s(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FTINT_W_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ftint_w_d(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FTINT_W_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ftint_l_s(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FTINT_L_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ftint_l_d(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FTINT_L_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ffint_s_w(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FFINT_S_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ffint_s_l(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FFINT_S_L;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ffint_d_w(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FFINT_D_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_ffint_d_l(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FFINT_D_L;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_frint_s(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FRINT_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_frint_d(IR2_OPND fd, IR2_OPND fj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FRINT_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fmadd_s(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk, IR2_OPND fa) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FMADD_S;
    pir2->op_count = 4;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    pir2->_opnd[3] = fa;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fmadd_d(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk, IR2_OPND fa) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FMADD_D;
    pir2->op_count = 4;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    pir2->_opnd[3] = fa;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fmsub_s(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk, IR2_OPND fa) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FMSUB_S;
    pir2->op_count = 4;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    pir2->_opnd[3] = fa;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fmsub_d(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk, IR2_OPND fa) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FMSUB_D;
    pir2->op_count = 4;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    pir2->_opnd[3] = fa;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fnmadd_s(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk, IR2_OPND fa) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FNMADD_S;
    pir2->op_count = 4;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    pir2->_opnd[3] = fa;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fnmadd_d(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk, IR2_OPND fa) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FNMADD_D;
    pir2->op_count = 4;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    pir2->_opnd[3] = fa;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fnmsub_s(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk, IR2_OPND fa) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FNMSUB_S;
    pir2->op_count = 4;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    pir2->_opnd[3] = fa;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fnmsub_d(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk, IR2_OPND fa) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FNMSUB_D;
    pir2->op_count = 4;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    pir2->_opnd[3] = fa;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfmadd_s(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk, IR2_OPND va) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFMADD_S;
    pir2->op_count = 4;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    pir2->_opnd[3] = va;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfmadd_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk, IR2_OPND va) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFMADD_D;
    pir2->op_count = 4;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    pir2->_opnd[3] = va;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfmsub_s(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk, IR2_OPND va) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFMSUB_S;
    pir2->op_count = 4;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    pir2->_opnd[3] = va;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfmsub_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk, IR2_OPND va) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFMSUB_D;
    pir2->op_count = 4;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    pir2->_opnd[3] = va;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfnmadd_s(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk, IR2_OPND va) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFNMADD_S;
    pir2->op_count = 4;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    pir2->_opnd[3] = va;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfnmadd_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk, IR2_OPND va) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFNMADD_D;
    pir2->op_count = 4;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    pir2->_opnd[3] = va;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfnmsub_s(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk, IR2_OPND va) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFNMSUB_S;
    pir2->op_count = 4;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    pir2->_opnd[3] = va;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfnmsub_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk, IR2_OPND va) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFNMSUB_D;
    pir2->op_count = 4;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    pir2->_opnd[3] = va;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfmadd_s(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk, IR2_OPND xa) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFMADD_S;
    pir2->op_count = 4;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    pir2->_opnd[3] = xa;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfmadd_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk, IR2_OPND xa) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFMADD_D;
    pir2->op_count = 4;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    pir2->_opnd[3] = xa;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfmsub_s(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk, IR2_OPND xa) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFMSUB_S;
    pir2->op_count = 4;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    pir2->_opnd[3] = xa;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfmsub_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk, IR2_OPND xa) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFMSUB_D;
    pir2->op_count = 4;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    pir2->_opnd[3] = xa;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfnmadd_s(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk, IR2_OPND xa) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFNMADD_S;
    pir2->op_count = 4;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    pir2->_opnd[3] = xa;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfnmadd_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk, IR2_OPND xa) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFNMADD_D;
    pir2->op_count = 4;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    pir2->_opnd[3] = xa;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfnmsub_s(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk, IR2_OPND xa) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFNMSUB_S;
    pir2->op_count = 4;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    pir2->_opnd[3] = xa;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfnmsub_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk, IR2_OPND xa) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFNMSUB_D;
    pir2->op_count = 4;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    pir2->_opnd[3] = xa;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fcmp_cond_s(IR2_OPND cd, IR2_OPND fj, IR2_OPND fk, int imm_condf) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FCMP_COND_S;
    pir2->op_count = 4;
    pir2->_opnd[0] = cd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    IR2_OPND op0 = create_immh_opnd(imm_condf);
    pir2->_opnd[3] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fcmp_cond_d(IR2_OPND cd, IR2_OPND fj, IR2_OPND fk, int imm_condf) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FCMP_COND_D;
    pir2->op_count = 4;
    pir2->_opnd[0] = cd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    IR2_OPND op0 = create_immh_opnd(imm_condf);
    pir2->_opnd[3] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfcmp_cond_s(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk, int imm_condf) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFCMP_COND_S;
    pir2->op_count = 4;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    IR2_OPND op0 = create_immh_opnd(imm_condf);
    pir2->_opnd[3] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfcmp_cond_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk, int imm_condf) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFCMP_COND_D;
    pir2->op_count = 4;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    IR2_OPND op0 = create_immh_opnd(imm_condf);
    pir2->_opnd[3] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfcmp_cond_s(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk, int imm_condf) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFCMP_COND_S;
    pir2->op_count = 4;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    IR2_OPND op0 = create_immh_opnd(imm_condf);
    pir2->_opnd[3] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfcmp_cond_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk, int imm_condf) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFCMP_COND_D;
    pir2->op_count = 4;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    IR2_OPND op0 = create_immh_opnd(imm_condf);
    pir2->_opnd[3] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_fsel(IR2_OPND fd, IR2_OPND fj, IR2_OPND fk, IR2_OPND ca) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_FSEL;
    pir2->op_count = 4;
    pir2->_opnd[0] = fd;
    pir2->_opnd[1] = fj;
    pir2->_opnd[2] = fk;
    pir2->_opnd[3] = ca;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitsel_v(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk, IR2_OPND va) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITSEL_V;
    pir2->op_count = 4;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    pir2->_opnd[3] = va;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitsel_v(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk, IR2_OPND xa) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITSEL_V;
    pir2->op_count = 4;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    pir2->_opnd[3] = xa;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vshuf_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk, IR2_OPND va) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSHUF_B;
    pir2->op_count = 4;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    pir2->_opnd[3] = va;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvshuf_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk, IR2_OPND xa) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSHUF_B;
    pir2->op_count = 4;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    pir2->_opnd[3] = xa;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vextr_v(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk, int imm_ui5h) {
    assert(imm_ui5h <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTR_V;
    pir2->op_count = 4;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    IR2_OPND op0 = create_immh_opnd(imm_ui5h);
    pir2->_opnd[3] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvextr_v(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk, int imm_ui5h) {
    assert(imm_ui5h <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTR_V;
    pir2->op_count = 4;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    IR2_OPND op0 = create_immh_opnd(imm_ui5h);
    pir2->_opnd[3] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfmaddsub_s(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk, IR2_OPND va) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFMADDSUB_S;
    pir2->op_count = 4;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    pir2->_opnd[3] = va;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfmaddsub_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk, IR2_OPND va) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFMADDSUB_D;
    pir2->op_count = 4;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    pir2->_opnd[3] = va;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfmsubadd_s(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk, IR2_OPND va) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFMSUBADD_S;
    pir2->op_count = 4;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    pir2->_opnd[3] = va;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfmsubadd_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk, IR2_OPND va) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFMSUBADD_D;
    pir2->op_count = 4;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    pir2->_opnd[3] = va;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfmaddsub_s(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk, IR2_OPND xa) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFMADDSUB_S;
    pir2->op_count = 4;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    pir2->_opnd[3] = xa;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfmaddsub_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk, IR2_OPND xa) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFMADDSUB_D;
    pir2->op_count = 4;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    pir2->_opnd[3] = xa;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfmsubadd_s(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk, IR2_OPND xa) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFMSUBADD_S;
    pir2->op_count = 4;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    pir2->_opnd[3] = xa;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfmsubadd_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk, IR2_OPND xa) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFMSUBADD_D;
    pir2->op_count = 4;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    pir2->_opnd[3] = xa;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vseq_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSEQ_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vseq_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSEQ_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vseq_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSEQ_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vseq_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSEQ_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsle_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLE_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsle_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLE_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsle_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLE_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsle_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLE_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsle_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLE_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsle_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLE_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsle_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLE_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsle_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLE_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslt_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLT_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslt_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLT_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslt_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLT_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslt_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLT_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslt_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLT_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslt_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLT_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslt_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLT_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslt_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLT_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vadd_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADD_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vadd_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADD_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vadd_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADD_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vadd_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADD_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsub_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUB_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsub_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUB_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsub_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUB_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsub_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUB_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddw_h_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDW_H_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddw_w_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDW_W_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddw_d_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDW_D_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddw_h_h_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDW_H_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddw_w_w_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDW_W_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddw_d_d_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDW_D_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubw_h_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBW_H_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubw_w_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBW_W_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubw_d_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBW_D_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubw_h_h_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBW_H_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubw_w_w_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBW_W_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubw_d_d_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBW_D_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsaddw_h_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSADDW_H_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsaddw_w_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSADDW_W_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsaddw_d_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSADDW_D_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsaddw_hu_hu_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSADDW_HU_HU_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsaddw_wu_wu_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSADDW_WU_WU_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsaddw_du_du_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSADDW_DU_DU_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssubw_h_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSUBW_H_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssubw_w_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSUBW_W_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssubw_d_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSUBW_D_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssubw_hu_hu_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSUBW_HU_HU_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssubw_wu_wu_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSUBW_WU_WU_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssubw_du_du_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSUBW_DU_DU_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwev_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWEV_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwev_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWEV_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwev_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWEV_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwev_q_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWEV_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwev_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWEV_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwev_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWEV_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwev_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWEV_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwev_q_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWEV_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwod_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWOD_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwod_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWOD_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwod_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWOD_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwod_q_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWOD_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwod_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWOD_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwod_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWOD_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwod_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWOD_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwod_q_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWOD_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwl_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWL_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwl_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWL_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwl_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWL_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwl_q_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWL_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwl_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWL_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwl_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWL_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwl_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWL_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwl_q_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWL_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwh_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWH_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwh_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWH_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwh_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWH_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwh_q_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWH_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwh_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWH_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwh_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWH_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwh_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWH_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwh_q_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWH_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwev_h_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWEV_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwev_w_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWEV_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwev_d_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWEV_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwev_q_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWEV_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwev_h_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWEV_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwev_w_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWEV_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwev_d_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWEV_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwev_q_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWEV_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwod_h_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWOD_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwod_w_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWOD_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwod_d_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWOD_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwod_q_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWOD_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwod_h_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWOD_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwod_w_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWOD_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwod_d_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWOD_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwod_q_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWOD_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwl_h_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWL_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwl_w_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWL_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwl_d_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWL_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwl_q_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWL_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwl_h_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWL_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwl_w_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWL_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwl_d_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWL_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwl_q_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWL_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwh_h_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWH_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwh_w_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWH_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwh_d_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWH_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwh_q_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWH_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwh_h_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWH_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwh_w_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWH_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwh_d_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWH_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubwh_q_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBWH_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwev_h_bu_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWEV_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwev_w_hu_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWEV_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwev_d_wu_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWEV_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwev_q_du_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWEV_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwod_h_bu_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWOD_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwod_w_hu_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWOD_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwod_d_wu_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWOD_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwod_q_du_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWOD_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwl_h_bu_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWL_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwl_w_hu_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWL_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwl_d_wu_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWL_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwl_q_du_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWL_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwh_h_bu_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWH_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwh_w_hu_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWH_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwh_d_wu_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWH_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddwh_q_du_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDWH_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsadd_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSADD_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsadd_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSADD_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsadd_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSADD_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsadd_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSADD_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssub_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSUB_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssub_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSUB_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssub_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSUB_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssub_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSUB_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsadd_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSADD_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsadd_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSADD_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsadd_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSADD_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsadd_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSADD_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssub_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSUB_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssub_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSUB_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssub_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSUB_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssub_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSUB_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssub_bu_bu_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSUB_BU_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssub_hu_hu_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSUB_HU_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssub_wu_wu_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSUB_WU_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssub_du_du_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSUB_DU_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssub_bu_b_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSUB_BU_B_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssub_hu_h_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSUB_HU_H_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssub_wu_w_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSUB_WU_W_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssub_du_d_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSUB_DU_D_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssub_b_bu_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSUB_B_BU_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssub_h_hu_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSUB_H_HU_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssub_w_wu_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSUB_W_WU_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssub_d_du_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSUB_D_DU_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhaddw_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHADDW_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhaddw_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHADDW_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhaddw_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHADDW_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhaddw_q_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHADDW_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhsubw_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHSUBW_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhsubw_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHSUBW_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhsubw_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHSUBW_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhsubw_q_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHSUBW_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhaddw_hu_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHADDW_HU_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhaddw_wu_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHADDW_WU_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhaddw_du_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHADDW_DU_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhaddw_qu_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHADDW_QU_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhsubw_hu_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHSUBW_HU_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhsubw_wu_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHSUBW_WU_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhsubw_du_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHSUBW_DU_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhsubw_qu_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHSUBW_QU_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vadda_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDA_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vadda_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDA_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vadda_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDA_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vadda_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDA_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsadda_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSADDA_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsadda_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSADDA_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsadda_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSADDA_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsadda_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSADDA_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vabsd_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VABSD_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vabsd_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VABSD_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vabsd_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VABSD_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vabsd_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VABSD_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vabsd_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VABSD_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vabsd_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VABSD_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vabsd_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VABSD_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vabsd_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VABSD_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vavg_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VAVG_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vavg_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VAVG_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vavg_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VAVG_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vavg_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VAVG_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vavg_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VAVG_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vavg_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VAVG_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vavg_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VAVG_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vavg_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VAVG_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vavgr_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VAVGR_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vavgr_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VAVGR_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vavgr_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VAVGR_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vavgr_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VAVGR_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vavgr_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VAVGR_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vavgr_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VAVGR_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vavgr_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VAVGR_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vavgr_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VAVGR_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhalfd_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHALFD_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhalfd_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHALFD_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhalfd_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHALFD_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhalfd_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHALFD_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhalfd_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHALFD_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhalfd_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHALFD_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhalfd_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHALFD_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhalfd_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHALFD_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmax_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMAX_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmax_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMAX_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmax_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMAX_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmax_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMAX_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmin_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMIN_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmin_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMIN_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmin_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMIN_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmin_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMIN_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmax_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMAX_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmax_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMAX_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmax_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMAX_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmax_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMAX_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmin_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMIN_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmin_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMIN_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmin_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMIN_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmin_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMIN_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaxa_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMAXA_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaxa_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMAXA_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaxa_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMAXA_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaxa_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMAXA_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmina_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMINA_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmina_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMINA_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmina_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMINA_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmina_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMINA_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsadw_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSADW_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsadw_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSADW_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsadw_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSADW_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsadw_h_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSADW_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsadw_w_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSADW_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsadw_d_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSADW_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaccsadw_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VACCSADW_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaccsadw_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VACCSADW_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaccsadw_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VACCSADW_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaccsadw_h_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VACCSADW_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaccsadw_w_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VACCSADW_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaccsadw_d_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VACCSADW_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmul_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMUL_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmul_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMUL_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmul_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMUL_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmul_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMUL_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmuh_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMUH_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmuh_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMUH_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmuh_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMUH_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmuh_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMUH_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmuh_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMUH_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmuh_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMUH_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmuh_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMUH_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmuh_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMUH_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmuh_bu_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMUH_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmuh_hu_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMUH_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmuh_wu_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMUH_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmuh_du_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMUH_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulxw_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULXW_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulxw_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULXW_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulxw_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULXW_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulxw_h_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULXW_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulxw_w_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULXW_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulxw_d_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULXW_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwev_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWEV_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwev_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWEV_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwev_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWEV_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwev_q_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWEV_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwod_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWOD_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwod_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWOD_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwod_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWOD_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwod_q_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWOD_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwl_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWL_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwl_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWL_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwl_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWL_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwl_q_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWL_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwh_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWH_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwh_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWH_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwh_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWH_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwh_q_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWH_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwev_h_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWEV_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwev_w_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWEV_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwev_d_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWEV_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwev_q_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWEV_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwod_h_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWOD_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwod_w_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWOD_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwod_d_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWOD_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwod_q_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWOD_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwl_h_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWL_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwl_w_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWL_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwl_d_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWL_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwl_q_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWL_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwh_h_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWH_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwh_w_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWH_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwh_d_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWH_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwh_q_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWH_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwev_h_bu_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWEV_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwev_w_hu_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWEV_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwev_d_wu_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWEV_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwev_q_du_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWEV_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwod_h_bu_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWOD_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwod_w_hu_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWOD_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwod_d_wu_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWOD_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwod_q_du_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWOD_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwl_h_bu_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWL_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwl_w_hu_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWL_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwl_d_wu_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWL_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwl_q_du_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWL_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwh_h_bu_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWH_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwh_w_hu_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWH_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwh_d_wu_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWH_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmulwh_q_du_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMULWH_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmadd_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADD_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmadd_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADD_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmadd_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADD_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmadd_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADD_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmsub_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMSUB_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmsub_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMSUB_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmsub_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMSUB_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmsub_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMSUB_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwev_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWEV_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwev_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWEV_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwev_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWEV_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwev_q_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWEV_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwod_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWOD_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwod_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWOD_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwod_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWOD_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwod_q_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWOD_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwl_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWL_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwl_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWL_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwl_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWL_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwl_q_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWL_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwh_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWH_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwh_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWH_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwh_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWH_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwh_q_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWH_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwev_h_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWEV_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwev_w_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWEV_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwev_d_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWEV_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwev_q_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWEV_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwod_h_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWOD_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwod_w_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWOD_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwod_d_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWOD_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwod_q_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWOD_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwl_h_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWL_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwl_w_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWL_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwl_d_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWL_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwl_q_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWL_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwh_h_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWH_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwh_w_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWH_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwh_d_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWH_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwh_q_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWH_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwev_h_bu_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWEV_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwev_w_hu_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWEV_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwev_d_wu_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWEV_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwev_q_du_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWEV_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwod_h_bu_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWOD_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwod_w_hu_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWOD_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwod_d_wu_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWOD_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwod_q_du_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWOD_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwl_h_bu_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWL_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwl_w_hu_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWL_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwl_d_wu_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWL_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwl_q_du_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWL_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwh_h_bu_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWH_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwh_w_hu_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWH_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwh_d_wu_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWH_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaddwh_q_du_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMADDWH_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2_q_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2_hu_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2_HU_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2_wu_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2_WU_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2_du_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2_DU_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2_qu_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2_QU_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2_h_bu_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2_w_hu_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2_d_wu_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2_q_du_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2add_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2ADD_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2add_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2ADD_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2add_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2ADD_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2add_q_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2ADD_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2add_h_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2ADD_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2add_w_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2ADD_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2add_d_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2ADD_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2add_q_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2ADD_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2add_h_bu_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2ADD_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2add_w_hu_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2ADD_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2add_d_wu_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2ADD_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2add_q_du_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2ADD_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2sub_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2SUB_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2sub_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2SUB_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2sub_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2SUB_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2sub_q_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2SUB_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2sub_h_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2SUB_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2sub_w_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2SUB_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2sub_d_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2SUB_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp2sub_q_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP2SUB_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp4_w_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP4_W_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp4_d_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP4_D_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp4_q_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP4_Q_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp4_w_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP4_W_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp4_d_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP4_D_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp4_q_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP4_Q_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp4_w_bu_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP4_W_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp4_d_hu_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP4_D_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp4_q_wu_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP4_Q_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp4add_w_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP4ADD_W_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp4add_d_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP4ADD_D_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp4add_q_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP4ADD_Q_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp4add_w_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP4ADD_W_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp4add_d_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP4ADD_D_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp4add_q_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP4ADD_Q_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp4add_w_bu_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP4ADD_W_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp4add_d_hu_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP4ADD_D_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdp4add_q_wu_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDP4ADD_Q_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdiv_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDIV_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdiv_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDIV_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdiv_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDIV_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdiv_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDIV_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmod_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMOD_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmod_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMOD_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmod_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMOD_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmod_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMOD_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdiv_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDIV_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdiv_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDIV_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdiv_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDIV_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vdiv_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VDIV_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmod_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMOD_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmod_hu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMOD_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmod_wu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMOD_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmod_du(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMOD_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsll_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLL_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsll_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLL_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsll_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLL_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsll_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLL_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrl_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRL_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrl_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRL_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrl_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRL_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrl_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRL_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsra_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRA_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsra_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRA_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsra_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRA_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsra_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRA_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vrotr_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VROTR_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vrotr_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VROTR_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vrotr_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VROTR_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vrotr_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VROTR_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrlr_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLR_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrlr_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLR_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrlr_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLR_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrlr_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLR_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrar_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRAR_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrar_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRAR_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrar_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRAR_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrar_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRAR_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrln_b_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLN_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrln_h_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLN_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrln_w_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLN_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsran_b_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRAN_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsran_h_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRAN_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsran_w_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRAN_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrlrn_b_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLRN_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrlrn_h_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLRN_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrlrn_w_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLRN_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrarn_b_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRARN_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrarn_h_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRARN_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrarn_w_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRARN_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrln_b_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLN_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrln_h_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLN_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrln_w_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLN_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssran_b_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRAN_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssran_h_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRAN_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssran_w_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRAN_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlrn_b_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLRN_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlrn_h_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLRN_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlrn_w_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLRN_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrarn_b_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRARN_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrarn_h_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRARN_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrarn_w_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRARN_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrln_bu_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLN_BU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrln_hu_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLN_HU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrln_wu_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLN_WU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssran_bu_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRAN_BU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssran_hu_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRAN_HU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssran_wu_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRAN_WU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlrn_bu_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLRN_BU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlrn_hu_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLRN_HU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlrn_wu_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLRN_WU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrarn_bu_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRARN_BU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrarn_hu_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRARN_HU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrarn_wu_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRARN_WU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitclr_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITCLR_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitclr_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITCLR_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitclr_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITCLR_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitclr_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITCLR_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitset_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITSET_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitset_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITSET_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitset_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITSET_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitset_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITSET_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitrev_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITREV_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitrev_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITREV_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitrev_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITREV_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitrev_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITREV_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbstrc12_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBSTRC12_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbstrc12_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBSTRC12_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbstrc12_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBSTRC12_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbstrc12_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBSTRC12_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbstrc21_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBSTRC21_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbstrc21_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBSTRC21_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbstrc21_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBSTRC21_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbstrc21_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBSTRC21_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpackev_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPACKEV_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpackev_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPACKEV_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpackev_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPACKEV_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpackev_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPACKEV_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpackod_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPACKOD_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpackod_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPACKOD_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpackod_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPACKOD_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpackod_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPACKOD_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vilvl_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VILVL_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vilvl_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VILVL_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vilvl_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VILVL_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vilvl_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VILVL_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vilvh_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VILVH_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vilvh_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VILVH_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vilvh_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VILVH_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vilvh_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VILVH_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpickev_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPICKEV_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpickev_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPICKEV_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpickev_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPICKEV_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpickev_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPICKEV_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpickod_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPICKOD_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpickod_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPICKOD_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpickod_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPICKOD_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpickod_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPICKOD_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vreplve_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VREPLVE_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vreplve_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VREPLVE_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vreplve_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VREPLVE_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vreplve_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VREPLVE_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vextrcol_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTRCOL_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vextrcol_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTRCOL_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vextrcol_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTRCOL_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vextrcol_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTRCOL_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vand_v(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VAND_V;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vor_v(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VOR_V;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vxor_v(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VXOR_V;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vnor_v(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VNOR_V;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vandn_v(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VANDN_V;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vorn_v(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VORN_V;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vrandsign_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VRANDSIGN_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vrandsign_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VRANDSIGN_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vrorsign_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VRORSIGN_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vrorsign_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VRORSIGN_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfrstp_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFRSTP_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfrstp_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFRSTP_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vclrstrr_v(IR2_OPND vd, IR2_OPND vj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VCLRSTRR_V;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vclrstrv_v(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VCLRSTRV_V;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vadd_q(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADD_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsub_q(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUB_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsigncov_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSIGNCOV_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsigncov_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSIGNCOV_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsigncov_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSIGNCOV_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsigncov_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSIGNCOV_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfadd_s(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFADD_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfadd_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFADD_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfsub_s(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFSUB_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfsub_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFSUB_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfaddsub_s(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFADDSUB_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfaddsub_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFADDSUB_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfsubadd_s(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFSUBADD_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfsubadd_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFSUBADD_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfmul_s(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFMUL_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfmul_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFMUL_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfdiv_s(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFDIV_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfdiv_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFDIV_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfmax_s(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFMAX_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfmax_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFMAX_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfmin_s(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFMIN_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfmin_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFMIN_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfmaxa_s(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFMAXA_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfmaxa_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFMAXA_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfmina_s(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFMINA_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfmina_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFMINA_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfscaleb_s(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFSCALEB_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfscaleb_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFSCALEB_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfcvt_h_s(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFCVT_H_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfcvt_s_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFCVT_S_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vffint_s_l(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFFINT_S_L;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftint_w_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINT_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftintrm_w_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINTRM_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftintrp_w_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINTRP_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftintrz_w_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINTRZ_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftintrne_w_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINTRNE_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhadd4_h_bu(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHADD4_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vshuf4_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSHUF4_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vshuf2_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSHUF2_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_aes128_enc(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AES128_ENC;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_aes128_dec(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AES128_DEC;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_aes192_enc(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AES192_ENC;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_aes192_dec(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AES192_DEC;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_aes256_enc(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AES256_ENC;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_aes256_dec(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AES256_DEC;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_aes_kg(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AES_KG;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_aes_fr_enc(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AES_FR_ENC;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_aes_fr_dec(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AES_FR_DEC;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_aes_lr_enc(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AES_LR_ENC;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_aes_lr_dec(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AES_LR_DEC;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_aes_mc_enc(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AES_MC_ENC;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_aes_mc_dec(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AES_MC_DEC;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_aes_sb_enc(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AES_SB_ENC;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_aes_sb_dec(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AES_SB_DEC;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_aes_sr_enc(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AES_SR_ENC;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_aes_sr_dec(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_AES_SR_DEC;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_md5_ms(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MD5_MS;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_md5_4r(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_MD5_4R;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_sha1_ms_1(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SHA1_MS_1;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_sha1_ms_2(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SHA1_MS_2;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_sha1_hash_4r(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SHA1_HASH_4R;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_sha256_ms_1(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SHA256_MS_1;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_sha256_ms_2(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SHA256_MS_2;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_sha256_hash_2r(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SHA256_HASH_2R;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_sha512_ms_1(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SHA512_MS_1;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_sha512_ms_2(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SHA512_MS_2;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_sha512_hash_r_1(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SHA512_HASH_R_1;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_sha512_hash_r_2(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_SHA512_HASH_R_2;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpmul_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPMUL_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpmul_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPMUL_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpmuh_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPMUH_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpmuh_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPMUH_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpmulacc_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPMULACC_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpmulacc_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPMULACC_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpmuhacc_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPMUHACC_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpmuhacc_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPMUHACC_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpmulwl_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPMULWL_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpmulwl_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPMULWL_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpmulwl_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPMULWL_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpmulwl_q_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPMULWL_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpmulwh_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPMULWH_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpmulwh_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPMULWH_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpmulwh_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPMULWH_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpmulwh_q_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPMULWH_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpmaddwl_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPMADDWL_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpmaddwl_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPMADDWL_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpmaddwl_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPMADDWL_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpmaddwl_q_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPMADDWL_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpmaddwh_h_b(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPMADDWH_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpmaddwh_w_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPMADDWH_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpmaddwh_d_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPMADDWH_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpmaddwh_q_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPMADDWH_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpdp2_q_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPDP2_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpdp2add_q_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPDP2ADD_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vcdp4_re_d_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VCDP4_RE_D_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vcdp4_im_d_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VCDP4_IM_D_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vcdp4add_re_d_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VCDP4ADD_RE_D_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vcdp4add_im_d_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VCDP4ADD_IM_D_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vcdp2_re_q_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VCDP2_RE_Q_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vcdp2_im_q_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VCDP2_IM_Q_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vcdp2add_re_q_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VCDP2ADD_RE_Q_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vcdp2add_im_q_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VCDP2ADD_IM_Q_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsignsel_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSIGNSEL_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsignsel_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSIGNSEL_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vshuf_h(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSHUF_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vshuf_w(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSHUF_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vshuf_d(IR2_OPND vd, IR2_OPND vj, IR2_OPND vk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSHUF_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    pir2->_opnd[2] = vk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vseqi_b(IR2_OPND vd, IR2_OPND vj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSEQI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vseqi_h(IR2_OPND vd, IR2_OPND vj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSEQI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vseqi_w(IR2_OPND vd, IR2_OPND vj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSEQI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vseqi_d(IR2_OPND vd, IR2_OPND vj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSEQI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslei_b(IR2_OPND vd, IR2_OPND vj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLEI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslei_h(IR2_OPND vd, IR2_OPND vj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLEI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslei_w(IR2_OPND vd, IR2_OPND vj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLEI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslei_d(IR2_OPND vd, IR2_OPND vj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLEI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslei_bu(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLEI_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslei_hu(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLEI_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslei_wu(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLEI_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslei_du(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLEI_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslti_b(IR2_OPND vd, IR2_OPND vj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLTI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslti_h(IR2_OPND vd, IR2_OPND vj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLTI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslti_w(IR2_OPND vd, IR2_OPND vj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLTI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslti_d(IR2_OPND vd, IR2_OPND vj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLTI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslti_bu(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLTI_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslti_hu(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLTI_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslti_wu(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLTI_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslti_du(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLTI_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddi_bu(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDI_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddi_hu(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDI_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddi_wu(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDI_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vaddi_du(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VADDI_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubi_bu(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBI_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubi_hu(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBI_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubi_wu(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBI_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsubi_du(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSUBI_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbsll_v(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBSLL_V;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbsrl_v(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBSRL_V;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaxi_b(IR2_OPND vd, IR2_OPND vj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMAXI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaxi_h(IR2_OPND vd, IR2_OPND vj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMAXI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaxi_w(IR2_OPND vd, IR2_OPND vj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMAXI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaxi_d(IR2_OPND vd, IR2_OPND vj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMAXI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmini_b(IR2_OPND vd, IR2_OPND vj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMINI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmini_h(IR2_OPND vd, IR2_OPND vj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMINI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmini_w(IR2_OPND vd, IR2_OPND vj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMINI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmini_d(IR2_OPND vd, IR2_OPND vj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMINI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaxi_bu(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMAXI_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaxi_hu(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMAXI_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaxi_wu(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMAXI_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmaxi_du(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMAXI_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmini_bu(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMINI_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmini_hu(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMINI_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmini_wu(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMINI_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmini_du(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMINI_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vrandsigni_b(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VRANDSIGNI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vrandsigni_h(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VRANDSIGNI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vrorsigni_b(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VRORSIGNI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vrorsigni_h(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VRORSIGNI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfrstpi_b(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFRSTPI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfrstpi_h(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFRSTPI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vclrstri_v(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VCLRSTRI_V;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmepatmsk_v(IR2_OPND vd, int imm_mode, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMEPATMSK_V;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    IR2_OPND op0 = create_immh_opnd(imm_mode);
    pir2->_opnd[1] = op0;
    IR2_OPND op1 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op1;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vclo_b(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VCLO_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vclo_h(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VCLO_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vclo_w(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VCLO_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vclo_d(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VCLO_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vclz_b(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VCLZ_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vclz_h(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VCLZ_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vclz_w(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VCLZ_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vclz_d(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VCLZ_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpcnt_b(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPCNT_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpcnt_h(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPCNT_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpcnt_w(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPCNT_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpcnt_d(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPCNT_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vneg_b(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VNEG_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vneg_h(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VNEG_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vneg_w(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VNEG_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vneg_d(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VNEG_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmskltz_b(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMSKLTZ_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmskltz_h(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMSKLTZ_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmskltz_w(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMSKLTZ_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmskltz_d(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMSKLTZ_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmskgez_b(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMSKGEZ_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmsknz_b(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMSKNZ_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmskcopy_b(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMSKCOPY_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vmskfill_b(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VMSKFILL_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfrstm_b(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFRSTM_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfrstm_h(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFRSTM_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vseteqz_v(IR2_OPND cd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSETEQZ_V;
    pir2->op_count = 2;
    pir2->_opnd[0] = cd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsetnez_v(IR2_OPND cd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSETNEZ_V;
    pir2->op_count = 2;
    pir2->_opnd[0] = cd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsetanyeqz_b(IR2_OPND cd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSETANYEQZ_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = cd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsetanyeqz_h(IR2_OPND cd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSETANYEQZ_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = cd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsetanyeqz_w(IR2_OPND cd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSETANYEQZ_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = cd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsetanyeqz_d(IR2_OPND cd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSETANYEQZ_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = cd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsetallnez_b(IR2_OPND cd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSETALLNEZ_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = cd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsetallnez_h(IR2_OPND cd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSETALLNEZ_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = cd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsetallnez_w(IR2_OPND cd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSETALLNEZ_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = cd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsetallnez_d(IR2_OPND cd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSETALLNEZ_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = cd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vflogb_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFLOGB_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vflogb_d(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFLOGB_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfclass_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFCLASS_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfclass_d(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFCLASS_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfsqrt_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFSQRT_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfsqrt_d(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFSQRT_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfrecip_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFRECIP_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfrecip_d(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFRECIP_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfrsqrt_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFRSQRT_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfrsqrt_d(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFRSQRT_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfrint_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFRINT_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfrint_d(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFRINT_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfrintrm_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFRINTRM_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfrintrm_d(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFRINTRM_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfrintrp_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFRINTRP_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfrintrp_d(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFRINTRP_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfrintrz_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFRINTRZ_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfrintrz_d(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFRINTRZ_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfrintrne_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFRINTRNE_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfrintrne_d(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFRINTRNE_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vextl_w_b(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTL_W_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vextl_d_b(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTL_D_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vextl_d_h(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTL_D_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vextl_w_bu(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTL_W_BU;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vextl_d_bu(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTL_D_BU;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vextl_d_hu(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTL_D_HU;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhadd8_d_bu(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHADD8_D_BU;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhminpos_w_hu(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHMINPOS_W_HU;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhminpos_d_hu(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHMINPOS_D_HU;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vhminpos_q_hu(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VHMINPOS_Q_HU;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vclrtail_b(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VCLRTAIL_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vclrtail_h(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VCLRTAIL_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfcvtl_s_h(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFCVTL_S_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfcvth_s_h(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFCVTH_S_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfcvtl_d_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFCVTL_D_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vfcvth_d_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFCVTH_D_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vffint_s_w(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFFINT_S_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vffint_s_wu(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFFINT_S_WU;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vffint_d_l(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFFINT_D_L;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vffint_d_lu(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFFINT_D_LU;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vffintl_d_w(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFFINTL_D_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vffinth_d_w(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFFINTH_D_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftint_w_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINT_W_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftint_l_d(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINT_L_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftintrm_w_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINTRM_W_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftintrm_l_d(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINTRM_L_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftintrp_w_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINTRP_W_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftintrp_l_d(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINTRP_L_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftintrz_w_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINTRZ_W_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftintrz_l_d(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINTRZ_L_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftintrne_w_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINTRNE_W_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftintrne_l_d(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINTRNE_L_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftint_wu_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINT_WU_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftint_lu_d(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINT_LU_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftintrz_wu_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINTRZ_WU_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftintrz_lu_d(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINTRZ_LU_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftintl_l_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINTL_L_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftinth_l_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINTH_L_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftintrml_l_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINTRML_L_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftintrmh_l_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINTRMH_L_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftintrpl_l_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINTRPL_L_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftintrph_l_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINTRPH_L_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftintrzl_l_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINTRZL_L_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftintrzh_l_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINTRZH_L_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftintrnel_l_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINTRNEL_L_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vftintrneh_l_s(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VFTINTRNEH_L_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vexth_h_b(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTH_H_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vexth_w_h(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTH_W_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vexth_d_w(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTH_D_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vexth_q_d(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTH_Q_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vexth_hu_bu(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTH_HU_BU;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vexth_wu_hu(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTH_WU_HU;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vexth_du_wu(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTH_DU_WU;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vexth_qu_du(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTH_QU_DU;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vreplgr2vr_b(IR2_OPND vd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VREPLGR2VR_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vreplgr2vr_h(IR2_OPND vd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VREPLGR2VR_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vreplgr2vr_w(IR2_OPND vd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VREPLGR2VR_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vreplgr2vr_d(IR2_OPND vd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VREPLGR2VR_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vrotri_b(IR2_OPND vd, IR2_OPND vj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VROTRI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vrotri_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VROTRI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vrotri_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VROTRI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vrotri_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VROTRI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrlri_b(IR2_OPND vd, IR2_OPND vj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLRI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrlri_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLRI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrlri_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLRI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrlri_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLRI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrari_b(IR2_OPND vd, IR2_OPND vj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRARI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrari_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRARI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrari_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRARI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrari_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRARI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vinsgr2vr_b(IR2_OPND vd, IR2_OPND rj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VINSGR2VR_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vinsgr2vr_h(IR2_OPND vd, IR2_OPND rj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VINSGR2VR_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vinsgr2vr_w(IR2_OPND vd, IR2_OPND rj, int imm_ui2) {
    assert(imm_ui2 <= 0x3);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VINSGR2VR_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui2);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vinsgr2vr_d(IR2_OPND vd, IR2_OPND rj, int imm_ui1) {
    assert(imm_ui1 <= 0x1);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VINSGR2VR_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui1);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpickve2gr_b(IR2_OPND rd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPICKVE2GR_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpickve2gr_h(IR2_OPND rd, IR2_OPND vj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPICKVE2GR_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpickve2gr_w(IR2_OPND rd, IR2_OPND vj, int imm_ui2) {
    assert(imm_ui2 <= 0x3);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPICKVE2GR_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui2);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpickve2gr_d(IR2_OPND rd, IR2_OPND vj, int imm_ui1) {
    assert(imm_ui1 <= 0x1);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPICKVE2GR_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui1);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpickve2gr_bu(IR2_OPND rd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPICKVE2GR_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpickve2gr_hu(IR2_OPND rd, IR2_OPND vj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPICKVE2GR_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpickve2gr_wu(IR2_OPND rd, IR2_OPND vj, int imm_ui2) {
    assert(imm_ui2 <= 0x3);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPICKVE2GR_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui2);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpickve2gr_du(IR2_OPND rd, IR2_OPND vj, int imm_ui1) {
    assert(imm_ui1 <= 0x1);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPICKVE2GR_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui1);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vreplvei_b(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VREPLVEI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vreplvei_h(IR2_OPND vd, IR2_OPND vj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VREPLVEI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vreplvei_w(IR2_OPND vd, IR2_OPND vj, int imm_ui2) {
    assert(imm_ui2 <= 0x3);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VREPLVEI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui2);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vreplvei_d(IR2_OPND vd, IR2_OPND vj, int imm_ui1) {
    assert(imm_ui1 <= 0x1);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VREPLVEI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui1);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vextrcoli_b(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTRCOLI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vextrcoli_h(IR2_OPND vd, IR2_OPND vj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTRCOLI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vextrcoli_w(IR2_OPND vd, IR2_OPND vj, int imm_ui2) {
    assert(imm_ui2 <= 0x3);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTRCOLI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui2);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vextrcoli_d(IR2_OPND vd, IR2_OPND vj, int imm_ui1) {
    assert(imm_ui1 <= 0x1);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTRCOLI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui1);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsllwil_h_b(IR2_OPND vd, IR2_OPND vj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLLWIL_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsllwil_w_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLLWIL_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsllwil_d_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLLWIL_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vextl_q_d(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTL_Q_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsllwil_hu_bu(IR2_OPND vd, IR2_OPND vj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLLWIL_HU_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsllwil_wu_hu(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLLWIL_WU_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsllwil_du_wu(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLLWIL_DU_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vextl_qu_du(IR2_OPND vd, IR2_OPND vj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTL_QU_DU;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitclri_b(IR2_OPND vd, IR2_OPND vj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITCLRI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitclri_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITCLRI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitclri_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITCLRI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitclri_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITCLRI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitseti_b(IR2_OPND vd, IR2_OPND vj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITSETI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitseti_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITSETI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitseti_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITSETI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitseti_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITSETI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitrevi_b(IR2_OPND vd, IR2_OPND vj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITREVI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitrevi_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITREVI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitrevi_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITREVI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitrevi_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITREVI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbstrc12i_b(IR2_OPND vd, IR2_OPND vj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBSTRC12I_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbstrc12i_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBSTRC12I_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbstrc12i_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBSTRC12I_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbstrc12i_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBSTRC12I_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbstrc21i_b(IR2_OPND vd, IR2_OPND vj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBSTRC21I_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbstrc21i_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBSTRC21I_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbstrc21i_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBSTRC21I_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbstrc21i_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBSTRC21I_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsat_b(IR2_OPND vd, IR2_OPND vj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSAT_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsat_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSAT_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsat_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSAT_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsat_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSAT_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsat_bu(IR2_OPND vd, IR2_OPND vj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSAT_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsat_hu(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSAT_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsat_wu(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSAT_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsat_du(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSAT_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslli_b(IR2_OPND vd, IR2_OPND vj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLLI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslli_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLLI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslli_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLLI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vslli_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSLLI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrli_b(IR2_OPND vd, IR2_OPND vj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrli_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrli_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrli_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrai_b(IR2_OPND vd, IR2_OPND vj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRAI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrai_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRAI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrai_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRAI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrai_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRAI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrlrneni_b_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLRNENI_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrlrneni_h_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLRNENI_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrlrneni_w_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLRNENI_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrlrneni_d_q(IR2_OPND vd, IR2_OPND vj, int imm_ui7) {
    assert(imm_ui7 <= 0x7f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLRNENI_D_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrarneni_b_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRARNENI_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrarneni_h_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRARNENI_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrarneni_w_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRARNENI_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrarneni_d_q(IR2_OPND vd, IR2_OPND vj, int imm_ui7) {
    assert(imm_ui7 <= 0x7f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRARNENI_D_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrlni_b_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLNI_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrlni_h_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLNI_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrlni_w_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLNI_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrlni_d_q(IR2_OPND vd, IR2_OPND vj, int imm_ui7) {
    assert(imm_ui7 <= 0x7f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLNI_D_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrlrni_b_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLRNI_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrlrni_h_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLRNI_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrlrni_w_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLRNI_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrlrni_d_q(IR2_OPND vd, IR2_OPND vj, int imm_ui7) {
    assert(imm_ui7 <= 0x7f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRLRNI_D_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlni_b_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLNI_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlni_h_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLNI_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlni_w_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLNI_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlni_d_q(IR2_OPND vd, IR2_OPND vj, int imm_ui7) {
    assert(imm_ui7 <= 0x7f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLNI_D_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlni_bu_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLNI_BU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlni_hu_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLNI_HU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlni_wu_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLNI_WU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlni_du_q(IR2_OPND vd, IR2_OPND vj, int imm_ui7) {
    assert(imm_ui7 <= 0x7f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLNI_DU_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlrni_b_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLRNI_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlrni_h_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLRNI_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlrni_w_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLRNI_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlrni_d_q(IR2_OPND vd, IR2_OPND vj, int imm_ui7) {
    assert(imm_ui7 <= 0x7f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLRNI_D_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlrni_bu_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLRNI_BU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlrni_hu_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLRNI_HU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlrni_wu_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLRNI_WU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlrni_du_q(IR2_OPND vd, IR2_OPND vj, int imm_ui7) {
    assert(imm_ui7 <= 0x7f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLRNI_DU_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrani_b_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRANI_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrani_h_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRANI_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrani_w_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRANI_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrani_d_q(IR2_OPND vd, IR2_OPND vj, int imm_ui7) {
    assert(imm_ui7 <= 0x7f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRANI_D_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrarni_b_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRARNI_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrarni_h_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRARNI_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrarni_w_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRARNI_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vsrarni_d_q(IR2_OPND vd, IR2_OPND vj, int imm_ui7) {
    assert(imm_ui7 <= 0x7f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSRARNI_D_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrani_b_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRANI_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrani_h_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRANI_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrani_w_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRANI_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrani_d_q(IR2_OPND vd, IR2_OPND vj, int imm_ui7) {
    assert(imm_ui7 <= 0x7f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRANI_D_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrani_bu_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRANI_BU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrani_hu_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRANI_HU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrani_wu_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRANI_WU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrani_du_q(IR2_OPND vd, IR2_OPND vj, int imm_ui7) {
    assert(imm_ui7 <= 0x7f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRANI_DU_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrarni_b_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRARNI_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrarni_h_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRARNI_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrarni_w_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRARNI_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrarni_d_q(IR2_OPND vd, IR2_OPND vj, int imm_ui7) {
    assert(imm_ui7 <= 0x7f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRARNI_D_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrarni_bu_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRARNI_BU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrarni_hu_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRARNI_HU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrarni_wu_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRARNI_WU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrarni_du_q(IR2_OPND vd, IR2_OPND vj, int imm_ui7) {
    assert(imm_ui7 <= 0x7f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRARNI_DU_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlrneni_b_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLRNENI_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlrneni_h_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLRNENI_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlrneni_w_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLRNENI_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlrneni_d_q(IR2_OPND vd, IR2_OPND vj, int imm_ui7) {
    assert(imm_ui7 <= 0x7f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLRNENI_D_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlrneni_bu_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLRNENI_BU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlrneni_hu_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLRNENI_HU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlrneni_wu_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLRNENI_WU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrlrneni_du_q(IR2_OPND vd, IR2_OPND vj, int imm_ui7) {
    assert(imm_ui7 <= 0x7f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRLRNENI_DU_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrarneni_b_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRARNENI_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrarneni_h_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRARNENI_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrarneni_w_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRARNENI_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrarneni_d_q(IR2_OPND vd, IR2_OPND vj, int imm_ui7) {
    assert(imm_ui7 <= 0x7f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRARNENI_D_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrarneni_bu_h(IR2_OPND vd, IR2_OPND vj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRARNENI_BU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrarneni_hu_w(IR2_OPND vd, IR2_OPND vj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRARNENI_HU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrarneni_wu_d(IR2_OPND vd, IR2_OPND vj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRARNENI_WU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vssrarneni_du_q(IR2_OPND vd, IR2_OPND vj, int imm_ui7) {
    assert(imm_ui7 <= 0x7f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSSRARNENI_DU_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vextrins_d(IR2_OPND vd, IR2_OPND vj, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTRINS_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vextrins_w(IR2_OPND vd, IR2_OPND vj, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTRINS_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vextrins_h(IR2_OPND vd, IR2_OPND vj, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTRINS_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vextrins_b(IR2_OPND vd, IR2_OPND vj, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXTRINS_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vshuf4i_b(IR2_OPND vd, IR2_OPND vj, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSHUF4I_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vshuf4i_h(IR2_OPND vd, IR2_OPND vj, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSHUF4I_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vshuf4i_w(IR2_OPND vd, IR2_OPND vj, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSHUF4I_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vshuf4i_d(IR2_OPND vd, IR2_OPND vj, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSHUF4I_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vshufi1_b(IR2_OPND vd, IR2_OPND vj, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSHUFI1_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vshufi2_b(IR2_OPND vd, IR2_OPND vj, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSHUFI2_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vshufi3_b(IR2_OPND vd, IR2_OPND vj, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSHUFI3_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vshufi4_b(IR2_OPND vd, IR2_OPND vj, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSHUFI4_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vshufi1_h(IR2_OPND vd, IR2_OPND vj, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSHUFI1_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vshufi2_h(IR2_OPND vd, IR2_OPND vj, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSHUFI2_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vseli_h(IR2_OPND vd, IR2_OPND vj, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSELI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vseli_w(IR2_OPND vd, IR2_OPND vj, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSELI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vseli_d(IR2_OPND vd, IR2_OPND vj, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VSELI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitseli_b(IR2_OPND vd, IR2_OPND vj, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITSELI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitmvzi_b(IR2_OPND vd, IR2_OPND vj, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITMVZI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vbitmvnzi_b(IR2_OPND vd, IR2_OPND vj, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VBITMVNZI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vandi_b(IR2_OPND vd, IR2_OPND vj, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VANDI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vori_b(IR2_OPND vd, IR2_OPND vj, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VORI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vxori_b(IR2_OPND vd, IR2_OPND vj, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VXORI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vnori_b(IR2_OPND vd, IR2_OPND vj, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VNORI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vldi(IR2_OPND vd, int imm_i13) {
    assert(imm_i13 <= 0x1fff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VLDI;
    pir2->op_count = 2;
    pir2->_opnd[0] = vd;
    IR2_OPND op0 = create_immh_opnd(imm_i13);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vpermi_w(IR2_OPND vd, IR2_OPND vj, int imm_ui8) {
    assert(imm_ui8 <= 0xff);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VPERMI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = vd;
    pir2->_opnd[1] = vj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvseq_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSEQ_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvseq_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSEQ_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvseq_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSEQ_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvseq_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSEQ_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsle_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLE_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsle_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLE_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsle_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLE_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsle_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLE_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsle_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLE_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsle_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLE_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsle_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLE_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsle_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLE_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslt_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLT_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslt_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLT_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslt_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLT_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslt_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLT_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslt_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLT_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslt_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLT_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslt_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLT_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslt_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLT_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvadd_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADD_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvadd_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADD_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvadd_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADD_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvadd_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADD_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsub_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUB_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsub_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUB_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsub_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUB_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsub_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUB_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddw_h_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDW_H_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddw_w_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDW_W_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddw_d_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDW_D_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddw_h_h_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDW_H_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddw_w_w_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDW_W_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddw_d_d_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDW_D_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubw_h_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBW_H_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubw_w_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBW_W_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubw_d_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBW_D_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubw_h_h_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBW_H_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubw_w_w_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBW_W_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubw_d_d_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBW_D_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsaddw_h_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSADDW_H_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsaddw_w_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSADDW_W_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsaddw_d_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSADDW_D_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsaddw_hu_hu_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSADDW_HU_HU_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsaddw_wu_wu_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSADDW_WU_WU_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsaddw_du_du_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSADDW_DU_DU_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssubw_h_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSUBW_H_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssubw_w_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSUBW_W_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssubw_d_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSUBW_D_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssubw_hu_hu_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSUBW_HU_HU_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssubw_wu_wu_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSUBW_WU_WU_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssubw_du_du_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSUBW_DU_DU_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwev_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWEV_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwev_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWEV_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwev_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWEV_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwev_q_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWEV_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwev_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWEV_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwev_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWEV_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwev_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWEV_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwev_q_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWEV_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwod_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWOD_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwod_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWOD_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwod_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWOD_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwod_q_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWOD_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwod_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWOD_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwod_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWOD_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwod_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWOD_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwod_q_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWOD_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwl_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWL_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwl_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWL_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwl_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWL_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwl_q_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWL_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwl_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWL_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwl_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWL_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwl_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWL_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwl_q_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWL_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwh_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWH_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwh_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWH_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwh_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWH_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwh_q_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWH_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwh_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWH_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwh_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWH_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwh_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWH_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwh_q_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWH_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwev_h_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWEV_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwev_w_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWEV_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwev_d_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWEV_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwev_q_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWEV_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwev_h_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWEV_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwev_w_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWEV_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwev_d_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWEV_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwev_q_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWEV_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwod_h_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWOD_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwod_w_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWOD_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwod_d_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWOD_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwod_q_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWOD_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwod_h_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWOD_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwod_w_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWOD_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwod_d_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWOD_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwod_q_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWOD_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwl_h_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWL_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwl_w_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWL_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwl_d_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWL_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwl_q_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWL_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwl_h_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWL_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwl_w_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWL_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwl_d_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWL_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwl_q_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWL_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwh_h_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWH_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwh_w_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWH_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwh_d_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWH_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwh_q_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWH_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwh_h_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWH_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwh_w_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWH_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwh_d_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWH_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubwh_q_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBWH_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwev_h_bu_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWEV_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwev_w_hu_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWEV_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwev_d_wu_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWEV_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwev_q_du_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWEV_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwod_h_bu_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWOD_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwod_w_hu_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWOD_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwod_d_wu_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWOD_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwod_q_du_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWOD_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwl_h_bu_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWL_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwl_w_hu_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWL_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwl_d_wu_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWL_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwl_q_du_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWL_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwh_h_bu_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWH_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwh_w_hu_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWH_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwh_d_wu_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWH_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddwh_q_du_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDWH_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsadd_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSADD_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsadd_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSADD_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsadd_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSADD_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsadd_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSADD_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssub_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSUB_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssub_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSUB_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssub_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSUB_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssub_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSUB_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsadd_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSADD_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsadd_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSADD_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsadd_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSADD_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsadd_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSADD_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssub_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSUB_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssub_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSUB_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssub_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSUB_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssub_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSUB_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssub_bu_bu_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSUB_BU_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssub_hu_hu_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSUB_HU_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssub_wu_wu_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSUB_WU_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssub_du_du_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSUB_DU_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssub_bu_b_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSUB_BU_B_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssub_hu_h_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSUB_HU_H_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssub_wu_w_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSUB_WU_W_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssub_du_d_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSUB_DU_D_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssub_b_bu_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSUB_B_BU_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssub_h_hu_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSUB_H_HU_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssub_w_wu_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSUB_W_WU_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssub_d_du_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSUB_D_DU_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhaddw_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHADDW_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhaddw_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHADDW_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhaddw_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHADDW_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhaddw_q_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHADDW_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhsubw_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHSUBW_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhsubw_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHSUBW_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhsubw_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHSUBW_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhsubw_q_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHSUBW_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhaddw_hu_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHADDW_HU_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhaddw_wu_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHADDW_WU_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhaddw_du_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHADDW_DU_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhaddw_qu_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHADDW_QU_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhsubw_hu_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHSUBW_HU_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhsubw_wu_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHSUBW_WU_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhsubw_du_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHSUBW_DU_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhsubw_qu_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHSUBW_QU_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvadda_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDA_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvadda_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDA_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvadda_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDA_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvadda_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDA_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsadda_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSADDA_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsadda_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSADDA_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsadda_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSADDA_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsadda_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSADDA_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvabsd_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVABSD_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvabsd_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVABSD_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvabsd_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVABSD_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvabsd_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVABSD_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvabsd_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVABSD_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvabsd_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVABSD_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvabsd_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVABSD_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvabsd_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVABSD_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvavg_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVAVG_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvavg_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVAVG_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvavg_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVAVG_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvavg_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVAVG_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvavg_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVAVG_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvavg_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVAVG_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvavg_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVAVG_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvavg_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVAVG_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvavgr_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVAVGR_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvavgr_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVAVGR_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvavgr_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVAVGR_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvavgr_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVAVGR_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvavgr_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVAVGR_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvavgr_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVAVGR_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvavgr_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVAVGR_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvavgr_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVAVGR_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhalfd_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHALFD_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhalfd_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHALFD_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhalfd_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHALFD_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhalfd_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHALFD_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhalfd_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHALFD_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhalfd_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHALFD_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhalfd_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHALFD_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhalfd_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHALFD_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmax_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMAX_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmax_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMAX_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmax_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMAX_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmax_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMAX_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmin_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMIN_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmin_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMIN_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmin_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMIN_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmin_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMIN_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmax_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMAX_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmax_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMAX_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmax_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMAX_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmax_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMAX_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmin_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMIN_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmin_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMIN_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmin_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMIN_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmin_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMIN_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaxa_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMAXA_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaxa_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMAXA_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaxa_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMAXA_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaxa_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMAXA_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmina_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMINA_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmina_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMINA_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmina_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMINA_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmina_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMINA_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsadw_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSADW_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsadw_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSADW_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsadw_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSADW_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsadw_h_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSADW_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsadw_w_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSADW_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsadw_d_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSADW_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaccsadw_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVACCSADW_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaccsadw_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVACCSADW_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaccsadw_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVACCSADW_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaccsadw_h_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVACCSADW_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaccsadw_w_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVACCSADW_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaccsadw_d_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVACCSADW_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmul_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMUL_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmul_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMUL_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmul_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMUL_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmul_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMUL_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmuh_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMUH_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmuh_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMUH_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmuh_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMUH_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmuh_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMUH_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmuh_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMUH_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmuh_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMUH_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmuh_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMUH_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmuh_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMUH_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmuh_bu_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMUH_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmuh_hu_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMUH_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmuh_wu_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMUH_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmuh_du_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMUH_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulxw_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULXW_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulxw_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULXW_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulxw_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULXW_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulxw_h_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULXW_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulxw_w_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULXW_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulxw_d_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULXW_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwev_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWEV_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwev_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWEV_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwev_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWEV_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwev_q_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWEV_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwod_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWOD_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwod_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWOD_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwod_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWOD_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwod_q_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWOD_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwl_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWL_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwl_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWL_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwl_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWL_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwl_q_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWL_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwh_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWH_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwh_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWH_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwh_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWH_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwh_q_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWH_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwev_h_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWEV_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwev_w_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWEV_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwev_d_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWEV_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwev_q_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWEV_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwod_h_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWOD_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwod_w_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWOD_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwod_d_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWOD_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwod_q_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWOD_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwl_h_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWL_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwl_w_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWL_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwl_d_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWL_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwl_q_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWL_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwh_h_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWH_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwh_w_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWH_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwh_d_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWH_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwh_q_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWH_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwev_h_bu_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWEV_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwev_w_hu_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWEV_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwev_d_wu_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWEV_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwev_q_du_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWEV_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwod_h_bu_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWOD_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwod_w_hu_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWOD_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwod_d_wu_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWOD_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwod_q_du_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWOD_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwl_h_bu_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWL_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwl_w_hu_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWL_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwl_d_wu_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWL_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwl_q_du_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWL_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwh_h_bu_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWH_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwh_w_hu_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWH_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwh_d_wu_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWH_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmulwh_q_du_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMULWH_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmadd_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADD_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmadd_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADD_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmadd_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADD_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmadd_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADD_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmsub_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMSUB_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmsub_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMSUB_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmsub_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMSUB_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmsub_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMSUB_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwev_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWEV_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwev_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWEV_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwev_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWEV_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwev_q_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWEV_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwod_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWOD_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwod_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWOD_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwod_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWOD_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwod_q_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWOD_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwl_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWL_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwl_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWL_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwl_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWL_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwl_q_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWL_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwh_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWH_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwh_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWH_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwh_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWH_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwh_q_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWH_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwev_h_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWEV_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwev_w_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWEV_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwev_d_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWEV_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwev_q_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWEV_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwod_h_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWOD_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwod_w_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWOD_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwod_d_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWOD_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwod_q_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWOD_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwl_h_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWL_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwl_w_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWL_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwl_d_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWL_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwl_q_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWL_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwh_h_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWH_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwh_w_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWH_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwh_d_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWH_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwh_q_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWH_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwev_h_bu_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWEV_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwev_w_hu_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWEV_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwev_d_wu_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWEV_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwev_q_du_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWEV_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwod_h_bu_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWOD_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwod_w_hu_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWOD_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwod_d_wu_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWOD_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwod_q_du_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWOD_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwl_h_bu_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWL_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwl_w_hu_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWL_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwl_d_wu_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWL_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwl_q_du_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWL_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwh_h_bu_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWH_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwh_w_hu_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWH_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwh_d_wu_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWH_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaddwh_q_du_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMADDWH_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2_q_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2_hu_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2_HU_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2_wu_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2_WU_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2_du_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2_DU_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2_qu_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2_QU_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2_h_bu_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2_w_hu_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2_d_wu_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2_q_du_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2add_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2ADD_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2add_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2ADD_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2add_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2ADD_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2add_q_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2ADD_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2add_h_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2ADD_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2add_w_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2ADD_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2add_d_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2ADD_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2add_q_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2ADD_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2add_h_bu_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2ADD_H_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2add_w_hu_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2ADD_W_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2add_d_wu_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2ADD_D_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2add_q_du_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2ADD_Q_DU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2sub_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2SUB_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2sub_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2SUB_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2sub_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2SUB_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2sub_q_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2SUB_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2sub_h_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2SUB_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2sub_w_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2SUB_W_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2sub_d_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2SUB_D_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp2sub_q_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP2SUB_Q_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp4_w_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP4_W_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp4_d_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP4_D_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp4_q_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP4_Q_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp4_w_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP4_W_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp4_d_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP4_D_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp4_q_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP4_Q_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp4_w_bu_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP4_W_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp4_d_hu_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP4_D_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp4_q_wu_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP4_Q_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp4add_w_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP4ADD_W_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp4add_d_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP4ADD_D_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp4add_q_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP4ADD_Q_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp4add_w_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP4ADD_W_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp4add_d_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP4ADD_D_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp4add_q_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP4ADD_Q_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp4add_w_bu_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP4ADD_W_BU_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp4add_d_hu_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP4ADD_D_HU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdp4add_q_wu_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDP4ADD_Q_WU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdiv_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDIV_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdiv_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDIV_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdiv_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDIV_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdiv_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDIV_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmod_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMOD_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmod_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMOD_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmod_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMOD_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmod_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMOD_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdiv_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDIV_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdiv_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDIV_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdiv_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDIV_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvdiv_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVDIV_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmod_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMOD_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmod_hu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMOD_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmod_wu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMOD_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmod_du(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMOD_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsll_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLL_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsll_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLL_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsll_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLL_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsll_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLL_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrl_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRL_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrl_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRL_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrl_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRL_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrl_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRL_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsra_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRA_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsra_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRA_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsra_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRA_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsra_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRA_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvrotr_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVROTR_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvrotr_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVROTR_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvrotr_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVROTR_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvrotr_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVROTR_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrlr_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLR_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrlr_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLR_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrlr_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLR_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrlr_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLR_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrar_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRAR_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrar_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRAR_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrar_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRAR_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrar_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRAR_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrln_b_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLN_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrln_h_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLN_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrln_w_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLN_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsran_b_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRAN_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsran_h_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRAN_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsran_w_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRAN_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrlrn_b_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLRN_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrlrn_h_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLRN_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrlrn_w_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLRN_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrarn_b_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRARN_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrarn_h_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRARN_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrarn_w_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRARN_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrln_b_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLN_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrln_h_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLN_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrln_w_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLN_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssran_b_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRAN_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssran_h_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRAN_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssran_w_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRAN_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlrn_b_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLRN_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlrn_h_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLRN_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlrn_w_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLRN_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrarn_b_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRARN_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrarn_h_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRARN_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrarn_w_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRARN_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrln_bu_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLN_BU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrln_hu_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLN_HU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrln_wu_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLN_WU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssran_bu_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRAN_BU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssran_hu_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRAN_HU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssran_wu_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRAN_WU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlrn_bu_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLRN_BU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlrn_hu_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLRN_HU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlrn_wu_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLRN_WU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrarn_bu_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRARN_BU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrarn_hu_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRARN_HU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrarn_wu_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRARN_WU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitclr_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITCLR_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitclr_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITCLR_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitclr_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITCLR_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitclr_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITCLR_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitset_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITSET_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitset_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITSET_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitset_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITSET_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitset_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITSET_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitrev_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITREV_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitrev_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITREV_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitrev_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITREV_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitrev_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITREV_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbstrc12_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBSTRC12_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbstrc12_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBSTRC12_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbstrc12_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBSTRC12_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbstrc12_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBSTRC12_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbstrc21_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBSTRC21_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbstrc21_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBSTRC21_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbstrc21_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBSTRC21_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbstrc21_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBSTRC21_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpackev_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPACKEV_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpackev_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPACKEV_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpackev_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPACKEV_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpackev_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPACKEV_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpackod_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPACKOD_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpackod_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPACKOD_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpackod_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPACKOD_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpackod_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPACKOD_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvilvl_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVILVL_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvilvl_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVILVL_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvilvl_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVILVL_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvilvl_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVILVL_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvilvh_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVILVH_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvilvh_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVILVH_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvilvh_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVILVH_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvilvh_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVILVH_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpickev_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPICKEV_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpickev_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPICKEV_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpickev_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPICKEV_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpickev_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPICKEV_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpickod_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPICKOD_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpickod_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPICKOD_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpickod_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPICKOD_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpickod_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPICKOD_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvreplve_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVREPLVE_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvreplve_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVREPLVE_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvreplve_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVREPLVE_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvreplve_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVREPLVE_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvextrcol_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTRCOL_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvextrcol_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTRCOL_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvextrcol_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTRCOL_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvextrcol_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTRCOL_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvand_v(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVAND_V;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvor_v(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVOR_V;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvxor_v(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVXOR_V;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvnor_v(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVNOR_V;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvandn_v(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVANDN_V;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvorn_v(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVORN_V;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvrandsign_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVRANDSIGN_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvrandsign_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVRANDSIGN_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvrorsign_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVRORSIGN_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvrorsign_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVRORSIGN_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfrstp_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFRSTP_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfrstp_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFRSTP_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvclrstrr_v(IR2_OPND xd, IR2_OPND xj, IR2_OPND rk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVCLRSTRR_V;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = rk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvclrstrv_v(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVCLRSTRV_V;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvadd_q(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADD_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsub_q(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUB_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsigncov_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSIGNCOV_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsigncov_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSIGNCOV_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsigncov_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSIGNCOV_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsigncov_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSIGNCOV_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfadd_s(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFADD_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfadd_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFADD_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfsub_s(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFSUB_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfsub_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFSUB_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfaddsub_s(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFADDSUB_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfaddsub_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFADDSUB_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfsubadd_s(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFSUBADD_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfsubadd_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFSUBADD_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfmul_s(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFMUL_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfmul_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFMUL_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfdiv_s(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFDIV_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfdiv_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFDIV_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfmax_s(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFMAX_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfmax_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFMAX_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfmin_s(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFMIN_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfmin_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFMIN_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfmaxa_s(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFMAXA_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfmaxa_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFMAXA_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfmina_s(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFMINA_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfmina_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFMINA_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfscaleb_s(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFSCALEB_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfscaleb_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFSCALEB_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfcvt_h_s(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFCVT_H_S;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfcvt_s_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFCVT_S_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvffint_s_l(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFFINT_S_L;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftint_w_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINT_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftintrm_w_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINTRM_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftintrp_w_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINTRP_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftintrz_w_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINTRZ_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftintrne_w_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINTRNE_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhadd4_h_bu(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHADD4_H_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvshuf4_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSHUF4_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvshuf2_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSHUF2_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpmul_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPMUL_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpmul_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPMUL_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpmuh_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPMUH_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpmuh_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPMUH_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpmulacc_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPMULACC_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpmulacc_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPMULACC_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpmuhacc_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPMUHACC_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpmuhacc_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPMUHACC_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpmulwl_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPMULWL_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpmulwl_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPMULWL_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpmulwl_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPMULWL_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpmulwl_q_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPMULWL_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpmulwh_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPMULWH_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpmulwh_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPMULWH_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpmulwh_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPMULWH_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpmulwh_q_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPMULWH_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpmaddwl_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPMADDWL_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpmaddwl_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPMADDWL_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpmaddwl_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPMADDWL_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpmaddwl_q_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPMADDWL_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpmaddwh_h_b(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPMADDWH_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpmaddwh_w_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPMADDWH_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpmaddwh_d_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPMADDWH_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpmaddwh_q_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPMADDWH_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpdp2_q_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPDP2_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpdp2add_q_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPDP2ADD_Q_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvcdp4_re_d_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVCDP4_RE_D_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvcdp4_im_d_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVCDP4_IM_D_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvcdp4add_re_d_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVCDP4ADD_RE_D_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvcdp4add_im_d_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVCDP4ADD_IM_D_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvcdp2_re_q_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVCDP2_RE_Q_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvcdp2_im_q_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVCDP2_IM_Q_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvcdp2add_re_q_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVCDP2ADD_RE_Q_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvcdp2add_im_q_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVCDP2ADD_IM_Q_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsignsel_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSIGNSEL_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsignsel_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSIGNSEL_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvshuf_h(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSHUF_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvshuf_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSHUF_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvshuf_d(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSHUF_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvperm_w(IR2_OPND xd, IR2_OPND xj, IR2_OPND xk) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPERM_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    pir2->_opnd[2] = xk;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvseqi_b(IR2_OPND xd, IR2_OPND xj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSEQI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvseqi_h(IR2_OPND xd, IR2_OPND xj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSEQI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvseqi_w(IR2_OPND xd, IR2_OPND xj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSEQI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvseqi_d(IR2_OPND xd, IR2_OPND xj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSEQI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslei_b(IR2_OPND xd, IR2_OPND xj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLEI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslei_h(IR2_OPND xd, IR2_OPND xj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLEI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslei_w(IR2_OPND xd, IR2_OPND xj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLEI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslei_d(IR2_OPND xd, IR2_OPND xj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLEI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslei_bu(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLEI_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslei_hu(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLEI_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslei_wu(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLEI_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslei_du(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLEI_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslti_b(IR2_OPND xd, IR2_OPND xj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLTI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslti_h(IR2_OPND xd, IR2_OPND xj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLTI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslti_w(IR2_OPND xd, IR2_OPND xj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLTI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslti_d(IR2_OPND xd, IR2_OPND xj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLTI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslti_bu(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLTI_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslti_hu(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLTI_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslti_wu(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLTI_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslti_du(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLTI_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddi_bu(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDI_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddi_hu(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDI_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddi_wu(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDI_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvaddi_du(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVADDI_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubi_bu(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBI_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubi_hu(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBI_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubi_wu(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBI_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsubi_du(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSUBI_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbsll_v(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBSLL_V;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbsrl_v(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBSRL_V;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaxi_b(IR2_OPND xd, IR2_OPND xj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMAXI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaxi_h(IR2_OPND xd, IR2_OPND xj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMAXI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaxi_w(IR2_OPND xd, IR2_OPND xj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMAXI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaxi_d(IR2_OPND xd, IR2_OPND xj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMAXI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmini_b(IR2_OPND xd, IR2_OPND xj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMINI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmini_h(IR2_OPND xd, IR2_OPND xj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMINI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmini_w(IR2_OPND xd, IR2_OPND xj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMINI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmini_d(IR2_OPND xd, IR2_OPND xj, int imm_si5) {
    assert(imm_si5 <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMINI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_si5);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaxi_bu(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMAXI_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaxi_hu(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMAXI_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaxi_wu(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMAXI_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmaxi_du(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMAXI_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmini_bu(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMINI_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmini_hu(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMINI_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmini_wu(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMINI_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmini_du(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMINI_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvrandsigni_b(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVRANDSIGNI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvrandsigni_h(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVRANDSIGNI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvrorsigni_b(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVRORSIGNI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvrorsigni_h(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVRORSIGNI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfrstpi_b(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFRSTPI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfrstpi_h(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFRSTPI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvclrstri_v(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVCLRSTRI_V;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmepatmsk_v(IR2_OPND xd, int imm_mode, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMEPATMSK_V;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    IR2_OPND op0 = create_immh_opnd(imm_mode);
    pir2->_opnd[1] = op0;
    IR2_OPND op1 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op1;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvclo_b(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVCLO_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvclo_h(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVCLO_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvclo_w(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVCLO_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvclo_d(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVCLO_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvclz_b(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVCLZ_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvclz_h(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVCLZ_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvclz_w(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVCLZ_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvclz_d(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVCLZ_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpcnt_b(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPCNT_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpcnt_h(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPCNT_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpcnt_w(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPCNT_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpcnt_d(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPCNT_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvneg_b(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVNEG_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvneg_h(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVNEG_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvneg_w(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVNEG_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvneg_d(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVNEG_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmskltz_b(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMSKLTZ_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmskltz_h(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMSKLTZ_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmskltz_w(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMSKLTZ_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmskltz_d(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMSKLTZ_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmskgez_b(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMSKGEZ_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmsknz_b(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMSKNZ_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmskcopy_b(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMSKCOPY_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvmskfill_b(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVMSKFILL_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfrstm_b(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFRSTM_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfrstm_h(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFRSTM_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvseteqz_v(IR2_OPND cd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSETEQZ_V;
    pir2->op_count = 2;
    pir2->_opnd[0] = cd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsetnez_v(IR2_OPND cd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSETNEZ_V;
    pir2->op_count = 2;
    pir2->_opnd[0] = cd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsetanyeqz_b(IR2_OPND cd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSETANYEQZ_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = cd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsetanyeqz_h(IR2_OPND cd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSETANYEQZ_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = cd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsetanyeqz_w(IR2_OPND cd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSETANYEQZ_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = cd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsetanyeqz_d(IR2_OPND cd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSETANYEQZ_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = cd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsetallnez_b(IR2_OPND cd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSETALLNEZ_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = cd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsetallnez_h(IR2_OPND cd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSETALLNEZ_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = cd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsetallnez_w(IR2_OPND cd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSETALLNEZ_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = cd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsetallnez_d(IR2_OPND cd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSETALLNEZ_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = cd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvflogb_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFLOGB_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvflogb_d(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFLOGB_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfclass_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFCLASS_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfclass_d(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFCLASS_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfsqrt_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFSQRT_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfsqrt_d(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFSQRT_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfrecip_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFRECIP_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfrecip_d(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFRECIP_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfrsqrt_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFRSQRT_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfrsqrt_d(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFRSQRT_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfrint_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFRINT_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfrint_d(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFRINT_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfrintrm_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFRINTRM_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfrintrm_d(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFRINTRM_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfrintrp_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFRINTRP_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfrintrp_d(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFRINTRP_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfrintrz_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFRINTRZ_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfrintrz_d(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFRINTRZ_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfrintrne_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFRINTRNE_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfrintrne_d(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFRINTRNE_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvextl_w_b(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTL_W_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvextl_d_b(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTL_D_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvextl_d_h(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTL_D_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvextl_w_bu(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTL_W_BU;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvextl_d_bu(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTL_D_BU;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvextl_d_hu(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTL_D_HU;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhadd8_d_bu(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHADD8_D_BU;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhminpos_w_hu(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHMINPOS_W_HU;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhminpos_d_hu(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHMINPOS_D_HU;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhminpos_q_hu(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHMINPOS_Q_HU;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvclrtail_b(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVCLRTAIL_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvclrtail_h(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVCLRTAIL_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfcvtl_s_h(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFCVTL_S_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfcvth_s_h(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFCVTH_S_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfcvtl_d_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFCVTL_D_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvfcvth_d_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFCVTH_D_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvffint_s_w(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFFINT_S_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvffint_s_wu(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFFINT_S_WU;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvffint_d_l(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFFINT_D_L;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvffint_d_lu(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFFINT_D_LU;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvffintl_d_w(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFFINTL_D_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvffinth_d_w(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFFINTH_D_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftint_w_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINT_W_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftint_l_d(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINT_L_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftintrm_w_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINTRM_W_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftintrm_l_d(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINTRM_L_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftintrp_w_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINTRP_W_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftintrp_l_d(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINTRP_L_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftintrz_w_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINTRZ_W_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftintrz_l_d(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINTRZ_L_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftintrne_w_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINTRNE_W_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftintrne_l_d(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINTRNE_L_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftint_wu_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINT_WU_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftint_lu_d(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINT_LU_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftintrz_wu_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINTRZ_WU_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftintrz_lu_d(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINTRZ_LU_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftintl_l_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINTL_L_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftinth_l_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINTH_L_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftintrml_l_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINTRML_L_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftintrmh_l_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINTRMH_L_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftintrpl_l_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINTRPL_L_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftintrph_l_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINTRPH_L_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftintrzl_l_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINTRZL_L_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftintrzh_l_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINTRZH_L_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftintrnel_l_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINTRNEL_L_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvftintrneh_l_s(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVFTINTRNEH_L_S;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvexth_h_b(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTH_H_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvexth_w_h(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTH_W_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvexth_d_w(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTH_D_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvexth_q_d(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTH_Q_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvexth_hu_bu(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTH_HU_BU;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvexth_wu_hu(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTH_WU_HU;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvexth_du_wu(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTH_DU_WU;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvexth_qu_du(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTH_QU_DU;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvreplgr2vr_b(IR2_OPND xd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVREPLGR2VR_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvreplgr2vr_h(IR2_OPND xd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVREPLGR2VR_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvreplgr2vr_w(IR2_OPND xd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVREPLGR2VR_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvreplgr2vr_d(IR2_OPND xd, IR2_OPND rj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVREPLGR2VR_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = rj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vext2xv_h_b(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXT2XV_H_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vext2xv_w_b(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXT2XV_W_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vext2xv_d_b(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXT2XV_D_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vext2xv_w_h(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXT2XV_W_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vext2xv_d_h(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXT2XV_D_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vext2xv_d_w(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXT2XV_D_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vext2xv_hu_bu(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXT2XV_HU_BU;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vext2xv_wu_bu(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXT2XV_WU_BU;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vext2xv_du_bu(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXT2XV_DU_BU;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vext2xv_wu_hu(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXT2XV_WU_HU;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vext2xv_du_hu(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXT2XV_DU_HU;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_vext2xv_du_wu(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_VEXT2XV_DU_WU;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvhseli_d(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVHSELI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvrotri_b(IR2_OPND xd, IR2_OPND xj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVROTRI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvrotri_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVROTRI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvrotri_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVROTRI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvrotri_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVROTRI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrlri_b(IR2_OPND xd, IR2_OPND xj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLRI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrlri_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLRI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrlri_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLRI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrlri_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLRI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrari_b(IR2_OPND xd, IR2_OPND xj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRARI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrari_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRARI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrari_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRARI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrari_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRARI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvinsgr2vr_w(IR2_OPND xd, IR2_OPND rj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVINSGR2VR_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvinsgr2vr_d(IR2_OPND xd, IR2_OPND rj, int imm_ui2) {
    assert(imm_ui2 <= 0x3);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVINSGR2VR_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = rj;
    IR2_OPND op0 = create_immh_opnd(imm_ui2);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpickve2gr_w(IR2_OPND rd, IR2_OPND xj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPICKVE2GR_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpickve2gr_d(IR2_OPND rd, IR2_OPND xj, int imm_ui2) {
    assert(imm_ui2 <= 0x3);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPICKVE2GR_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui2);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpickve2gr_wu(IR2_OPND rd, IR2_OPND xj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPICKVE2GR_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpickve2gr_du(IR2_OPND rd, IR2_OPND xj, int imm_ui2) {
    assert(imm_ui2 <= 0x3);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPICKVE2GR_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = rd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui2);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvrepl128vei_b(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVREPL128VEI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvrepl128vei_h(IR2_OPND xd, IR2_OPND xj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVREPL128VEI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvrepl128vei_w(IR2_OPND xd, IR2_OPND xj, int imm_ui2) {
    assert(imm_ui2 <= 0x3);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVREPL128VEI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui2);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvrepl128vei_d(IR2_OPND xd, IR2_OPND xj, int imm_ui1) {
    assert(imm_ui1 <= 0x1);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVREPL128VEI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui1);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvextrcoli_b(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTRCOLI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvextrcoli_h(IR2_OPND xd, IR2_OPND xj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTRCOLI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvextrcoli_w(IR2_OPND xd, IR2_OPND xj, int imm_ui2) {
    assert(imm_ui2 <= 0x3);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTRCOLI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui2);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvextrcoli_d(IR2_OPND xd, IR2_OPND xj, int imm_ui1) {
    assert(imm_ui1 <= 0x1);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTRCOLI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui1);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvinsve0_w(IR2_OPND xd, IR2_OPND xj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVINSVE0_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvinsve0_d(IR2_OPND xd, IR2_OPND xj, int imm_ui2) {
    assert(imm_ui2 <= 0x3);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVINSVE0_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui2);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpickve_w(IR2_OPND xd, IR2_OPND xj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPICKVE_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpickve_d(IR2_OPND xd, IR2_OPND xj, int imm_ui2) {
    assert(imm_ui2 <= 0x3);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPICKVE_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui2);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvreplve0_b(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVREPLVE0_B;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvreplve0_h(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVREPLVE0_H;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvreplve0_w(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVREPLVE0_W;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvreplve0_d(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVREPLVE0_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvreplve0_q(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVREPLVE0_Q;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsllwil_h_b(IR2_OPND xd, IR2_OPND xj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLLWIL_H_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsllwil_w_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLLWIL_W_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsllwil_d_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLLWIL_D_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvextl_q_d(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTL_Q_D;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsllwil_hu_bu(IR2_OPND xd, IR2_OPND xj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLLWIL_HU_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsllwil_wu_hu(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLLWIL_WU_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsllwil_du_wu(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLLWIL_DU_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvextl_qu_du(IR2_OPND xd, IR2_OPND xj) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTL_QU_DU;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitclri_b(IR2_OPND xd, IR2_OPND xj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITCLRI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitclri_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITCLRI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitclri_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITCLRI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitclri_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITCLRI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitseti_b(IR2_OPND xd, IR2_OPND xj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITSETI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitseti_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITSETI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitseti_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITSETI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitseti_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITSETI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitrevi_b(IR2_OPND xd, IR2_OPND xj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITREVI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitrevi_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITREVI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitrevi_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITREVI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitrevi_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITREVI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbstrc12i_b(IR2_OPND xd, IR2_OPND xj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBSTRC12I_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbstrc12i_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBSTRC12I_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbstrc12i_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBSTRC12I_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbstrc12i_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBSTRC12I_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbstrc21i_b(IR2_OPND xd, IR2_OPND xj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBSTRC21I_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbstrc21i_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBSTRC21I_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbstrc21i_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBSTRC21I_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbstrc21i_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBSTRC21I_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsat_b(IR2_OPND xd, IR2_OPND xj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSAT_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsat_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSAT_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsat_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSAT_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsat_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSAT_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsat_bu(IR2_OPND xd, IR2_OPND xj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSAT_BU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsat_hu(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSAT_HU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsat_wu(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSAT_WU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsat_du(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    assert(imm_ui6 <= 0x3f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSAT_DU;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslli_b(IR2_OPND xd, IR2_OPND xj, int imm_ui3) {
    assert(imm_ui3 <= 0x7);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLLI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslli_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    assert(imm_ui4 <= 0xf);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLLI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslli_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    assert(imm_ui5l <= 0x1f);
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLLI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvslli_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSLLI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrli_b(IR2_OPND xd, IR2_OPND xj, int imm_ui3) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrli_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrli_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrli_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrai_b(IR2_OPND xd, IR2_OPND xj, int imm_ui3) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRAI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui3);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrai_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRAI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrai_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRAI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrai_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRAI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrlrneni_b_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLRNENI_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrlrneni_h_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLRNENI_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrlrneni_w_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLRNENI_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrlrneni_d_q(IR2_OPND xd, IR2_OPND xj, int imm_ui7) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLRNENI_D_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrarneni_b_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRARNENI_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrarneni_h_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRARNENI_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrarneni_w_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRARNENI_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrarneni_d_q(IR2_OPND xd, IR2_OPND xj, int imm_ui7) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRARNENI_D_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrlni_b_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLNI_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrlni_h_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLNI_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrlni_w_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLNI_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrlni_d_q(IR2_OPND xd, IR2_OPND xj, int imm_ui7) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLNI_D_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrlrni_b_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLRNI_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrlrni_h_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLRNI_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrlrni_w_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLRNI_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrlrni_d_q(IR2_OPND xd, IR2_OPND xj, int imm_ui7) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRLRNI_D_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlni_b_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLNI_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlni_h_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLNI_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlni_w_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLNI_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlni_d_q(IR2_OPND xd, IR2_OPND xj, int imm_ui7) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLNI_D_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlni_bu_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLNI_BU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlni_hu_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLNI_HU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlni_wu_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLNI_WU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlni_du_q(IR2_OPND xd, IR2_OPND xj, int imm_ui7) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLNI_DU_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlrni_b_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLRNI_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlrni_h_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLRNI_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlrni_w_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLRNI_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlrni_d_q(IR2_OPND xd, IR2_OPND xj, int imm_ui7) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLRNI_D_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlrni_bu_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLRNI_BU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlrni_hu_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLRNI_HU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlrni_wu_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLRNI_WU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlrni_du_q(IR2_OPND xd, IR2_OPND xj, int imm_ui7) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLRNI_DU_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrani_b_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRANI_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrani_h_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRANI_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrani_w_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRANI_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrani_d_q(IR2_OPND xd, IR2_OPND xj, int imm_ui7) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRANI_D_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrarni_b_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRARNI_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrarni_h_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRARNI_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrarni_w_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRARNI_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvsrarni_d_q(IR2_OPND xd, IR2_OPND xj, int imm_ui7) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSRARNI_D_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrani_b_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRANI_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrani_h_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRANI_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrani_w_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRANI_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrani_d_q(IR2_OPND xd, IR2_OPND xj, int imm_ui7) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRANI_D_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrani_bu_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRANI_BU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrani_hu_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRANI_HU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrani_wu_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRANI_WU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrani_du_q(IR2_OPND xd, IR2_OPND xj, int imm_ui7) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRANI_DU_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrarni_b_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRARNI_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrarni_h_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRARNI_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrarni_w_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRARNI_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrarni_d_q(IR2_OPND xd, IR2_OPND xj, int imm_ui7) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRARNI_D_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrarni_bu_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRARNI_BU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrarni_hu_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRARNI_HU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrarni_wu_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRARNI_WU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrarni_du_q(IR2_OPND xd, IR2_OPND xj, int imm_ui7) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRARNI_DU_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlrneni_b_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLRNENI_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlrneni_h_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLRNENI_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlrneni_w_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLRNENI_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlrneni_d_q(IR2_OPND xd, IR2_OPND xj, int imm_ui7) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLRNENI_D_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlrneni_bu_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLRNENI_BU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlrneni_hu_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLRNENI_HU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlrneni_wu_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLRNENI_WU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrlrneni_du_q(IR2_OPND xd, IR2_OPND xj, int imm_ui7) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRLRNENI_DU_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrarneni_b_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRARNENI_B_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrarneni_h_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRARNENI_H_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrarneni_w_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRARNENI_W_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrarneni_d_q(IR2_OPND xd, IR2_OPND xj, int imm_ui7) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRARNENI_D_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrarneni_bu_h(IR2_OPND xd, IR2_OPND xj, int imm_ui4) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRARNENI_BU_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui4);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrarneni_hu_w(IR2_OPND xd, IR2_OPND xj, int imm_ui5l) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRARNENI_HU_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui5l);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrarneni_wu_d(IR2_OPND xd, IR2_OPND xj, int imm_ui6) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRARNENI_WU_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui6);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvssrarneni_du_q(IR2_OPND xd, IR2_OPND xj, int imm_ui7) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSSRARNENI_DU_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui7);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvextrins_d(IR2_OPND xd, IR2_OPND xj, int imm_ui8) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTRINS_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvextrins_w(IR2_OPND xd, IR2_OPND xj, int imm_ui8) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTRINS_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvextrins_h(IR2_OPND xd, IR2_OPND xj, int imm_ui8) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTRINS_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvextrins_b(IR2_OPND xd, IR2_OPND xj, int imm_ui8) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVEXTRINS_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvshuf4i_b(IR2_OPND xd, IR2_OPND xj, int imm_ui8) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSHUF4I_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvshuf4i_h(IR2_OPND xd, IR2_OPND xj, int imm_ui8) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSHUF4I_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvshuf4i_w(IR2_OPND xd, IR2_OPND xj, int imm_ui8) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSHUF4I_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvshuf4i_d(IR2_OPND xd, IR2_OPND xj, int imm_ui8) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSHUF4I_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvshufi1_b(IR2_OPND xd, IR2_OPND xj, int imm_ui8) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSHUFI1_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvshufi2_b(IR2_OPND xd, IR2_OPND xj, int imm_ui8) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSHUFI2_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvshufi3_b(IR2_OPND xd, IR2_OPND xj, int imm_ui8) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSHUFI3_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvshufi4_b(IR2_OPND xd, IR2_OPND xj, int imm_ui8) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSHUFI4_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvshufi1_h(IR2_OPND xd, IR2_OPND xj, int imm_ui8) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSHUFI1_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvshufi2_h(IR2_OPND xd, IR2_OPND xj, int imm_ui8) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSHUFI2_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvseli_h(IR2_OPND xd, IR2_OPND xj, int imm_ui8) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSELI_H;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvseli_w(IR2_OPND xd, IR2_OPND xj, int imm_ui8) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSELI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvseli_d(IR2_OPND xd, IR2_OPND xj, int imm_ui8) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVSELI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitseli_b(IR2_OPND xd, IR2_OPND xj, int imm_ui8) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITSELI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitmvzi_b(IR2_OPND xd, IR2_OPND xj, int imm_ui8) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITMVZI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvbitmvnzi_b(IR2_OPND xd, IR2_OPND xj, int imm_ui8) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVBITMVNZI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvandi_b(IR2_OPND xd, IR2_OPND xj, int imm_ui8) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVANDI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvori_b(IR2_OPND xd, IR2_OPND xj, int imm_ui8) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVORI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvxori_b(IR2_OPND xd, IR2_OPND xj, int imm_ui8) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVXORI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvnori_b(IR2_OPND xd, IR2_OPND xj, int imm_ui8) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVNORI_B;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvldi(IR2_OPND xd, int imm_i13) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVLDI;
    pir2->op_count = 2;
    pir2->_opnd[0] = xd;
    IR2_OPND op0 = create_immh_opnd(imm_i13);
    pir2->_opnd[1] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpermi_w(IR2_OPND xd, IR2_OPND xj, int imm_ui8) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPERMI_W;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpermi_d(IR2_OPND xd, IR2_OPND xj, int imm_ui8) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPERMI_D;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

IR2_INST *la_xvpermi_q(IR2_OPND xd, IR2_OPND xj, int imm_ui8) {
    IR2_INST *pir2 = ir2_allocate();
    pir2->_opcode = LISA_XVPERMI_Q;
    pir2->op_count = 3;
    pir2->_opnd[0] = xd;
    pir2->_opnd[1] = xj;
    IR2_OPND op0 = create_immh_opnd(imm_ui8);
    pir2->_opnd[2] = op0;
    ir2_append(pir2);
    lsassertm(ir2_op_check(pir2), "Maybe you should check the type of operand");
    return pir2;
}

