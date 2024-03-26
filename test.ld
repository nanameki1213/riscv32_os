OUTPUT_ARCH("riscv")
ENTRY(_start)

MEMORY
{
  ram(rxa)      : o = 0x80000000, l = 0x100000
  userstack(rw) : o = 0x80120000, l = 0x0
  intrstack(rw) : o = 0x80130000, l = 0x0
  bootstack(rw) : o = 0x8014ffff, l = 0x0
  memcho(rw)    : o = 0x80150000, l = 0x008000
  freearea(rxa) : o = 0x80158000, l = 0x0
}

SECTIONS
{
  . = 0x80000000;
  kernel_base = .;
  .text : {
    text_start = .;
    *(.text)
    etext = . ;
  } > ram
  
  .rodata : {
    rodata_start = .;
    *(.strings)
    *(.rodata)
    *(.rodata.*)
    erodata = .;
  } > ram

  .data : {
    *(.data)
  } > ram

  .bss : {
    *(.bss)
    *(COMMON)
  } > ram

  . = ALIGN(4);
  end = .;
  
  .userstack : {
    userstack = .;
  } > userstack

  .intrstack : {
    intrstack = .;
  } > intrstack

  .bootstack : {
    bootstack = .;
  } > bootstack

  .memcho : {
    memcho_start = .;
    memcho.o(.data)
  } > memcho

  kernel_end = .;

  .freearea : {
    freearea = .;
  } > freearea
}
