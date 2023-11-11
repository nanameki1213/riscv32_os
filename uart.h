#ifndef UART_H
#define UART_H

#define UART_BASE_ADDR (0x10000000)
#define NS16550_RBR 0
#define NS16550_THR 0
#define NS16550_LSR 5
#define NS16550_LSR_RX_READY (1<<0)
#define NS16550_LSR_TX_IDLE (1<<5)

int uart_tx(unsigned char c);
unsigned char uart_rx();

#endif