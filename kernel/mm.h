#ifndef _MM_H_
#define _MM_H_

/* 
 * NOTE: this is a temporary file that defines the virt board MMIO 
 * for testing purposes, later, better and more flexible methods 
 * will be used
 */

#include "riscv.h"

#define MMCLINT                  0x02000000
#define MMCLINT_MTIMECMP(hartid) (MMCLINT + 0x4000 + 8 * (hartid))	
#define MMCLINT_MTIME            (MMCLINT + 0xBFF8)

#endif /* _MM_H_ */
