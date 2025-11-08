#ifndef _LEN_HART_H_
#define _LEN_HART_H_

#include <len/defs.h>
#include <len/csr.h>

#define HART_MAX_NUM  16

struct hart {
	uint64_t id;  /* logical ID */
	uint64_t hid; /* hardware ID */

	uint32_t irq_disable_nesting;
	int irq_enabled;
};

void hart_init(struct hart* ht, uint64_t id, uint64_t hid);

int irq_state(void);
void irq_enable(void);
void irq_disable(void);

extern struct hart harts[HART_MAX_NUM];

static inline struct hart* hartid(void)
{
	uintptr_t ptr = (uintptr_t)r_tp();
	return (struct hart*)ptr;
}

#endif /* _LEN_HART_H_ */
