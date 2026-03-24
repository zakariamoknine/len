use crate::error::{KernelError, Result};
use crate::memory::{
    address::{AddressTranslator, PA},
    region::PhysMemoryRegion,
};
use core::{
    marker::PhantomData,
    ops::{Index, IndexMut},
};

#[derive(Clone)]
pub struct RegionList {
    count: usize,
    max: usize,
    regions: *mut PhysMemoryRegion,
}

impl RegionList {
    pub fn is_empty(&self) -> bool {
        self.count == 0
    }

    pub const fn new(max: usize, region_ptr: *mut PhysMemoryRegion) -> Self {
        Self {
            count: 0,
            max,
            regions: region_ptr,
        }
    }

    fn remove_at(&mut self, index: usize) {
        if index >= self.count {
            return;
        }

        unsafe {
            self.regions
                .add(index)
                .copy_from(self.regions.add(index + 1), self.count - index - 1);
        }

        self.count -= 1;
    }

    pub fn insert_region(&mut self, mut new_region: PhysMemoryRegion) {
        if self.count == self.max {
            panic!("Cannot insert into full region list");
        }

        let mut i = 0;

        while i < self.count {
            let existing = self[i];
            if let Some(merged) = existing.merge(new_region) {
                new_region = merged;

                self.remove_at(i);
                i = 0;
            } else {
                i += 1;
            }
        }

        let insert_idx = self
            .iter()
            .position(|x| x.start_address() > new_region.start_address())
            .unwrap_or(self.count);

        unsafe {
            self.regions
                .add(insert_idx + 1)
                .copy_from(self.regions.add(insert_idx), self.count - insert_idx);
        }

        self.count += 1;
        self[insert_idx] = new_region;
    }

    pub fn iter(&self) -> impl Iterator<Item = PhysMemoryRegion> {
        (0..self.count).map(|x| self[x])
    }

    fn requires_reallocation(&self) -> bool {
        self.count >= self.max - 1
    }
}

impl Index<usize> for RegionList {
    type Output = PhysMemoryRegion;

    fn index(&self, index: usize) -> &Self::Output {
        if index >= self.count {
            panic!(
                "Bmalloc region index {} > region count {}",
                index, self.count
            );
        }

        unsafe { &(*self.regions.add(index)) }
    }
}

impl IndexMut<usize> for RegionList {
    fn index_mut(&mut self, index: usize) -> &mut Self::Output {
        if index >= self.count {
            panic!(
                "Bmalloc region index {} > region count {}",
                index, self.count
            );
        }

        unsafe { &mut (*self.regions.add(index)) }
    }
}

pub struct Bmalloc<T: AddressTranslator<()>> {
    pub memory: RegionList,
    pub res: RegionList,
    permit_region_realloc: bool,
    _phantom: PhantomData<T>,
}

enum RegionListType {
    Mem,
    Res,
}

unsafe impl<T: AddressTranslator<()>> Send for Bmalloc<T> {}

impl<T: AddressTranslator<()>> Bmalloc<T> {
    pub const fn new(memory: RegionList, reserved_list: RegionList) -> Self {
        Self {
            memory,
            res: reserved_list,
            permit_region_realloc: false,
            _phantom: PhantomData,
        }
    }

    pub unsafe fn permit_region_list_reallocs(&mut self) {
        self.permit_region_realloc = true;
    }

    fn find_allocation_location(&self, size: usize, align: usize) -> Option<PA> {
        let mut res_idx = 0;

        for mem_region in self.memory.iter() {
            let mut candidate =
                PhysMemoryRegion::new(mem_region.start_address().align_up(align), size);

            while mem_region.contains(candidate) {
                while res_idx < self.res.count
                    && self.res[res_idx].end_address().addr() <= candidate.start_address().addr()
                {
                    res_idx += 1;
                }

                if res_idx < self.res.count {
                    let res_region = self.res[res_idx];
                    if candidate.overlaps(res_region) {
                        candidate =
                            candidate.with_start_address(res_region.end_address().align_up(align));
                        continue;
                    }
                }

                return Some(candidate.start_address());
            }
        }

        None
    }

    pub fn alloc(&mut self, size: usize, align: usize) -> Result<PA> {
        if self.res.requires_reallocation() {
            self.grow_region_list(RegionListType::Res)?;
        }

        let address = self
            .find_allocation_location(size, align)
            .ok_or(KernelError::NoMemory)?;

        self.res.insert_region(PhysMemoryRegion::new(address, size));

        Ok(address)
    }

    pub fn free(&mut self, addr: PA, size: usize) -> Result<()> {
        let region_to_remove = PhysMemoryRegion::new(addr, size);

        let index_opt = self.res.iter().position(|r| r.contains(region_to_remove));

        if let Some(index) = index_opt {
            let old = self.res[index];

            if self.res.requires_reallocation() {
                self.grow_region_list(RegionListType::Res)?;
            }

            unsafe {
                self.res
                    .regions
                    .add(index)
                    .copy_from(self.res.regions.add(index + 1), self.res.count - index - 1);
            }

            self.res.count -= 1;

            let start_val = old.start_address().addr();
            let end_val = old.end_address().addr();

            let remove_start = region_to_remove.start_address().addr();
            let remove_end = region_to_remove.end_address().addr();

            if remove_start > start_val {
                let left =
                    PhysMemoryRegion::new(PA::from_value(start_val), remove_start - start_val);
                self.res.insert_region(left);
            }

            if remove_end < end_val {
                let right = PhysMemoryRegion::new(PA::from_value(remove_end), end_val - remove_end);
                self.res.insert_region(right);
            }

            Ok(())
        } else {
            Err(KernelError::NoMemory)
        }
    }

    fn grow_region_list(&mut self, list_type: RegionListType) -> Result<()> {
        if !self.permit_region_realloc {
            return Err(KernelError::NoMemory);
        }

        let mut list = match list_type {
            RegionListType::Mem => self.memory.clone(),
            RegionListType::Res => self.res.clone(),
        };

        let old_max = list.max;
        let new_max = old_max * 2;
        let new_size = new_max * core::mem::size_of::<PhysMemoryRegion>();
        let align = core::mem::align_of::<PhysMemoryRegion>();

        if let Some(addr) = self.find_allocation_location(new_size, align) {
            let va = addr.to_va::<T>();
            let new_ptr: *mut PhysMemoryRegion = va.as_mut_ptr().cast();

            unsafe {
                new_ptr.copy_from(list.regions, list.count);
            }

            let old_size = old_max * core::mem::size_of::<PhysMemoryRegion>();
            let old_ptr = PA::from_value(list.regions.expose_provenance());

            list.regions = new_ptr;
            list.max = new_max;

            match list_type {
                RegionListType::Mem => self.memory = list,
                RegionListType::Res => self.res = list,
            }

            let _ = self.free(old_ptr, old_size);

            self.res.insert_region(PhysMemoryRegion::new(
                PA::from_value(new_ptr.expose_provenance()),
                new_size,
            ));

            Ok(())
        } else {
            Err(KernelError::NoMemory)
        }
    }

    pub fn add_reservation(&mut self, region: PhysMemoryRegion) -> Result<()> {
        if self.res.requires_reallocation() {
            self.grow_region_list(RegionListType::Res)?;
        }

        self.res.insert_region(region);

        Ok(())
    }

    pub fn base_ram_base_address(&self) -> Option<PA> {
        if self.memory.is_empty() {
            None
        } else {
            Some(self.memory[0].start_address())
        }
    }

    pub fn add_memory(&mut self, region: PhysMemoryRegion) -> Result<()> {
        if self.memory.requires_reallocation() {
            self.grow_region_list(RegionListType::Mem)?;
        }

        self.memory.insert_region(region);

        Ok(())
    }

    pub fn iter_memory(&self) -> impl Iterator<Item = PhysMemoryRegion> {
        self.memory.iter()
    }

    pub fn get_memory_list(&self) -> RegionList {
        self.memory.clone()
    }
}
