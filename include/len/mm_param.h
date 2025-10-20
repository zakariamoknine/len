#ifndef _LEN_MM_PARAM_H_
#define _LEN_MM_PARAM_H_

#define VM_KERNEL_BASE       0xffffffc000000000UL

#define VM_PAGE_SHIFT	     12
#define VM_PAGE_SIZE	     (1 << VM_PAGE_SHIFT)
#define VM_PAGE_MASK	     (VM_PAGE_SIZE - 1)

#define VM_LEVEL0_SHIFT	     39
#define VM_LEVEL0_SIZE	     (1UL << VM_LEVEL0_SHIFT)
#define VM_LEVEL0_OFFSET     (VM_LEVEL0_SIZE - 1)

#define VM_LEVEL1_SHIFT	     30
#define VM_LEVEL1_SIZE 	     (1UL << VM_LEVEL1_SHIFT)
#define VM_LEVEL1_OFFSET     (VM_LEVEL1_SIZE - 1)

#define VM_LEVEL2_SHIFT	     21
#define VM_LEVEL2_SIZE 	     (1UL << VM_LEVEL2_SHIFT)
#define VM_LEVEL2_OFFSET     (VM_LEVEL2_SIZE - 1)

#define VM_LEVEL3_SHIFT	     12
#define VM_LEVEL3_SIZE 	     (1UL << VM_LEVEL3_SHIFT)
#define VM_LEVEL3_OFFSET     (VM_LEVEL3_SIZE - 1)

#define VM_LEVELN_ENTRIES_SHIFT 9
#define VM_LEVELN_ENTRIES    (1 << VM_LEVELN_ENTRIES_SHIFT)
#define VM_LEVELN_ADDR_MASK  (VM_LEVELN_ENTRIES - 1)

#define VM_PTE_D	     (1 << 7)
#define VM_PTE_A	     (1 << 6)
#define VM_PTE_G	     (1 << 5)
#define VM_PTE_U	     (1 << 4)
#define VM_PTE_X	     (1 << 3)
#define VM_PTE_W	     (1 << 2)
#define VM_PTE_R	     (1 << 1)
#define VM_PTE_V	     (1 << 0)
#define VM_PTE_RX	     (VM_PTE_R | VM_PTE_X)
#define VM_PTE_RWX	     (VM_PTE_R | VM_PTE_W | VM_PTE_X)
#define VM_PTE_KERNEL	     (VM_PTE_V | VM_PTE_R | VM_PTE_W | \
	      	              VM_PTE_A | VM_PTE_D)

#define VM_PTE_PPN0_SHIFT    10
#define VM_PTE_PPN1_SHIFT    19
#define VM_PTE_PPN2_SHIFT    28
#define VM_PTE_PPN3_SHIFT    37
#define VM_PTE_SIZE	     8

#define VM_SATP_MODE_SV39    (8ULL << 60)

#endif /* _LEN_MM_PARAM_H_ */
