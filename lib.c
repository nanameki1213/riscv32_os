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
  int i = 0;
  unsigned char c;
  do {
    c = getc();
    if(c == '\n')
      c = '\0';
    buf[i++] = c;
  } while(c);

  return i - 1;
}

size_t strlen(const char *s)
{
  size_t len = 0;
  while(*s++)
    len++;
  
  return len;
}

char *strcpy(char *s1, const char *s2)
{
  char *tmp = s1;
  while(*s1++ = *s2++)
    ;
  return tmp;
}

char *strncpy(char *s1, const char *s2, size_t n)
{
  char *tmp = s1;
  while(n) {
    if(!(*s1++ = *s2++)) break;
    n--;
  }
  while(n--)
    *s1++ = '\0';

  return tmp;
}

int strcmp(const char *s1, const char *s2)
{
  while(*s1 == *s2) {
    if(*s1 == '\0')
      return 0;
    s1++;
    s2++;
  }

  return (unsigned char)*s1 - (unsigned char)*s2;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
  while(n && *s1 && *s2) {
    if(*s1 != *s2)
      return (unsigned char)*s1 - (unsigned char)*s2;
    s1++;
    s2++;
    n--;
  }
  if(!n) return 0;
  if(*s1) return 1;

  return -1;
}

void *memcpy(void *dest, const void *src, size_t n)
{
  char *d = dest;
  const char *s = src;
  for(; n > 0; n--) {
    *d++ = *s++;
  }

  return dest;
}

int memcmp(const void *buf1, const void *buf2, size_t n)
{
  const char *b1 = buf1, *b2 = buf2;
  for(; n > 0; n--) {
    if(*b1 != *b2)
      return (unsigned char)*b1 - (unsigned char)*b2;
    b1++;
    b2++;
  }

  return 0;
}