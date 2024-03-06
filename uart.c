#include "lib.h"
#include "uart.h"

int uart_tx(unsigned char c)
{
  unsigned char *tx = (unsigned char*)UART_BASE_ADDR;
  // txdataレジスタに値を設定
  *tx = c;

  return 0;
}

int uart_is_recv_enable()
{
  volatile unsigned char *lsr = (unsigned char*)(UART_BASE_ADDR + NS16550_LSR);

  return (*lsr & NS16550_LSR_RX_READY);
}

unsigned char uart_rx()
{
  volatile unsigned char *rx = (unsigned char*)UART_BASE_ADDR;
  volatile unsigned char *fcr = (unsigned char*)(UART_BASE_ADDR + NS16550_FCR);
  unsigned char c;

  while(!uart_is_recv_enable()) { // 文字が来るまで待つ
    ;
  }
  c = *rx;

  return c;
}

int uart_intr_is_recv_enable()
{
  volatile unsigned char *ier = (unsigned char*)(UART_BASE_ADDR + NS16550_IER);

  return (*ier & NS16550_IER_RX_INTR);
}

void uart_intr_recv_enable()
{
  volatile unsigned char *ier = (unsigned char*)(UART_BASE_ADDR + NS16550_IER);

  *ier |= NS16550_IER_RX_INTR;
}

void uart_intr_recv_disable()
{
  volatile unsigned char *ier = (unsigned char*)(UART_BASE_ADDR + NS16550_IER);

  *ier &= ~NS16550_IER_RX_INTR;
}
