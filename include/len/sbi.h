#ifndef _LEN_SBI_H_
#define _LEN_SBI_H_

#include <len/defs.h>

#define	SBI_SET_TIMER	     0
#define	SBI_CONSOLE_PUTCHAR  1
#define	SBI_CONSOLE_GETCHAR  2

#define	SBI_CALL0(e, f)			    SBI_CALL5(e, f, \
						0, 0, 0, 0, 0)
#define	SBI_CALL1(e, f, a1)		    SBI_CALL5(e, f, a1, \
						0, 0, 0, 0)
#define	SBI_CALL2(e, f, a1, a2)		    SBI_CALL5(e, f, a1, \
						a2, 0, 0, 0)
#define	SBI_CALL3(e, f, a1, a2, a3)	    SBI_CALL5(e, f, a1, \
						a2, a3, 0, 0)
#define	SBI_CALL4(e, f, a1, a2, a3, a4)	    SBI_CALL5(e, f, a1, \
						a2, a3, a4, 0)
#define	SBI_CALL5(e, f, a1, a2, a3, a4, a5) sbi_call(e, f, a1, \
						a2, a3, a4, a5)
struct sbi_ret {
	long error;
	long value;
};

static inline struct sbi_ret sbi_call(uint64_t extid, uint64_t fid,
		uint64_t arg0, uint64_t arg1, uint64_t arg2,
	       	uint64_t arg3, uint64_t arg4)
{
	struct sbi_ret ret;

	register uintptr_t a0 asm("a0") = (uintptr_t)(arg0);
	register uintptr_t a1 asm("a1") = (uintptr_t)(arg1);
	register uintptr_t a2 asm("a2") = (uintptr_t)(arg2);
	register uintptr_t a3 asm("a3") = (uintptr_t)(arg3);
	register uintptr_t a4 asm("a4") = (uintptr_t)(arg4);
	register uintptr_t a6 asm("a6") = (uintptr_t)(fid);
	register uintptr_t a7 asm("a7") = (uintptr_t)(extid);

	asm volatile("ecall" : "+r"(a0), "+r"(a1) : "r"(a2), "r"(a3),
			"r"(a4), "r"(a6), "r"(a7) : "memory");

	ret.error = a0;
	ret.value = a1;
	return ret;
}

static inline void sbi_console_putchar(int c)
{
	(void)SBI_CALL1(SBI_CONSOLE_PUTCHAR, 0, c);
}

#endif /* _LEN_SBI_H_ */
