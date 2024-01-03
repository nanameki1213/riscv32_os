#include "timer.h"
#include "defines.h"

static void start_timer(int s)
{
  // タイマ割込み有効化
  timer_intr_enable();

  uint64 *mtimecmp = MTIMECMP;
  uint64 *mtime = MTIME;

  *mtime = 0;
  *mtimecmp = s * 1000;
}