#![no_std]
#![no_main]

mod arch;
mod memory;

mod error;
mod panic;

#[unsafe(no_mangle)]
extern "C" fn start_kernel() -> ! {
    loop {}
}
