#ifndef RISCV_H_
#define RISCV_H_

#define INTR_SOURCES_NUM 96
#define INTR_PRIORITY_MAX 7

#define xIE_xEIE (1<<9) // 外部割込み許可
#define xIE_xTIE (1<<5) // タイマ割込み許可
#define xIE_xSIE (1<<1) // ソフトウェア割込み許可

#define xIP_xEIP (1<<9) // 外部割込みペンディング
#define xIP_xTIP (1<<5) // タイマ割り込みペンディング
#define xIP_xSIP (1<<1) // ソフトウェア割込みペンディング

#define MODE_VECTOR 1U
#define MODE_DIRECT 0U

enum {
  UART0_IRQ = 10,
  RTC_IRQ = 11,
  VIRTIO_IRQ = 1
};

static inline int get_mhartid()
{
  int x;
  asm volatile("csrr %0, mhartid" : "=r"(x));
  return x;
}


static inline int get_mideleg()
{
  int x;
  asm volatile("csrr %0, mideleg" : "=r"(x));
  return x;
}

static inline void set_mideleg(int x)
{
  asm volatile("csrw mideleg, %0" : "=r"(x));
}

static inline int get_mtvec()
{
  int x;
  asm volatile("csrr %0, mtvec" : "=r"(x));
  return x;
}

static inline void set_mtvec(int x)
{
  asm volatile("csrw mtvec, %0" : "=r"(x));
}

static inline int get_stvec()
{
  int x;
  asm volatile("csrr %0, stvec" : "=r"(x));
  return x;
}

static inline void set_stvec(int x)
{
  asm volatile("csrw stvec, %0" : "=r"(x));
}

static inline int get_sie()
{
  int x;
  asm volatile("csrr %0, sip" : "=r"(x));
  return x;
}

static inline void set_sie(int x)
{
  asm volatile("csrw sip, %0" : "=r"(x));
}

static inline int get_mie()
{
  int x;
  asm volatile("csrr %0, mip" : "=r"(x));
  return x;
}

static inline void set_mie(int x)
{
  asm volatile("csrw mip, %0" : "=r"(x));
}

#endif