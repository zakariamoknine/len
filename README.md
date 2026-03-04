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
    * Bmalloc: used for early allocations and memory revervations, 
 * **Modular Device Drivers**

# Building

# QEMU

# Contributing

