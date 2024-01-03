#include "timer.h"
#include "defines.h"

void start_timer(int ms)
{
  // タイマ割込み有効化
  timer_intr_enable();

  uint64 *mtimecmp = MTIMECMP;
  uint64 *mtime = MTIME;

  *mtime = 0;
  *mtimecmp = ms * 10000;
}