pub mod address;
pub mod region;
pub mod bmalloc;

pub const PAGE_SIZE: usize = 4096;
pub const PAGE_MASK: usize = PAGE_SIZE - 1;
pub const PAGE_SHIFT: usize = PAGE_SIZE.trailing_zeros() as usize;
