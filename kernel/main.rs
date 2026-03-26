#![no_std]
#![no_main]

mod arch;
mod memory;
mod sync;
mod drivers;
mod fs;

mod error;
mod panic;

#[unsafe(no_mangle)]
extern "C" fn start_kernel() -> ! {
    loop {}
}
