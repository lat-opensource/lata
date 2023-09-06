#ifndef _MACRO_INST_H_
#define _MACRO_INST_H_

/* Load immediate */
void li_d(IR2_OPND opnd2, int64_t value);
void li_w(IR2_OPND opnd2, uint32_t lo32);
void li_wu(IR2_OPND opnd2, uint32_t lo32);

#ifdef TARGET_RISCV64
#define li_addr(opnd2, value)   li_d(opnd2, value)
#else
#define li_addr(opnd2, value)   li_wu(opnd2, value)
#endif

/* load/store by opnd size */
void la_ld_by_op_size(IR2_OPND rd, IR2_OPND rj, int imm_si12, int op_size);
void la_st_by_op_size(IR2_OPND rd, IR2_OPND rj, int imm_si12, int op_size);
#endif
