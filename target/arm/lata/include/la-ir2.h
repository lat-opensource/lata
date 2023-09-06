#ifndef __LA_IR2_H_
#define __LA_IR2_H_

#include "latx-types.h"



//0-byte 1-half 2-word 3-double
#define VLDI_IMM_TYPE0(mode, imm) ((mode&0x3)<<10|(imm&0x3ff))
//vseti
#define VLDI_IMM_TYPE1(mode, imm) (1<<12|(mode&0x3f)<<8|(imm&0xff))
#define VEXTRINS_IMM_4_0(n, m) ((n&0xf)<<4|(m&0xf))

/* IR2_JCC; */
typedef enum {
    COND_A,		//!CF && !ZF
    COND_AE,	//!CF
    COND_B,     // CF
    COND_BE,    // CF || ZF
    COND_E,     // ZF
    COND_NE,    //!ZF
    COND_G,     //!ZF && (SF == OF)
    COND_GE,    // SF == OF
    COND_L,     // SF != OF
    COND_LE,    // ZF && (SF != OF)
    COND_S,     // SF
    COND_NS,    //!SF
    COND_O,     // OF
    COND_NO,    //!OF
    COND_PE,    // PF
    COND_PO,    //!PF
} IR2_JCC;

/* IR2_FCMP_COND; */
typedef enum {
    FCMP_COND_CAF, //0
    FCMP_COND_SAF,
    FCMP_COND_CLT,
    FCMP_COND_SLT,
    FCMP_COND_CEQ, //4
    FCMP_COND_SEQ,
    FCMP_COND_CLE,
    FCMP_COND_SLE,
    FCMP_COND_CUN, //8
    FCMP_COND_SUN,
    FCMP_COND_CULT,
    FCMP_COND_SULT,
    FCMP_COND_CUEQ, //c
    FCMP_COND_SUEQ,
    FCMP_COND_CULE,
    FCMP_COND_SULE,
    FCMP_COND_CNE, //10
    FCMP_COND_SNE,
    FCMP_COND_UND1,
    FCMP_COND_UND2,
    FCMP_COND_COR, //14
    FCMP_COND_SOR,
    FCMP_COND_UND3,
    FCMP_COND_UND4,
    FCMP_COND_CUNE, //18
    FCMP_COND_SUNE,
} IR2_FCMP_COND;

/* IR2_OPCODE; */
typedef enum {
#include "ir2-opcode.h"
} IR2_OPCODE;

/* IR2_OPND_TYPE */
typedef enum {
    IR2_OPND_NONE = 80,
    IR2_OPND_GPR,
    IR2_OPND_SCR,
    IR2_OPND_FPR,
    IR2_OPND_FCSR,      /* immediate used in cfc1/ctc1 */
    IR2_OPND_CC,        /* condition code, FCC field in FCSR */
    IR2_OPND_IMM,       /* immediate */
    IR2_OPND_LABEL,
    IR2_OPND_MEM,       /* middle type. not used as backend */
} IR2_OPND_TYPE;

/* IR2_OPND */
typedef struct IR2_OPND {
    IR2_OPND_TYPE _type;
    union {
        uint32 val; /* used for backend */
        struct {   /* used for IR2 data */
            union { /* _data */
                int32 _data;
                int32 _reg_num;  /* GPR/FPR/MEM.reg */
                uint32 _imm32;   /* IMM */
                int32 _label_id; /* LABEL */
            };
            union { /* _assist */
                int32 _assist;
                int32 _offset;   /* used to store MEM.offset */
            };
        };
    };
} IR2_OPND;

typedef struct IR2_INST {
    ADDR _addr;
    int16 _opcode;
    int16 _id;
    int16 _prev;
    int16 _next;
    int op_count;
    IR2_OPND _opnd[4]; /*LA has 4 opnds*/
} IR2_INST;


#endif
