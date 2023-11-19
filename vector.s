.text
.globl vectors
.type vectors, @function

vectors:
  j 0x0
  .balign 4
  j 0x0
  .balign 4
  j 0x0
  .balign 4
  j 0x0
  .balign 4
  j 0x0
  .balign 4
  j 0x0
  .balign 4
  j 0x0
  .balign 4
  j 0x0
  .balign 4
  j 0x0
  .balign 4
  j 0x0
  .balign 4
  j intr_serial
  .balign 4