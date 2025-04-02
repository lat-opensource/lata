#ifndef _INSTS_PATTERN_H_
#define _INSTS_PATTERN_H_

// typedef void AArch64Pattern(DisasContext *s, uint32_t insn);

typedef enum dt_aarch64_insn
{
    INVALID = 0,
    NZCV_GENERATE,
    CMP_IMM,
    CMP_EREG,
    CMP_SREG,
    B_EQ,
    B_NE,
    B_CS,
    B_CC,
    B_HI,
    B_LS,
    B_GE,
    B_LT,
    B_GT,
    B_LE,
    B_AL,
    B_NV,
} dt_aarch64_insn;

typedef struct AArch64PatternTable
{
    uint32_t pattern;
    uint32_t mask;
    dt_aarch64_insn aarch_pattern;
} AArch64PatternTable;

bool insts_pattern(DisasContext *s, DisasContext *s2);
void nzcv_use(TranslationBlock *tb, uint32_t insn);
#endif