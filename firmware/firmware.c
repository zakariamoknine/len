/*
 * MEMORY:
 * 
 * 0x00000000 -> 0x00007FFF: general-purpose RAM
 * 0x80000000 -> 0x800000FF: TX, RX and status
 * 0x8FFFF6A0 -> 0x8FFFFFFF: VGA 80x30 text mode framebuffer
 */

volatile char* uart_tx     = (volatile char*)(0x80000000);
volatile char* uart_rx     = (volatile char*)(0x80000004);
volatile char* uart_status = (volatile char*)(0x80000008);

volatile char* vga_mem     = (volatile char*)(0x8FFFF6A0);

void vga_clear(void)
{
	for (int i = 0; i < 80 * 30; i++) {
		vga_mem[i] = 0x00;
	}
}

void vga_putchar(char c)
{
	static int row = 0, col = 0;

	switch (c) {
		case '\r':
			col = 0;
			row++;
			break;
		case '\b':
			if (col > 0) {
				col--;
				vga_mem[row * 80 + col] = 0x00;
			}
			break;
		default:
			vga_mem[row * 80 + col] = c;
			col++;
			break;
	}

	if (col >= 80) {
		col = 0;
		row++;
	}
	if (row >= 30) {
		vga_clear();
		col = 0;
		row = 0;
	}
}

void uart_putchar(char c)
{
	while (!(*uart_status & 0x02));
	*uart_tx = c;
}

char uart_getchar(void)
{
	while (!(*uart_status & 0x01));
	return *uart_rx;
}

void main(void)
{
	while (1) {
		char c = uart_getchar();
		vga_putchar(c);
		uart_putchar(c);
	}
} 
