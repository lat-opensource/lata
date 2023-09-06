#include "lata.h"
#include "../include/translate.h"
static bool fp_access_check_only(DisasContext *s)
{
    if (s->fp_excp_el) {
        assert(!s->fp_access_checked);
        s->fp_access_checked = true;

        gen_exception_insn_el(s, 0, EXCP_UDEF,
                              syn_fp_access_trap(1, 0xe, false, 0),
                              s->fp_excp_el);
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
        gen_exception_insn(s, 0, EXCP_UDEF,
                           syn_smetrap(SME_ET_Streaming, false));
        return false;
    }
    return true;
}

//#include "target/arm/translate-a64.c"
/*
 * Load register (literal)
 *
 *  31 30 29   27  26 25 24 23                5 4     0
 * +-----+-------+---+-----+-------------------+-------+
 * | opc | 0 1 1 | V | 0 0 |     imm19         |  Rt   |
 * +-----+-------+---+-----+-------------------+-------+
 *
 * V: 1 -> vector (simd/fp)
 * opc (non-vector): 00 -> 32 bit, 01 -> 64 bit,
 *                   10-> 32 bit signed, 11 -> prefetch
 * opc (vector): 00 -> 32 bit, 01 -> 64 bit, 10 -> 128 bit (11 unallocated)
 */
void trans_ld_lit(DisasContext *s, uint32_t insn)
{
    int rt = extract32(insn, 0, 5);
    int64_t imm = sextract32(insn, 5, 19) << 2;
    bool is_vector = extract32(insn, 26, 1);
    int opc = extract32(insn, 30, 2);
    bool is_signed = false;
    int size = 2;
    //TCGv_i64 tcg_rt, clean_addr;

    if (is_vector) {
        if (opc == 3) {
            unallocated_encoding(s);
            return;
        }
        size = 2 + opc;
        if (!fp_access_check(s)) {
            return;
        }
    } else {
        if (opc == 3) {
            /* PRFM (literal) : prefetch */
            return;
        }
        size = 2 + extract32(opc, 0, 1);
        is_signed = extract32(opc, 1, 1);
    }

    //tcg_rt = cpu_reg(s, rt);

    //clean_addr = tcg_const_i64(s->pc_curr + imm);
    if (is_vector) {
        //do_fp_ld(s, rt, clean_addr, size);
    } else {
        /* Only unsigned 32bit loads target 32bit registers.  */
        //bool iss_sf = opc != 0;
        //#ifdef CONFIG_LATA
        translate_ldr_lit(rt,s->pc_curr + imm,size + is_signed * MO_SIGN);
        // #else
        // do_gpr_ld(s, tcg_rt, clean_addr, size + is_signed * MO_SIGN,
        //           false, true, rt, iss_sf, false);
        // #endif
    }
    //tcg_temp_free_i64(clean_addr);
}


