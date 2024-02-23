#pragma once

typedef short intr_type_t;

// typedef void (*softvec_handler_t)(softvec_type_t type, unsigned int )

void interrupt(intr_type_t type, unsigned int sp);