#include <len/internal.h>
#include <len/spinlock.h>

void spinlock_acquire(struct spinlock* lock)
{
	interrupts_disable();

	while (__sync_lock_test_and_set(&lock->locked, 1));

	__sync_synchronize();
}

void spinlock_release(struct spinlock* lock)
{
	__sync_synchronize();

	__sync_lock_release(&lock->locked);

	interrupts_enable();
}
