#include "intr_handler.h"
#include "lib.h"
#include "interrupt.h"

void uart(softvec_type_t type, unsigned long sp)
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

void timer(softvec_type_t type, unsigned long sp)
{
  printf("time up!\n");
  timer_intr_disable();
}
