# len

> [!WARNING]
> len is under development and is not yet stable.

len is a minimal, monolithic, multi-core UNIX-like kernel written in Rust.

# Features

Several crucial features are implemented to ensure proper operation of the kernel.

 * **POSIX system calls**
    * Device I/O: `open()`, `read()`, `write()`, `ioctl()`, `close()`
    * Process Management: `fork()`, `execve()`, `clone()`, `kill()`
    * Memory Management: `mmap()`, `munmap()`, `sbrk()`
    * Filesystem Management: `chmod()`, `mkdir()`, `rmdir()`, `chdir()`
 * **Modular Memory Management**
    * Bmalloc: Used for early boot allocations and memory reservations. 
    * FrameAllocator: Implements a buddy allocator system. It's used to request frames and map them accordingly 
    * #[global_allocator] configuration, making Rust's Core library heap-allocated types available in the kernel
 * **Modular Device Drivers**
    * TTY subsystem that's different from the standard linux tty, no line control, just a simple ringbuffer
    * Basic PS/2 serial driver
    * FAT32 filesystem driver
    * `tmpfs` in-memory filesystem driver for temporary storage

# Building

### Build

Make sure Rust is installed. Follow the official installation guide available [here](https://rust-lang.org/tools/install/).

You'll need to also install Rust's `riscv64imac` toolchain, which will be installed by default after running the following command, this will also build the kernel.

```bash
$ cargo build --release --target riscv64imac-unknown-none-elf
```

### Emulation

You'll need QEMU installed with support for riscv64 emulation

```bash
$ sudo apt install qemu-system-riscv64
```

You can run QEMU with

```bash
$ cargo run --target riscv64imac-unknown-none-elf
```

# Contributing

All contributions are welcome! Whether you're fixing bugs, adding new features, improving documentation, or suggesting design ideas, your help is very much appreciated.
