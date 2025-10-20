#include <len/internal.h>
#include <len/sbi.h>

int legacy_sbi = false;

int sbi_has_time_ext;
int sbi_has_putchar;

void sbi_init(void)
{
	struct sbi_ret sbi_spec;
		
	sbi_spec = sbi_get_spec_version();

	if (sbi_spec.error != 0) {
		legacy_sbi = true;
		return;
	}

	/*
	 * First look for lagacy extensions, whilst we are on modern
	 * SBI here, we still need those, one example is early
	 * kernel logging
	 */
	if(sbi_probe_extension(SBI_CONSOLE_PUTCHAR) != 0) {
		sbi_has_putchar = true;
		log(LOG_INFO, "putchar extension supported");
	}

	/*
	 * Now we look for modern extensions, those are necessary,
	 * if they're not available we panic
	 */
	if(sbi_probe_extension(SBI_EXT_ID_TIME) != 0) {
		log(LOG_INFO, "time extension supported");
	}
}
