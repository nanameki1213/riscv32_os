#include "riscv.h"
#include "page.h"

extern char pte_addr;

void pte_init()
{
  // とりあえず128Kまでアイデンティティマップする
  pte_t pp1 = 0;

  // PTEを有効にする
  pp1 |= PTE_V;
  // 1層目なので次へのポインタとする
  pp1 &= ~(PTE_X | PTE_W | PTE_R);
  pp1 |= PPN(&pte_addr);

  set_satp(SATP_SV32 | SATP_PPN((int)(&pte_addr) / 4096));
}