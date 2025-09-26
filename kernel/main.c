#include <len/internal.h>

extern void kernel_trap(void);

int kernel_main(void)
{
	w_stvec((uint64)kernel_trap);

	while (1);

	return 0;
}
