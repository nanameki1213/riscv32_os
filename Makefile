ARCH = riscv32-unknown-linux-gnu
ADDNAME = $(ARCH)-

AR			= $(ADDNAME)ar
AS			= $(ADDNAME)as
CC			= $(ADDNAME)gcc
LD			= $(ADDNAME)ld
OBJCOPY	= $(ADDNAME)objcopy
OBJDUMP	= $(ADDNAME)objdump
RANLIB 	= $(ADDNAME)ranlib
STRIP  	= $(ADDNAME)strip

OBJS = start.o intr.o intr_handler.o main.o lib.o uart.o vector.o interrupt.o plic.o timer.o
OBJS += page.o memcho.o memory.o virt.o virt_disk.o
OBJS += proc.o syscall.o

TARGET = testboot

CFLAGS = -O0 -march=rv32gc -Wno-builtin-declaration-mismatch

LFLAGS = -m elf32lriscv -b elf32-littleriscv --no-relax -nostdlib

all : $(TARGET)

$(TARGET) : $(OBJS)
						$(LD) $(LFLAGS) -T test.ld $(OBJS) -o testboot.elf

.c.o	:	$<
				$(CC) -c $(CFLAGS) $<

.s.o	: $<
				$(CC) -c $(CFLAGS) $<

.S.o	:	$<
				$(CC) -c $(CFLAGS) $<

clean	:
				rm -f $(OBJS) $(TARGET) $(TARGET).elf
