#include "interrupt.h"
#include "riscv.h"
#include "memlayout.h"
#include "defines.h"

// S，Mモード両方の割込みを有効にする
void intr_enable()
{
  // ペンディングを表すレジスタをクリア
  // set_sip(get_sip() & ~(MIP_MEIP | MIP_MTIP | MIP_MSIP));
  set_mip(get_mip() & ~(MIP_MEIP | MIP_MTIP | MIP_MSIP | xIP_SEIP | xIP_STIP | xIP_SSIP));

  // set_sie(get_sie() | xIE_SEIE | xIE_STIE | xIE_SSIE);
  set_mie(get_mie() | MIE_MEIE | MIE_MTIE | MIE_MSIE | xIE_SEIE | xIE_STIE | xIE_SSIE);

  set_mstatus(get_mstatus() | MSTATUS_SIE | MSTATUS_MIE);
}

// S，Mモード両方の割込みを無効にする
void intr_disable()
{
  // set_sie(get_sie() & ~(xIE_SEIE | xIE_STIE | xIE_SSIE));
  set_mie(get_mie() & ~(MIE_MEIE | MIE_MTIE | MIE_MSIE | xIE_SEIE | xIE_STIE | xIE_SSIE));

  set_mstatus(get_mstatus() & ~(uint32)(MSTATUS_SIE | MSTATUS_MIE));
}

int is_intr_enable()
{
  int value = get_mstatus();
  if(!(value & MSTATUS_SIE))
    return 0;
  if(!(value & MSTATUS_MIE))
    return 0;

  return 1;
}

void interrupt(softvec_type_t type, unsigned int sp)
{
  // 多重割り込み禁止
  intr_disable();
  int id = get_mhartid();

  // PLICに割込みclaimを送信
  int irq = *(uint32*)PILC_CLAIM(id);

  if(irq == UART0_IRQ) {
    intr(type, sp);
  }

  // PLICにcomplete通知を行う
  *(uint32*)PILC_CLAIM(id) = irq;

  // int pc = get_mepc();
  // printf("ret pc = 0x%x\n", pc);

  intr_enable();
}