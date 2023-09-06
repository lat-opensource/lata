#ifndef _ENV_H_
#define _ENV_H_

#include "target/arm/cpu.h"

static inline int env_offset_gpr(int num)
{
    return offsetof(CPUARMState,xregs[num]);//cpu->xregs == gpr
}

static inline int env_offset_pc(void)
{
    return offsetof(CPUARMState,pc);
}

static inline int env_offset_fpr(int num)
{
    return offsetof(CPUARMState,vfp.zregs[num]);
}

#endif
