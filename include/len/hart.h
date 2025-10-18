#ifndef _LEN_HART_H_
#define _LEN_HART_H_

#include <len/config.h>
#include <len/defs.h>

struct hart {
	uint32_t interrupts_disable_nesting;
	int interrupts_enabled;
};

struct hart* current_hart();
int interrupts_state(void);
void interrupts_enable(void);
void interrupts_disable(void);

extern struct hart harts[HART_MAX_NUM];

#endif /* _LEN_HART_H_ */
