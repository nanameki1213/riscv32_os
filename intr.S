#include "intr.h"

.text
.globl intr_serial
.type intr_serial, @function

intr_serial:
  sw ra, 0*8(sp)
  sw a0, 1*8(sp)
  sw a1, 2*8(sp)
  sw a2, 3*8(sp)
  sw a3, 4*8(sp)
  sw a4, 5*8(sp)
  sw a5, 6*8(sp)
  sw a6, 7*8(sp)
  sw a7, 8*8(sp)
  sw t0, 9*8(sp)
  sw t1, 10*8(sp)
  sw t2, 11*8(sp)
  sw t3, 12*8(sp)
  sw t4, 13*8(sp)
  sw t5, 14*8(sp)
  sw t6, 15*8(sp)
  sw s0, 16*8(sp)
  sw sp, (a1) # スタックポインタを第二引数として渡す
  la sp, intrstack # 割り込みスタックを利用する
  sw a0, 0*8(sp) # 旧スタックポインタの値を割り込みスタックに保存する
  li a0, SOFTVEC_TYPE_SERINTR
  addi sp, sp, 8
  call interrupt
  addi sp, sp, -8
  lw t0, (sp)
  mv sp, t0 # 旧スタックポインタの値を割り込みスタックから復旧する
  lw ra, 0*8(sp)
  lw a0, 1*8(sp)
  lw a1, 2*8(sp)
  lw a2, 3*8(sp)
  lw a3, 4*8(sp)
  lw a4, 5*8(sp)
  lw a5, 6*8(sp)
  lw a6, 7*8(sp)
  lw a7, 8*8(sp)
  lw t0, 9*8(sp)
  lw t1, 10*8(sp)
  lw t2, 11*8(sp)
  lw t3, 12*8(sp)
  lw t4, 13*8(sp)
  lw t5, 14*8(sp)
  lw t6, 15*8(sp)
  lw s0, 16*8(sp)
  mret