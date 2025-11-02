#include <len/internal.h>
#include <len/printk.h>
#include <len/sbi.h>

/*
 * len never includes compiler headers, but stdarg.h is an exception
 * must be included for compiler-specific variadic macros, used to
 * implement printk(cosnt char* str, ...)
 */
#include <stdarg.h>

static struct spinlock printk_spinlock = { 0 };

static void pchar(char c)
{
	sbi_console_putchar(c);
}

static void pstring(const char* str)
{
	while(*str) {
		pchar(*str++);
	}
}

static const char* digits = "0123456789abcdef";

static void pnumber(uint64_t num, uint8_t base, uint8_t sign)
{
	char buf[32];
	int32_t i = 0;

	if (sign && (int64_t)num < 0) {
		pchar('-');
		num = -(int64_t)num;
	}

	if (num == 0) {
		pchar('0');
		return;
	}

	while (num > 0) {
		buf[i++] = digits[num % base];
		num /= base;
	}

	while (--i >= 0) {
		pchar(buf[i]);
	}
}

static void vprintk(const char* str, va_list ap)
{
	spinlock_acquire(&printk_spinlock);

	for (const char* c = str; *c; c++) {
		if (*c != '%') {
			pchar(*c);
			continue;
		}

		c++;

		switch (*c) {
		case 'c': {
			char ch = (char)va_arg(ap, int);
			pchar(ch);
			break;
		}
		case 's': {
			const char* s = va_arg(ap, const char*);
			pstring(s ? s : "(null)");
			break;
		}
		case 'd': {
			int val = va_arg(ap, int);
			pnumber(val, 10, 1);
			break;
		}
		case 'u': {
			uint32_t val = va_arg(ap, unsigned);
			pnumber(val, 10, 0);
			break;
		}
		case 'x': {
			uint32_t val = va_arg(ap, unsigned);
			pnumber(val, 16, 0);
			break;
		}
		case 'p': {
			uintptr_t val = (uintptr_t)va_arg(ap, void*);
			pstring("0x");
			pnumber(val, 16, 0);
			break;
		}
		case '%':
			pchar('%');
			break;
		default:
			pchar('%');
			pchar(*c);
			break;
		}
	}

	spinlock_release(&printk_spinlock);
}

void printk(const char* str, ...)
{
	va_list ap;
	va_start(ap, str);
	vprintk(str, ap);
	va_end(ap);
}

void log(int level, const char* str, ...)
{
	switch (level) {
	case LOG_INFO:
		printk("[    OK    ]: ");
		break;
	case LOG_WARNING:
		printk("[  WARNING  ]: ");
		break;
	case LOG_ERROR:
		printk("[   ERROR   ]: ");
		break;
	default:
		return;
	}

	va_list ap;
	va_start(ap, str);
	vprintk(str, ap);
	va_end(ap);

	printk("\n");
}

void panic(const char* str, ...)
{
	printk("[ PANIC ]: ");

	va_list ap;
	va_start(ap, str);
	vprintk(str, ap);
	va_end(ap);

	printk("\n");

	while (true);
}
