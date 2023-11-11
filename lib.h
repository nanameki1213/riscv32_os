#ifndef LIB_H
#define LIB_H

typedef unsigned int size_t;

int putc(unsigned char c);
int puts(char *s);

unsigned char getc();
int gets(unsigned char *buf);

size_t strlen(const char *s);
char *strcpy(char *s1, const char *s2);
char *strncpy(char *s1, const char *s2, size_t n);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *dest, int val, size_t n);
int memcmp(const void *buf1, const void *buf2, size_t n);

#endif