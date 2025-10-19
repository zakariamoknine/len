#ifndef _LEN_SPINLOCK_H_
#define _LEN_SPINLOCK_H_

#include <len/defs.h>
#include <len/hart.h>

struct spinlock {
	int locked;
};

void spinlock_acquire(struct spinlock* lock);
void spinlock_release(struct spinlock* lock);

#endif /* _LEN_SPINLOCK_H_ */
