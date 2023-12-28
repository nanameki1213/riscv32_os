.text
.globl vectors
.type vectors, @function
.balign 256
vectors:
  j intr_serial
  .balign 4
  j intr_serial
  .balign 4
  j intr_serial
  .balign 4
  j intr_serial
  .balign 4
  j intr_serial
  .balign 4
  j intr_serial
  .balign 4
  j intr_serial
  .balign 4
  j intr_serial
  .balign 4
  j intr_serial
  .balign 4
  j intr_serial
  .balign 4
  j intr_serial

undefined_handler:
  mret
  