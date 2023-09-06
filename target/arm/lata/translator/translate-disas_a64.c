#include "lata.h"
#include "env.h"
#include "../include/translate.h"

bool translate_svc(DisasContext *ctx,uint32_t excp, uint32_t syndrome)
{
    // IR2_OPND t = ra_alloc_itemp();
    // li_d(t, ctx->base.pc_next);
    // la_st_d(t , env_ir2_opnd, env_offset_pc());
    // latr_gen_call_helper_prologue(tcg_ctx);
    // la_mov64(a0_ir2_opnd, env_ir2_opnd);
    // li_d(t, RISCV_EXCP_U_ECALL);
    // la_mov64(a1_ir2_opnd, t);
    // li_d(t, helper_raise_exception);
    // la_jirl(ra_ir2_opnd, t, 0);

    // ctx->base.is_jmp = DISAS_NORETURN;
    // return true;

    IR2_OPND t = ra_alloc_itemp();
    li_d(t, ctx->base.pc_next);
    la_st_d(t,env_ir2_opnd,env_offset_pc());
    lata_gen_call_helper_prologue(tcg_ctx);
    //CPUARMState *env, uint32_t excp,
    la_mov64(a0_ir2_opnd, env_ir2_opnd);

    li_d(a1_ir2_opnd, excp);
    li_d(a2_ir2_opnd, syndrome);
    //li_d(a3_ir2_opnd, target_el);
    li_d(t, (uint64_t)helper_exception_with_syndrome);
    la_jirl(ra_ir2_opnd, t, 0);
    ctx->base.is_jmp = DISAS_NORETURN;
    return true;
}

bool translate_movz(int rd ,uint64_t imm)
{
    IR2_OPND dst = alloc_gpr_dst(rd);
    //la_and(dst,dst,zero_ir2_opnd);
    li_d(dst,imm);
    return true;
}

bool translate_ldr_lit(int rd ,target_ulong addr, MemOp op)
{
    IR2_OPND dst = alloc_gpr_dst(rd);
    //la_and(dst,dst,zero_ir2_opnd);

    int lo12 = addr % 4096;
    int hi12 = addr /4096;
    la_lu12i_w(dst,hi12);
    switch (op)
    {
    case MO_64:
        la_ld_d(dst,dst,lo12);
        break;
    default:
        printf("ldr_lit not processed this op.\n");
        break;
    }
    
    return true;
}

void trans_uncond_b_imm(DisasContext *s, uint32_t insn)
{
    // uint64_t addr = s->pc_curr + sextract32(insn, 0, 26) * 4;

    // if (insn & (1U << 31)) {
    //     /* BL Branch with link */
    //     //tcg_gen_movi_i64(cpu_reg(s, 30), s->base.pc_next);
    //     translate_movz(30,s->base.pc_next);
    // }

    /* B Branch / BL Branch with link */
    // reset_btype(s);
    // gen_goto_tb(s, 0, addr);
}