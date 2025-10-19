#include <len/config.h>

.section .text.boot

/* Used by the linker script: kernel.ld */
.global virtual_kernel_base
.set virtual_kernel_base, VM_KERNEL_BASE

/*
 * The kernel's entry point
 *
 * After SBI has finished inialization, and the bootloader
 * has copied the kernel image to memory, the bootloader will
 * jump here, we expect:
 *
 *	a0 = hart ID
 *	a1 = pointer to DTB
 */
.global _entry
_entry:
	/* Set the global pointer, defined in kernel.ld */
	lla     gp, __global_pointer$

	/* Store the boot processor's hart ID */
	lla     t0, boot_hart_id
	sd      a0, 0(t0)

	/* Store the DTB (Device Tree Blob) entry pointer */
	lla     t0, dtb_entry
	sd      a1, 0(t0)

boot_kernel_pagetables:
	/*
	 * After this routine is called, the physical address
	 * of the base of the kernel is on the s11 register
	 */
	jal     kernel_base_phys_addr

	/*
	 * First, we contruct a 1GB 1:1 identity map where:
	 * 
	 * virtual address = physical address
	 *
	 * This is done because the instant we turn sapt.MODE
	 * to Sv39 the processor starts interpreting the PC
	 * as a virtual address, which will immediatly crash
	 * because we haven't mapped any physical pages yet.
	 *
	 * After the identity mapping is done, we can safely
	 * turn on paging, because the kernel code is mapped
	 * and kernel page tables exist.
	 */
	lla     s1, boot_pagetable_level1

	srli    s2, s11, VM_LEVEL1_SHIFT
	andi    a5, s2, VM_LEVELN_ADDR_MASK

	li      t4, (VM_PTE_KERNEL)
	slli    s2, s2, VM_PTE_PPN2_SHIFT
	or      t6, t4, s2

	li      a6, VM_PTE_SIZE
	mulw    a5, a5, a6
	add     t0, s1, a5
	sd      t6, (t0)

	/*
	 * Second, we construct a virtual address space for the
	 * kernel.
	 * 
	 * len implements the higher-half kernel technique where
	 * the kernel itself lives on a high virtual address space
	 * VM_KERNEL_BASE.
	 */
	lla     s1, boot_pagetable_level1
	lla     s2, boot_pagetable_level2
	srli    s2, s2, VM_PAGE_SHIFT

	li      a5, VM_KERNEL_BASE
	srli	a5, a5, VM_LEVEL1_SHIFT
	andi	a5, a5, VM_LEVELN_ADDR_MASK
	li      t4, VM_PTE_V
	slli	t5, s2, VM_PTE_PPN0_SHIFT
	or      t6, t4, t5

	li      a6, VM_PTE_SIZE
	mulw	a5, a5, a6
	add     t0, s1, a5
	sd      t6, (t0)

	lla     s1, boot_pagetable_level2
	srli	t4, s11, VM_LEVEL2_SHIFT
	li      t2, VM_LEVELN_ENTRIES
	add     t3, t4, t2
	li      t0, (VM_PTE_KERNEL | VM_PTE_X)

1:
	slli	t2, t4, VM_PTE_PPN1_SHIFT
	or      t5, t0, t2
	sd      t5, (s1)
	addi	s1, s1, VM_PTE_SIZE

	addi	t4, t4, 1
	bltu	t4, t3, 1b

	/*
	 * Enabling virtual memory may cause a page fault,
	 * we setup a temporary stvec just in case a page
	 * fault occurs, where we simply continues execution
	 * as if nothing happened.
	 */
	lla     t0, temp_stvec
	sub     t0, t0, s11
	li      t1, VM_KERNEL_BASE
	add     t0, t0, t1
	csrw	stvec, t0

	/* Enable paging */
	lla     s2, boot_pagetable_level1
	srli	s2, s2, VM_PAGE_SHIFT
	li      t0, VM_SATP_MODE_SV39
	or      s2, s2, t0
	sfence.vma
	csrw	satp, s2

	.p2align 4
temp_stvec:
	/*
	 * Set the global pointer again, now with the virtual
	 * address
	 */
	lla     gp, __global_pointer$

	/* Set the trap vector to the real handler */
	la      t0, exception_handler
	csrw    stvec, t0

	/* Clear BSS */
	la      t0, _provided_bss_start
	la      t1, _provided_bss_end
1:
	sd      x0, 0(t0)
	addi	t0, t0, 8
	bltu	t0, t1, 1b

	/* Clear the scratch register */
	li      t0, 0
	csrw    sscratch, t0

	/* Initialize the stack */
	la      sp, boot_stack

	/* Initialize args, a0 and a1 are already initialized */
	mv      a2, s11

	/* Jump to kernel/start.c */
	call    kernel_start

hang:
	wfi
	j       hang

kernel_base_phys_addr:
	lla     t0, arbitrary_address
	ld      t1, 0(t0)
	sub     t1, t1, t0
	li      t2, VM_KERNEL_BASE
	sub     s11, t2, t1
	ret

.p2align 3
arbitrary_address:
	.quad arbitrary_address

.section .data

.p2align 3
.global boot_hard_id
boot_hart_id:
	.space 8
.p2align 3
.global dtb_entry
dtb_entry:
	.space 8

.balign VM_PAGE_SIZE
boot_pagetable_level1:
	.space VM_PAGE_SIZE
boot_pagetable_level2:
	.space VM_PAGE_SIZE

.balign VM_PAGE_SIZE
.space VM_PAGE_SIZE
boot_stack:
