use super::{
    PAGE_SHIFT, PAGE_SIZE,
    address::{MemoryKind, Address, Physical, Virtual, User},
};

#[derive(Copy, Clone, PartialEq, Eq)]
pub struct MemoryRegion<T: MemoryKind> {
    address: Address<T, ()>,
    size: usize,
}


impl<T: MemoryKind> MemoryRegion<T> {
    pub const fn new(addr: Address<T, ()>, size: usize) -> Self {
        Self {
            address: addr,
            size: size,
        }
    }

    pub const fn empty() -> Self {
        Self {
            address: Address::from_value(0),
            size: 0,
        }
    }
    pub const fn from_start_end_address(start: Address<T, ()>, end: Address<T, ()>) -> Self {
        assert!(end.addr() >= start.addr());

        Self {
            address: start,
            size: (end.addr() - start.addr()),
        }
    }

    pub fn with_start_address(mut self, new_start: Address<T, ()>) -> Self {
        self.address = new_start;
        self
    }

    pub const fn start_address(self) -> Address<T, ()> {
        self.address
    }

    /// Return the size of the region in bytes.
    pub const fn size(self) -> usize {
        self.size
    }

    pub const fn end_address(self) -> Address<T, ()> {
        Address::from_value(self.address.addr() + self.size)
    }

    pub const fn end_address_inclusive(self) -> Address<T, ()> {
        Address::from_value(self.address.addr() + self.size.saturating_sub(1))
    }

    pub fn is_page_aligned(self) -> bool {
        self.address.is_page_aligned()
    }

    /// Return a new region with the given size, keeping the same start address.
    pub fn with_size(self, size: usize) -> Self {
        Self {
            address: self.address,
            size,
        }
    }

    pub fn overlaps(self, other: Self) -> bool {
        let start1 = self.start_address().addr();
        let end1 = self.end_address().addr();
        let start2 = other.start_address().addr();
        let end2 = other.end_address().addr();

        !(end1 <= start2 || end2 <= start1)
    }

    pub fn is_before(self, other: Self) -> bool {
        self.end_address() <= other.start_address()
    }

    pub fn is_after(self, other: Self) -> bool {
        self.start_address() >= other.end_address()
    }

    pub fn merge(self, other: Self) -> Option<Self> {
        let start1 = self.address;
        let end1 = self.end_address();
        let start2 = other.address;
        let end2 = other.end_address();

        if end1 >= start2 && start1 <= end2 {
            let merged_start = core::cmp::min(start1, start2);
            let merged_end = core::cmp::max(end1, end2);

            Some(Self {
                address: merged_start,
                size: merged_end.addr() - merged_start.addr(),
            })
        } else {
            None
        }
    }

    pub fn contains(self, other: Self) -> bool {
        self.start_address().addr() <= other.start_address().addr()
            && self.end_address().addr() >= other.end_address().addr()
    }

    pub fn contains_address(self, addr: Address<T, ()>) -> bool {
        let val = addr.addr();
        val >= self.start_address().addr() && val < self.end_address().addr()
    }

    pub fn add_pages(self, n: usize) -> Self {
        let offset: Address<T, ()> = Address::from_value(n << PAGE_SHIFT);
        Self {
            address: Address::from_value(self.address.addr() + offset.addr()),
            size: self.size.saturating_sub(offset.addr()),
        }
    }


    pub fn intersection(self, other: Self) -> Option<Self> {
        let intersection_start = core::cmp::max(self.start_address(), other.start_address());
        let intersection_end = core::cmp::min(self.end_address(), other.end_address());

        if intersection_start < intersection_end {
            Some(Self::from_start_end_address(
                intersection_start,
                intersection_end,
            ))
        } else {
            None
        }
    }
}

pub type PhysMemoryRegion = MemoryRegion<Physical>;
pub type VirtMemoryRegion = MemoryRegion<Virtual>;
pub type UserMemoryRegion = MemoryRegion<User>;
