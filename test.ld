OUTPUT_ARCH("riscv")
ENTRY(_start)

MEMORY
{
  ram(rxa)      : o = 0x80000000, l = 0x100000
  intrstack(rw) : o = 0x80100000, l = 0x050000
  bootstack(rw) : o = 0x8014ffff, l = 0x0
  memcho(rw)    : o = 0x80150000, l = 0x8000
  freearea(rxa) : o = 0x80158000, l = 0x0
}

SECTIONS
{
  .text : {
    text_start = . ;
    *(.text)
    etext = . ;
  } > ram
  
  .rodata : {
    rodata_start = . ;
    *(.strings)
    *(.rodata)
    *(.rodata.*)
    erodata = . ;
  } > ram

  .data : {
    *(.data)
  } > ram

  .bss : {
    *(.bss)
    *(COMMON)
  } > ram

  . = ALIGN(4);
  end = . ;

  .intrstack : {
    intrstack = . ;
  } > intrstack

  .bootstack : {
    bootstack = . ;
  } > bootstack

  .memcho : {
    memcho_start = . ;
    memcho.o(.data)
  } > memcho


  .freearea : {
    freearea = .;
  } > freearea

  .pte : {
    pte_addr = . ;
    pte.o(.data)
  } > ram
}