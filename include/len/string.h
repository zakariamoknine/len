#ifndef _LEN_STRING_H_
#define _LEN_STRING_H_

#include <len/defs.h>

int strcmp(const char* str1, const char* str2);
int strncmp(const char* str1, const char* str2, size_t n);
size_t strlen(const char* str);
size_t strnlen(const char* str, size_t maxlen);
char* strrchr(const char* str, int c);

void* memset(void* ptr, int val, size_t n);
void* memcpy(void* dest, const void* src, size_t n);
int memcmp(const void* a, const void* b, size_t n);
void* memchr(const void* ptr, int val, size_t n);
void* memmove(void* dest, const void* src, size_t n);

#endif /* _LEN_STRING_H_ */
