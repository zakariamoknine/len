#ifndef _LEN_PRINTK_H_
#define _LEN_PRINTK_H_

extern void uart_putc(const char c);
extern void uart_puts(const char* str);
extern char uart_getc(void);

void printk(const char* str, ...);
void panic(const char* str, ...);

#endif /* _LEN_PRINTK_H_ */
