#ifndef RISCV_H_
#define RISCV_H_

#define INTR_SOURCES_NUM 96
#define INTR_PRIORITY_MAX 7

#define MIE_MEIE (1<<11) // 外部割込み許可
#define MIE_MTIE (1<<7) // タイマ割込み許可
#define MIE_MSIE (1<<3) // ソフトウェア割込み許可

#define xIE_SEIE (1<<9) // 外部割込み許可(Sモード)
#define xIE_STIE (1<<5) // タイマ割込み許可(Sモード)
#define xIE_SSIE (1<<1) // ソフトウェア割込み許可(Sモード)

#define MIP_xEIP (1<<9) // 外部割込みペンディング
#define MIP_xTIP (1<<5) // タイマ割り込みペンディング
#define MIP_xSIP (1<<1) // ソフトウェア割込みペンディング

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
  asm volatile("csrr %0, sie" : "=r"(x));
  return x;
}

static inline void set_sie(int x)
{
  asm volatile("csrw sie, %0" : "=r"(x));
}

static inline int get_mie()
{
  int x;
  asm volatile("csrr %0, mie" : "=r"(x));
  return x;
}

static inline void set_mie(int x)
{
  asm volatile("csrw mie, %0" : "=r"(x));
}

static inline void get_sip()
{
  int x;
  asm volatile("csrr %0, sip" : "=r"(x));
  return x;
}

static inline void set_sip(int x)
{
  asm volatile("csrw sip, %0" : "=r"(x));
}

static inline void get_mip()
{
  int x;
  asm volatile("csrr %0, mip" : "=r"(x));
  return x;
}

static inline void set_mip(int x)
{
  asm volatile("csrw mip, %0" : "=r"(x));
}


#endif