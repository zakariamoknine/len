#ifndef _TIMER_H_
#define _TIMER_H_

#include <len/internal.h>

struct timer_operations {
	void (*init)(uint64 hid);
	void (*tick)(uint64 interval);
	uint64 (*now)(void);
};

extern struct timer_operations* timer;

#endif /* _TIMER_H_ */
