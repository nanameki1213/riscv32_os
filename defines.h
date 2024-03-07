#pragma once

#define UART_IRQ 10

#include "lib.h"

static inline void panic(char *str)
{
	printf("%s\n", str);
	while(1) {
		;
	}
}
