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
pub enum FsError {
    #[error("The path or file was not found")]
    NotFound,

    #[error("The path component is not a directory.")]
    NotADirectory,

    #[error("The path component is a directory.")]
    IsADirectory,

    #[error("The file or directory already exists.")]
    AlreadyExists,

    #[error("The directory is not empty.")]
    DirectoryNotEmpty,

    #[error("The filesystem is corrupted or has an invalid format.")]
    InvalidFs,

    #[error("The operation is not permitted.")]
    PermissionDenied,

    #[error("Too many open files")]
    TooManyFiles,
}

#[derive(Error, Debug, PartialEq, Eq, Clone)]
pub enum KernelError {
    #[error("Page table mapping failed: {0}")]
    PagingError(#[from] PagingError),

    #[error("Filesystem operation failed: {0}")]
    FsError(#[from] FsError),

    #[error("Cannot allocate memory")]
    NoMemory,

    #[error("Invalid value")]
    InvalidValue,

    #[error("In use")]
    InUse,

    #[error("{0}")]
    Other(&'static str),
}

pub type Result<T> = core::result::Result<T, KernelError>;

impl From<Infallible> for KernelError {
    fn from(error: Infallible) -> Self {
        match error {}
    }
}
