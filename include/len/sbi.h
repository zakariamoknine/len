#ifndef _LEN_SBI_H_
#define _LEN_SBI_H_

#include <len/defs.h>

/* SBI Base Extension */
#define SBI_EXT_ID_BASE                  0x10
#define SBI_BASE_GET_SPEC_VERSION        0
#define SBI_BASE_GET_IMPL_ID             1
#define SBI_BASE_GET_IMPL_VERSION        2
#define SBI_BASE_PROBE_EXTENSION         3
#define SBI_BASE_GET_MVENDORID           4
#define SBI_BASE_GET_MARCHID             5
#define SBI_BASE_GET_MIMPID              6

/* Timer (TIME) Extension */
#define SBI_EXT_ID_TIME                  0x54494D45
#define SBI_TIME_SET_TIMER               0

/* IPI (IPI) Extension */                
#define SBI_EXT_ID_IPI                   0x735049
#define SBI_IPI_SEND_IPI                 0
                                         
/* RFENCE (RFNC) Extension */            
#define SBI_EXT_ID_RFNC                  0x52464E43
#define SBI_RFNC_REMOTE_FENCE_I          0
#define SBI_RFNC_REMOTE_SFENCE_VMA       1
#define SBI_RFNC_REMOTE_SFENCE_VMA_ASID  2
#define SBI_RFNC_REMOTE_HFENCE_GVMA_VMID 3
#define SBI_RFNC_REMOTE_HFENCE_GVMA      4
#define SBI_RFNC_REMOTE_HFENCE_VVMA_ASID 5
#define SBI_RFNC_REMOTE_HFENCE_VVMA      6

/* Hart State Management (HSM) Extension */
#define SBI_EXT_ID_HSM		         0x48534D
#define SBI_HSM_HART_START	         0
#define SBI_HSM_HART_STOP	         1
#define SBI_HSM_HART_STATUS	         2
#define SBI_HSM_STATUS_STARTED	         0
#define SBI_HSM_STATUS_STOPPED	         1
#define SBI_HSM_STATUS_START_PENDING     2
#define SBI_HSM_STATUS_STOP_PENDING      3

/* System Reset (SRST) Extension */
#define SBI_EXT_ID_SRST			 0x53525354
#define SBI_SRST_SYSTEM_RESET		 0
#define SBI_SRST_TYPE_SHUTDOWN		 0
#define SBI_SRST_TYPE_COLD_REBOOT	 1
#define SBI_SRST_TYPE_WARM_REBOOT	 2
#define SBI_SRST_REASON_NONE		 0
#define SBI_SRST_REASON_SYSTEM_FAILURE	 1

/* Legacy Extensions */
#define SBI_SET_TIMER		         0
#define SBI_CONSOLE_PUTCHAR	         1
#define SBI_CONSOLE_GETCHAR	         2
#define SBI_CLEAR_IPI		         3
#define SBI_SEND_IPI		         4
#define SBI_REMOTE_FENCE_I	         5
#define SBI_REMOTE_SFENCE_VMA	         6
#define SBI_REMOTE_SFENCE_VMA_ASID       7
#define SBI_SHUTDOWN		         8

#define SBI_CALL0(e, f)			    SBI_CALL5(e, f, 0,  \
						0, 0, 0, 0)
#define SBI_CALL1(e, f, a1)		    SBI_CALL5(e, f, a1, \
						0, 0, 0, 0)
#define SBI_CALL2(e, f, a1, a2)		    SBI_CALL5(e, f, a1, \
						a2, 0, 0, 0)
#define SBI_CALL3(e, f, a1, a2, a3)	    SBI_CALL5(e, f, a1, \
						a2, a3, 0, 0)
#define SBI_CALL4(e, f, a1, a2, a3, a4)	    SBI_CALL5(e, f, a1, \
						a2, a3, a4, 0)
#define SBI_CALL5(e, f, a1, a2, a3, a4, a5) sbi_call(e, f, a1,  \
						a2, a3, a4, a5)
struct sbi_ret {
	long error;
	long value;
};

void sbi_init(void);
int sbi_is_ext_supported(uint64_t id);

int sbi_hsm_hart_start(uint64_t hart, uint64_t start_addr,
		       uint64_t priv);
void sbi_hsm_hart_stop(void);
int sbi_hsm_hart_status(uint64_t hart);

static inline struct sbi_ret sbi_call(uint64_t extid, uint64_t fid,
				      uint64_t arg0, uint64_t arg1, 
				      uint64_t arg2, uint64_t arg3,
				      uint64_t arg4)
{
	struct sbi_ret ret;

	register uintptr_t a0 asm("a0") = (uintptr_t)(arg0);
	register uintptr_t a1 asm("a1") = (uintptr_t)(arg1);
	register uintptr_t a2 asm("a2") = (uintptr_t)(arg2);
	register uintptr_t a3 asm("a3") = (uintptr_t)(arg3);
	register uintptr_t a4 asm("a4") = (uintptr_t)(arg4);
	register uintptr_t a6 asm("a6") = (uintptr_t)(fid);
	register uintptr_t a7 asm("a7") = (uintptr_t)(extid);

	asm volatile("ecall" : "+r"(a0), "+r"(a1) : "r"(a2), "r"(a3),
			"r"(a4), "r"(a6), "r"(a7) : "memory");

	ret.error = a0;
	ret.value = a1;
	return ret;
}

static inline void sbi_console_putchar(int c)
{
	if (sbi_is_ext_supported(SBI_CONSOLE_PUTCHAR)) {
		(void)SBI_CALL1(SBI_CONSOLE_PUTCHAR, 0, c);
	}
}

static inline long sbi_probe_extension(long id)
{
	return (SBI_CALL1(SBI_EXT_ID_BASE, SBI_BASE_PROBE_EXTENSION,
			       	id).value);
}

static inline struct sbi_ret sbi_get_spec_version(void)
{
	return (SBI_CALL0(SBI_EXT_ID_BASE,
			       	SBI_BASE_GET_SPEC_VERSION));
}

#endif /* _LEN_SBI_H_ */
