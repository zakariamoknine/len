use crate::error::{KernelError, Result};
use crate::memory::{
    address::{VA, UA},
    page::{PageFrame, PtePermissions},
    region::{PhysMemoryRegion, VirtMemoryRegion},
};
use crate::sync::spinlock::SpinLockIrq;

use alloc::string::String;
use alloc::sync::Arc;
use alloc::vec::Vec;

pub trait CpuOps: 'static {
    fn id() -> usize;

    fn halt() -> !;

    fn disable_interrupts() -> usize;

    fn restore_interrupt_state(flags: usize);

    fn enable_interrupts();
}

pub trait UserAddressSpace: Send + Sync {

    fn new() -> Result<Self>
    where
        Self: Sized;

    fn activate(&self);

    fn deactivate(&self);

    fn map_page(&mut self, page: PageFrame, va: VA, perms: PtePermissions) -> Result<()>;

    fn unmap(&mut self, va: VA) -> Result<PageFrame>;

    fn remap(&mut self, va: VA, new_page: PageFrame, perms: PtePermissions) -> Result<PageFrame>;

    fn protect_range(&mut self, va_range: VirtMemoryRegion, perms: PtePermissions) -> Result<()>;

    fn unmap_range(&mut self, va_range: VirtMemoryRegion) -> Result<Vec<PageFrame>>;
}

pub trait KernAddressSpace: Send {
    fn map_mmio(&mut self, region: PhysMemoryRegion) -> Result<VA>;

    fn map_normal(
        &mut self,
        phys_range: PhysMemoryRegion,
        virt_range: VirtMemoryRegion,
        perms: PtePermissions,
    ) -> Result<()>;
}

pub trait VirtualMemory: CpuOps + Sized {
    type PageTableRoot;

    type ProcessAddressSpace: UserAddressSpace;

    type KernelAddressSpace: KernAddressSpace;

    const PAGE_OFFSET: usize;

    fn kern_address_space() -> &'static SpinLockIrq<Self::KernelAddressSpace, Self>;
}

pub trait Arch: CpuOps + VirtualMemory {
    type UserContext: Sized + Send + Sync + Clone;

    fn name() -> &'static str;

    fn cpu_count() -> usize;

    fn new_user_context(entry_point: VA, stack_top: VA) -> Self::UserContext;

    // Will be available when Task is defined
    //fn context_switch(new: Arc<Task>);

    fn power_off() -> !;

    fn restart() -> !;

    fn get_cmdline() -> Option<String>;

    unsafe fn copy_from_user(src: UA, dst: *mut (), len: usize) -> Result<()>;

    unsafe fn try_copy_from_user(src: UA, dst: *mut (), len: usize) -> Result<()>;

    unsafe fn copy_to_user(src: *const (), dst: UA, len: usize) -> Result<()>;

    unsafe fn copy_strn_from_user(src: UA, dst: *mut u8, len: usize) -> Result<usize>;
}

#[cfg(target_arch = "aarch64")]
mod aarch64;
#[cfg(target_arch = "aarch64")]
pub use self::aarch64::Aarch64 as ArchImpl;

#[cfg(target_arch = "riscv64")]
mod riscv64;
#[cfg(target_arch = "riscv64")]
pub use self::riscv64::Riscv64 as ArchImpl;
