#!/bin/bash
set -xue

QEMU=/home/nanameki/qemu/build/qemu-system-riscv32

$QEMU -machine virt -nographic -serial mon:stdio -device loader,file=testboot.elf -bios none \
			-global virtio-mmio.force-legacy=false \
      -drive id=drive0,file=lorem.txt,if=none,format=raw \
      -device virtio-blk-device,drive=drive0,bus=virtio-mmio-bus.0 \
#--trace events=./trace-events,file=trace.log \
#-d in_asm
