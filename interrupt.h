#ifndef INTERRUPT_H
#define INTERRUPT_H

extern char softvec;
#define SOFTVEC_ADDR (&softvec)

typedef short softvec_type_t;

// typedef void (*softvec_handler_t)(softvec_type_t type, unsigned int )

void interrupt(softvec_type_t type, unsigned int sp);

#endif