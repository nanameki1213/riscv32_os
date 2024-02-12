#ifndef VIRT_H
#define VIRT_H

#define VIRT_MMIO_NUM 8
#define VIRTQ_ENTRY_NUM 128
#define VIRT_BLK_DEVICEID 2

#define VIRT_MMIO_MAGIC 0x00
#define VIRT_MMIO_VERSION 0x04
#define VIRT_MMIO_DEVICEID 0x08
#define VIRT_MMIO_VENDERID 0x0c
#define VIRT_MMIO_DEVICE_FEATURES 0x10
#define VIRT_MMIO_DEVICE_FEATURES_SEL 0x14
#define VIRT_MMIO_DRIVER_FEATURES 0x20
#define VIRT_MMIO_DRIVER_FEATURES_SEL 0x24
#define VIRT_MMIO_QUEUE_SEL 0x30
#define VIRT_MMIO_QUEUE_MAX 0x34
#define VIRT_MMIO_QUEUE_NUM 0x38
#define VIRT_MMIO_QUEUE_READY 0x44
#define VIRT_MMIO_QUEUE_NOTIFY 0x50
#define VIRT_MMIO_INTR_STAT 0x60
#define VIRT_MMIO_INTR_ACK 0x64
#define VIRT_MMIO_STAT 0x70
#define VIRT_MMIO_QUEUE_DESC_LOW 0x80
#define VIRT_MMIO_QUEUE_DESC_HIGH 0x84
#define VIRT_MMIO_QUEUE_DRIVER_LOW 0x90
#define VIRT_MMIO_QUEUE_DRIVER_HIGH 0x94
#define VIRT_MMIO_QUEUE_DEVICE_LOW 0xa0
#define VIRT_MMIO_QUEUE_DEVICE_HIGH 0xa4
#define VIRT_MMIO_CONFIG 0xfc

#define VIRT_MMIO_STAT_ACKNOWLEDGE (1<<0)
#define VIRT_MMIO_STAT_DRIVER (1<<1)
#define VIRT_MMIO_STAT_DRIVER_OK (1<<2)
#define VIRT_MMIO_STAT_FEATURES_OK (1<<3)
#define VIRT_MMIO_STAT_DEVICE_NEEDS_RESET (1<<4)
#define VIRT_MMIO_STAT_FAILED (1<<5)

struct VRingDesc {
  uint64 addr;
  uint32 len;
  uint16 flags;
  uint16 next;
};

struct VRingAvail {
  uint16 flags;
  uint16 idx;
  uint16 ring[VIRTQ_ENTRY_NUM];
};

struct VRingUsedElem {
  uint32 id;
  uint32 len;
};

struct VRingUsed {
  uint16 flags;
  uint16 idx;
  struct VRingUsedElem ring[VIRTQ_ENTRY_NUM];
};

struct VRing {
  uint32 num;
  uint32 num_default;
  uint32 align;
  struct VRingDesc desc;
  struct VRingAvail avail;
  struct VRingUsed used;
};

struct VirtQueue {
  struct VRing vring;
};

static inline uint32 get_virt_mmio(uint32 *base, int offset)
{
  return *(uint32*)((uint32)base + offset);
}

static inline void set_virt_mmio(uint32 *base, int offset, uint32 value)
{
  *(uint32*)((uint32)base + offset) = value;
}

#endif