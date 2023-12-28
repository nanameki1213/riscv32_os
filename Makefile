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

OBJS = start.o main.o lib.o uart.o vector.o intr_test.o interrupt.o plic.o

TARGET = testboot

CFLAGS = -I. -march=rv32gc

LFLAGS = -m elf32lriscv -b elf32-littleriscv -nostdlib --no-relax

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
