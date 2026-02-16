use core::marker::PhantomData;
use super::{PAGE_SIZE, PAGE_MASK};

mod sealed {
    pub trait Sealed {}
}

pub trait MemoryKind: sealed::Sealed + Ord + Clone + Copy + PartialEq + Eq {}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub struct Virtual;

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub struct Physical;

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub struct User;

impl sealed::Sealed for Virtual {}
impl sealed::Sealed for Physical {}
impl sealed::Sealed for User {}

impl MemoryKind for Virtual {}
impl MemoryKind for Physical {}
impl MemoryKind for User {}

#[derive(PartialEq, Eq, PartialOrd, Ord)]
#[repr(C)]
pub struct Address<K: MemoryKind, T> {
    addr: usize,
    _phantom: PhantomData<K>,
    _phantom_type: PhantomData<T>,
}

impl<K: MemoryKind, T> Copy for Address<K, T> {}

impl<K: MemoryKind, T> Clone for Address<K, T> {
    fn clone(&self) -> Self {
        *self
    }
}

impl<K: MemoryKind, T> Address<K, T> {
    pub const fn from_value(addr: usize) -> Self {
        Self {
            addr: addr,
            _phantom: PhantomData,
            _phantom_type: PhantomData,
        }
    }

    pub const fn addr(self) -> usize {
        self.addr
    }

    pub const fn is_page_aligned(self) -> bool {
        self.addr & PAGE_MASK == 0
    }

    pub fn page_offset(self) -> usize {
        self.addr & PAGE_MASK
    }

    pub const fn align(self, align: usize) -> Self {
        assert!(align.is_power_of_two());
        Self::from_value(self.addr & !(align - 1))
    }

    pub const fn align_up(self, align: usize) -> Self {
        assert!(align.is_power_of_two());
        Self::from_value((self.addr + (align - 1)) & !(align - 1))
    }

    pub const fn page_aligned(self) -> Self {
        Self::from_value(self.addr & !PAGE_MASK)
    }

    pub const fn add_pages(self, count: usize) -> Self {
        Self::from_value(self.addr + (PAGE_SIZE * count))
    }

    pub const fn add_bytes(self, n: usize) -> Self {
        Self::from_value(self.addr() + n)
    }

    pub fn sub_bytes(self, n: usize) -> Self {
        Self::from_value(self.addr() - n)
    }

    pub const fn null() -> Self {
        Self::from_value(0)
    }

    pub fn is_null(self) -> bool {
        self.addr == 0
    }
}

impl<K: MemoryKind, T: Sized> Address<K, T> {
    pub fn add_objs(self, n: usize) -> Self {
        Self::from_value(self.addr() + core::mem::size_of::<T>() * n)
    }

    pub fn sub_objs(self, n: usize) -> Self {
        Self::from_value(self.addr() - core::mem::size_of::<T>() * n)
    }
}

pub type PA<T = ()> = Address<Physical, T>;
pub type VA<T = ()> = Address<Virtual, T>;
pub type UA<T = ()> = Address<User, T>;

impl<T> PA<T> {
    pub unsafe fn as_ptr(self) -> *const T {
        self.addr() as *const T
    }

    pub unsafe fn as_mut_ptr(self) -> *mut T {
        self.addr() as *mut T
    }

    pub fn to_untyped(self) -> PA {
        PA::from_value(self.addr())
    }

    pub fn to_va<A: AddressTranslator<T>>(self) -> VA<T> {
        A::phys_to_virt(self)
    }

    pub fn cast<U>(self) -> PA<U> {
        PA::from_value(self.addr())
    }
}

impl<T> VA<T> {
    pub fn as_ptr(self) -> *const T {
        self.addr() as *const T
    }

    pub fn as_mut_ptr(self) -> *mut T {
        self.addr() as *mut T
    }

    pub fn from_ptr(ptr: *const T) -> VA<T> {
        Self::from_value(ptr.addr())
    }

    pub fn from_ptr_mut(ptr: *mut T) -> VA<T> {
        Self::from_value(ptr.addr())
    }

    pub fn to_untyped(self) -> VA {
        VA::from_value(self.addr())
    }

    pub fn to_pa<A: AddressTranslator<T>>(self) -> PA<T> {
        A::virt_to_phys(self)
    }

    pub fn cast<U>(self) -> VA<U> {
        VA::from_value(self.addr())
    }
}

impl<T> UA<T> {
    pub fn to_untyped(self) -> UA {
        UA::from_value(self.addr())
    }

    pub fn cast<U>(self) -> UA<U> {
        UA::from_value(self.addr())
    }
}

pub trait AddressTranslator<T>: 'static + Send + Sync {
    fn virt_to_phys(va: VA<T>) -> PA<T>;
    fn phys_to_virt(pa: PA<T>) -> VA<T>;
}

pub struct IdentityTranslator {}

impl<T> AddressTranslator<T> for IdentityTranslator {
    fn virt_to_phys(va: VA<T>) -> PA<T> {
        PA::from_value(va.addr())
    }

    fn phys_to_virt(pa: PA<T>) -> VA<T> {
        VA::from_value(pa.addr())
    }
}
