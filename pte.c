#include "page.h"
#include "riscv.h"

// PPN1のページ番号は，アドレス/0x400000
pte_t ppn1[1024] = {0};
