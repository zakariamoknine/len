# len

> [!WARNING]
> len is under developement and is not yet stable.

len is an minimal, monolithic, multi-core UNIX-like kernel written in Rust.

# Features

Several crucial features are implemented to ensure proper operation of the kernel

 * **POSIX system calls**
    * Device I/O: `open()`, `read()`, `write()`, `ioctl()`, `close()`
    * Process Management: `fork()`, `execve()`, `clone()`, `kill()`
    * Memory Management: `mmap()`, `munmap()`, `sbrk()`
    * Filesystem Management: `chmod()`, `mkdir()`, `rmdir()`, `chdir()`
 * **Modular Memory Management**
    * Bmalloc: Used for early boot allocations and memory revervations, 
    * FrameAllocator: Implements a buddy allocator system, used to request frames and map them accordingly 
    * #[global_allocator] configuration, making Rust's Core library heap-allocated types available in the kernel
 * **Modular Device Drivers**
    * TTY: The tty subsystem is different than the standard linux tty, no line control, just a simple ringbuffer
    * Basic PS/2 serial driver
    * FAT32 filesystem driver
    * `tmpfs` In-memory filesystem driver for temporary storage

# Building

# QEMU

# Contributing
