#ifndef _LEN_BIT_H_
#define _LEN_BIT_H_

#include <len/defs.h>

#define ALIGN_UP(x, av)   (((x) + ((av)-1)) & ~((av)-1))
#define ALIGN_DOWN(x, av) ((x) & ~((av)-1))

static inline uint32_t bswap32(uint32_t val) {
	return  ((val & 0x000000FFU) << 24) |
		((val & 0x0000FF00U) <<  8) |
		((val & 0x00FF0000U) >>  8) |
		((val & 0xFF000000U) >> 24);
}

static inline uint64_t bswap64(uint64_t val) {
	return  ((val & 0x00000000000000FFULL) << 56) |
		((val & 0x000000000000FF00ULL) << 40) |
		((val & 0x0000000000FF0000ULL) << 24) |
		((val & 0x00000000FF000000ULL) <<  8) |
		((val & 0x000000FF00000000ULL) >>  8) |
		((val & 0x0000FF0000000000ULL) >> 24) |
		((val & 0x00FF000000000000ULL) >> 40) |
		((val & 0xFF00000000000000ULL) >> 56);
}

static inline uint32_t be32_to_cpu(uint32_t val) {
	return bswap32(val);
}

static inline uint32_t cpu_to_be32(uint32_t val) {
	return bswap32(val);
}

static inline uint64_t be64_to_cpu(uint64_t val) {
	return bswap64(val);
}

static inline uint64_t cpu_to_be64(uint64_t val) {
	return bswap64(val);
}

#endif /* _LEN_BIT_H_ */
