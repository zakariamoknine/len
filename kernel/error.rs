use core::convert::Infallible;
use thiserror::Error;

#[derive(Debug, Error, PartialEq, Eq, Clone)]
pub enum PagingError {
    #[error("Physical address not page aligned")]
    PhysNotAligned,

    #[error("Physical address not page aligned")]
    VirtNotAligned,

    #[error("Physical and virtual range sizes do not match")]
    SizeMismatch,

    #[error("Failed to walk to the next level page table")]
    WalkFailed,

    #[error("Invalid page table descriptor encountered")]
    InvalidDescriptor,

    #[error("The region to be mapped is smaller than PAGE_SIZE")]
    TooSmall,

    #[error("The VA range is has already been mapped")]
    AlreadyMapped,
}

#[derive(Error, Debug, PartialEq, Eq, Clone)]
pub enum KernelError {
    #[error("Page table mapping failed: {0}")]
    PagingError(#[from] PagingError),

    #[error("Cannot allocate memory")]
    NoMemory,

    #[error("Invalid value")]
    InvalidValue,

    #[error("{0}")]
    Other(&'static str),
}

pub type Result<T> = core::result::Result<T, KernelError>;

impl From<Infallible> for KernelError {
    fn from(error: Infallible) -> Self {
        match error {}
    }
}
