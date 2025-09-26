#include <len/internal.h>

#define UART0_BASE  0x10000000L
#define UART_THR    0x00
#define UART_LSR    0x05
#define LSR_TX_IDLE 0x20

static inline void uart_putc(char c) {
    volatile unsigned char *thr = (unsigned char *)(UART0_BASE + UART_THR);
    volatile unsigned char *lsr = (unsigned char *)(UART0_BASE + UART_LSR);

    while ((*lsr & LSR_TX_IDLE) == 0);

    *thr = c;
}

void uart_puts(const char *s) {
    while (*s) {
        if (*s == '\n')
            uart_putc('\r');
        uart_putc(*s++);
    }
}

void kernel_trap_handler(void) 
{
	uart_puts("len is going to rise and rise only\n");
}
