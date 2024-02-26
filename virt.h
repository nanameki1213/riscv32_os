#pragma once

#include "defines.h"
#include "memlayout.h"

#define SECTOR_SIZE 512

#define VIRT_BLK_REQ_MAX_NUM 32

#define VIRT_MMIO_NUM 8
#define VIRTQ_ENTRY_NUM 32
#define VIRT_BLK_DEVICEID 2

#define VIRT_MMIO_MAGIC 0x00
#define VIRT_MMIO_VERSION 0x04
#define VIRT_MMIO_DEVICEID 0x08
#define VIRT_MMIO_VENDERID 0x0c
#define VIRT_MMIO_HOST_FEATURES 0x10
#define VIRT_MMIO_HOST_FEATURES_SEL 0x14
#define VIRT_MMIO_GUEST_FEATURES 0x20
#define VIRT_MMIO_GUEST_FEATURES_SEL 0x24
#define VIRT_MMIO_GUEST_PAEG_SIZE 0x28
#define VIRT_MMIO_QUEUE_SEL 0x30
#define VIRT_MMIO_QUEUE_MAX 0x34
#define VIRT_MMIO_QUEUE_NUM 0x38
#define VIRT_MMIO_QUEUE_ALIGN 0x3c
#define VIRT_MMIO_QUEUE_PFN 0x40
#define VIRT_MMIO_QUEUE_NOTIFY 0x50
#define VIRT_MMIO_INTR_STATUS 0x60
#define VIRT_MMIO_INTR_ACK 0x64
#define VIRT_MMIO_STATUS 0x70
#define VIRT_MMIO_CONFIG 0x100

#define VIRT_MMIO_STAT_ACKNOWLEDGE (1<<0)
#define VIRT_MMIO_STAT_DRIVER (1<<1)
#define VIRT_MMIO_STAT_DRIVER_OK (1<<2)
#define VIRT_MMIO_STAT_FEATURES_OK (1<<3)
#define VIRT_MMIO_STAT_DEVICE_NEEDS_RESET (1<<4)
#define VIRT_MMIO_STAT_FAILED (1<<5)

struct VRingDesc {
  uint64 *addr;
  uint32 len;

#define VIRTQ_DESC_F_NEXT (1<<0)
#define VIRTQ_DESC_F_WRITE (1<<1)
#define VIRTQ_DESC_F_INDIRECT (1<<2)

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
  struct VRingDesc desc[VIRTQ_ENTRY_NUM];
  struct VRingAvail avail;
  struct VRingUsed used;
  uint32 num;
  uint32 num_default;
  uint32 align;
};

struct VirtQueue {
  struct VRing vring;
  int desc_idx;
  int top_desc_idx;
  unsigned last_used_idx;
};

#define VIRTIO_BLK_T_IN           0
#define VIRTIO_BLK_T_OUT          1
#define VIRTIO_BLK_T_FLUSH        4
#define VIRTIO_BLK_T_DISCARD      11
#define VIRTIO_BLK_T_WRITE_ZEROES 13

#define VIRTIO_BLK_S_OK     0
#define VIRTIO_BLK_S_IOERR  1
#define VIRTIO_BLK_S_UNSUPP 2

struct virtio_blk_req {
  uint32 type;
  uint32 reserved;
  uint64 sector;
  uint8 data[512];
  uint8 status;
};

struct virtio_blk_discard_write_zeroes {
  uint64 sector;
  uint32 num_sectors;
  uint32 flags;
};

struct virtio_blk_config {
  uint64 capacity;
  uint32 size_max;
  uint32 seg_max;
  struct virtio_blk_geometry {
    uint16 cylinders;
    uint8 heads;
    uint8 sectors;
  } geometry;
  uint32 blk_size;
  struct virtio_blk_topology {
    uint8 physical_block_exp;
    uint8 alignment_offset;
    uint16 min_io_size;
    uint32 opt_io_size;
  } topology;
  uint8 writeback;
  uint8 unused0[3];
  uint32 max_discard_sectors;
  uint32 max_discard_seg;
  uint32 discard_sector_alignment;
  uint32 max_write_zeroes_sectors;
  uint32 max_write_zeroes_seg;
  uint8 write_zeroes_may_unmap;
  uint8 unused1[3];
};

static inline uint32 get_virt_mmio(unsigned offset)
{
  return *(uint32*)(VIRT_DISC_MMIO + offset);
}

static inline void set_virt_mmio(unsigned offset, uint32 value)
{
  *(uint32*)(VIRT_DISC_MMIO + offset) = value;
}