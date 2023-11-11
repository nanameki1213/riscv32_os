OUTPUT_ARCH("riscv")
ENTRY(_start)

MEMORY
{
  ram(rwx) : ORIGIN = 0x80000000, LENGTH = 0x40000000
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