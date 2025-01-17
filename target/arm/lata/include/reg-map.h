#ifndef _REG_MAP_H_
#define _REG_MAP_H_

#define REG_MAP_DEF(x86_index, la_index)        \
[x86_index] = la_index


#define GPR_NUM         8
#define XMM_NUM         8


#define NOT_DEF         -1

/*
 * X86 GPR
 */
#define eax_index       0
#define ecx_index       1
#define edx_index       2
#define ebx_index       3
#define esp_index       4
#define ebp_index       5
#define esi_index       6
#define edi_index       7

/*
 * Segment registers
 */
#define es_index        0
#define cs_index        1
#define ss_index        2
#define ds_index        3
#define fs_index        4
#define gs_index        5

/*
 * Virtual registers (self-defined registers)
 */
#define STATIC_NUM      4
#define S_UD0           0
#define S_UD1           1
#define S_UD2           2
#define S_ENV           3
#define S_EFLAGS        4

/*
 * Trace registers (self-defined registers)
 */
#define TRACE           0

/*
 * temporary registers
 */
#define INVALID_TEMP    -1
#define ITEMP0          0
#define ITEMP1          1
#define ITEMP2          2
#define ITEMP3          3
#define ITEMP4          4
#define ITEMP5          5
#define ITEMP6          6
#define ITEMP7          7
#define ITEMP8          8
#define ITEMP9          9
#define ITEMP10         10
#define FTEMP0          0
#define FTEMP1          1
#define FTEMP2          2
#define FTEMP3          3
#define FTEMP4          4
#define FTEMP5          5
#define FTEMP6          6

/*
 * LA registers
 */
#define la_zero    0
#define la_ra      1
#define la_tp      2
#define la_sp      3
#define la_a0      4
#define la_a1      5
#define la_a2      6
#define la_a3      7
#define la_a4      8
#define la_a5      9
#define la_a6      10
#define la_a7      11
#define la_t0      12
#define la_t1      13
#define la_t2      14
#define la_t3      15
#define la_t4      16
#define la_t5      17
#define la_t6      18
#define la_t7      19
#define la_t8      20
#define la_r21     21
#define la_fp      22
#define la_s0      23
#define la_s1      24
#define la_s2      25
#define la_s3      26
#define la_s4      27
#define la_s5      28
#define la_s6      29
#define la_s7      30
#define la_s8      31

//ARMv8 64-bit GP registers
//parameter and result regs
#define armv8_x0      0
#define armv8_x1      1
#define armv8_x2      2
#define armv8_x3      3
#define armv8_x4      4
#define armv8_x5      5
#define armv8_x6      6
#define armv8_x7      7
//indirect result location reg
#define armv8_x8      8
//caller saved temporary regs
#define armv8_x9      9
#define armv8_x10     10
#define armv8_x11     11
#define armv8_x12     12
#define armv8_x13     13
#define armv8_x14     14
#define armv8_x15     15
//intra-procedure call scratch regs
#define armv8_x16     16  //IP0
#define armv8_x17     17  //IP1
//platform reg
#define armv8_x18     18  //PR
//callee saved regs
#define armv8_x19     19
#define armv8_x20     20
#define armv8_x21     21
#define armv8_x22     22
#define armv8_x23     23
#define armv8_x24     24
#define armv8_x25     25
#define armv8_x26     26
#define armv8_x27     27
#define armv8_x28     28
//frame pointer
#define armv8_x29     29  //FP
//procedure link regester
#define armv8_x30     30  //LR
//zero reg or sp reg 
#define armv8_xzr     31   

//LA float registers
#define la_fsmask     15
//ARMv8 128-bit FP registers
//parameter and result regs
#define armv8_v0      0
#define armv8_v1      1
#define armv8_v2      2
#define armv8_v3      3
#define armv8_v4      4
#define armv8_v5      5
#define armv8_v6      6
#define armv8_v7      7
//callee must preserve lower 64 bits across subroutine calls
#define armv8_v8      8
#define armv8_v9      9
#define armv8_v10     10
#define armv8_v11     11
#define armv8_v12     12
#define armv8_v13     13
#define armv8_v14     14
#define armv8_v15     15
//Not preserved by callee
//caller should preserve these before calls if the registers are in use
#define armv8_v16     16
#define armv8_v17     17
#define armv8_v18     18
#define armv8_v19     19
#define armv8_v20     20
#define armv8_v21     21
#define armv8_v22     22
#define armv8_v23     23
#define armv8_v24     24
#define armv8_v25     25
#define armv8_v26     26
#define armv8_v27     27
#define armv8_v28     28
#define armv8_v29     29
#define armv8_v30     30   
#define armv8_v31     31   

extern const int reg_gpr_map[];
extern const int reg_fpr_map[];
extern const int reg_xmm_map[];
extern const int reg_statics_map[];
extern const int reg_itemp_map[];
extern const int reg_ftemp_map[];
extern const int reg_itemp_reverse_map[];
extern const int reg_ftemp_reverse_map[];

#endif
