#ifndef _LEN_DTB_H_
#define _LEN_DTB_H_

#include <len/internal.h>
#include <len/mm.h>
#include <len/file.h>

struct dt_resources {
	struct device_node* root;
};

void early_parse_dtb_for_memory(void);

void parse_dtb(void);

/* Defined in kernel/entry.S */
extern uintptr_t dtb_entry;

#endif /* _LEN_DTB_H_ */
