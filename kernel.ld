OUTPUT_ARCH(riscv)
ENTRY(_entry)

SECTIONS
{
	. = virtual_kernel_base;

	.text : {
		KEEP(*(.text.boot))
		*(.text .text.*)
	}

	.rodata : {
		. = ALIGN(16);
		*(.srodata .srodata.*)
		. = ALIGN(16);
		*(.rodata .rodata.*)
	}

	.data : {
		. = ALIGN(16);
		__global_pointer$ = . + 0x800;
		*(.sdata .sdata.*)
		. = ALIGN(16);
		*(.data .data.*)
	}

	.bss : {
		. = ALIGN(16);
		*(.sbss .sbss.*)
		. = ALIGN(16);
		*(.bss .bss.*)
	}

	PROVIDE(_provided_kernel_end = .);
}
