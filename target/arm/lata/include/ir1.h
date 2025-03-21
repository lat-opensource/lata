#ifndef _IR1_H_
#define _IR1_H_

#define TRANS_FUNC(name) trans_ ## name
#define TRANS_DISAS_FUNC(name) disas_data_proc_ ## name
#define TRANS_FUNC_GEN_REAL(opcode, function) \
[opcode] = function
#define TRANS_FUNC_GEN(opcode, function) \
TRANS_FUNC_GEN_REAL(opcode, TRANS_FUNC(function))
#define TRANS_DISAS_FUNC_GEN(opcode, function) \
TRANS_FUNC_GEN_REAL(opcode, TRANS_DISAS_FUNC(function))

#endif