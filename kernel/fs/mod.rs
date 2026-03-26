use crate::{
    error::{KernelError, Result},
    sync::SpinLock,
    drivers::CharDevDescriptor,
};
use core::{
    time::Duration,
    any::Any,
    sync::atomic::{AtomicU64, Ordering},
};
use alloc::{
    collections::btree_map::{BTreeMap, Entry},
    sync::Arc,
    vec::Vec,
};

#[derive(Debug, Copy, Clone, PartialEq, Eq)]
pub enum FileType {
    File,
    Directory,
    Symlink,
    BlockDevice(CharDevDescriptor),
    CharDevice(CharDevDescriptor),
    Fifo,
    Socket,
}


bitflags::bitflags! {
    #[derive(Clone, Copy, Debug)]
    pub struct FilePermissions: u16 {
        const S_IXOTH = 0x0001;
        const S_IWOTH = 0x0002;
        const S_IROTH = 0x0004;
        const S_IXGRP = 0x0008;
        const S_IWGRP = 0x0010;
        const S_IRGRP = 0x0020;
        const S_IXUSR = 0x0040;
        const S_IWUSR = 0x0080;
        const S_IRUSR = 0x0100;
        const S_ISVTX = 0x0200;
        const S_ISGID = 0x0400;
        const S_ISUID = 0x0800;
    }
}

#[derive(Debug, Clone)]
pub struct FileAttr {
    pub id: InodeId,
    pub size: u64,
    pub block_size: u32,
    pub blocks: u64,
    pub atime: Duration,
    pub btime: Duration,
    pub mtime: Duration,
    pub ctime: Duration,
    pub file_type: FileType,
    pub permissions: FilePermissions,
    pub nlinks: u32,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub struct InodeId(u64, u64);

pub trait Inode {
    fn id(&self) -> InodeId;

    fn read_at(&self, offset: u64, buf: &mut [u8]) -> Result<usize>;

    fn write_at(&self, offset: u64, buf: &[u8]) -> Result<usize>;

    fn getattr(&self) -> Result<FileAttr>;

    fn setattr(&self, _attr: FileAttr) -> Result<()>;

    fn lookup(&self, _name: &str) -> Result<Arc<dyn Inode>>;

    fn create(
        &self,
        name: &str,
        file_type: FileType,
        permissions: FilePermissions,
        time: Option<Duration>,
    ) -> Result<Arc<dyn Inode>>;

    fn unlink(&self, _name: &str) -> Result<()>;

    fn link(&self, _name: &str, _inode: Arc<dyn Inode>) -> Result<()>;

    fn exchange(
        &self,
        first_name: &str,
        second_parent: Arc<dyn Inode>,
        second_name: &str,
    ) -> Result<()>;

    fn sync(&self) -> Result<()>;
}

pub trait Filesystem: Send + Sync {
    fn root_inode(&self) -> Result<Arc<dyn Inode>>;

    fn id(&self) -> u64;

    fn magic(&self) -> u64;

    fn sync(&self) -> Result<()>;
}

pub trait BlockDevice: Send + Sync {
    fn read(&self, block_id: u64, buf: &mut [u8]) -> Result<()>;

    fn write(&self, block_id: u64, buf: &[u8]) -> Result<()>;

    fn block_size(&self) -> usize;

    fn sync(&self) -> Result<()>;
}
