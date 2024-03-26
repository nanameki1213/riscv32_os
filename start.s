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
  sw a0, 0(sp)
  lw a0, 0*8(sp)
  lw a1, 1*8(sp)
  lw a2, 2*8(sp)
  lw a3, 3*8(sp)
  lw a4, 4*8(sp)
  lw a5, 5*8(sp)
  lw a6, 6*8(sp)
  lw a7, 7*8(sp)
  lw t0, 8*8(sp)
  lw t1, 9*8(sp)
  lw t2, 10*8(sp)
  lw t3, 11*8(sp)
  lw t4, 12*8(sp)
  lw t5, 13*8(sp)
  lw t6, 14*8(sp)
  lw s0, 15*8(sp)
  lw ra, 16*8(sp) 
  addi sp, sp, 8*17
  ret

