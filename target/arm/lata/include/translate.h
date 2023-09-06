#ifndef _TRANSLATE_H_
#define _TRANSLATE_H_
#include "target/arm/tcg/translate.h"
#include "include/exec/helper-proto.h"
#include "../include/lata.h"
void trans_movw_imm(DisasContext *s, uint32_t insn);
void trans_ld_lit(DisasContext *s, uint32_t insn);
void trans_uncond_b_imm(DisasContext *s, uint32_t insn);
bool translate_movz(int rd ,uint64_t imm);
bool translate_ldr_lit(int rd ,target_ulong addr, MemOp op);
bool translate_svc(DisasContext *ctx,uint32_t excp, uint32_t syndrome);
#endif