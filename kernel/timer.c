#include <len/internal.h>
#include <len/timer.h>

struct timer_operations* timer = NULL;
extern struct timer_operations* default_timer;

void timer_register(struct timer_operations* t)
{
	timer = t;
}

void timer_init(void)
{
	if (!timer) {
		timer_register(default_timer);
	}

	timer->init(hartid());
}

void timer_tick(uint64 interval) {
	if (timer) {
		timer->tick(interval);
	}
}

uint64 timer_now(void) {
	if (timer) {
		return timer->now();
	}

	return 0;
}
