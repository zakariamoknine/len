use crate::{
    error::{KernelError, Result},
    sync::SpinLock,
};
use core::{
    sync::atomic::{AtomicU64, Ordering},
};
use alloc::{
    collections::btree_map::{BTreeMap, Entry},
    sync::Arc,
    vec::Vec,
};

#[repr(u64)]
pub enum ReservedMajors {
    Null = 1,
    Zero = 2,
    Random = 3,
    Uart = 4,
}

pub trait Driver: {
    fn name(&self) -> &'static str;
}

pub trait OpenableDevice: {
    //fn open(&self, args: OpenFlags) -> Result<Arc<OpenFile>>;
}

#[derive(Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Debug)]
pub struct CharDevDescriptor {
    pub major: u64,
    pub minor: u64,
}

pub trait CharDriver: 'static {
    fn get_device(&self, minor: u64) -> Option<Arc<dyn OpenableDevice>>;
}

pub struct DriverManager {
    active_drivers: Vec<Arc<dyn Driver>>,
    char_drivers: BTreeMap<u64, Arc<dyn CharDriver>>,
}

impl DriverManager {
    pub const fn new() -> Self {
        Self {
            active_drivers: Vec::new(),
            char_drivers: BTreeMap::new(),
        }
    }

    pub fn insert_driver(&mut self, driver: Arc<dyn Driver>) {
        self.active_drivers.push(driver);
    }

    pub fn find_by_name(&self, name: &str) -> Option<Arc<dyn Driver>> {
        self.active_drivers.iter().find_map(|drv| {
            if drv.name() == name {
                Some(drv.clone())
            } else {
                None
            }
        })
    }


    pub fn register_char_driver(&mut self, major: u64, driver: Arc<dyn CharDriver>) -> Result<()> {
        match self.char_drivers.entry(major) {
            Entry::Vacant(vacant_entry) => {
                vacant_entry.insert(driver);
                Ok(())
            }
            Entry::Occupied(_) => Err(KernelError::InUse),
        }
    }

    pub fn find_char_driver(&self, major: u64) -> Option<Arc<dyn CharDriver>> {
        self.char_drivers.get(&major).cloned()
    }
}

pub static DRIVERS_MANAGER: SpinLock<DriverManager> = SpinLock::new(DriverManager::new());
