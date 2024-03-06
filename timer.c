#include "timer.h"
#include "interrupt.h"
#include "stdint.h"

void start_timer(int ms)
{
  // タイマ割込み有効化
  timer_intr_enable();

  uint64_t *mtimecmp = MTIMECMP;
  uint64_t *mtime = MTIME;

  *mtime = 0;
  *mtimecmp = ms * 10000;
}
