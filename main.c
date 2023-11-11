#include "lib.h"
#include "uart.h"

int main(void)
{
  char buf[32];

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

  return 0;
}