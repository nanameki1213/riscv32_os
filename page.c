#include "page.h"

void pte_init()
{
  // とりあえず128Kまでアイデンティティマップする
  pte_t pp1 = 0;

  // PTEを有効にする
  pp1 |= PTE_V;
  // 1層目なので次へのポインタとする
  pp1 &= ~(PTE_X | PTE_W | PTE_R);
}