#include "lib.h"
#include "uart.h"

int uart_tx(unsigned char c)
{
  unsigned char *tx = UART_BASE_ADDR;
  // txdataレジスタに値を設定
  *tx = c;

  return 0;
}

int uart_is_recv_enable()
{
  volatile unsigned char *lsr = UART_BASE_ADDR + NS16550_LSR;

  return (*lsr & NS16550_LSR_RX_READY);
}

unsigned char uart_rx()
{
  volatile unsigned char *rx = UART_BASE_ADDR;
  volatile unsigned char *fcr = UART_BASE_ADDR + NS16550_FCR;
  unsigned char c;

  while(!uart_is_recv_enable()) { // 文字が来るまで待つ
    // uart_tx('1');
    ;
  }
  // uart_tx('!');
  c = *rx;

  return c;
}