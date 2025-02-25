#ifndef _REG_ALLOC_H_
#define _REG_ALLOC_H_

#include "ir2.h"
#include "reg-map.h"
#include "stdio.h"
#include "lata.h"
//extern __thread TRANSLATION_DATA *tr_data;

extern IR2_OPND env_ir2_opnd;
extern IR2_OPND zero_ir2_opnd;
extern IR2_OPND ra_ir2_opnd;
extern IR2_OPND tp_ir2_opnd;
extern IR2_OPND sp_ir2_opnd;
extern IR2_OPND a0_ir2_opnd;
extern IR2_OPND a1_ir2_opnd;
extern IR2_OPND a2_ir2_opnd;
extern IR2_OPND a3_ir2_opnd;
extern IR2_OPND a4_ir2_opnd;
extern IR2_OPND a5_ir2_opnd;
extern IR2_OPND a6_ir2_opnd;
extern IR2_OPND a7_ir2_opnd;
extern IR2_OPND t0_ir2_opnd;
extern IR2_OPND t1_ir2_opnd;
extern IR2_OPND t2_ir2_opnd;
extern IR2_OPND t3_ir2_opnd;
extern IR2_OPND t4_ir2_opnd;
extern IR2_OPND t5_ir2_opnd;
extern IR2_OPND t6_ir2_opnd;
extern IR2_OPND t7_ir2_opnd;
extern IR2_OPND t8_ir2_opnd;
extern IR2_OPND r21_ir2_opnd;
extern IR2_OPND fp_ir2_opnd;
extern IR2_OPND s0_ir2_opnd;
extern IR2_OPND s1_ir2_opnd;
extern IR2_OPND s2_ir2_opnd;
extern IR2_OPND s3_ir2_opnd;
extern IR2_OPND s4_ir2_opnd;
extern IR2_OPND s5_ir2_opnd;
extern IR2_OPND s6_ir2_opnd;
extern IR2_OPND s7_ir2_opnd;
extern IR2_OPND s8_ir2_opnd;
extern IR2_OPND fsmask_ir2_opnd;
extern IR2_OPND fcsr_ir2_opnd;
extern IR2_OPND fcsr1_ir2_opnd;
extern IR2_OPND fcsr2_ir2_opnd;
extern IR2_OPND fcsr3_ir2_opnd;
extern IR2_OPND fcc0_ir2_opnd;
extern IR2_OPND fcc1_ir2_opnd;
extern IR2_OPND fcc2_ir2_opnd;
extern IR2_OPND fcc3_ir2_opnd;
extern IR2_OPND fcc4_ir2_opnd;
extern IR2_OPND fcc5_ir2_opnd;
extern IR2_OPND fcc6_ir2_opnd;
extern IR2_OPND fcc7_ir2_opnd;
extern IR2_OPND scr0_ir2_opnd;
extern IR2_OPND scr1_ir2_opnd;
extern IR2_OPND scr2_ir2_opnd;
extern IR2_OPND scr3_ir2_opnd;

#define V0_RENAME_OPND a7_ir2_opnd

#define ra_free_temp(opnd)                                              \
    do {                                                                \
        if (ir2_opnd_is_itemp(&opnd)) {                                 \
            ra_free_itemp(opnd._reg_num);                               \
        } else if (ir2_opnd_is_ftemp(&opnd)) {                          \
            ra_free_ftemp(opnd._reg_num);                               \
        } else {                                                        \
            lsassertm(0, "attempt to free a non-temp register");        \
        }                                                               \
    } while (0)

#define ra_free_temp_auto(opnd)                                         \
    do {                                                                \
        if (ir2_opnd_is_itemp(&opnd)) {                                 \
            ra_free_itemp(opnd._reg_num);                               \
        } else if (ir2_opnd_is_ftemp(&opnd)) {                          \
            ra_free_ftemp(opnd._reg_num);                               \
        }                                                               \
    } while (0)

/*
 * Template functions
 */
#define RA_ALLOC_FUNC_DEF(name) IR2_OPND ra_alloc_##name(int)
#define RA_ALLOC_FUNC(name, type)                                       \
IR2_OPND ra_alloc_##name(int index)                                     \
{                                                                       \
    lsassert(index >= 0 &&                                              \
             index < (sizeof(reg_##name##_map) / sizeof(int)));         \
    IR2_OPND opnd;                                                      \
    opnd._type = type;                                                  \
    opnd._reg_num = reg_##name##_map[index];                            \
    opnd._assist = 0;                                                   \
    return opnd;                                                        \
}
/*
 * Calculate the number of used temp reg
 */

#define RA_ALLOC_TEMP_NUM_FUNC(name)                                    \
static int ra_alloc_##name##_num(void)                                  \
{                                                                       \
    int name##_num, cto_num;                                            \
    __asm("cto.w %0, %1\n\t"                                            \
    : "=r"(cto_num)                                                     \
    : "r"(tr_data->name##_status));                                     \
    lsassertm(cto_num < name##_status_num,                              \
              "\n%s:%d alloc " #name " failed, cto_num %d\n",           \
              __func__, __LINE__, cto_num);                             \
    name##_num = reg_##name##_map[cto_num];                             \
    BITS_SET(tr_data->name##_status, 1 << cto_num);                     \
    return name##_num;                                                  \
}

#define RA_FREE_CODE(name, phy_id)                                      \
    do {                                                                \
        int virt_id = reg_##name##_reverse_map[phy_id];                 \
        lsassertm(virt_id >= 0 &&                                       \
                ((tr_data->name##_status) & (1 << virt_id)),            \
                "\n%s:%d free " #name " failed, phy_id %d virt_id %d "  \
                "tr_data->"#name"_status 0x%x\n",                       \
                 __func__, __LINE__, phy_id, virt_id,                   \
                tr_data->name##_status);                                \
        tr_data->name##_status &= ~(1 << virt_id);                      \
    } while (0)

#define INIT_RA(type, phyno)                                            \
(IR2_OPND){._type = type, ._reg_num = phyno, ._assist = 0}

/*
 * Function declarations
 */
IR2_OPND get_la_ir2_opnd(int);
/* ra_alloc_gpr */
RA_ALLOC_FUNC_DEF(gpr);
/* ra_alloc_fpr */
RA_ALLOC_FUNC_DEF(fpr);
/* ra_alloc_mmx */
RA_ALLOC_FUNC_DEF(mmx);
/* ra_alloc_xmm */
RA_ALLOC_FUNC_DEF(xmm);
/* ra_alloc_statics */
RA_ALLOC_FUNC_DEF(statics);
/* ra_alloc_trace */
RA_ALLOC_FUNC_DEF(trace);
/* ra_alloc_src */
RA_ALLOC_FUNC_DEF(scr);
/* allocate and free temp register */
IR2_OPND ra_alloc_itemp(void);
IR2_OPND ra_alloc_ftemp(void);
#define ra_free_itemp(phy_id)   RA_FREE_CODE(itemp, phy_id)
#define ra_free_ftemp(phy_id)   RA_FREE_CODE(ftemp, phy_id)
void ra_free_all(void);
IR2_OPND ra_alloc_num_4095(void);
void ra_free_num_4095(IR2_OPND);

/*
 * Alias
 */
#define ra_alloc_dbt_arg1() ra_alloc_statics(S_UD0)
#define ra_alloc_dbt_arg2() ra_alloc_statics(S_UD1)
// #define ra_alloc_ss() ra_alloc_statics(S_UD2)
// #define ra_alloc_env() ra_alloc_statics(S_ENV)
// #define ra_alloc_eflags() ra_alloc_statics(S_EFLAGS)
#define ra_alloc_itemp_internal ra_alloc_itemp
#define ra_alloc_ftemp_internal ra_alloc_ftemp

/* allocate x86 register */
IR2_OPND ra_alloc_st(int);
IR2_OPND ra_alloc_mda(void);

#endif
