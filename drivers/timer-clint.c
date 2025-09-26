#include <len/internal.h>
#include <len/timer.h>

uint64 clint_timer_scratch[HART_MAX_NUM][5];

void clint_timer_init(uint64 hid) 
{
	*(uint64*)MM_CLINT_MTIMECMP(hid) = *(uint64*)MM_CLINT_MTIME + TIMER_INTERVAL;
	uint64* scratch = &clint_timer_scratch[hid][0];
	scratch[3] = MM_CLINT_MTIMECMP(hid);
	scratch[4] = TIMER_INTERVAL;
	w_mscratch((uint64)scratch);
}

void clint_timer_tick(uint64 interval) 
{
}

uint64 clint_timer_now(void) 
{
	return 0;
}

struct timer_operations clint_timer_operations = {
    .init = clint_timer_init,
    .tick = clint_timer_tick,
    .now = clint_timer_now,
};

struct timer_operations* default_timer = &clint_timer_operations;
