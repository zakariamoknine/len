use core::arch::global_asm;

mod paging;

global_asm!(include_str!("start.S"));
