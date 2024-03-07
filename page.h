#pragma once

#include "stdint.h"
#include "stddef.h"

typedef unsigned int pte_t;

typedef uint16_t va2_t;

#define PAGE_LEVEL 2
#define PAGE_SIZE 0x1000

#define PPN(ppn) ((uint32_t)(ppn)<<10)
#define PPN_1(ppn1) ((uint32_t)(ppn1)<<20)
#define PPN_2(ppn2) ((uint32_t)(ppn2)<<10)

#define PTE_D (1<<7)
#define PTE_A (1<<6)
#define PTE_G (1<<5)
#define PTE_U (1<<4)
#define PTE_X (1<<3)
#define PTE_W (1<<2)
#define PTE_R (1<<1)
#define PTE_V (1<<0)

void set_kernel_page();
void page_enable();
void page_disable();
void setup_pages(uint32_t *addr, size_t page_num);
