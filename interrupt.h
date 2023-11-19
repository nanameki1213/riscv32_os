#ifndef INTERRUPT_H
#define INTERRUPT_H

extern char softvec;
#define SOFTVEC_ADDR (&softvec)

typedef short softvec_type_t;

// typedef void (*softvec_handler_t)(softvec_type_t type, unsigned int )

#define INTR_ENABLE asm volatile ("csrsi 0x300, 0x00000008")
#define INTR_DISABLE asm volatile ("csrci 0x300, 0x00000008")

int softvec_init(void);

void interrupt(unsigned int sp);

#endif