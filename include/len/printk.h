#ifndef _LEN_PRINTK_H_
#define _LEN_PRINTK_H_

#define LOG_INFO    0
#define LOG_WARNING 1
#define LOG_ERROR   2

void printk(const char* str, ...);
void log(int level, const char* str, ...);

void panic(const char* str, ...);

#endif /* _LEN_PRINTK_H_ */
