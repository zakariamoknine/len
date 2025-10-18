#ifndef _LEN_RISCV_H_
#define _LEN_RISCV_H_

/* define the target architecture */
#if __riscv_xlen == 32
/* notify no 32-bit support at compile-time */
#error "len doesn't support 32-bit RISC-V processors"
#define ARCHITECTURE_32BIT
#elif __riscv_xlen == 64
#define ARCHITECTURE_64BIT
#else
#error "undefined __riscv_xlen"
#endif

/* 
 * MSTATUS: Machine Status Registers
 * MPP: Machine Previous Privilege
 *
 * M: Machine mode
 * S: Supervisor mode
 * U: User mode
 */
#define MSTATUS_MPP_MASK (3L << 11)
#define MSTATUS_MPP_M    (3L << 11)
#define MSTATUS_MPP_S    (1L << 11)
#define MSTATUS_MPP_U    (0L << 11)

/* 
 * SSTATUS: Supervisor Status Registers
 *
 * SPP:  Supervisor Previous Privilege
 * SPIE: Supervisor Previous Interrupt Enable
 * UPIE: User Previous Interrupt Enable
 * SIE:  Supervisor Interrupt Enable
 * UIE:  User Interrupt Enable
 */
#define SSTATUS_SPP      (1L << 8)
#define SSTATUS_SPIE     (1L << 5)
#define SSTATUS_UPIE     (1L << 4)
#define SSTATUS_SIE      (1L << 1)
#define SSTATUS_UIE      (1L << 0)

/*
 * SIE: Supervisor Interrupt Enable
 *
 * SEIE: External interrupts
 * STIE: Timer interrupts
 * SSIE: Software interrupts
 */
#define SIE_SEIE (1L << 9)
#define SIE_STIE (1L << 5)
#define SIE_SSIE (1L << 1)

/*
 * MIE: Machine Interrupt Enable
 *
 * MEIE: External interrupts
 * MTIE: Timer interrupts     
 * MSIE: Software interrupts 
 */
#define MIE_MEIE    (1L << 11)
#define MIE_MTIE    (1L << 7)
#define MIE_MSIE    (1L << 3)
#define MSTATUS_MIE (1L << 3)

/* PMP: Physical Memory Protection */
#ifdef ARCHITECTURE_64BIT
#define PMP_ADDR0 0x3FFFFFFFFFFFFFULL
#else
#define PMP_ADDR0 0x3FFFFFFF
#endif
#define PMP_CFG0      0xF

#endif /* _LEN_RISCV_H_ */
