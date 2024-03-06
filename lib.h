#pragma once

#include "defines.h"

#define va_list __builtin_va_list
#define va_start(v, ...) __builtin_va_start(v, 0)
#define va_arg(v,l) __builtin_va_arg(v,l)
#define va_end(v) __builtin_va_end(v)

void printf(const char *fmt, ...);

int putc(unsigned char c);
int puts(char *s);

unsigned char getc();
int gets(unsigned char *buf);

