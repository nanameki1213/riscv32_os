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
  lw ra, 17*8(sp)
  ret

