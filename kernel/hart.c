#include <len/internal.h>
#include <len/hart.h>

struct hart harts[HART_MAX_NUM] = { 0 };

void hart_init(struct hart* ht, uint64_t id, uint64_t hid)
{
	memset(ht, 0, sizeof(struct hart));

	ht->id = id;
	ht->hid = hid;

	w_tp((uint64_t)ht);
}

int interrupts_state(void)
{
	uint64_t state = r_sstatus();
	return ((state & SSTATUS_SIE) == 0) ? 0 : 1;
}

void interrupts_enable(void)
{
	struct hart* this_hart = hartid();

	this_hart->interrupts_disable_nesting--;

	if ((this_hart->interrupts_disable_nesting == 0) 
			&& this_hart->interrupts_enabled) {
		/* enable interrupts */
		w_sstatus(r_sstatus() | SSTATUS_SIE);
	}
}

void interrupts_disable(void)
{
	int state = interrupts_state();
	struct hart* this_hart = hartid();

	/* disable interrupts */
	w_sstatus(r_sstatus() & ~SSTATUS_SIE);

	if (this_hart->interrupts_disable_nesting == 0) {
		this_hart->interrupts_enabled = state;
	}

	this_hart->interrupts_disable_nesting++;
}
