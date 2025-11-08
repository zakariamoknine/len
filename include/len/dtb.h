#ifndef _LEN_DTB_H_
#define _LEN_DTB_H_

#include <len/kernel.h>
#include <len/mm.h>
#include <len/file.h>

struct dt_resources {
	struct device_node* root;
};

void dtb_find_memory(uintptr_t dtb_entry);

void dtb_parse(uintptr_t dtb_entry);

#endif /* _LEN_DTB_H_ */
