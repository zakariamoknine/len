#include <len/config.h>
#include <len/internal.h>

uint64 timer_scratch[HART_MAX_NUM][5];

/* implemented in trap.S */
extern void timer_handler(void);

static uint64 timer_read_mtime(void)
{
#ifdef ARCHITECTURE_64BIT
	return *(volatile uint64*)(uintptr)MMCLINT_MTIME;
#else /* ARCHITECTURE_32BIT */
	volatile uint32* mtime = (uint32*)(uintptr)MMCLINT_MTIME;
	uint32 hi, lo;
	do {
	    hi = mtime[1];
	    lo = mtime[0];
	} while (hi != mtime[1]);
	return ((uint64)hi << 32) | lo;
#endif
}

static void timer_write_mtimecmp(uint64 hartid, uint64 val)
{
#ifdef ARCHITECTURE_64BIT
	*(volatile uint64*)(uintptr)MMCLINT_MTIMECMP(hartid) = val;
#else /* ARCHITECTURE_32BIT */
	volatile uint32* mtimecmp = (uint32*)(uintptr)MMCLINT_MTIMECMP(hartid);
	mtimecmp[1] = 0xFFFFFFFF;
	mtimecmp[0] = (uint32)val;
	mtimecmp[1] = (uint32)(val >> 32);
#endif
}

void init_timer(void)
{
	uint64 hartid;
	__asm__ __volatile__("csrr %0, mhartid" : "=r" (hartid));

	uint32 interval = 1000000;
	
	uint64 next_mtime = timer_read_mtime() + interval;
	timer_write_mtimecmp(hartid, next_mtime);

	uint64* curr_timer_scratch = &(timer_scratch[hartid][0]);
	curr_timer_scratch[3] = MMCLINT_MTIMECMP(hartid);
	curr_timer_scratch[4] = interval;

	__asm__ __volatile__("csrw mscratch, %0" : : "r" (curr_timer_scratch));
	__asm__ __volatile__("csrw mtvec, %0" : : "r" (timer_handler));
	
	uint32 curr_mstatus;
	__asm__ __volatile__("csrr %0, mstatus" : "=r" (curr_mstatus));
	curr_mstatus |= MSTATUS_MIE;
	__asm__ __volatile__("csrw mstatus, %0" : : "r" (curr_mstatus));

	uint32 curr_mie;
	__asm__ __volatile__("csrr %0, mie" : "=r" (curr_mie));
	curr_mie |= MIE_MTIE;
	__asm__ __volatile__("csrw mie, %0" : : "r" (curr_mie));
}
