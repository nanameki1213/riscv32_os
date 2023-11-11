.text
.globl _start
.type _start, @function

_start:
  li sp, 0x80ffffff
  call main
loop:
  j loop
