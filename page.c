#include "riscv.h"
#include "page.h"
#include "memory.h"
#include "lib.h"
#include "stddef.h"
#include "stdint.h"

extern char kernel_base, kernel_end;

void va_to_array(uint32_t *src_addr, va2_t *va)
{
  va[0] = ((size_t)src_addr & 0xfff);
  va[1] = ((size_t)src_addr & 0x3fffff) >> 12;
  va[2] = (size_t)src_addr>>22;
}

uint32_t *array_to_va(va2_t *va)
{
  return (uint32_t*)(va[2]<<22 | va[1]<<12 | va[0]);
}

void set_kernel_page()
{
  uint32_t *kernel_size = (uint32_t*)(&kernel_end - &kernel_base);
  int devide_page_size = (uint32_t)kernel_size / PAGE_SIZE;

  size_t kernel_page_size = ((uint32_t)kernel_size % PAGE_SIZE == 0) ? devide_page_size : devide_page_size + 1;

  // printf("base: %x\n", &kernel_base);
  // printf("end: %x\n", &kernel_end);
  // printf("kernel size: %x\n", kernel_page_size);
  setup_pages((uint32_t*)&kernel_base, kernel_page_size);
}

void page_enable()
{
  set_satp(get_satp() | SATP_SV32);
}

void page_disable()
{
  set_satp(get_satp() & ~SATP_SV32);
}

void identity_map(pte_t last_entry, uint32_t *addr)
{
  uint32_t *ram_page = (uint32_t*)(((uint32_t)addr - RAM_BASE_ADDR) / PAGE_SIZE);

  

}

pte_t *set_new_map(pte_t entry)
{
  if(entry & PTE_G) {
    return (pte_t*)(entry>>10);
  }

  uint32_t *top_addr = alloc_page();
  if(top_addr == NULL) {
    return NULL;
  }

  int page = (uint32_t)top_addr / PAGE_SIZE;

  entry |= PPN(page);
  entry |= PTE_V;

  return (pte_t*)top_addr;
}

size_t setup_page(pte_t *pte, int level, va2_t *addr, size_t page_num)
{
  while(page_num > 0) {
    uint16_t entry_index = addr[level];

    pte_t *child_pte = set_new_map(pte[entry_index]);

    pte[entry_index] |= PTE_V;

    if(level == 1) {
      page_num--;
      pte[entry_index] |= (PTE_X | PTE_W | PTE_R);
    } else {
      size_t remain_pages = setup_page(child_pte, level - 1, addr, page_num);
      page_num = remain_pages;
      pte[entry_index] &= ~(PTE_X | PTE_W | PTE_R);
    }

    if(entry_index == 1023) {
      break;
    }

    addr[level] += 1;
    for(int i = level - 1; i >= 1; i--) {
      addr[i] = 0;
    }
  }

  return page_num;
}

void setup_pages(uint32_t *addr, size_t page_num)
{
  va2_t va[3];
  va_to_array(addr, va);

  uint32_t *pte = alloc_page();
  set_satp(SATP_PPN((uint32_t)pte / 0x1000));

  setup_page(pte, PAGE_LEVEL, (va2_t*)addr, page_num);
}

void clean_page()
{

}

void clean_pages(va2_t *addr)
{

}
