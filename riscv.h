#ifndef RISCV_H_
#define RISCV_H_

static inline int r_mhartid()
{
  int x;
  asm volatile("csrr %0, mhartid" : "=r"(x));
  return x;
}

static inline int r_sie()
{
  int x;
  asm volatile("csrr %0, sip" : "=r"(x));
  return x;
}

static inline void w_sie(int x)
{
  asm volatile("csrw sip, %0" : "=r"(x));
}

#endif