#ifndef _INTERNAL_H_
#define _INTERNAL_H_

#include <len/config.h>
#include <len/riscv.h>
#include <len/mm.h>

#ifndef NULL
	#define NULL ((void*)0)
#endif

#ifdef ARCHITECTURE_64BIT
    typedef int                 int32;
    typedef long                int64;
    typedef long                intptr;
    typedef unsigned int        uint32;
    typedef unsigned long       uint64;
    typedef unsigned long       uintptr;
#else /* ARCHITECTURE_32BIT */
    typedef int                 int32;
    typedef long long           int64;
    typedef int                 intptr;
    typedef unsigned int        uint32;
    typedef unsigned long long  uint64;
    typedef unsigned int        uintptr;
#endif

static inline uint64 hartid(void)
{
	uint64 hid;
	asm volatile("mv %0, tp" : "=r" (hid));
	return hid;
}

static inline uint64 r_mstatus()
{
	uint64 status;
	asm volatile("csrr %0, mstatus" : "=r" (status));
	return status;
}

static inline uint64 r_mie()
{
	uint64 mie;
	asm volatile("csrr %0, mie" : "=r" (mie));
	return mie;
}

static inline void w_mscratch(uint64 val)
{
	asm volatile("csrw mscratch, %0" : : "r" (val));
}

static inline void w_mtvec(uint64 val)
{
	asm volatile("csrw mtvec, %0" : : "r" (val));
}

static inline void w_mstatus(uint64 val)
{
	asm volatile("csrw mstatus, %0" : : "r" (val));
}

static inline void w_mie(uint64 val)
{
	asm volatile("csrw mie, %0" : : "r" (val));
}

#endif /* _INTERNAL_H_ */
