use crate::memory::{
    PAGE_SHIFT, PAGE_SIZE,
    address::PA,
    region::PhysMemoryRegion,
}

use core::fmt::Display;

#[derive(Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Debug)]
pub struct PageFrame {
    n: usize,
}

impl Display for PageFrame {
    fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
        self.n.fmt(f)
    }
}

impl PageFrame {
    pub fn from_pfn(n: usize) -> Self {
        Self { n }
    }

    pub fn pa(&self) -> PA {
        PA::from_value(self.n << PAGE_SHIFT)
    }

    pub fn as_phys_range(&self) -> PhysMemoryRegion {
        PhysMemoryRegion::new(self.pa(), PAGE_SIZE)
    }

    pub fn value(&self) -> usize {
        self.n
    }

    pub(crate) fn buddy(self, order: usize) -> Self {
        Self {
            n: self.n ^ (1 << order),
        }
    }

    #[must_use]
    pub fn add_pages(self, n: usize) -> Self {
        Self { n: self.n + n }
    }
}

#[derive(Clone, Copy, PartialEq, Eq, Default)]
pub struct PtePermissions {
    read: bool,
    write: bool,
    execute: bool,
    user: bool,
}

impl PtePermissions {
    #[inline]
    pub(crate) const fn from_raw_bits(
        read: bool,
        write: bool,
        execute: bool,
        user: bool,
    ) -> Self {
        Self {
            read,
            write,
            execute,
            user,
        }
    }

    pub const fn ro(user: bool) -> Self {
        Self {
            read: true,
            write: false,
            execute: false,
            user,
        }
    }

    pub const fn rw(user: bool) -> Self {
        Self {
            read: true,
            write: true,
            execute: false,
            user,
        }
    }

    pub const fn rx(user: bool) -> Self {
        Self {
            read: true,
            write: false,
            execute: true,
            user,
        }
    }

    pub const fn rwx(user: bool) -> Self {
        Self {
            read: true,
            write: true,
            execute: true,
            user,
        }
    }

    pub const fn is_read(&self) -> bool {
        self.read
    }

    pub const fn is_write(&self) -> bool {
        self.write
    }

    pub const fn is_execute(&self) -> bool {
        self.execute
    }

    pub const fn is_user(&self) -> bool {
        self.user
    }
}
