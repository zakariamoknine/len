use super::{
    PAGE_SHIFT, PAGE_SIZE,
    address::{MemoryKind, Address, Physical, Virtual, User},
};

#[derive(Copy, Clone, PartialEq, Eq)]
pub struct MemoryRegion<T: MemoryKind> {
    addr: Address<T, ()>,
    size: usize,
}


impl<T: MemoryKind> MemoryRegion<T> {
    pub const fn new(addr: Address<T, ()>, size: usize) -> Self {
        Self {
            addr: addr,
            size: size,
        }
    }

    pub const fn empty() -> Self {
        Self {
            addr: Address::from_value(0),
            size: 0,
        }
    }
}

pub type PhysMemoryRegion = MemoryRegion<Physical>;
pub type VirtMemoryRegion = MemoryRegion<Virtual>;
pub type UserMemoryRegion = MemoryRegion<User>;
