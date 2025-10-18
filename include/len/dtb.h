#ifndef _LEN_DTB_H_
#define _LEN_DTB_H_

#include <len/internal.h>
#include <len/mm.h>
#include <len/file.h>

/* 
 * Defined in the spec:
 *
 * https://devicetree-specification.readthedocs.io/en/stable/flattened-format.html 
 */
struct dtb_header {
	uint32_t magic;
	uint32_t totalsize;
	uint32_t off_dt_struct;
	uint32_t off_dt_strings;
	uint32_t off_mem_rsvmap;
	uint32_t version;
	uint32_t last_comp_version;
	uint32_t boot_cpuid_phys;
	uint32_t size_dt_strings;
	uint32_t size_dt_struct;
};

struct dt_resources {
	struct device_node* root;

	/* Used for early memory subsystem initalization */
	struct mm_resources mm_resrcs;
};

struct mm_resources* early_parse_dtb_for_memory(void);

struct dt_resources* parse_dtb(void);

/* Defined in kernel/entry.S */
extern uintptr_t dtb_entry;

#endif /* _LEN_DTB_H_ */
