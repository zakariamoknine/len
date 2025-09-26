#include <len/config.h>
#include <len/riscv.h>

.section .text

.global _start
_start:
	/* first setup the stack pointer for each HART */
	la      sp, kernel_stacks
	li      a0, KERNEL_STACK_SIZE
	csrr    a1, mhartid

	mul     a0, a0, a1

	sub     sp, sp, a0

	/* set MPP (Machine Previous Privilege) to S-mode */
	csrr    t0, mstatus
	
	li      t1, ~(MSTATUS_MPP_MASK)
	and     t0, t0, t1

	li      t1, MSTATUS_MPP_S
	or      t0, t0, t1

	csrw    mstatus, t0

	/* set the function pointer mret will return to */
	la      t0, kernel_main
	csrw    mepc, t0

	/* disable paging */
	csrw    satp, x0

	/* delegate interrupts and exceptions to S-mode */
	li      t0, 0xFFFF
	csrw    medeleg, t0
	csrw    mideleg, t0
	
	csrr    t0, sie
	li      t1, (SIE_SEIE | SIE_STIE | SIE_SSIE)
	or      t0, t0, t1
	csrw    sie, t0

	/* let S-mode access all physical memory, disable PMP for S-mode */
	li      t0, PMP_ADDR0
	csrw    pmpaddr0, t0

	li      t0, PMP_CFG0
	csrw    pmpcfg0, t0

	/* store mhartid on the tp register for easier access */	
	csrr    t0, mhartid
	mv      tp, t0

	/* schedule timer interrupts */
	la      t0, timer_trap
    csrw    mtvec, t0

    csrr    t0, mstatus
    ori     t0, t0, MSTATUS_MIE
    csrw    mstatus, t0

    csrr    t0, mie
    ori     t0, t0, MIE_MTIE
    csrw    mie, t0

	call    timer_init

	/* jump to kernel_main */
	mret

.section .bss

.p2align 4
.space KERNEL_STACK_SIZE * HART_MAX_NUM
.global kernel_stacks
kernel_stacks:
