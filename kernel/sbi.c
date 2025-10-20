#include <len/internal.h>
#include <len/sbi.h>

static int sbi_legacy = false;

static int sbi_has_time_ext = false;
static int sbi_has_ipi_ext = false;
static int sbi_has_rfnc_ext = false;
static int sbi_has_srst_ext = false;

static int sbi_has_putchar = false;
static int sbi_has_getchar = false;
static int sbi_has_time = false;
static int sbi_has_ipi = false;
static int sbi_has_fence_i= false;
static int sbi_has_sfence_vma = false;
static int sbi_has_sfence_vma_asid = false;
static int sbi_has_shutdown = false;

int sbi_hsm_hart_start(uint64_t hart, uint64_t start_addr,
		       uint64_t priv)
{
	struct sbi_ret ret;

	ret = SBI_CALL3(SBI_EXT_ID_HSM, SBI_HSM_HART_START, hart,
		       	start_addr, priv);

	return (ret.error != 0 ? (int)ret.error : 0);
}

void sbi_hsm_hart_stop(void)
{
	(void)SBI_CALL0(SBI_EXT_ID_HSM, SBI_HSM_HART_STOP);
}

int sbi_hsm_hart_status(uint64_t hart)
{
	struct sbi_ret ret;

	ret = SBI_CALL1(SBI_EXT_ID_HSM, SBI_HSM_HART_STATUS, hart);

	return (ret.error != 0 ? (int)ret.error : (int)ret.value);
}

void sbi_init(void)
{
	struct sbi_ret sbi_spec;
		
	sbi_spec = sbi_get_spec_version();

	if (sbi_spec.error != 0) {
		sbi_legacy = true;
		return;
	}

	/*
	 * First look for lagacy extensions, whilst we are on modern
	 * SBI here, we still rely on those if modern ones aren't
	 * available.
	 *
	 * Also putchar is not defined in modern SBI ut it's very
	 * nice to have for early printk()
	 */
	if(sbi_probe_extension(SBI_CONSOLE_PUTCHAR) != 0) {
		sbi_has_putchar = true;
		log(LOG_INFO, "legacy putchar extension supported");
	}
	if(sbi_probe_extension(SBI_CONSOLE_GETCHAR) != 0) {
		sbi_has_getchar = true;
		log(LOG_INFO, "legacy getchar extension supported");
	}
	if(sbi_probe_extension(SBI_SET_TIMER) != 0) {
		sbi_has_time = true;
		log(LOG_INFO, "legacy timer extension supported");
	}
	if(sbi_probe_extension(SBI_SEND_IPI) != 0) {
		sbi_has_ipi = true;
		log(LOG_INFO, "legacy ipi extension supported");
	}
	if(sbi_probe_extension(SBI_REMOTE_FENCE_I) != 0) {
		sbi_has_fence_i = true;
		log(LOG_INFO, "legacy fence_i extension supported");
	}
	if(sbi_probe_extension(SBI_REMOTE_SFENCE_VMA) != 0) {
		sbi_has_sfence_vma = true;
		log(LOG_INFO, "legacy sfence_vma extension "
				"supported");
	}
	if(sbi_probe_extension(SBI_REMOTE_SFENCE_VMA_ASID) != 0) {
		sbi_has_sfence_vma_asid = true;
		log(LOG_INFO, "legacy sfence_vma_asid extension "
			       "supported");
	}
	if(sbi_probe_extension(SBI_SHUTDOWN) != 0) {
		sbi_has_shutdown = true;
		log(LOG_INFO, "legacy reset/shutdown extension "
			       "supported");
	}

	/* Now we look for modern extensions */
	if(sbi_probe_extension(SBI_EXT_ID_TIME) != 0) {
		sbi_has_time_ext = true;
		log(LOG_INFO, "TIME extension supported");
	}
	if(sbi_probe_extension(SBI_EXT_ID_IPI) != 0) {
		sbi_has_ipi_ext = true;
		log(LOG_INFO, "IPI extension supported");
	}
	if(sbi_probe_extension(SBI_EXT_ID_RFNC) != 0) {
		sbi_has_rfnc_ext = true;
		log(LOG_INFO, "RFNC extension supported");
	}
	if(sbi_probe_extension(SBI_EXT_ID_SRST) != 0) {
		sbi_has_srst_ext = true;
		log(LOG_INFO, "SRST extension supported");
	}

	/*
	 * If an important SBI functionality is not available on
	 * both legacy and modern extensions, panic()
	 */
	if (!(sbi_has_time_ext || sbi_has_time)) {
		panic("timer extension not available");
	}
	if (!(sbi_has_ipi_ext || sbi_has_ipi)) {
		panic("ipi extension not available");
	}
	if (!(sbi_has_rfnc_ext || (sbi_has_fence_i &&
				   sbi_has_sfence_vma &&
				   sbi_has_sfence_vma_asid))) {
		panic("fence extension not available");
	}
}

int sbi_is_ext_supported(uint64_t id)
{
	switch (id)
	{
		case SBI_CONSOLE_PUTCHAR:
			return sbi_has_putchar;
			break;
	}
}
