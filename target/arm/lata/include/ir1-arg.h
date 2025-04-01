#ifndef _IR1_ARG_H_
#define _IR1_ARG_H_

typedef struct {
    int rs;
    int rn;
    int rt;
    int a;
    int r;
    int sz;
} arg_atomic;

typedef struct {
    int rd;
    int rn;
    int sf;
    int immr;
    int imms;
} arg_bitfield;

typedef struct {
    int rn;
    int rm;
    int m;
} arg_bra;

typedef struct {
    int rn;
    int m;
} arg_braz;

typedef struct {
    int rt;
    int imm;
    int sf;
    int nz;
} arg_cbz;

typedef struct {
    int cond;
    int imm;
} arg_decode_insn3211;

typedef struct {
} arg_decode_insn3215;

typedef struct {
    int domain;
    int types;
} arg_decode_insn3216;

typedef struct {
    int mask;
    int imm;
} arg_decode_insn3217;

typedef struct {
    int l;
    int op1;
    int crn;
    int crm;
    int op2;
    int rt;
    int op0;
} arg_decode_insn3218;

typedef struct {
    int rs;
    int rn;
    int rt;
    int sz;
} arg_decode_insn3221;

typedef struct {
    int sz;
    int rn;
    int rt;
} arg_decode_insn3227;

typedef struct {
    int m;
    int w;
    int rn;
    int rt;
    int imm;
} arg_decode_insn3228;

typedef struct {
    int q;
    int p;
    int rm;
    int scale;
    int rn;
    int rt;
    int selem;
} arg_decode_insn3232;

typedef struct {
    int rd;
    int rn;
    int rm;
    int imm;
    int sf;
} arg_extract;

typedef struct {
    int imm;
} arg_i;

typedef struct {
    int rn;
    int rt;
    int imm;
    int sz;
    int sign;
    int ext;
} arg_ldapr_stlr_i;

typedef struct {
    int rt;
    int imm;
    int sz;
    int sign;
} arg_ldlit;

typedef struct {
    int rm;
    int rn;
    int rt;
    int sign;
    int ext;
    int sz;
    int opt;
    int s;
} arg_ldst;

typedef struct {
    int rt;
    int rn;
    int imm;
    int sz;
    int sign;
    int w;
    int p;
    int unpriv;
    int ext;
} arg_ldst_imm;

typedef struct {
    int rm;
    int rn;
    int rt;
    int sz;
    int q;
    int p;
    int rpt;
    int selem;
} arg_ldst_mult;

typedef struct {
    int rm;
    int rn;
    int rt;
    int p;
    int selem;
    int index;
    int scale;
} arg_ldst_single;

typedef struct {
    int rn;
    int rt;
    int imm;
    int p;
    int w;
} arg_ldst_tag;

typedef struct {
    int rt2;
    int rt;
    int rn;
    int imm;
    int sz;
    int sign;
    int w;
    int p;
} arg_ldstpair;

typedef struct {
    int rd;
    int sf;
    int imm;
    int hw;
} arg_movw;

typedef struct {
    int rn;
} arg_r;

typedef struct {
    int m;
} arg_reta;

typedef struct {
    int rd;
    int imm;
} arg_ri;

typedef struct {
    int rd;
    int rn;
    int sf;
    int dbm;
} arg_rri_log;

typedef struct {
    int rd;
    int rn;
    int imm;
    int sf;
} arg_rri_sf;

typedef struct {
    int rd;
    int rn;
    int uimm6;
    int uimm4;
} arg_rri_tag;

typedef struct {
    int rn;
    int rt;
    int sz;
    int lasr;
} arg_stlr;

typedef struct {
    int rn;
    int rt;
    int rt2;
    int rs;
    int sz;
    int lasr;
} arg_stxr;

typedef struct {
    int rt;
    int imm;
    int nz;
    int bitpos;
} arg_tbz;

typedef arg_ri arg_ADR;
typedef arg_ri arg_ADRP;
typedef arg_rri_sf arg_ADD_i;
typedef arg_rri_sf arg_ADDS_i;
typedef arg_rri_sf arg_SUB_i;
typedef arg_rri_sf arg_SUBS_i;
typedef arg_rri_tag arg_ADDG_i;
typedef arg_rri_tag arg_SUBG_i;
typedef arg_rri_log arg_AND_i;
typedef arg_rri_log arg_ORR_i;
typedef arg_rri_log arg_EOR_i;
typedef arg_rri_log arg_ANDS_i;
typedef arg_movw arg_MOVN;
typedef arg_movw arg_MOVZ;
typedef arg_movw arg_MOVK;
typedef arg_bitfield arg_SBFM;
typedef arg_bitfield arg_BFM;
typedef arg_bitfield arg_UBFM;
typedef arg_extract arg_EXTR;
typedef arg_i arg_B;
typedef arg_i arg_BL;
typedef arg_cbz arg_CBZ;
typedef arg_tbz arg_TBZ;
typedef arg_decode_insn3211 arg_B_cond;
typedef arg_r arg_BR;
typedef arg_r arg_BLR;
typedef arg_r arg_RET;
typedef arg_braz arg_BRAZ;
typedef arg_braz arg_BLRAZ;
typedef arg_reta arg_RETA;
typedef arg_bra arg_BRA;
typedef arg_bra arg_BLRA;
typedef arg_decode_insn3215 arg_ERET;
typedef arg_reta arg_ERETA;
typedef arg_decode_insn3215 arg_YIELD;
typedef arg_decode_insn3215 arg_WFE;
typedef arg_decode_insn3215 arg_WFI;
typedef arg_decode_insn3215 arg_XPACLRI;
typedef arg_decode_insn3215 arg_PACIA1716;
typedef arg_decode_insn3215 arg_PACIB1716;
typedef arg_decode_insn3215 arg_AUTIA1716;
typedef arg_decode_insn3215 arg_AUTIB1716;
typedef arg_decode_insn3215 arg_ESB;
typedef arg_decode_insn3215 arg_PACIAZ;
typedef arg_decode_insn3215 arg_PACIASP;
typedef arg_decode_insn3215 arg_PACIBZ;
typedef arg_decode_insn3215 arg_PACIBSP;
typedef arg_decode_insn3215 arg_AUTIAZ;
typedef arg_decode_insn3215 arg_AUTIASP;
typedef arg_decode_insn3215 arg_AUTIBZ;
typedef arg_decode_insn3215 arg_AUTIBSP;
typedef arg_decode_insn3215 arg_NOP;
typedef arg_decode_insn3215 arg_CLREX;
typedef arg_decode_insn3216 arg_DSB_DMB;
typedef arg_decode_insn3215 arg_ISB;
typedef arg_decode_insn3215 arg_SB;
typedef arg_decode_insn3215 arg_CFINV;
typedef arg_decode_insn3215 arg_XAFLAG;
typedef arg_decode_insn3215 arg_AXFLAG;
typedef arg_i arg_MSR_i_UAO;
typedef arg_i arg_MSR_i_PAN;
typedef arg_i arg_MSR_i_SPSEL;
typedef arg_i arg_MSR_i_SBSS;
typedef arg_i arg_MSR_i_DIT;
typedef arg_i arg_MSR_i_TCO;
typedef arg_i arg_MSR_i_DAIFSET;
typedef arg_i arg_MSR_i_DAIFCLEAR;
typedef arg_decode_insn3217 arg_MSR_i_SVCR;
typedef arg_decode_insn3218 arg_SYS;
typedef arg_i arg_SVC;
typedef arg_i arg_HVC;
typedef arg_i arg_SMC;
typedef arg_i arg_BRK;
typedef arg_i arg_HLT;
typedef arg_stxr arg_STXR;
typedef arg_stxr arg_LDXR;
typedef arg_stlr arg_STLR;
typedef arg_stlr arg_LDAR;
typedef arg_stxr arg_STXP;
typedef arg_stxr arg_LDXP;
typedef arg_decode_insn3221 arg_CASP;
typedef arg_decode_insn3221 arg_CAS;
typedef arg_ldlit arg_LD_lit;
typedef arg_ldlit arg_LD_lit_v;
typedef arg_ldstpair arg_STP;
typedef arg_ldstpair arg_LDP;
typedef arg_ldstpair arg_STP_v;
typedef arg_ldstpair arg_LDP_v;
typedef arg_ldstpair arg_STGP;
typedef arg_ldst_imm arg_STR_i;
typedef arg_ldst_imm arg_LDR_i;
typedef arg_ldst_imm arg_STR_v_i;
typedef arg_ldst_imm arg_LDR_v_i;
typedef arg_ldst arg_STR;
typedef arg_ldst arg_LDR;
typedef arg_ldst arg_STR_v;
typedef arg_ldst arg_LDR_v;
typedef arg_atomic arg_LDADD;
typedef arg_atomic arg_LDCLR;
typedef arg_atomic arg_LDEOR;
typedef arg_atomic arg_LDSET;
typedef arg_atomic arg_LDSMAX;
typedef arg_atomic arg_LDSMIN;
typedef arg_atomic arg_LDUMAX;
typedef arg_atomic arg_LDUMIN;
typedef arg_atomic arg_SWP;
typedef arg_decode_insn3227 arg_LDAPR;
typedef arg_decode_insn3228 arg_LDRA;
typedef arg_ldapr_stlr_i arg_STLR_i;
typedef arg_ldapr_stlr_i arg_LDAPR_i;
typedef arg_ldst_mult arg_ST_mult;
typedef arg_ldst_mult arg_LD_mult;
typedef arg_ldst_single arg_ST_single;
typedef arg_ldst_single arg_LD_single;
typedef arg_decode_insn3232 arg_LD_single_repl;
typedef arg_ldst_tag arg_STZGM;
typedef arg_ldst_tag arg_STG;
typedef arg_ldst_tag arg_LDG;
typedef arg_ldst_tag arg_STZG;
typedef arg_ldst_tag arg_STGM;
typedef arg_ldst_tag arg_ST2G;
typedef arg_ldst_tag arg_LDGM;
typedef arg_ldst_tag arg_STZ2G;

/* arguments for translation function*/
union a64_u{
    arg_atomic f_atomic;
    arg_bitfield f_bitfield;
    arg_bra f_bra;
    arg_braz f_braz;
    arg_cbz f_cbz;
    arg_decode_insn3211 f_decode_insn3211;
    arg_decode_insn3215 f_decode_insn3215;
    arg_decode_insn3216 f_decode_insn3216;
    arg_decode_insn3217 f_decode_insn3217;
    arg_decode_insn3218 f_decode_insn3218;
    arg_decode_insn3221 f_decode_insn3221;
    arg_decode_insn3227 f_decode_insn3227;
    arg_decode_insn3228 f_decode_insn3228;
    arg_decode_insn3232 f_decode_insn3232;
    arg_extract f_extract;
    arg_i f_i;
    arg_ldapr_stlr_i f_ldapr_stlr_i;
    arg_ldlit f_ldlit;
    arg_ldst f_ldst;
    arg_ldst_imm f_ldst_imm;
    arg_ldst_mult f_ldst_mult;
    arg_ldst_single f_ldst_single;
    arg_ldst_tag f_ldst_tag;
    arg_ldstpair f_ldstpair;
    arg_movw f_movw;
    arg_r f_r;
    arg_reta f_reta;
    arg_ri f_ri;
    arg_rri_log f_rri_log;
    arg_rri_sf f_rri_sf;
    arg_rri_tag f_rri_tag;
    arg_stlr f_stlr;
    arg_stxr f_stxr;
    arg_tbz f_tbz;
} ;

#endif