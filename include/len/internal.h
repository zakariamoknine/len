#ifndef _INTERNAL_H_
#define _INTERNAL_H_

#include "riscv.h"
#include "mm.h"

#ifdef ARCHITECTURE_64BIT
    typedef unsigned int        uint32;
    typedef unsigned long       uint64;
    typedef unsigned long       uintptr;
#else /* ARCHITECTURE_32BIT */
    typedef unsigned int        uint32;
    typedef unsigned long long  uint64;
    typedef unsigned int        uintptr;
#endif

#endif /* _INTERNAL_H_ */
