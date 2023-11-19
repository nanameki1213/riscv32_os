#include "intr.h"

.globl intr_serial
.type intr_serial,@function

intr_serial:
  sd s0, 0*4(sp)
  sd s1, 1*4(sp)
  sd s2, 2*4(sp)
  sd s3, 3*4(sp)
  sd s4, 4*4(sp)
  sd s5, 5*4(sp)
  sd s6, 6*4(sp)
  sd s7, 7*4(sp)
  sd s8, 8*4(sp)
  sd s9, 9*4(sp)
  sd s10, 10*4(sp)
  sd s11, 11*4(sp)
  sd ra, 12*8(sp)
  sd #_intrstack, sp
  sd sp, (a0)
  sd #SOFTVEC_TYPE_SERINTR, (a1)
  call interrupt
  ld sp, (a0)