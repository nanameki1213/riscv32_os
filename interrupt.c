#include "interrupt.h"
#include "riscv.h"
#include "memlayout.h"
#include "intr.h"
#include <stdint.h>
#include <stddef.h>
#include "intr_handler.h"
#include "uart.h"
#include "defines.h"

// 外部割込み有効化
// PLICに登録されているデバイスはそれぞれ割り込み設定が必要
void external_intr_enable()
{
  set_mie(get_medeleg() | MIE_MEIE | xIE_SEIE);

  set_mstatus(get_mstatus() | MSTATUS_MIE | MSTATUS_SIE);
}

// 外部割込み無効化
// PLICに登録されているデバイスはそれぞれ割り込み設定が必要
void external_intr_disable()
{
  set_mie(get_mie() & ~(uint32_t)(MIE_MEIE | xIE_SEIE));
}

// タイマ割込み有効化
void timer_intr_enable()
{
  set_mie(get_mie() | MIE_MTIE | xIE_STIE);
}

// タイマ割込み無効化
void timer_intr_disable()
{
  set_mie(get_mie() & ~(uint32_t)(MIE_MTIE | xIE_STIE));
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
  set_mie(get_mie() & ~(uint32_t)(MIE_MSIE | xIE_SSIE));
}

// S，Mモード両方の割込みを有効にする
void intr_enable()
{
  // ペンディングを表すレジスタをクリア
  // set_sip(get_sip() & ~(MIP_MEIP | MIP_MTIP | MIP_MSIP));
  set_mip(get_mip() & ~(MIP_MEIP | MIP_MTIP | MIP_MSIP | xIP_SEIP | xIP_STIP | xIP_SSIP));

  // タイマ割込みは現在有効にすると動作開始してしまうためつかうときだけ有効にする
  // set_sie(get_sie() | xIE_SEIE | xIE_STIE | xIE_SSIE);
  // set_mie(get_mie() | MIE_MEIE | MIE_MTIE | MIE_MSIE | xIE_SEIE | xIE_STIE | xIE_SSIE);

  external_intr_enable();
}

// S，Mモード両方の割込みを無効にする
void intr_disable()
{
  // set_sie(get_sie() & ~(xIE_SEIE | xIE_STIE | xIE_SSIE));
  // set_mie(get_mie() & ~(uint32_t)(MIE_MEIE | MIE_MTIE | MIE_MSIE | xIE_SEIE | xIE_STIE | xIE_SSIE));
  external_intr_disable();
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

void init_softvec()
{
  for (int i = 0; i < SOFTVEC_TYPE_NUM; i++) {
    softvec[i] = NULL;
  }
}

void softvec_setintr(softvec_type_t type, softvec_handler_t handler)
{
  softvec[type] = handler;  
}

void interrupt(softvec_type_t type, unsigned int sp)
{
  printf("共通割り込みハンドラ interrupt 突入\n");
  // 多重割り込み禁止
  intr_disable();
  int id = get_mhartid();  

  if(type == SOFTVEC_TYPE_EXLINTR) {
    // PLICに割込みclaimを送信
    int irq = *(uint32_t*)PILC_CLAIM(id);
    if(irq == UART0_IRQ) {
      uart(type, sp);
    } else if(irq == VIRTIO_IRQ) {
    }
    // PLICにcomplete通知を行う
    *(uint32_t*)PILC_CLAIM(id) = irq;
  } else {
    softvec_handler_t handler = softvec[type];
    if(handler) {
      handler(type, sp);
    } else {
      printf("ハンドラが設定されていません\n割込み種別番号: %d\n", type);
    }
  }

  // int pc = get_mepc();
  // printf("ret pc = 0x%x\n", pc);

  // 割込み有効化
  intr_enable();
}
