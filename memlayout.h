#ifndef MEM_LAYOUT_H_
#define MEM_LAYOUT_H_

#define PLIC_BASE 0x0c000000
#define PLIC_PRIORITY(source) (PLIC_BASE + (source)*0x4)
#define PLIC_PENDING (PLIC_BASE + 0x1000)
#define PLIC_ENABLE(hart) (PLIC_BASE + 0x2000 + (hart)*0x80)
#define PLIC_THRESHOLD(hart) (PLIC_BASE + 0x200000 + (hart)*0x1000)
#define PILC_CLAIM(hart) (PLIC_BASE + 0x200004 + (hart)*0x1000)

#define VIRT_DISC_MMIO 0x10001000

#endif