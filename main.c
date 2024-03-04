#include "lib.h"
#include "uart.h"
#include "interrupt.h"
#include "memlayout.h"
#include "memory.h"
#include "defines.h"
#include "riscv.h"
#include "timer.h"
#include "disk.h"
#include "stdbool.h"

extern int m_vectors, s_vectors;
extern int intr_serial;

void uart(intr_type_t type, unsigned int sp)
{
  int c;
  static char buf[64];
  static int len = 0;

  c = getc();

  switch(c) {
    case '\n':
      buf[len++] = '\0';
      if(!strncmp(buf, "echo", 4)) {
        puts(buf + 4);
        puts("\n");
      } else if(!strncmp(buf, "exit", 4)) { 

      } else {
        puts("unknown.\n");
      }
      puts("$ ");
      len = 0;
      break;
    case '\b':
      if(len == 0) {
        putc(' ');
        return;
      }
      len--;
      break;
    default:
      buf[len++] = c;
  }
}

void timer(intr_type_t type, unsigned int sp)
{
  printf("time up!\n");
  timer_intr_disable();
}

int main(void)
{
  // 初期設定中は外部割り込みを無効にする
  intr_disable();

  printf("kernel is booting\n");

  int id = get_mhartid();
  // printf("target id = %d\n", id);

  // PLICの初期設定

  // スレッショルドビットをクリア
  *(uint32*)PLIC_THRESHOLD(id) = 0;

  // プライオリティを設定
  *(uint32*)PLIC_PRIORITY(UART0_IRQ) = INTR_PRIORITY_MAX;
  *(uint32*)PLIC_PRIORITY(VIRTIO_IRQ) = INTR_PRIORITY_MAX;

  // UARTの割り込みを許可
  int column = UART0_IRQ % 32;
  int row = UART0_IRQ / 32;
  *(uint32*)(PLIC_ENABLE(id) + row) |= (1U<<column);
  // printf("column = %d\nrow = %d\n", column, row);
  // printf("plic enable addr = 0x%x\n", PLIC_ENABLE(id) + row);

  column = VIRTIO_IRQ % 32;
  row = VIRTIO_IRQ / 32;
  *(uint32*)(PLIC_ENABLE(id) + row) |= (1U<<column);

  set_mideleg(get_mideleg() | xIE_SEIE | xIE_STIE | xIE_SSIE | MIE_MEIE | MIE_MTIE | MIE_MSIE); // Mモードから割り込みを移譲
  // set_mtvec((uint32)(&intr_serial) & MODE_DIRECT); // 割り込みベクタを登録
  // set_stvec((uint32)(&intr_serial) & MODE_DIRECT); // 割り込みベクタを登録
  set_mtvec((uint32)(&m_vectors) | MODE_VECTOR);
  set_stvec((uint32)(&s_vectors) | MODE_VECTOR);

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

  printf("size of uint8: %d\n", sizeof(uint8));
  printf("size of uint16: %d\n", sizeof(uint16));
  printf("size of uint32: %d\n", sizeof(uint32));
  printf("size of uint64: %d\n", sizeof(uint64));

  extern char freearea;
  printf("freearea: 0x%x\n", &freearea);

  uart_intr_recv_enable();

  init_memory();
  // ディスクの初期化
  init_disk();
  char buf[512];
  read_write_disk(buf, 0, 0);
  printf("sector 0 : %s\n", buf);

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
