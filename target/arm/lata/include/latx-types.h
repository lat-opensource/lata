#ifndef _TYPES_H_
#define _TYPES_H_

#define BITS_ARE_SET_ANY(value, bits) (((value) & (bits)) != 0)
#define BITS_ARE_SET_ALL(value, bits) (((value) & (bits)) == (bits))
#define BITS_ARE_SET(value, bits) BITS_ARE_SET_ALL(value, bits)
#define BITS_ARE_CLEAR_ANY(value, bits) (((value) & (bits)) != (bits))
#define BITS_ARE_CLEAR_ALL(value, bits) (((value) & (bits)) == 0)
#define BITS_ARE_CLEAR(value, bits) BITS_ARE_CLEAR_ALL(value, bits)
#define BITS_SET(value, bits) ((value) |= (bits))
#define BITS_CLEAR(value, bits) ((value) &= ~(bits))
#define HAS_OTHER_BITS(int, bits) (((int)&(~(bits))) != 0)


typedef char int8;
typedef short int16;
typedef int int32;
//#if __WORDSIZE == 64
typedef long int64;
// #else
// typedef long long int64;
// #endif

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
//#if __WORDSIZE == 64
typedef unsigned long uint64;
// #else
// typedef unsigned long long uint64;
// #endif
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


#ifndef bool
#define bool _Bool
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif



#endif

