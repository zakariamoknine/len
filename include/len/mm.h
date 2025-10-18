#ifndef _LEN_MM_H_
#define _LEN_MM_H_

#include <len/internal.h>

struct mm_resources {
	struct {
		uintptr_t base;
		uint64_t size;
	} mm_region[MEMORY_REGION_MAX_NUM];

	uint32_t region_count;
};

void mm_init(struct mm_resources* resources);

#endif /* _LEN_MM_H_ */
