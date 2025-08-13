/* 
 * NOTE: this is a temporary file that defines the virt board MMIO 
 * for testing purposes, later, better and more flexible methods 
 * will be used
 */

#define MMCLINT 0x20000000
#define MMCLINT_MTIMECMP(hartid) (MMCLINT + 0x4000 + 8 * (hartid))	
