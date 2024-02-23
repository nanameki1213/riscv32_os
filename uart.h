#pragma once

#define UART_BASE_ADDR (0x10000000)
#define NS16550_RBR 0
#define NS16550_THR 0
#define NS16550_IER 1
#define NS16550_FCR 2
#define NS16550_LSR 5
#define NS16550_FCR_FIFO_ENABLE (1<<0)
#define NS16550_LSR_RX_READY (1<<0)
#define NS16550_LSR_TX_IDLE (1<<5)
#define NS16550_IER_RX_INTR (1<<0)
#define NS16550_IER_TX_INTR (1<<1)

int uart_tx(unsigned char c);
unsigned char uart_rx();

// int uart_intr_is_send_enable();
// void uart_intr_send_enable();
// void uart_intr_send_disable();

int uart_intr_is_recv_enable();
void uart_intr_recv_enable();
void uart_intr_recv_disable();