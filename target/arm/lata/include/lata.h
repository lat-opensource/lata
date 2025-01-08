#ifndef _LATA_H_
#define _LATA_H_

#include <stdint.h>

#include "qemu/osdep.h"
#include "mem.h"
#include "ir2.h"
#include "la-append.h"
#include "reg-alloc.h"
#include "macro-inst.h"

#include "tcg/tcg.h"
//#include "env.h"



#define B_STUB_SIZE 4

typedef struct TRANSLATION_DATA {

    void *curr_tb; /* from QEMU */

    /* ir1 */
    // IR1_INST *curr_ir1_inst;
    // int curr_ir1_count;
    /* uint8       ir1_dump_threshold[MAX_IR1_NUM_PER_TB]; */

    /* ir2 */
    IR2_INST *ir2_inst_array;
    int ir2_inst_num_max;
    int ir2_inst_num_current;
    int real_ir2_inst_num;

    /* the list of ir2 */
    IR2_INST *first_ir2;
    IR2_INST *last_ir2;

    /* label number */
    int label_num;

    /* temp reg */
    union {
        uint32 all_temp_status;
        struct {
            uint16 itemp_status;
            uint16 ftemp_status;
        };
    };

    /* TODO : support static translation */
    uint8 curr_ir1_skipped_eflags; /* these eflag calculation can be skipped */
                                   /* (because of flag pattern, etc) */
} TRANSLATION_DATA;




typedef struct ENV {
    void *cpu_state;            /* from QEMU,CPUArchState */
    TRANSLATION_DATA *tr_data;  /* from LATR */
} ENV;


void lata_tr_data_init(void);
void tr_init(void*);
void tr_fini(void);

extern const int arm_la_map[];
extern __thread TRANSLATION_DATA *tr_data;
extern __thread ENV *lsenv;
extern bool lata_dump;

extern uint64_t context_switch_bt_to_native;
extern uint64_t context_switch_native_to_bt_ret_0;
extern uint64_t context_switch_native_to_bt;

int lata_gen_prologue(CPUState *cs, TCGContext *tcg_ctx);
int lata_gen_epilogue(CPUState *cs, TCGContext *tcg_ctx);


int tr_ir2_assemble(const void *code_start_addr);
// int INS_translate(CPUState *cs, INS INS);
// void INS_append_exit(INS INS, uint32_t index);

void lata_gen_call_helper_prologue(TCGContext *tcg_ctx);
void lata_gen_call_helper_epilogue(TCGContext *tcg_ctx);

IR2_OPND alloc_gpr_src(int i);
IR2_OPND alloc_gpr_src_sp(int i);
IR2_OPND alloc_gpr_dst(int i);
IR2_OPND alloc_gpr_dst_sp(int i);
void store_gpr_dst(int i, IR2_OPND opnd);
void store_gpr_zero(int i);
void free_alloc_gpr(IR2_OPND opnd);

IR2_OPND alloc_fpr_src(int i);
IR2_OPND alloc_fpr_dst(int i);
void store_fpr_dst(int i, IR2_OPND opnd);
void free_alloc_fpr(IR2_OPND opnd);

#endif
