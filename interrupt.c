#include "interrupt.h"


void interrupt(softvec_type_t type, unsigned int sp)
{
  intr(type, sp);
}