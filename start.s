.text
.globl _start
.type _start, @function

_start:
  la sp, bootstack
  call main
loop:
  j loop

.text
.globl dispatch
dispatch:
  lw sp, 0(a0)
  lw a0, 0*4(sp)
  lw a1, 1*4(sp)
  lw a2, 2*4(sp)
  lw a3, 3*4(sp)
  lw a4, 4*4(sp)
  lw a5, 5*4(sp)
  lw a6, 6*4(sp)
  lw a7, 7*4(sp)
  lw t0, 8*4(sp)
  lw t1, 9*4(sp)
  lw t2, 10*4(sp)
  lw t3, 11*4(sp)
  lw t4, 12*4(sp)
  lw t5, 13*4(sp)
  lw t6, 14*4(sp)
  lw s0, 15*4(sp)
  lw ra, 16*4(sp) 
  addi sp, sp, 4*17
  ret

