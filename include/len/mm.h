#ifndef _MM_H_
#define _MM_H_

/* 
 * NOTE: this is a temporary file that defines the virt board MMIO 
 * used for testing purposes
 * 
 * later, better and more flexible methods will be used
 */

#define MM_CLINT                  0x02000000
#define MM_CLINT_MTIMECMP(hid)    (MM_CLINT + 0x4000 + 8 * (hid))	
#define MM_CLINT_MTIME            (MM_CLINT + 0xBFF8)

#endif /* _MM_H_ */
