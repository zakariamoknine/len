#include "internal.h"

uint64 timer_scratch[HART_MAX_NUM][5];

/* implemented in trap.S */
extern void timer_handler(void);

void init_timer(void)
{
	uint64 hartid;
	__asm__ __volatile__("csrr %0, mhartid" : "=r" (hartid));

	uint32 interval = 1000000;

	*(volatile uint64*)((uintptr)MMCLINT_MTIMECMP(hartid)) = *(volatile uint64*)((uintptr)MMCLINT_MTIME) + interval;

	uint64* curr_timer_scratch = &(timer_scratch[hartid][0]);
	curr_timer_scratch[3] = MMCLINT_MTIMECMP(hartid);
	curr_timer_scratch[4] = interval;

	__asm__ __volatile__("csrw mscratch, %0" : : "r" (curr_timer_scratch));
	__asm__ __volatile__("csrw mtvec, %0" : : "r" (timer_handler));
	
	uint32 curr_mstatus;
	__asm__ __volatile__("csrr %0, mstatus" : "=r" (curr_mstatus));
	__asm__ __volatile__("crsw mstatus, %0" : : "r" (curr_mstatus | MSTATUS_MIE));

	uint32 curr_mie;
	__asm__ __volatile__("csrr %0, mie" : "=r" (curr_mie));
	__asm__ __volatile__("crsw mie, %0" : : "r" (curr_mie | MIE_MTIE));
}
