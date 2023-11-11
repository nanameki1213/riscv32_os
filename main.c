#include "lib.h"
#include "uart.h"

extern char getchar();

int main(void)
{

  char buf[32];
  gets(buf);

  puts(buf);

  // asm volatile("li s0, 0x4649");

  while(1);

  return 0;
}