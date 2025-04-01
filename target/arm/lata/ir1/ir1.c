#include "qemu/osdep.h"
#include "target/arm/lata/include/translate.h"
#include "ir1.h"

int ir1_is_branch(DisasContext *ir1)
{
    switch (ir1->insn_type) {
    case AARCH64_A64_B_cond:
    case AARCH64_A64_TBZ:
    case AARCH64_A64_CBZ:
        return 1;
    default:
        return 0;
    }
}

int ir1_is_jmp(DisasContext *ir1)
{
    switch (ir1->insn_type) {
    case AARCH64_A64_BR:
    case AARCH64_A64_B:
        return 1;
    default:
        return 0;
    }
}

int ir1_is_call(DisasContext *ir1)
{
    switch (ir1->insn_type) {
    case AARCH64_A64_BLR:
    case AARCH64_A64_BL:
        return 1;
    default:
        return 0;
    }
}

int ir1_is_ret(DisasContext *ir1)
{
    switch (ir1->insn_type) {
    case AARCH64_A64_RET:
        return 1;
    default:
        return 0;
    }
}

int ir1_is_syscall(DisasContext *ir1)
{
    switch (ir1->insn_type) {
    case AARCH64_A64_SVC:
        return 1;
    default:
        return 0;
    }
}
