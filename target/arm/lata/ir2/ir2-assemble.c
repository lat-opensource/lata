#include "ir2.h"
// #include "translate.h"
#include "ir2-la-assemble.h"

// void set_operand_into_instruction(GM_OPERAND_TYPE operand_type,
//                                   IR2_OPND *p_opnd, uint32 *result);

/*
 * FIXME: This is a intial port code, there is no any verification!!!
 */
uint32 ir2_assemble(IR2_INST *ir2){
    GM_LA_OPCODE_FORMAT format = lisa_format_table[ir2->_opcode - LISA_INVALID];
    lsassert(format.type == ir2->_opcode);
    lsassert(format.opcode != 0);

    uint32 ins = format.opcode;
    lsassertm(ins, "Cannot use a pseudo opcode!");
    for (int i = 0; i < 4; i++) {
        GM_OPERAND_TYPE opnd_type = format.opnd[i];
        if (opnd_type == OPD_INVALID)
            break;

        GM_OPERAND_PLACE_RELATION bit_field = bit_field_table[opnd_type];
        lsassert(opnd_type == bit_field.type);

        int start = bit_field.bit_range_0.start;
        int end = bit_field.bit_range_0.end;
        int bit_len = end - start + 1;
        //FIXME: this is a unoin here.
        int val = ir2->_opnd[i].val;
        int mask = (1 << bit_len) - 1;

        ins |= (val & mask) << start;

        if (bit_field.bit_range_1.start >= 0) {
            val = val >> bit_len;
            start = bit_field.bit_range_1.start;
            end = bit_field.bit_range_1.end;
            bit_len = end - start + 1;
            mask = (1 << bit_len) - 1;
            ins |= (val & mask) << start;
        }
    }

    return ins;
}

bool ir2_op_check(IR2_INST *ir2)
{
    /* get the instrution format in LA*/
    if (ir2->_opcode - LISA_INVALID < 0) {
        return true;
    }
    GM_LA_OPCODE_FORMAT format = lisa_format_table[ir2->_opcode - LISA_INVALID];
    lsassert(format.type == ir2->_opcode);
    /* check every opnd */
    for (int i = 0; i < 4; ++i) {
        GM_OPERAND_TYPE opnd_type = format.opnd[i];
        IR2_OPND opnd = ir2->_opnd[i];
        if (opnd_type == OPD_INVALID) {
            break;
        }
        GM_LA_OPERAND_PLACE_TYPE place = ir2_type_check_table[opnd_type];
        /*
         * There are three main operand type in LA.
         *
         * 1. Regsiter.
         *    Include GPR, FPR, SCR, FCSR and FCC. The check of them is rigorous.
         * 2. Immediate num.
         *    Include IMMD, IMMH. Either of them can pass the check.
         * 3. Invalid
         *    The operand type is not real invalid, but I don't know how to
         *    define them, please ues these type carefully.
         *    The check will pass, if the type of place is invalid.
         *
         */
        if (place.type == IR2_OPND_NONE) {
            break;
        }
        if (place.type == IR2_OPND_IMM) {
            if (!(opnd._type == IR2_OPND_LABEL ||
                  opnd._type == IR2_OPND_IMM)) {
                return false;
            }
        } else if (opnd._type != place.type) {
            return false;
        }
    }
    return true;
}
