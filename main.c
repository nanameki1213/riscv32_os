#include "lib.h"
#include "uart.h"
#include "interrupt.h"

int main(void)
{
  char *str = "hello, world\n";

  printf("%d\n", 3);
  // char buf[32];

  // INTR_ENABLE;

  // while(1) {
  //   puts("$ ");
  //   gets(buf);
  //   if(!strncmp(buf, "echo", 4)) {
  //     puts(buf + 4);
  //     puts("\n");
  //   } else if(!strcmp(buf, "exit")) {
  //     break;
  //   } else {
  //     puts("unknown.\n");
  //   }
  // }

  // extern char text_start;
  // extern char etext;
  // extern char rodata_start;
  // extern char erodata;
  // extern char intrstack;
  // extern char bootstack;

  // puts("text_start: 0x");
  // putxval(&text_start, 16);
  // puts("etext: 0x");
  // putxval(&etext, 16);
  // puts("rodata_start: 0x");
  // putxval(&rodata_start, 16);
  // puts("erodata: 0x");
  // putxval(&erodata, 16);
  // puts("intrstack: 0x");
  // putxval(&intrstack, 16);
  // puts("bootstack: 0x");
  // putxval(&bootstack, 16);

  return 0;
}