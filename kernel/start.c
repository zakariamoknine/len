#include <len/internal.h>
#include <len/hart.h>
#include <len/dtb.h>
#include <len/mm.h>

void kernel_start(uint64_t hid, phys_addr_t dtb_entry,
		phys_addr_t kernel_base)
{
	log(LOG_INFO, "hid=%u", hid);
	log(LOG_INFO, "dtb_entry=%p", dtb_entry);
	log(LOG_INFO, "kernel_base=%p", kernel_base);

	struct hart* boot_hart;

	boot_hart = &harts[0];
	hart_init(boot_hart, 0, hid);

	for (size_t i = 0; i < HART_MAX_NUM; i++) {
		printk("id=%u, hid=%u\n", harts[i].id, harts[i].hid);
	}

	early_parse_dtb_for_memory();

	mm_init();

	parse_dtb();

	printk("loop forever...\n");
	while (true);
}
