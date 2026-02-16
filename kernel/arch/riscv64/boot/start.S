.section .text.boot

/*
 * Kernel's entry point
 *
 * ABI:
 *   a0: physical address to FDT
 *   a1: mhartid
 */
.global _start
_start:
	lla     gp, __global_pointer$
	lla     sp, __boot_stack

	/* Store the fdt pointer and mhartid */
	mv      t5, a0
	mv      t6, a1

	/* Zero .bss */
	lla     t0, __bss_start
	lla     t1, __bss_end
1:
	sd      x0, 0(t0)
	addi	t0, t0, 8
	bltu	t0, t1, 1b

	/* Setup boot kernel pagetables and enable MMU */
	lla     a0, __boot_pagetables_start
	lla     a1, __image_start
	mv      a2, t5
	call    setup_boot_pagetables

	/* Reset gp and sp, now with virtual addresses */
	lla     gp, __global_pointer$
	lla     sp, __boot_stack

	/* Clear the scratch register */
	li      t0, 0
	csrw    sscratch, t0

	/* Off we go */
	tail    start_kernel
2:
	wfi
	j       2b
