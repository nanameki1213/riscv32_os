#include "riscv.h"
#include "page.h"
#include "defines.h"
#include "lib.h"

extern char kernel_base, kernel_end;

void va_to_2level(uint32 *src_addr, va2_t *va)
{
  va[0] = ((size_t)src_addr & 0x3fffff) >> 12;
  va[1] = (size_t)src_addr>>22;
}

void set_kernel_page()
{
  uint32 *kernel_size = (&kernel_end - &kernel_base) / 1024;
  int devide_page_size = (uint32)kernel_size / PAGE_SIZE;

  int kernel_page_size = ((uint32)kernel_size % PAGE_SIZE == 0) ? devide_page_size : devide_page_size + 1;

  // printf("base: %x\n", &kernel_base);
  // printf("end: %x\n", &kernel_end);
  // printf("kernel size: %x\n", kernel_page_size);
  setup_pages(&kernel_base, kernel_page_size);
}

void page_enable()
{
  set_satp(get_satp() | SATP_SV32);
}

void page_disable()
{
  set_satp(get_satp() & ~SATP_SV32);
}

pte_t *set_new_map(pte_t entry)
{
  if(entry & PTE_G) {
    return (pte_t*)(entry>>10);
  }

  uint32 *page = alloc_page();
  if(page == NULL) {
    return NULL;
  }

  entry |= PPN(page);
  entry |= PTE_G;

  return (pte_t*)page;
}

size_t setup_page(pte_t *pte, int level, va2_t *addr, size_t page_num)
{
  while(page_num > 0) {
    uint16 entry_index = addr[level];

    pte_t *child_pte = set_new_map(pte[entry_index]);

    pte[entry_index] |= PTE_V;

    if(level == 0) {
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

void setup_pages(uint32 *addr, size_t page_num)
{
  va2_t va[2];
  va_to_2level(addr, va);

  uint32 *pte = alloc_page();
  set_satp(SATP_PPN((uint32)pte / 0x1000));

  setup_page(pte, PAGE_LEVEL - 1, addr, page_num);
}

void clean_page()
{

}

void clean_pages(va2_t *addr)
{

}