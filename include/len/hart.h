#ifndef _LEN_HART_H_
#define _LEN_HART_H_

#include <len/config.h>
#include <len/defs.h>

struct hart {
	uint64_t id;  /* logical ID */
	uint64_t hid; /* hardware ID */

	uint32_t interrupts_disable_nesting;
	int interrupts_enabled;
};

void hart_init(struct hart* ht, uint64_t id, uint64_t hid);

int interrupts_state(void);
void interrupts_enable(void);
void interrupts_disable(void);

extern struct hart harts[HART_MAX_NUM];

static inline struct hart* hartid(void)
{
	uintptr_t ptr = (uintptr_t)r_tp();
	return (struct hart*)ptr;
}

#endif /* _LEN_HART_H_ */
