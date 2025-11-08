#include <len/kernel.h>
#include <len/sbi.h>
#include <len/hart.h>
#include <len/dtb.h>
#include <len/mm.h>

void kernel_start(uint64_t hid, phys_addr_t dtb_entry, 
		  phys_addr_t kernel_base)
{
	struct hart* boot_hart;

	/* initalize the boot hart, set as hart 0 */
	boot_hart = &harts[0];
	hart_init(boot_hart, 0, hid);

	sbi_init();

	dtb_find_memory((uintptr_t)dtb_entry);

	dtb_parse((uintptr_t)dtb_entry);

	while (true);
}
