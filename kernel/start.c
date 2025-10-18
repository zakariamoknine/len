#include <len/internal.h>

void kernel_start(void)
{
	sbi_console_putchar('a');
	while (1);
}
