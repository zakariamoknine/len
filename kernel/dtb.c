#include <len/internal.h>
#include <len/dtb.h>

#include <len/libfdt.h>

/*
 * let 1 be the default
 */
static uint32_t dt_root_addr_cells = 1;
static uint32_t dt_root_size_cells = 1;

static inline uint64_t read_cells(const be32* cell, int size)
{
	uint64_t r = 0;
	for (; size--; cell++)
		r = (r << 32) | be32_to_cpu(*cell);
	return r;
}

static uint64_t dt_next_cell(int s, const be32** cellp)
{
	const be32 *p = *cellp;

	*cellp = p + s;
	return read_cells(p, s);
}

static int dt_find_root_cells(void)
{
	const void* dtb = (const void*)dtb_entry;
	const be32* prop;

	int node = fdt_path_offset(dtb, "/");

	if (node < 0) {
		return false;
	}

	prop = fdt_getprop(dtb, node, "#size-cells", NULL);
	if (prop) {
		dt_root_size_cells = be32_to_cpu(*prop);
	}

	prop = fdt_getprop(dtb, node, "#address-cells", NULL);
	if (prop) {
		dt_root_addr_cells = be32_to_cpu(*prop);
	}

	return true;
}

/*
 * early_parse_dtb_for_memory is used right after entering
 * supervisor mode to find physical addresses of available
 * memory regions on the system.
 *
 * the memory regions are then passed to the memory subsystem 
 * to initalize it's state and get allocators to work properly
 *
 * after mm_init() is issued and allocators are initialized,
 * now we can parse the whole DTB using proper data structures
 * and extract all the resources we need: devices, interrupts..
 *
 * NOTE: we do not parse for reserved memory regions, that's
 * something we might support later, for now every region found
 * in a memory node is excpected to be fully usable, which
 * is indeed a naive assumption
 */
void early_parse_dtb_for_memory(void)
{
	const void* dtb = (const void*)dtb_entry;

	if(fdt_check_header(dtb)) {
		panic("DTB: invalid header");
	}

	if (!dt_find_root_cells()) {
		log(LOG_WARNING, "DTB: root address/size cells not "
			       	"found, default values are used");
	}

	int memory_found = 0;
	int32_t node = 0;
	int32_t nodeee = 0;

	fdt_for_each_subnode(node, dtb, 0) {
		const char* type = fdt_getprop(dtb, node,
				"device_type", NULL);

		if (type == NULL || strcmp(type, "memory") != 0) {
			continue;
		}

		const char* memory_status = fdt_getprop(dtb, node,
				"status", NULL);

		if (memory_status) {
			if (strcmp(memory_status, "ok") && 
				strcmp(memory_status, "okay")) {
				continue;
			}
		}

		const be32* reg;
		const be32* endp;
		int32_t len;

		reg = fdt_getprop(dtb, node, "reg", &len);

		if (!reg) {
			continue;
		}

		endp = reg + (len / sizeof(be32));

		while ((endp - reg) >= (dt_root_addr_cells +
				       	dt_root_size_cells)) {
			uint64_t base, size, index;

			base = dt_next_cell(dt_root_addr_cells, &reg);
			size = dt_next_cell(dt_root_size_cells, &reg);
			
			if (size == 0) {
				continue;
			}

			memory_found = 1;

			// map region
		}
	}

	if (!memory_found) {
		panic("DTB: memory not found");
	}
}

void parse_dtb(void)
{
	/* ... parsing */
}
