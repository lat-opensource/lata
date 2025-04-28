// #include "common.h"
// #include "ir1.h"
// #include "lsenv.h"
#include "reg-alloc.h"
// #include "translate.h"
// #include "latx-options.h"
#include <string.h>

#include "ir2.h"
#include "lata.h"

extern void qemu_log(const char *fmt, ...);

static const char *ir2_name(int value)
{
#ifdef CONFIG_LATA_DEBUG
    /*
     * Todo: Add below defination:
     */
    #if 0
    static const char *ir2_scr_name[] = {
        "$scr0" , "$scr1" , "$scr2" , "$scr3",
    };

    static const char *ir2_cc_name[] = {
        "$cc0" , "$cc1" , "$cc2" , "$cc3" ,
        "$cc4" , "$cc5" , "$cc6" , "$cc7" ,
    };
    #endif
    const char *g_ir2_names[] = {
        "$zero" , "$ra" , "$tp" , "$sp" , "$a0" , "$a1" , "$a2" , "$a3" ,
        "$a4"   , "$a5" , "$a6" , "$a7" , "$t0" , "$t1" , "$t2" , "$t3" ,
        "$t4"   , "$t5" , "$t6" , "$t7" , "$t8" , "$x"  , "$fp" , "$s0" ,
        "$s1"   , "$s2" , "$s3" , "$s4" , "$s5" , "$s6" , "$s7" , "$s8" ,
        "", "", "", "", "", "", "", "", /*  32-39 */
        "$fa0"  , "$fa1"  , "$fa2"  , "$fa3"  ,
        "$fa4"  , "$fa5"  , "$fa6"  , "$fa7"  ,
        "$ft0"  , "$ft1"  , "$ft2"  , "$ft3"  ,
        "$ft4"  , "$ft5"  , "$ft6"  , "$ft7"  ,
        "$ft8"  , "$ft9"  , "$ft10" , "$ft11" ,
        "$ft12" , "$ft13" , "$ft14" , "$ft15" ,
        "$fs0"  , "$fs1"  , "$fs2"  , "$fs3"  ,
        "$fs4"  , "$fs5"  , "$fs6"  , "$s7"   ,
        "", "", "", "", "", "", "", "", /* 72-79 */
        "NONE", "GPR", "SCR", "FPR", "FCSR", "IMMD", "IMMH", "LABEL",/* 80-87 */
        "", "", "", "", "", "", "", "", /* 88-95 */
        "ax", "sx", "zx", "bx", "AD", "AX", "", "",  /* 96-103 */
        "", "", "", "", "", "", "", "", /* 104-111 */
        "", "", "", "", "", "", "", "", /* 112-119 */
        "", "", "", "", "", "", "", "", /* 120-127 */
        /* clang-format on */
        //"", "label", "", "", "", "", "", "", /* 128-135 */
        //"", "", "", "", "", "", "", "", /* 136-143 */
        //"", "", "", "", "", "", "", "", /* 144-151 */
#include "ir2-name.h"

    };
    lsassert(value <= LISA_ENDING);
    return g_ir2_names[value];
#else
     return "undef";
#endif
}

void ir2_opnd_build_none(IR2_OPND *opnd)
{
    opnd->_type = IR2_OPND_NONE;
    opnd->_data = 0;
    opnd->_assist = 0;
}

IR2_OPND ir2_opnd_new_none(void)
{
    IR2_OPND opnd;
    ir2_opnd_build_none(&opnd);
    return opnd;
}

void ir2_opnd_build_type(IR2_OPND *opnd, IR2_OPND_TYPE t)
{
    lsassert(t == IR2_OPND_LABEL);
    opnd->_type = t;
    opnd->_label_id = ++(lsenv->tr_data->label_num);
}

IR2_OPND ir2_opnd_new_type(IR2_OPND_TYPE t)
{
    IR2_OPND opnd;
    ir2_opnd_build_type(&opnd, t);
    return opnd;
}

void ir2_opnd_build(IR2_OPND *opnd, IR2_OPND_TYPE t, int value)
{
    if (t == IR2_OPND_GPR || t == IR2_OPND_FPR ||
        t == IR2_OPND_FCSR || t == IR2_OPND_SCR) {
        opnd->_type = t;
        opnd->_reg_num = value;
    } else if (t == IR2_OPND_IMM) {
        opnd->_type = t;
        opnd->_imm32 = value;
    } else {
        lsassertm(0, "[LATX] [error] not implemented in %s : %d", __func__,
                  __LINE__);
    }
}

IR2_OPND ir2_opnd_new(IR2_OPND_TYPE type, int value)
{
    IR2_OPND opnd;

    ir2_opnd_build(&opnd, type, value);

    return opnd;
}

void ir2_opnd_build2(IR2_OPND *opnd, IR2_OPND_TYPE type, int base, int16 offset)
{
    lsassert(type == IR2_OPND_MEM);
    opnd->_type = type;
    opnd->_reg_num = base;
    opnd->_offset = offset;
}

IR2_OPND ir2_opnd_new2(IR2_OPND_TYPE type, int base, int16 offset)
{
    IR2_OPND opnd;

    ir2_opnd_build2(&opnd, type, base, offset);

    return opnd;
}

int32 ir2_opnd_imm(const IR2_OPND *opnd) { return opnd->_imm32; }

int32 ir2_opnd_offset(const IR2_OPND *opnd)
{
    lsassertm(ir2_opnd_is_mem(opnd), "This IR2 must be a MEM type!");
    return opnd->_offset;
}


int ir2_opnd_is_ireg(const IR2_OPND *opnd) { return opnd->_type == IR2_OPND_GPR; }

int ir2_opnd_is_freg(const IR2_OPND *opnd) { return opnd->_type == IR2_OPND_FPR; }

int ir2_opnd_is_creg(const IR2_OPND *opnd) { return opnd->_type == IR2_OPND_FCSR; }

int32 ir2_opnd_label_id(const IR2_OPND *opnd) { return opnd->_label_id; }

int ir2_opnd_is_itemp(const IR2_OPND *opnd)
{
    return (ir2_opnd_is_ireg(opnd) &&
           (reg_itemp_reverse_map[ir2_opnd_base_reg_num(opnd)] >= 0));
}

int ir2_opnd_is_ftemp(const IR2_OPND *opnd)
{
    return (ir2_opnd_is_freg(opnd) &&
           (reg_ftemp_reverse_map[ir2_opnd_base_reg_num(opnd)] >= 0));
}

int ir2_opnd_is_mem(const IR2_OPND *opnd) { return opnd->_type == IR2_OPND_MEM; }

int ir2_opnd_is_imm(const IR2_OPND *opnd) { return opnd->_type == IR2_OPND_IMM; }

int ir2_opnd_is_label(const IR2_OPND *opnd) { return opnd->_type == IR2_OPND_LABEL; }

int ir2_opnd_base_reg_num(const IR2_OPND *opnd) { return opnd->_reg_num; }

IR2_OPND_TYPE ir2_opnd_type(const IR2_OPND *opnd)
{
    return (IR2_OPND_TYPE)opnd->_type;
}

void ir2_set_opnd_type(IR2_OPND *opnd, const IR2_OPND_TYPE type)
{
    opnd->_type = type;
}

int ir2_opnd_cmp(const IR2_OPND *opnd1, const IR2_OPND *opnd2)
{
    return opnd1->_type == opnd2->_type && opnd1->val == opnd2->val;
}

void ir2_opnd_convert_label_to_imm(IR2_OPND *opnd, int imm)
{
    lsassert(ir2_opnd_is_label(opnd));
    opnd->_type = IR2_OPND_IMM;
    opnd->_imm32 = imm;
}

int ir2_opnd_to_string(IR2_OPND *opnd, char *str, bool hex)
{

    int base_reg_num = ir2_opnd_base_reg_num(opnd);

    switch (ir2_opnd_type(opnd)) {
    case IR2_OPND_NONE:
        return 0;
    case IR2_OPND_GPR: {
        if (ir2_opnd_is_itemp(opnd)) {
            return sprintf(str, "\033[3%dmitmp%d\033[m", base_reg_num % 6 + 1,
                           reg_itemp_reverse_map[base_reg_num]);
        } else {
            strcpy(str, ir2_name(base_reg_num));
            return strlen(str);
        }
    }
    case IR2_OPND_FPR: {
        if (ir2_opnd_is_ftemp(opnd)) {
            return sprintf(str, "\033[3%dmftmp%d\033[m", base_reg_num % 6 + 1,
                           reg_ftemp_reverse_map[base_reg_num]);
        } else {
            strcpy(str, ir2_name(40 + base_reg_num));
            return strlen(str);
        }
    }
    case IR2_OPND_FCSR: {
        return sprintf(str, "$c%d", base_reg_num);
    }
    case IR2_OPND_CC: {
        return sprintf(str, "$c%d", base_reg_num);
    }
    // case IR2_OPND_MEM: {
    //    if (ir2_opnd_imm(opnd) > -1024 && ir2_opnd_imm(opnd) < 1024) {
    //        if (ir2_opnd_is_mem_base_itemp(opnd)) {
    //            return sprintf(str, "%d(\033[3%dmitmp%d\033[m)",
    //                           ir2_opnd_imm(opnd), base_reg_num % 6 + 1,
    //                           base_reg_num);
    //        } else {
    //            return sprintf(str, "%d(%s)", ir2_opnd_imm(opnd),
    //                           ir2_name(base_reg_num));
    //        }
    //    } else {
    //        if (ir2_opnd_is_mem_base_itemp(opnd)) {
    //            return sprintf(str, "0x%x(\033[3%dmitmp%d\033[m)",
    //                           ir2_opnd_imm(opnd), base_reg_num % 6 + 1,
    //                           base_reg_num);
    //        } else {
    //            return sprintf(str, "0x%x(%s)", ir2_opnd_imm(opnd),
    //                           ir2_name(base_reg_num));
    //        }
    //    }
    // }
    case IR2_OPND_IMM: {
        if (hex) {
            return sprintf(str, "0x%x", (uint32)ir2_opnd_imm(opnd));
        } else {
            return sprintf(str, "%d", ir2_opnd_imm(opnd));
        }
    }
    case IR2_OPND_SCR:
        return sprintf(str, "%d", ir2_opnd_imm(opnd));
    case IR2_OPND_LABEL:
        return sprintf(str, "LABEL %d", ir2_opnd_imm(opnd));
    default:
        lsassertm(0, "type = %d\n", ir2_opnd_type(opnd));
        return 0;
    }
}

bool ir2_opcode_is_branch(IR2_OPCODE opcode)
{
    return (opcode >= LISA_BEQZ && opcode <= LISA_BCNEZ) ||
           (opcode >= LISA_B && opcode <= LISA_BGEU);
}

bool ir2_opcode_is_branch_with_3opnds(IR2_OPCODE opcode)
{
    if (opcode >= LISA_BEQ && opcode <= LISA_BGEU) {
        return true;
    }
    return false;
}

bool ir2_opcode_is_branch_with_2opnds(IR2_OPCODE opcode)
{
    return (opcode == LISA_BEQZ || opcode == LISA_BNEZ);
}

bool ir2_opcode_is_f_branch(IR2_OPCODE opcode)
{
    return (opcode == LISA_BCEQZ || opcode == LISA_BCNEZ);
}

bool ir2_opcode_is_convert(IR2_OPCODE opcode)
{
    switch (opcode) {
    case LISA_FCVT_D_S:
    case LISA_FCVT_S_D:
    case LISA_FFINT_D_W:
    case LISA_FFINT_D_L:
    case LISA_FFINT_S_W:
    case LISA_FFINT_S_L:
    case LISA_FTINT_L_D:
    case LISA_FTINT_L_S:
    case LISA_FTINT_W_D:
    case LISA_FTINT_W_S:
        return true;
    default:
        return false;
    }
}

bool ir2_opcode_is_fcmp(IR2_OPCODE opcode)
{
    return (opcode == LISA_FCMP_COND_S || opcode == LISA_FCMP_COND_D);
}

void ir2_set_id(IR2_INST *ir2, int id) { ir2->_id = id; }

int ir2_get_id(const IR2_INST *ir2) { return ir2->_id; }

IR2_OPCODE ir2_opcode(const IR2_INST *ir2) { return (IR2_OPCODE)(ir2->_opcode); }

void ir2_set_opcode(IR2_INST *ir2, IR2_OPCODE type) {
    ir2->_opcode = type;
}

IR2_INST *ir2_prev(const IR2_INST *ir2)
{
    if (ir2->_prev == -1) {
        return NULL;
    } else {
        return lsenv->tr_data->ir2_inst_array + ir2->_prev;
    }
}

IR2_INST *ir2_next(const IR2_INST *ir2)
{
    if (ir2->_next == -1) {
        return NULL;
    } else {
        return lsenv->tr_data->ir2_inst_array + ir2->_next;
    }
}

int ir2_to_string(IR2_INST *ir2, char *str)
{
    int length = 0;
    int i = 0;
    bool print_hex_imm = false;

    length = sprintf(str, "%-8s  ", ir2_name(ir2_opcode(ir2)));

    if (ir2_opcode(ir2) == LISA_ANDI || ir2_opcode(ir2) == LISA_ORI ||
        ir2_opcode(ir2) == LISA_XORI || ir2_opcode(ir2) == LISA_LU12I_W) {
        print_hex_imm = true;
    }

    for (i = 0; i < ir2->op_count; ++i) {
        if (ir2_opnd_type(&ir2->_opnd[i]) == IR2_OPND_NONE) {
            return length;
        } else {
            if (i > 0) {
                strcat(str, ",");
                length += 1;
            }
            /*length += _opnd[i].to_string(str+length, print_hex_imm);*/
            length +=
                ir2_opnd_to_string(&ir2->_opnd[i], str + length, print_hex_imm);
        }
    }

    return length;
}

static int ir1_id;

int ir2_dump(IR2_INST *ir2)
{
   char str[64];
    int size = 0;

    if (ir2_opcode(ir2) ==
        0) { /* an empty IR2_INST was inserted into the ir2 */
             /* list, but not assigned yet. */
        return 0;
    }

    size = ir2_to_string(ir2, str);
    if (ir2->_id == 0) {
        ir1_id = 0;
        qemu_log("[%d, %d] %s\n", ir2->_id, ir1_id, str);
    } else if (str[0] == '-') {
        ir1_id ++;
        qemu_log("[%d, %d] %s\n", ir2->_id, ir1_id, str);
    } else {
        qemu_log("%s\n", str);
    }


    return size;
}

void ir2_build(IR2_INST *ir2, IR2_OPCODE opcode, IR2_OPND opnd0, IR2_OPND opnd1,
               IR2_OPND opnd2)
{
    ir2->_opcode = opcode;
    ir2->_opnd[0] = opnd0;
    ir2->_opnd[1] = opnd1;
    ir2->_opnd[2] = opnd2;
    ir2->_opnd[3] = ir2_opnd_new_none();
}

void ir2_append(IR2_INST *ir2)
{
    TRANSLATION_DATA *t = lsenv->tr_data;
    IR2_INST *former_last = t->last_ir2;

    if (former_last != NULL) {
        lsassert(t->first_ir2 != NULL);
        ir2->_prev = ir2_get_id(former_last);
        ir2->_next = -1;
        t->last_ir2 = ir2;
        former_last->_next = ir2_get_id(ir2);
    } else {
        lsassert(t->first_ir2 == NULL);
        ir2->_prev = -1;
        ir2->_next = -1;
        t->last_ir2 = ir2;
        t->first_ir2 = ir2;
    }

    if(ir2->_opcode >= LISA_GR2SCR)
        t->real_ir2_inst_num++;
}

void ir2_remove(IR2_INST *ir2)
{
    TRANSLATION_DATA *t = lsenv->tr_data;

    IR2_INST *next = ir2_next(ir2);
    IR2_INST *prev = ir2_prev(ir2);

    if (t->first_ir2 == ir2) {
        if (t->last_ir2 == ir2) { /* head and tail */
            t->first_ir2 = NULL;
            t->last_ir2 = NULL;
        } else { /* head but not tail */
            t->first_ir2 = next;
            next->_prev = -1;
        }
    } else if (t->last_ir2 == ir2) { /* tail but not head */
        t->last_ir2 = prev;
        prev->_next = -1;
    } else {
        prev->_next = ir2_get_id(next);
        next->_prev = ir2_get_id(prev);
    }

    ir2->_prev = -1;
    ir2->_next = -1;
}

void ir2_insert_before(IR2_INST *ir2, IR2_INST *next)
{
    TRANSLATION_DATA *t = lsenv->tr_data;

    if (t->first_ir2 == next) {
        t->first_ir2 = ir2;
        ir2->_prev = -1;
        ir2->_next = ir2_get_id(next);
        next->_prev = ir2_get_id(ir2);
    } else {
        IR2_INST *prev = ir2_prev(next);

        ir2->_prev = ir2_get_id(prev);
        prev->_next = ir2_get_id(ir2);

        ir2->_next = ir2_get_id(next);
        next->_prev = ir2_get_id(ir2);
    }
}

void ir2_insert_after(IR2_INST *ir2, IR2_INST *prev)
{
    TRANSLATION_DATA *t = lsenv->tr_data;

    if (t->last_ir2 == prev) {
        t->last_ir2 = ir2;
        ir2->_next = -1;
        ir2->_prev = ir2_get_id(prev);
        prev->_next = ir2_get_id(ir2);
    } else {
        IR2_INST *next = ir2_next(prev);

        ir2->_next = ir2_get_id(next);
        next->_prev = ir2_get_id(ir2);

        ir2->_prev = ir2_get_id(prev);
        prev->_next = ir2_get_id(ir2);
    }
}

IR2_INST *ir2_allocate(void)
{
    TRANSLATION_DATA *t = lsenv->tr_data;

    /* 1. make sure we have enough space */
    if (t->ir2_inst_num_current == t->ir2_inst_num_max) {
        /* printf("[LATX] [error] not implemented in %s : %d", __func__, */
        /* __LINE__); exit(-1); 1.1. current array size in bytes */
        int bytes = sizeof(IR2_INST) * t->ir2_inst_num_max;

        /* 1.2. double the array */
        t->ir2_inst_num_max *= 2;
        IR2_INST *back_ir2_inst_array = t->ir2_inst_array;
        t->ir2_inst_array =
            (IR2_INST *)mm_realloc(t->ir2_inst_array, bytes << 1);
        t->first_ir2 =
            (IR2_INST *)((ADDR)t->first_ir2 - (ADDR)back_ir2_inst_array +
                         (ADDR)t->ir2_inst_array);
        t->last_ir2 =
            (IR2_INST *)((ADDR)t->last_ir2 - (ADDR)back_ir2_inst_array +
                         (ADDR)t->ir2_inst_array);
    }

    /* 2. allocate one */
    IR2_INST *p = t->ir2_inst_array + t->ir2_inst_num_current;
    ir2_set_id(p, t->ir2_inst_num_current);
    t->ir2_inst_num_current++;

    /* 3. return it */
    return p;
}

/********************************************
 *                                          *
 *     LA IR2 implementation.               *
 *                                          *
 ********************************************/
bool la_ir2_opcode_is_load(IR2_OPCODE opcode)
{
    if (opcode >= LISA_LD_B && opcode <=  LISA_LD_D) {
        return true;
    }
    if (opcode >=  LISA_LD_BU && opcode <=  LISA_LD_WU) {
        return true;
    }
    if (opcode == LISA_LL_W || opcode == LISA_LL_D) {
        return true;
    }
    if (opcode == LISA_LDPTR_W || opcode == LISA_LDPTR_D) {
        return true;
    }
    if (opcode == LISA_FLD_S || opcode == LISA_FLD_D) {
        return true;
    }
    if (opcode == LISA_VLD || opcode == LISA_XVLD){
        return true;
    }
    if (opcode >= LISA_LDL_W && opcode <= LISA_LDR_D){
        return true;
    }
    if (opcode >= LISA_VLDREPL_D && opcode <= LISA_VLDREPL_B){
        return true;
    }
    if (opcode >= LISA_XVLDREPL_D && opcode <= LISA_XVLDREPL_B){
        return true;
    }
    if (opcode == LISA_PRELD) {
        return true;
    }
    return false;
}

bool la_ir2_opcode_is_store(IR2_OPCODE opcode)
{
    if (opcode >= LISA_ST_B && opcode <= LISA_ST_D) {
        return true;
    }
    if (opcode >= LISA_STL_W && opcode <= LISA_STR_D) {
        return true;
    }
    if (opcode == LISA_SC_D || opcode == LISA_SC_W) {
        return true;
    }
    if (opcode == LISA_STPTR_W || opcode == LISA_STPTR_D) {
        return true;
    }
    if (opcode == LISA_FST_S || opcode == LISA_FST_D) {
        return true;
    }
    if (opcode == LISA_XVST || opcode == LISA_VST){
        return true;
    }
    return false;
}

IR2_OPND create_immh_opnd(int val)
{
    IR2_OPND res;
    res._imm32 = val;
    res._type = IR2_OPND_IMM;
    return res;
}

// void la_append_ir2_jmp_far(ADDR jmp_offset)
// {
//     long insn_offset = jmp_offset >> 2;
// #ifdef CONFIG_LATX_LARGE_CC
//     if (jmp_offset == sextract64(jmp_offset, 0, 28)) {
//         IR2_OPND ir2_opnd_addr;
//         ir2_opnd_build(&ir2_opnd_addr, IR2_OPND_IMM, insn_offset);
//         la_b(ir2_opnd_addr);
//         la_and(zero_ir2_opnd, zero_ir2_opnd, zero_ir2_opnd);
//     } else {
//         int upper, lower;
//         upper = (((insn_offset) + (1 << 15)) >> 16) << 44 >> 44;
//         lower = (insn_offset) << 48 >> 48;
//         IR2_OPND tmp = ra_alloc_itemp();
//         la_pcaddu18i(tmp, upper);
//         la_jirl(zero_ir2_opnd, tmp, lower);
//         ra_free_temp(tmp);
//     }
// #else
//     /* B has +- 128M range */
//     lsassert(jmp_offset == sextract64(jmp_offset, 0, 28));
//     IR2_OPND ir2_opnd_addr;
//     ir2_opnd_build(&ir2_opnd_addr, IR2_OPND_IMM, insn_offset);
//     la_b(ir2_opnd_addr);
// #endif
// }
