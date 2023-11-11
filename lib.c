#include "lib.h"
#include "uart.h"

int putc(unsigned char c)
{
  // 改行文字の場合はCRLF(\r\n)にして送信
  if(c == '\n') {
    uart_tx('\r');
    uart_tx('\n');
  } else {
    uart_tx(c);
  }

  return 0;
}

int puts(char *s)
{
  while(*s != '\0') {
    putc(*s);
    s++;
  }

  return 0;
}

unsigned char getc()
{
  unsigned char c = uart_rx();
  c = (c == '\r') ? '\n' : c;
  putc(c); // エコーバック

  return c;
}

int gets(unsigned char *buf)
{
  int i;
  unsigned char c;
  do {
    c = getc();
    if(c == '\n')
      c = '\0';
    buf[i++] = c;
  } while(c);

  return i - 1;
}