/* 
 * MMP: Previous Privilege Mode
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
