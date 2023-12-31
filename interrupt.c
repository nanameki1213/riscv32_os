#include "interrupt.h"
#include "riscv.h"

// S，Mモード両方の割込みを有効にする
void intr_enable()
{
  // ペンディングを表すレジスタをクリア
  set_sip(0);
  set_mip(0);

  // set_sie(get_sie() | xIE_SEIE | xIE_STIE | xIE_SSIE);
  set_mie(get_mie() | MIE_MEIE | MIE_MTIE | MIE_MSIE | xIE_SEIE | xIE_STIE | xIE_SSIE);
}

// S，Mモード両方の割込みを無効にする
void intr_disable()
{
  // set_sie(get_sie() & ~(xIE_SEIE | xIE_STIE | xIE_SSIE));
  set_mie(get_mie() & ~(MIE_MEIE | MIE_MTIE | MIE_MSIE | xIE_SEIE | xIE_STIE | xIE_SSIE));
}

void interrupt(softvec_type_t type, unsigned int sp)
{
  intr(type, sp);
}