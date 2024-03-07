#include "lib.h"
#include "uart.h"
#include "interrupt.h"
#include "memlayout.h"
#include "memory.h"
#include "defines.h"
#include "riscv.h"
#include "timer.h"
#include "disk.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "page.h"

extern int m_vectors, s_vectors;
extern int intr_serial;

int main(void)
{
  // 初期設定中は外部割り込みを無効にする
  intr_disable();

  printf("kernel is booting\n");

  int id = get_mhartid();
  // printf("target id = %d\n", id);

  // PLICの初期設定

  // スレッショルドビットをクリア
  *(uint32_t*)PLIC_THRESHOLD(id) = 0;

  // プライオリティを設定
  *(uint32_t*)PLIC_PRIORITY(UART0_IRQ) = INTR_PRIORITY_MAX;
  *(uint32_t*)PLIC_PRIORITY(VIRTIO_IRQ) = INTR_PRIORITY_MAX;

  // UARTの割り込みを許可
  int column = UART0_IRQ % 32;
  int row = UART0_IRQ / 32;
  *(uint32_t*)(PLIC_ENABLE(id) + row) |= (1U<<column);
  // printf("column = %d\nrow = %d\n", column, row);
  // printf("plic enable addr = 0x%x\n", PLIC_ENABLE(id) + row);

  column = VIRTIO_IRQ % 32;
  row = VIRTIO_IRQ / 32;
  *(uint32_t*)(PLIC_ENABLE(id) + row) |= (1U<<column);

  set_mideleg(get_mideleg() | xIE_SEIE | xIE_STIE | xIE_SSIE | MIE_MEIE | MIE_MTIE | MIE_MSIE); // Mモードから割り込みを移譲
  // set_mtvec((uint32_t)(&intr_serial) & MODE_DIRECT); // 割り込みベクタを登録
  // set_stvec((uint32_t)(&intr_serial) & MODE_DIRECT); // 割り込みベクタを登録
  set_mtvec((uint32_t)(&m_vectors) | MODE_VECTOR);
  set_stvec((uint32_t)(&s_vectors) | MODE_VECTOR);

  // extern char text_start;
  // extern char etext;
  // extern char rodata_start;
  // extern char erodata;
  // extern char intrstack;
  // extern char bootstack;

  // printf("text_start:   0x%x\n", &text_start);
  // printf("etext:        0x%x\n", &etext);
  // printf("rodata_start: 0x%x\n", &rodata_start);
  // printf("erodata:      0x%x\n", &erodata);
  // printf("intrstack:    0x%x\n", &intrstack);
  // printf("bootstack:    0x%x\n", &bootstack);

  printf("size of uint8_t_t: %d\n", sizeof(uint8_t));
  printf("size of uint16_t: %d\n", sizeof(uint16_t));
  printf("size of uint32_t: %d\n", sizeof(uint32_t));
  printf("size of uint64_t: %d\n", sizeof(uint64_t));

  extern char freearea;
  printf("freearea: 0x%x\n", &freearea);

  uart_intr_recv_enable();

  init_memory();

  // ディスクの初期化
  init_disk();
  char buf[512];
  read_write_disk(buf, 0, 0);
  printf("sector 0 : %s\n", buf);
	read_write_disk(buf, 1, 0);
  // 割込み有効化
  external_intr_enable();
  intr_enable();

  // set_kernel_page();
  page_enable();

  puts("$ ");
  // start_timer(1000);
  while(1) {
    ;
  }

  return 0;
}
