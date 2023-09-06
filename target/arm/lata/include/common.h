#ifndef _COMMON_H_
#define _COMMON_H_

#include "latx-types.h"
#include "mem.h"
#include "error.h"

#define BITS_ARE_SET_ANY(value, bits) (((value) & (bits)) != 0)
#define BITS_ARE_SET_ALL(value, bits) (((value) & (bits)) == (bits))
#define BITS_ARE_SET(value, bits) BITS_ARE_SET_ALL(value, bits)
#define BITS_ARE_CLEAR_ANY(value, bits) (((value) & (bits)) != (bits))
#define BITS_ARE_CLEAR_ALL(value, bits) (((value) & (bits)) == 0)
#define BITS_ARE_CLEAR(value, bits) BITS_ARE_CLEAR_ALL(value, bits)
#define BITS_SET(value, bits) ((value) |= (bits))
#define BITS_CLEAR(value, bits) ((value) &= ~(bits))
#define HAS_OTHER_BITS(int, bits) (((int)&(~(bits))) != 0)

#define X86_PAGE_SIZE_ZERO_BITS_NUM (12)
#define X86_PAGE_SIZE (1 << X86_PAGE_SIZE_ZERO_BITS_NUM) /* 4096 */
#define X86_PAGE_MASK (~(X86_PAGE_SIZE - 1))
#define X86_PAGE_OFFSET(addr) (addr & (X86_PAGE_SIZE - 1))
#define X86_PAGE_IS_ALIGN(addr) (((addr) & (X86_PAGE_SIZE - 1)) == 0)
#define X86_PAGE_ALIGN_CEIL(addr) (((addr) + X86_PAGE_SIZE - 1) & X86_PAGE_MASK)
#define X86_PAGE_ALIGN_FLOOR(addr) ((addr)(&X86_PAGE_MASK))

#define MIPS_PAGE_SIZE_ZERO_BITS_NUM (14)
#define MIPS_PAGE_SIZE (1 << MIPS_PAGE_SIZE_ZERO_BITS_NUM) /* 16384 */
#define MIPS_PAGE_MASK (~(MIPS_PAGE_SIZE - 1))
#define MIPS_PAGE_OFFSET(addr) (addr & (MIPS_PAGE_SIZE - 1))
#define MIPS_PAGE_IS_ALIGN(addr) (((addr) & (MIPS_PAGE_SIZE - 1)) == 0)
#define MIPS_PAGE_ALIGN_CEIL(addr) \
    (((addr) + MIPS_PAGE_SIZE - 1) & MIPS_PAGE_MASK)
#define MIPS_PAGE_ALIGN_FLOOR(addr) ((addr)(&MIPS_PAGE_MASK))

#ifndef offsetof
#define offsetof(st, m) __builtin_offsetof(st, m)
#endif

#ifndef _Bool
#define _Bool char
#endif

#ifndef bool
#define bool _Bool
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

// enum {
//     UNKNOWN_EXTENSION = 96,
//     SIGN_EXTENSION,
//     ZERO_EXTENSION,
// #ifndef TARGET_X86_64
//     EM_MIPS_ADDRESS,
//     EM_X86_ADDRESS,
// #endif
// };
// typedef int8 EXTENSION_MODE;

#ifndef N64
#define N64
#endif

typedef char int8;
typedef short int16;
typedef int int32;
#if __WORDSIZE == 64
typedef long int64;
#else
typedef long long int64;
#endif

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
#if __WORDSIZE == 64
typedef unsigned long uint64;
#else
typedef unsigned long long uint64;
#endif
typedef unsigned long ADDR;
#define PRIADDR "lx"

#ifndef N64
#define N64
#endif

#if defined N64
#define X86_MEMORY_LIMIT (0xc0000000)
#elif defined N32 || defined O32
#define X86_MEMORY_LIMIT (0x30000000)
#else
#error Must define N32, O32, or N64 in Makefile
#endif

typedef int32 longx;
typedef uint32 ulongx;
typedef uint32 ADDRX;
#define PRIADDRX PRIx32
#define PRILONGX PRIx32


#endif /* _COMMON_H_ */
