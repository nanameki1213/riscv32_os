#include "interrupt.h"
#include "riscv.h"
#include "memlayout.h"
#include "defines.h"
#include "intr.h"

// 外部割込み有効化
void external_intr_enable()
{
  set_mie(get_medeleg() | MIE_MEIE | xIE_SEIE);

  set_mstatus(get_mstatus() | MSTATUS_MIE | MSTATUS_SIE);
}

// 外部割込み無効化
void external_intr_disable()
{
  set_mie(get_mie() & ~(uint32)(MIE_MEIE | xIE_SEIE));
}

// タイマ割込み有効化
void timer_intr_enable()
{
  set_mie(get_mie() | MIE_MTIE | xIE_STIE);

  set_mstatus(get_mstatus() | MSTATUS_MIE | MSTATUS_SIE);
}

// タイマ割込み無効化
void timer_intr_disable()
{
  set_mie(get_mie() & ~(uint32)(MIE_MTIE | xIE_STIE));
}

// ソフトウェア割込み有効化
void software_intr_enable()
{
  set_mie(get_mie() | MIE_MSIE | xIE_SSIE);

  set_mstatus(get_mstatus() | MSTATUS_MIE | MSTATUS_SIE);
}

// ソフトウェア割込み無効化
void software_intr_disable()
{
  set_mie(get_mie() & ~(uint32)(MIE_MSIE | xIE_SSIE));
}

// S，Mモード両方の割込みを有効にする
void intr_enable()
{
  // ペンディングを表すレジスタをクリア
  // set_sip(get_sip() & ~(MIP_MEIP | MIP_MTIP | MIP_MSIP));
  set_mip(get_mip() & ~(MIP_MEIP | MIP_MTIP | MIP_MSIP | xIP_SEIP | xIP_STIP | xIP_SSIP));

  // set_sie(get_sie() | xIE_SEIE | xIE_STIE | xIE_SSIE);
  // set_mie(get_mie() | MIE_MEIE | MIE_MTIE | MIE_MSIE | xIE_SEIE | xIE_STIE | xIE_SSIE);

  set_mstatus(get_mstatus() | MSTATUS_SIE | MSTATUS_MIE);
}

// S，Mモード両方の割込みを無効にする
void intr_disable()
{
  // set_sie(get_sie() & ~(xIE_SEIE | xIE_STIE | xIE_SSIE));
  // set_mie(get_mie() & ~(uint32)(MIE_MEIE | MIE_MTIE | MIE_MSIE | xIE_SEIE | xIE_STIE | xIE_SSIE));

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

void interrupt(intr_type_t type, unsigned int sp)
{
  // 多重割り込み禁止
  intr_disable();
  int id = get_mhartid();

  if(type == INTR_TYPE_EXLINTR) {
    // PLICに割込みclaimを送信
    int irq = *(uint32*)PILC_CLAIM(id);
    if(irq == UART0_IRQ) {
      uart(type, sp);
    } else if(irq == VIRTIO_IRQ) {
    }
    // PLICにcomplete通知を行う
    *(uint32*)PILC_CLAIM(id) = irq;
  } else if(type == INTR_TYPE_TIMINTR) {
    timer(type, sp);
  }

  // int pc = get_mepc();
  // printf("ret pc = 0x%x\n", pc);

  // 割込み有効化
  intr_enable();
}