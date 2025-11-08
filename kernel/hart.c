#include <len/kernel.h>
#include <len/hart.h>
#include <len/csr.h>

struct hart harts[HART_MAX_NUM] = { 0 };

void hart_init(struct hart* ht, uint64_t id, uint64_t hid)
{
	memset(ht, 0, sizeof(struct hart));

	ht->id = id;
	ht->hid = hid;

	w_tp((uint64_t)ht);
}

int irq_state(void)
{
	uint64_t state = r_sstatus();
	return ((state & SSTATUS_SIE) == 0) ? 0 : 1;
}

void irq_enable(void)
{
	struct hart* this_hart = hartid();

	this_hart->irq_disable_nesting--;

	if ((this_hart->irq_disable_nesting == 0) 
			&& this_hart->irq_enabled) {
		/* enable interrupts */
		w_sstatus(r_sstatus() | SSTATUS_SIE);
	}
}

void irq_disable(void)
{
	int state = irq_state();
	struct hart* this_hart = hartid();

	/* disable interrupts */
	w_sstatus(r_sstatus() & ~SSTATUS_SIE);

	if (this_hart->irq_disable_nesting == 0) {
		this_hart->irq_enabled = state;
	}

	this_hart->irq_disable_nesting++;
}
