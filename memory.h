#pragma once

#include "riscv.h"

typedef unsigned char memstat;

#define RAM_SIZE 0x8000000
#define RAM_PAGE_NUM (RAM_SIZE / 0x1000)

#define MEMSTAT_AVA  0 // 使用可能
#define MEMSTAT_OS   1 // OSが使用中
#define MEMSTAT_PERL 2 // ペリフェラルが使用中

// メモリ初期化
void init_memory();
// ページ割り当て
void *alloc_page();
// ページの解放
void free_page(void *mem);