#pragma once

#include "riscv.h"

typedef unsigned char memstat;

#define RAM_SIZE 0x8000000
#define RAM_PAGE_NUM (RAM_SIZE / 0x1000)

#define MEMSTAT_AVA  0 // 使用可能
#define MEMSTAT_OS   1 // OSが使用中
#define MEMSTAT_PERL 2 // ペリフェラルが使用中