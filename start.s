.text
.globl _start
.type _start, @function

_start:
  la sp, bootstack
  call main
loop:
  j loop
