#include "lib.h"
#include "uart.h"
#include "interrupt.h"
#include "memlayout.h"
#include "defines.h"
#include "riscv.h"

extern int vectors;

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

  int id = r_mhartid();
  asm volatile("mv tp, %0" : "=r"(id));
  
  // PLICの初期設定
  uint32 *enable_addr = PLIC_ENABLE(id);
  *enable_addr |= (1 << UART_IRQ);
  
  // printf("vectors addr: %x\n", &vectors);
  SET_VECTOR_ADDRESS(&vectors);
  

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

  INTR_ENABLE;
  asm volatile ("csrsi 0x304, 0x001e");
  // puts("$ ");
  // while(1) {
  //   ;
  // }

  return 0;
}