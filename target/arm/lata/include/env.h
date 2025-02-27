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

static inline int env_offset_CF(void)
{
    return offsetof(CPUARMState,CF);
}

static inline int env_offset_VF(void)
{
    return offsetof(CPUARMState,VF);
}

static inline int env_offset_ZF(void)
{
    return offsetof(CPUARMState,ZF);
}

static inline int env_offset_NF(void)
{
    return offsetof(CPUARMState,NF);
}

static inline int env_offset_PSTATE(void)
{
    return offsetof(CPUARMState,pstate);
}

static inline int env_offset_FPSCR(void)
{
    return offsetof(CPUARMState, vfp.xregs[ARM_VFP_FPSCR]);
}

static inline int env_offset_jmp_cache(void)
{
    return offsetof(CPUARMState, pc_map_cache);
}

static inline int env_offset_fpstatus(void)
{
    return offsetof(CPUARMState, vfp.fp_status);
}

static inline int env_offset_RMODE(void)
{
    return offsetof(CPUARMState, vfp.fp_status.float_rounding_mode);
}

static inline int env_offset_Exclusive(void)
{
    return offsetof(CPUARMState,exclusive_addr);
}

#define env_offset(item) offsetof(CPUARMState, item)

#endif
