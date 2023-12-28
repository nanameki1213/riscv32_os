#ifndef INTERRUPT_H
#define INTERRUPT_H

extern char softvec;
#define SOFTVEC_ADDR (&softvec)

typedef short softvec_type_t;

// typedef void (*softvec_handler_t)(softvec_type_t type, unsigned int )

#define INTR_ENABLE asm volatile ("csrsi 0x300, 0x0000000a")
#define INTR_DISABLE asm volatile ("csrci 0x300, 0x00000008")

#define SET_VECTOR_ADDRESS(addr) asm volatile ("csrw 0x305, %0; csrw 0x105, %0;" : : "r"(addr))

#define W_MSTATUS(x) asm volatile("csrw 0x300, %0" : : "r"(x))
#define W_MEDELEG(x) asm volatile("csrw 0x302, %0" : : "r"(x))

void interrupt(softvec_type_t type, unsigned int sp);

#endif