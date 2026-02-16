PAGE_SIZE = 4K;

OUTPUT_ARCH(riscv)
ENTRY(_start)
SECTIONS
{
	. = 0xffff800000000000;
	__image_start = .;

	__text_start = .;
	.text.boot : {
		KEEP(*(.text.boot._start))
		*(.text.boot)
	}
	.text : { *(.text*) }
	__text_end = .;

	.rodata : {
		*(.srodata .srodata.*)
		*(.rodata .rodata.*)
	}

	.data : {
		. = ALIGN(16);
		__global_pointer$ = . + 0x800;
		*(.sdata .sdata.*)
		*(.data .data.*)
	}

	.bss : {
		. = ALIGN(16);
		__bss_start = .;
		*(.sbss .sbss.*)
		*(.bss .bss.*)
		. = ALIGN(16);
		__bss_end = .;
	}

	. = ALIGN(PAGE_SIZE);
	__boot_pagetables_start = .;
	. += PAGE_SIZE * 64;
	__boot_pagetables_end = .;

	. += 4K;
	__boot_stack = .;

	__image_end = .;
}
