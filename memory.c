#include "memory.h"
#include "stddef.h"
#include "stdint.h"

extern char memcho_start, freearea;
extern memstat memcho[4096];

void init_memory()
{
  uint32_t *memcho_addr = &memcho_start;

  uint32_t *free_addr = &freearea;

  free_addr = (uint32_t*)((int)(free_addr) & ~(0xfff)); // 0x1000でアラインメント

  uint32_t i;
  for(i = 0; i < ((uint32_t)free_addr - RAM_BASE_ADDR)/0x1000; i += 1) {
    memcho[i] = MEMSTAT_OS;
  }

  printf("Kernel is using page until %d\n", i);
}

void *alloc_page()
{
  int i;
  for(i = 0; i < RAM_PAGE_NUM; i++) {
    // printf("memcho[%d]: %s\n", i, (memcho[i] == MEMSTAT_AVA) ? "利用可能" : "OSが使用中");
    if(memcho[i] == MEMSTAT_AVA) {
      break;
    }
  }
  if(i == RAM_PAGE_NUM) { // メモリが足りない
    printf("メモリが足りません\n");
    return NULL;
  }
  memcho[i] = MEMSTAT_OS;
  printf("alloc page: %d\n", i);

  return (void*)(i * 0x1000 + RAM_BASE_ADDR);
}

void free_page(void *mem)
{
  int page = ((uint32_t)mem - RAM_BASE_ADDR) / 0x1000;
  memcho[page] = MEMSTAT_AVA;
}
