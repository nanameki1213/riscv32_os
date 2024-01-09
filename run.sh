#!/bin/bash
set -xue

QEMU=qemu-system-riscv32

$QEMU -machine virt -nographic -serial mon:stdio -device loader,file=testboot.elf -bios none \
      -drive id=drive0,file=lorem.txt,format=raw \
      -device virtio-blk-device,drive=drive0,bus=virtio-mmio-bus.0 \
# -d in_asm
