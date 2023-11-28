#include "lib.h"
#include "uart.h"
#include "interrupt.h"

int main(void)
{
  char buf[32];

  INTR_ENABLE;

  while(1) {
    puts("$ ");
    gets(buf);
    if(!strncmp(buf, "echo", 4)) {
      puts(buf + 4);
      puts("\n");
    } else if(!strcmp(buf, "exit")) {
      break;
    } else {
      puts("unknown.\n");
    }
  }

  extern char text_start;
  extern char etext;
  extern char rodata_start;
  extern char erodata;
  extern char intrstack;
  extern char bootstack;

  printf("test_start:   0x%x\n", &text_start);
  printf("etext:        0x%x\n", &etext);
  printf("rodata_start: 0x%x\n", &rodata_start);
  printf("erodata:      0x%x\n", &erodata);
  printf("intrstack:    0x%x\n", &intrstack);
  printf("bootstack:    0x%x\n", &bootstack);
  return 0;
}