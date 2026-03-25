use crate::arch::ArchImpl;

pub mod spinlock;

pub type SpinLock<T> = spinlock::SpinLockIrq<T, ArchImpl>;
