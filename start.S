.text
.globl _start
.type _start, @function

_start:
  li sp, 0x800fffff
  call main
loop:
  j loop
