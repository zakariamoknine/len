#include "config.h"
#include "riscv.h"

.section .text
.global _entry
_entry:
	/* setup the stack for each HART */
	la      sp, __stack_top
	li      a0, STACK_SIZE
	csrr    a1, mhartid

	mul     a0, a0, a1

	sub     sp, sp, a0

	/* set MMP bit to S-mode */
	csrr    t0, mstatus
	
	li      t1, ~(MSTATUS_MPP_MASK)
	and     t0, t0, t1

	li      t1, MSTATUS_MPP_S
	or      t0, t0, t1

	csrw    mstatus, t0

	/* set the function pointer mret will return to */
	la      t0, kernel_start
	csrw    mepc, t0
	
	/* disable paging */
	csrw    satp, x0

	/* delegate interrupts and exceptions to S-mode */
	li      t0, 0xFFFF
	csrw    medeleg, t0
	csrw    mideleg, t0
	
	csrr    t0, sie
	li      t1, SIE_SEIE | SIE_STIE | SIE_SSIE
	or      t0, t0, t1
	csrw    sie, t0

	/* let S-mode access all physical memory, disable PMP for S-mode */
	li      t0, 0x3FFFFFFF
	csrw    pmpaddr0, t0

	li      t0, 0xF
	csrw    pmpcfg0, t0

	/* TODO: initialize timer for each HART */
	
	/* store mhartid on the tp register for faster access */	
	csrr    t0, mhartid
	mv      tp, t0

	/* jump to kernel_main */
	mret


.section .bss
.p2align 4
.space STACK_SIZE * HART_MAX_NUM
.global __stack_top
__stack_top:
