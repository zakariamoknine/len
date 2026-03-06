use core::cell::UnsafeCell;
use core::hint::spin_loop;
use core::marker::PhantomData;
use core::ops::{Deref, DerefMut};
use core::sync::atomic::{AtomicBool, Ordering};

use crate::arch::CpuOps;

pub struct SpinLockIrq<T: ?Sized, CPU: CpuOps> {
    lock: AtomicBool,
    _phantom: PhantomData<CPU>,
    data: UnsafeCell<T>,
}

unsafe impl<T: ?Sized + Send, CPU: CpuOps> Send for SpinLockIrq<T, CPU> {}
unsafe impl<T: ?Sized + Send, CPU: CpuOps> Sync for SpinLockIrq<T, CPU> {}

impl<T, CPU: CpuOps> SpinLockIrq<T, CPU> {
    pub const fn new(data: T) -> Self {
        Self {
            lock: AtomicBool::new(false),
            _phantom: PhantomData,
            data: UnsafeCell::new(data),
        }
    }
}

impl<T: ?Sized, CPU: CpuOps> SpinLockIrq<T, CPU> {
    pub fn lock_save_irq(&self) -> SpinLockIrqGuard<'_, T, CPU> {
        let saved_irq_flags = CPU::disable_interrupts();

        while self
            .lock
            .compare_exchange_weak(false, true, Ordering::Acquire, Ordering::Relaxed)
            .is_err()
        {
            while self.lock.load(Ordering::Relaxed) {
                spin_loop();
            }
        }

        SpinLockIrqGuard {
            lock: self,
            irq_flags: saved_irq_flags,
            _marker: PhantomData,
        }
    }
}

#[must_use]
pub struct SpinLockIrqGuard<'a, T: ?Sized + 'a, CPU: CpuOps> {
    lock: &'a SpinLockIrq<T, CPU>,
    irq_flags: usize,
    _marker: PhantomData<*const ()>,
}

impl<'a, T: ?Sized, CPU: CpuOps> Deref for SpinLockIrqGuard<'a, T, CPU> {
    type Target = T;

    fn deref(&self) -> &Self::Target {
        unsafe { &*self.lock.data.get() }
    }
}

impl<'a, T: ?Sized, CPU: CpuOps> DerefMut for SpinLockIrqGuard<'a, T, CPU> {
    fn deref_mut(&mut self) -> &mut Self::Target {
        unsafe { &mut *self.lock.data.get() }
    }
}

impl<'a, T: ?Sized, CPU: CpuOps> Drop for SpinLockIrqGuard<'a, T, CPU> {
    fn drop(&mut self) {
        self.lock.lock.store(false, Ordering::Release);

        CPU::restore_interrupt_state(self.irq_flags);
    }
}
