#ifndef _TU_H_
#define _TU_H_

#ifdef CONFIG_LATA_TU

#define MAX_TB_IN_TU 50
#define MAX_TB_IN_CACHE (MAX_TB_IN_TU + 10)
#define MAX_TB_SIZE 10240
#define MAX_TU_SIZE (MAX_TB_IN_TU * MAX_TB_SIZE)

/* TUControl manages the translation of TU */
typedef struct TUControl {
    /* the numbers of TB in current TU */
    uint32_t tb_num;
    /* current TB in TU */
    uint32_t curr_num;
    /* current TB insns in TU */
    uint32_t curr_insns;
    /* all ir1 number in current TU, which is used for calculate ir1 offset */
    uint32_t ir1_num_in_tu;
    /* all TB pointer in current TU  */
    TranslationBlock *tb_list[MAX_TB_IN_CACHE];
    /* Search GPC to TB in current TU  */
    GTree *tree;

} TUControl;

typedef enum TU_TB_START_TYPE {
    TU_TB_START_NONE = 0,
    TU_TB_START_JMP,
    TU_TB_START_NORMAL,
    TU_TB_START_ENTRY
} TU_TB_START_TYPE;

extern __thread TUControl *tu_data;

void tu_control_init(void);

TranslationBlock *tu_gen_code(CPUState *cpu, uint64_t pc,
                              uint64_t cs_base, uint32_t flags,
                              int cflags);
void tu_aarch64_optimization(struct TranslationBlock *tb);
int translate_tb_in_tu(struct TranslationBlock *tb);
void lata_pre_translate(void** list, int num, CPUState *cpu,
                        uint64_t cs_base, uint32_t flags, uint32_t cflags);
TranslationBlock *tu_tree_lookup(target_ulong pc);
void solve_tb_overlap(TranslationBlock **tb_list, int max_insns);
void translate_tu(TranslationBlock **tb_list);
void tu_relocat_branch(TranslationBlock * tb, int n);

#endif
#endif