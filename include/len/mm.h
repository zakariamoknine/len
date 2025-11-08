#ifndef _LEN_MM_H_
#define _LEN_MM_H_

#include <len/kernel.h>

#define MEMORY_REGIONS_MAX_NUM  10

struct mm_region {
	phys_addr_t base;
	uint64_t size;
};

void mm_insert_region(struct mm_region region);

void mm_exclude_region(struct mm_region region);

#endif /* _LEN_MM_H_ */
