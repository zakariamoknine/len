#ifndef _LEN_CSR_H_
#define _LEN_CSR_H_

#include <len/defs.h>
#include <len/riscv.h>

static inline uint64_t r_mstatus()
{
	uint64_t val;
	asm volatile("csrr %0, mstatus" : "=r" (val));
	return val;
}

static inline uint64_t r_mie()
{
	uint64_t val;
	asm volatile("csrr %0, mie" : "=r" (val));
	return val;
}

static inline void w_mscratch(uint64_t val)
{
	asm volatile("csrw mscratch, %0" : : "r" (val));
}

static inline void w_mtvec(uint64_t val)
{
	asm volatile("csrw mtvec, %0" : : "r" (val));
}

static inline void w_mstatus(uint64_t val)
{
	asm volatile("csrw mstatus, %0" : : "r" (val));
}

static inline void w_mie(uint64_t val)
{
	asm volatile("csrw mie, %0" : : "r" (val));
}

static inline void w_stvec(uint64_t val)
{
	asm volatile("csrw stvec, %0" : : "r" (val));
}

static inline uint64_t r_stvec()
{
	uint64_t val;
	asm volatile("csrr %0, stvec" : "=r" (val));
	return val;
}

static inline void w_satp(uint64_t val)
{
	asm volatile("csrw satp, %0" : : "r" (val));
}

static inline void w_mepc(uint64_t val)
{
	asm volatile("csrw mepc, %0" : : "r" (val));
}

static inline uint64_t r_mideleg()
{
	uint64_t val;
	asm volatile("csrr %0, mideleg" : "=r" (val));
	return val;
}

static inline void w_mideleg(uint64_t val)
{
	asm volatile("csrw mideleg, %0" : : "r" (val));
}

static inline uint64_t r_medeleg()
{
	uint64_t val;
	asm volatile("csrr %0, medeleg" : "=r" (val));
	return val;
}

static inline void w_medeleg(uint64_t x)
{
	asm volatile("csrw medeleg, %0" : : "r" (x));
}

static inline uint64_t r_sie()
{
	uint64_t val;
	asm volatile("csrr %0, sie" : "=r" (val));
	return val;
}

static inline void w_sie(uint64_t val)
{
	asm volatile("csrw sie, %0" : : "r" (val));
}

static inline void w_pmpcfg0(uint64_t val)
{
	asm volatile("csrw pmpcfg0, %0" : : "r" (val));
}

static inline void w_pmpaddr0(uint64_t val)
{
	asm volatile("csrw pmpaddr0, %0" : : "r" (val));
}

static inline void w_tp(uint64_t val)
{
	asm volatile("mv tp, %0" : : "r" (val));
}

static inline uint64_t r_mhartid()
{
	uint64_t val;
	asm volatile("csrr %0, mhartid" : "=r" (val) );
	return val;
}

static inline uint64_t r_sstatus()
{
	uint64_t val;
	asm volatile("csrr %0, sstatus" : "=r" (val) );
	return val;
}

static inline void w_sstatus(uint64_t val)
{
	asm volatile("csrw sstatus, %0" : : "r" (val));
}

static inline uint64_t hartid(void)
{
	uint64_t val;
	asm volatile("mv %0, tp" : "=r" (val));
	return val;
}

static inline void x_mret(void)
{
	asm volatile("mret");
}

#endif /* _LEN_CSR_H_ */
