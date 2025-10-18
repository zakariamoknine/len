#ifndef _LEN_DEFS_H_
#define _LEN_DEFS_H_

#include <len/riscv.h>

#undef NULL
#define NULL ((void*)0)

enum {
	true  = 1,
	false = 0
};

#ifdef ARCHITECTURE_64BIT

	typedef char                int8_t;
	typedef short               int16_t;
	typedef int                 int32_t;
	typedef long                int64_t;
	
	typedef unsigned char       uint8_t;
	typedef unsigned short      uint16_t;
	typedef unsigned int        uint32_t;
	typedef unsigned long       uint64_t;
	typedef unsigned long       uintptr_t;
	
	typedef uintptr_t           size_t;
	typedef long                ptrdiff_t;

#else /* ARCHITECTURE_32BIT */

	typedef char                int8_t;
	typedef short               int16_t;
	typedef int                 int32_t;
	typedef long long           int64_t;
	
	typedef unsigned char       uint8_t;
	typedef unsigned short      uint16_t;
	typedef unsigned int        uint32_t;
	typedef unsigned long long  uint64_t;
	typedef unsigned int        uintptr_t;
	
	typedef uintptr_t           size_t;
	typedef int                 ptrdiff_t;

#endif /* ARCHITECTURE_32BIT */

typedef uint16_t                    be16;
typedef uint32_t                    be32;
typedef uint64_t                    be64;

#endif /* _LEN_DEFS_H_ */
