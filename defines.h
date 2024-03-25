#pragma once

#define UART_IRQ 10

#include "lib.h"
#include <stdint.h>

static inline void panic(char *str)
{
	printf("%s\n", str);
	while(1) {
		;
	}
}

typedef uint32_t kz_thread_id_t;
typedef int (*kz_func_t)(int argc, char *argv[]);
typedef void (*kz_handler_t)(void);
