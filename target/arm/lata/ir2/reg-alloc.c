#include <string.h>
#include "reg-alloc.h"
#include "ir2.h"
// #include "translate.h"
// #include "common.h"

/*
 * Init in global_register_init()
 */
IR2_OPND env_ir2_opnd;
IR2_OPND zero_ir2_opnd;
IR2_OPND ra_ir2_opnd;
IR2_OPND tp_ir2_opnd;
IR2_OPND sp_ir2_opnd;
IR2_OPND a0_ir2_opnd;
IR2_OPND a1_ir2_opnd;
IR2_OPND a2_ir2_opnd;
IR2_OPND a3_ir2_opnd;
IR2_OPND a4_ir2_opnd;
IR2_OPND a5_ir2_opnd;
IR2_OPND a6_ir2_opnd;
IR2_OPND a7_ir2_opnd;
IR2_OPND t0_ir2_opnd;
IR2_OPND t1_ir2_opnd;
IR2_OPND t2_ir2_opnd;
IR2_OPND t3_ir2_opnd;
IR2_OPND t4_ir2_opnd;
IR2_OPND t5_ir2_opnd;
IR2_OPND t6_ir2_opnd;
IR2_OPND t7_ir2_opnd;
IR2_OPND t8_ir2_opnd;
IR2_OPND r21_ir2_opnd;
IR2_OPND fp_ir2_opnd;
IR2_OPND s0_ir2_opnd;
IR2_OPND s1_ir2_opnd;
IR2_OPND s2_ir2_opnd;
IR2_OPND s3_ir2_opnd;
IR2_OPND s4_ir2_opnd;
IR2_OPND s5_ir2_opnd;
IR2_OPND s6_ir2_opnd;
IR2_OPND s7_ir2_opnd;
IR2_OPND s8_ir2_opnd;
IR2_OPND fsmask_ir2_opnd;
IR2_OPND fcsr_ir2_opnd;
IR2_OPND fcsr1_ir2_opnd;
IR2_OPND fcsr2_ir2_opnd;
IR2_OPND fcsr3_ir2_opnd;
IR2_OPND fcc0_ir2_opnd;
IR2_OPND fcc1_ir2_opnd;
IR2_OPND fcc2_ir2_opnd;
IR2_OPND fcc3_ir2_opnd;
IR2_OPND fcc4_ir2_opnd;
IR2_OPND fcc5_ir2_opnd;
IR2_OPND fcc6_ir2_opnd;
IR2_OPND fcc7_ir2_opnd;

IR2_OPND scr0_ir2_opnd;
IR2_OPND scr1_ir2_opnd;
IR2_OPND scr2_ir2_opnd;
IR2_OPND scr3_ir2_opnd;

/*
 * LA opnd mapping table
 */
const uint64 la_ir2_opnd_tab[] = {
    /* {reg_index, opnd addr} */
    REG_MAP_DEF(la_zero, (uint64)&zero_ir2_opnd),
    REG_MAP_DEF(la_ra, (uint64)&ra_ir2_opnd),
    REG_MAP_DEF(la_tp, (uint64)&tp_ir2_opnd),
    REG_MAP_DEF(la_sp, (uint64)&sp_ir2_opnd),
    REG_MAP_DEF(la_a0, (uint64)&a0_ir2_opnd),
    REG_MAP_DEF(la_a1, (uint64)&a1_ir2_opnd),
    REG_MAP_DEF(la_a2, (uint64)&a2_ir2_opnd),
    REG_MAP_DEF(la_a3, (uint64)&a3_ir2_opnd),
    REG_MAP_DEF(la_a4, (uint64)&a4_ir2_opnd),
    REG_MAP_DEF(la_a5, (uint64)&a5_ir2_opnd),
    REG_MAP_DEF(la_a6, (uint64)&a6_ir2_opnd),
    REG_MAP_DEF(la_a7, (uint64)&a7_ir2_opnd),
    REG_MAP_DEF(la_t0, (uint64)&t0_ir2_opnd),
    REG_MAP_DEF(la_t1, (uint64)&t1_ir2_opnd),
    REG_MAP_DEF(la_t2, (uint64)&t2_ir2_opnd),
    REG_MAP_DEF(la_t3, (uint64)&t3_ir2_opnd),
    REG_MAP_DEF(la_t4, (uint64)&t4_ir2_opnd),
    REG_MAP_DEF(la_t5, (uint64)&t5_ir2_opnd),
    REG_MAP_DEF(la_t6, (uint64)&t6_ir2_opnd),
    REG_MAP_DEF(la_t7, (uint64)&t7_ir2_opnd),
    REG_MAP_DEF(la_t8, (uint64)&t8_ir2_opnd),
    REG_MAP_DEF(la_r21, (uint64)&r21_ir2_opnd),
    REG_MAP_DEF(la_fp, (uint64)&fp_ir2_opnd),
    REG_MAP_DEF(la_s0, (uint64)&s0_ir2_opnd),
    REG_MAP_DEF(la_s1, (uint64)&s1_ir2_opnd),
    REG_MAP_DEF(la_s2, (uint64)&s2_ir2_opnd),
    REG_MAP_DEF(la_s3, (uint64)&s3_ir2_opnd),
    REG_MAP_DEF(la_s4, (uint64)&s4_ir2_opnd),
    REG_MAP_DEF(la_s5, (uint64)&s5_ir2_opnd),
    REG_MAP_DEF(la_s6, (uint64)&s6_ir2_opnd),
    REG_MAP_DEF(la_s7, (uint64)&s7_ir2_opnd),
    REG_MAP_DEF(la_s7, (uint64)&s7_ir2_opnd),
};

/*
 * Register mapping table
 */
const int reg_gpr_map[] = {
    REG_MAP_DEF(eax_index, la_s0),
    REG_MAP_DEF(ecx_index, la_s1),
    REG_MAP_DEF(edx_index, la_s2),
    REG_MAP_DEF(ebx_index, la_s3),
    REG_MAP_DEF(esp_index, la_s4),
    REG_MAP_DEF(ebp_index, la_s5),
    REG_MAP_DEF(esi_index, la_s6),
    REG_MAP_DEF(edi_index, la_s7),
#ifdef TARGET_X86_64
    REG_MAP_DEF(r8_index, la_a2),
    REG_MAP_DEF(r9_index, la_a3),
    REG_MAP_DEF(r10_index, la_a4),
    REG_MAP_DEF(r11_index, la_a5),
    REG_MAP_DEF(r12_index, la_t5),
    REG_MAP_DEF(r13_index, la_t6),
    REG_MAP_DEF(r14_index, la_t7),
    REG_MAP_DEF(r15_index, la_t8),
#endif
};

const int reg_fpr_map[] = {
    REG_MAP_DEF(0, 0),
    REG_MAP_DEF(1, 1),
    REG_MAP_DEF(2, 2),
    REG_MAP_DEF(3, 3),
    REG_MAP_DEF(4, 4),
    REG_MAP_DEF(5, 5),
    REG_MAP_DEF(6, 6),
    REG_MAP_DEF(7, 7),
};
#define reg_mmx_map reg_fpr_map

const int reg_xmm_map[] = {
    REG_MAP_DEF(0, 16),
    REG_MAP_DEF(1, 17),
    REG_MAP_DEF(2, 18),
    REG_MAP_DEF(3, 19),
    REG_MAP_DEF(4, 20),
    REG_MAP_DEF(5, 21),
    REG_MAP_DEF(6, 22),
    REG_MAP_DEF(7, 23),
#ifdef TARGET_X86_64
    REG_MAP_DEF(8, 24),
    REG_MAP_DEF(9, 25),
    REG_MAP_DEF(10, 26),
    REG_MAP_DEF(11, 27),
    REG_MAP_DEF(12, 28),
    REG_MAP_DEF(13, 29),
    REG_MAP_DEF(14, 30),
    REG_MAP_DEF(15, 31),
#endif
};

const int reg_statics_map[] = {
    /* last exec tb */
    REG_MAP_DEF(S_UD0, la_fp),
    /* next x86 addr */
    REG_MAP_DEF(S_UD1, la_r21),
#ifndef TARGET_X86_64
    /* we don't use shadow stack */
    REG_MAP_DEF(S_UD2, NOT_DEF),
#endif
    REG_MAP_DEF(S_ENV, la_s8),
    REG_MAP_DEF(S_EFLAGS, la_a6),
};

const int reg_trace_map[] = {
    REG_MAP_DEF(TRACE, NOT_DEF),
};

const int reg_itemp_map[] = {
    REG_MAP_DEF(ITEMP0, la_t0),
    REG_MAP_DEF(ITEMP1, la_t1),
    REG_MAP_DEF(ITEMP2, la_t2),
    REG_MAP_DEF(ITEMP3, la_t3),
    REG_MAP_DEF(ITEMP4, la_t4),
    REG_MAP_DEF(ITEMP5, la_t5),
    // REG_MAP_DEF(ITEMP6, la_t6),
    // REG_MAP_DEF(ITEMP7, la_t7),
    // REG_MAP_DEF(ITEMP8, la_t8),
    // REG_MAP_DEF(ITEMP9, la_ra),
    // REG_MAP_DEF(ITEMP10, la_a7),
};

const int reg_itemp_reverse_map[32] = {
    REG_MAP_DEF(0 ... 31, INVALID_TEMP),
    REG_MAP_DEF(la_t0, ITEMP0),
    REG_MAP_DEF(la_t1, ITEMP1),
    REG_MAP_DEF(la_t2, ITEMP2),
    REG_MAP_DEF(la_t3, ITEMP3),
    REG_MAP_DEF(la_t4, ITEMP4),
    REG_MAP_DEF(la_t5, ITEMP5),
    // REG_MAP_DEF(la_t6, ITEMP6),
    // REG_MAP_DEF(la_t7, ITEMP7),
    // REG_MAP_DEF(la_t8, ITEMP8),
    // REG_MAP_DEF(la_ra, ITEMP9),
    // REG_MAP_DEF(la_a7, ITEMP10),
};

const int reg_ftemp_map[] = {
    REG_MAP_DEF(FTEMP0, 9),
    REG_MAP_DEF(FTEMP1, 10),
    REG_MAP_DEF(FTEMP2, 11),
    REG_MAP_DEF(FTEMP3, 12),
    REG_MAP_DEF(FTEMP4, 13),
    REG_MAP_DEF(FTEMP5, 14),
    // REG_MAP_DEF(FTEMP6, 15),
};

const int reg_ftemp_reverse_map[32] = {
    REG_MAP_DEF(0 ... 31, INVALID_TEMP),
    REG_MAP_DEF(9, FTEMP0),
    REG_MAP_DEF(10, FTEMP1),
    REG_MAP_DEF(11, FTEMP2),
    REG_MAP_DEF(12, FTEMP3),
    REG_MAP_DEF(13, FTEMP4),
    REG_MAP_DEF(14, FTEMP5),
    // REG_MAP_DEF(15, FTEMP6),
    // REG_MAP_DEF(16 ... 31, INVALID_TEMP),
};

const int reg_scr_map[] = {
    REG_MAP_DEF(0, 0),
    REG_MAP_DEF(1, 1),
    REG_MAP_DEF(2, 2),
    REG_MAP_DEF(3, 3),
};

/*
 * Base interface
 */
IR2_OPND get_la_ir2_opnd(int la_reg_num)
{
    return *(IR2_OPND *)la_ir2_opnd_tab[la_reg_num];
}

/* ra_alloc_gpr(int index) */
RA_ALLOC_FUNC(gpr, IR2_OPND_GPR)
/* ra_alloc_fpr(int index) */
RA_ALLOC_FUNC(fpr, IR2_OPND_FPR)
/* ra_alloc_mmx(int index) */
RA_ALLOC_FUNC(mmx, IR2_OPND_FPR)
/* ra_alloc_xmm(int index) */
RA_ALLOC_FUNC(xmm, IR2_OPND_FPR)
/* ra_alloc_statics */
RA_ALLOC_FUNC(statics, IR2_OPND_GPR)
/* ra_alloc_trace */
RA_ALLOC_FUNC(trace, IR2_OPND_GPR)
/* ra_alloc_src */
RA_ALLOC_FUNC(scr, IR2_OPND_SCR);
/* ra_alloc_itemp_num() */
RA_ALLOC_TEMP_NUM_FUNC(itemp)
/* ra_alloc_ftemp_num() */
RA_ALLOC_TEMP_NUM_FUNC(ftemp)

/*
 * Some encapsulation of base interface
 */
IR2_OPND ra_alloc_st(int st_num)
{
    return ra_alloc_fpr(st_num);
}

IR2_OPND ra_alloc_itemp(void)
{
    IR2_OPND ir2_opnd;
    int itemp_num;

    itemp_num = ra_alloc_itemp_num();

    ir2_opnd_build(&ir2_opnd, IR2_OPND_GPR, itemp_num);

    return ir2_opnd;
}

IR2_OPND ra_alloc_ftemp(void)
{
    IR2_OPND ir2_opnd;
    ir2_opnd_build(&ir2_opnd, IR2_OPND_FPR, ra_alloc_ftemp_num());
    return ir2_opnd;
}

IR2_OPND ra_alloc_num_4095(void)
{
    IR2_OPND num = ra_alloc_itemp();
    lsassertm(0, "ori do not generate");
    // la_ori(num, zero_ir2_opnd, 0xfff);
    return num;
}

inline __attribute__((__always_inline__)) void ra_free_num_4095(IR2_OPND opnd)
{
    ra_free_itemp(opnd._reg_num);
}

inline __attribute__((__always_inline__)) void ra_free_all(void)
{
    tr_data->all_temp_status = 0;
}

/*
 * Future patches may remove it.
 */
IR2_OPND ra_alloc_mda(void)
{
    return ra_ir2_opnd;
}
