#!/bin/bash
set -xue

QEMU=qemu-system-riscv32

$QEMU -machine virt -nographic -serial mon:stdio -device loader,file=testboot.elf -bios none
