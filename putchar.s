  .text
  .globl putchar
  .type putchar, @function
putchar:
  li t0, 0x10000000
  sb a0, 0(t0)
  ret
