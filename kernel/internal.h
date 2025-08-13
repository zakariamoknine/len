#include "config.h"
#include "riscv.h"
#include "mm.h"

#ifdef ARCHITECTURE_64BIT
    typedef unsigned int        uint32;
    typedef unsigned long       uint64;
    typedef unsigned long       uintptr;
#else
    typedef unsigned int        uint32;
    typedef unsigned long long  uint64;
    typedef unsigned int        uintptr;
#endif
