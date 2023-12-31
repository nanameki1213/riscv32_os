#include "interrupt.h"
#include "riscv.h"

void intr_enable()
{
  set_sie(get_sie() | xIE_xEIE | xIE_xTIE | xIE_xSIE);
  set_mie(get_mie() | xIE_xEIE | xIE_xTIE | xIE_xSIE);
}

void intr_disable()
{
  set_sie(get_sie() & ~(xIE_xEIE | xIE_xTIE | xIE_xSIE));
  set_mie(get_mie() & ~(xIE_xEIE | xIE_xTIE | xIE_xSIE));
}

void interrupt(softvec_type_t type, unsigned int sp)
{
  intr(type, sp);
}