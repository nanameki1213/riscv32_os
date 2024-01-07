#ifndef PAGE_H
#define PAGE_H


typedef unsigned int pte_t;

#define PPN(ppn) (ppn<<10)
#define PPN_1(ppn1) (ppn1<<20)
#define PPN_2(ppn2) (ppn2<<10)

#define PTE_D (1<<7)
#define PTE_A (1<<6)
#define PTE_G (1<<5)
#define PTE_U (1<<4)
#define PTE_X (1<<3)
#define PTE_W (1<<2)
#define PTE_R (1<<1)
#define PTE_V (1<<0)

#endif