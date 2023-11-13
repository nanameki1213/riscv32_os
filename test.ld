OUTPUT_ARCH("riscv")
ENTRY(_start)

MEMORY
{
  rom(rxa) : ORIGIN = 0x80000000, LENGTH = 512K
  ram(wxa) : ORIGIN = 0x80080000, LENGTH = 512K
}

SECTIONS
{
  . = 0x8000000;

  

  .text : {
    *(.text)
  }

  .rodata : {
    *(.strings)
    *(.rodata)
    *(.rodata.*)
  }

  .data : {
    *(.data)
  }

  .bss : {
    *(.bss)
    *(COMMON)
  }
}