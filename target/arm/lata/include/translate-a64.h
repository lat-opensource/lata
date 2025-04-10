#ifndef TARGET_ARM_TRANSLATE_A64_H
#define TARGET_ARM_TRANSLATE_A64_H

bool logic_imm_decode_wmask(uint64_t *result, unsigned int immn,
                            unsigned int imms, unsigned int immr);
bool sve_access_check(DisasContext *s);

DisasContext *get_ir1_list(CPUState *cpu, TranslationBlock *tb, vaddr pc, int max_insns);
void target_disasm(struct TranslationBlock *tb, int *max_insns, CPUState *cpu);
bool tr_ir2_generate(struct TranslationBlock *tb);
void translate_aarch64_insn(DisasContext *s, CPUState *cpu);
#ifdef CONFIG_LATA_TU
void get_last_info(TranslationBlock *tb, DisasContext *s);
#endif

static inline void assert_fp_access_checked(DisasContext *s)
{
#ifdef CONFIG_DEBUG_TCG
    if (unlikely(!s->fp_access_checked || s->fp_excp_el)) {
        fprintf(stderr, "target-arm: FP access check missing for "
                "instruction 0x%08x\n", s->insn);
        abort();
    }
#endif
}

/* Return the byte size of the "whole" vector register, VL / 8.  */
static inline int vec_full_reg_size(DisasContext *s)
{
    return s->vl;
}


/* Return the offset info CPUARMState of the "whole" vector register Qn.  */
static inline int vec_full_reg_offset(DisasContext *s, int regno)
{
    assert_fp_access_checked(s);
    return offsetof(CPUARMState, vfp.zregs[regno]);
}

#endif /* TARGET_ARM_TRANSLATE_A64_H */
