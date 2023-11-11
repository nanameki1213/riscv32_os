PREFIX = ~/riscv64_github
ARCH = riscv64-unknown-elf
BINDIR = $(PREFIX)/bin
ADDNAME = $(ARCH)-

AR			= $(BINDIR)/$(ADDNAME)ar
AS			= $(BINDIR)/$(ADDNAME)as
CC			= $(BINDIR)/$(ADDNAME)gcc
LD			= $(BINDIR)/$(ADDNAME)ld
OBJCOPY	= $(BINDIR)/$(ADDNAME)objcopy
OBJDUMP	= $(BINDIR)/$(ADDNAME)objdump
RANLIB 	= $(BINDIR)/$(ADDNAME)ranlib
STRIP  	= $(BINDIR)/$(ADDNAME)strip

OBJS = start.o main.o lib.o uart.o putchar.o

TARGET = testboot

CFLAGS = -march=rv32if -mabi=ilp32f
CFLAGS += -I.

LFLAGS = -m elf32lriscv -b elf32-littleriscv --no-relax -T

all : $(TARGET)

$(TARGET) : $(OBJS)
						$(LD) $(LFLAGS) test.ld $(OBJS) -o testboot.elf

.c.o	:	$<
				$(CC) -c $(CFLAGS) $<

.s.o	: $<
				$(CC) -c $(CFLAGS) $<

.S.o	:	$<
				$(CC) -c $(CFLAGS) $<

clean	:
				rm -f $(OBJS) $(TARGET) $(TARGET).elf