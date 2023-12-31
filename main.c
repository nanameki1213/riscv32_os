#include "lib.h"
#include "uart.h"
#include "interrupt.h"
#include "memlayout.h"
#include "defines.h"
#include "riscv.h"

extern int intr_serial;

void intr(softvec_type_t type, unsigned int sp)
{
  int c;
  static char buf[64];
  static int len;

  c = getc();

  if(c != '\n') {
    buf[len++] = c;
  } else {
    buf[len++] = '\0';
    if(!strncmp(buf, "echo", 4)) {
      puts(buf + 4);
      puts("\n");
    } else {
      puts("unknown.\n");
    }
    puts("$ ");
    len = 0;
  }
}

int main(void)
{
  // 初期設定中は外部割り込みを無効にする
  intr_disable();

  int id = get_mhartid();
  printf("target id = %d\n", id);

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
  printf("column = %d\nrow = %d\n", column, row);
  printf("plic enable addr = 0x%x\n", PLIC_ENABLE(id) + row);

  column = VIRTIO_IRQ % 32;
  row = VIRTIO_IRQ / 32;
  *(uint32*)(PLIC_ENABLE(id) + row) |= (1U<<column);

  set_mideleg(get_mideleg() | xIE_SEIE | xIE_STIE | xIE_SSIE); // Mモードから割り込みを移譲
  set_mtvec((uint32)(&intr_serial) & MODE_DIRECT); // 割り込みベクタを登録
  set_stvec((uint32)(&intr_serial) & MODE_DIRECT); // 割り込みベクタを登録

  extern char text_start;
  extern char etext;
  extern char rodata_start;
  extern char erodata;
  extern char intrstack;
  extern char bootstack;

  printf("text_start:   0x%x\n", &text_start);
  printf("etext:        0x%x\n", &etext);
  printf("rodata_start: 0x%x\n", &rodata_start);
  printf("erodata:      0x%x\n", &erodata);
  printf("intrstack:    0x%x\n", &intrstack);
  printf("bootstack:    0x%x\n", &bootstack);

  // 割込み有効化
  intr_enable();
  uart_intr_recv_enable();

  puts("$ ");
  while(1) {
    ;
  }

  return 0;
}