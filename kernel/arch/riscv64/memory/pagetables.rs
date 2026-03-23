use crate::{
    error::{PagingError, Result},
    memory::{
        PAGE_SIZE,
        address::{PA, VA},
        region::{PhysMemoryRegion, VirtMemoryRegion},
    },
};
use core::marker::PhantomData;


pub const DESCRIPTORS_PER_PAGE: usize = PAGE_SIZE / core::mem::size_of::<u64>();
pub const LEVEL_MASK: usize = DESCRIPTORS_PER_PAGE - 1;

pub trait PageTableEntry: Sized + Copy + Clone {
    fn is_valid(self) -> bool;

    fn as_raw(self) -> u64;

    fn from_raw(v: u64) -> Self;

    fn invalid() -> Self;
}


#[derive(Clone)]
#[repr(C, align(4096))]
pub struct PageTableArray<K: PageTable> {
    pages: [u64; DESCRIPTORS_PER_PAGE],
    _phantom: PhantomData<K>,
}

impl<K: PageTable> PageTableArray<K> {
    pub const fn new() -> Self {
        Self {
            pages: [0; DESCRIPTORS_PER_PAGE],
            _phantom: PhantomData,
        }
    }
}

impl<K: PageTable> Default for PageTableArray<K> {
    fn default() -> Self {
        Self::new()
    }
}

pub trait PageTable: Clone + Copy {
    const SHIFT: usize;

    type Descriptor: PageTableEntry;

    fn from_ptr(ptr: VA<PageTableArray<Self>>) -> Self;

    fn to_raw_ptr(self) -> *mut u64;

    fn pg_index(va: VA) -> usize {
        (va.addr() >> Self::SHIFT) & LEVEL_MASK
    }

    fn get_desc(self, va: VA) -> Self::Descriptor;

    fn get_idx(self, idx: usize) -> Self::Descriptor;
}

pub trait PageAllocator {
    fn allocate_page_table<T: PageTable>(&mut self) -> Result<PA<PageTableArray<T>>>;
}
