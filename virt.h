#pragma once

#include "stdint.h"
#include "memlayout.h"
#include "stdbool.h"

#define SECTOR_SIZE 512

#define VIRT_BLK_REQ_MAX_NUM 128

#define VIRT_DISK_DEFAULT_QUEUE 0

#define VIRT_MMIO_NUM 8
#define VIRTQ_ENTRY_NUM 32
#define VIRT_BLK_DEVICEID 2
#define VIRT_VERSION 0x1

#define VIRT_MMIO_MAGIC 0x00
#define VIRT_MMIO_VERSION 0x04
#define VIRT_MMIO_DEVICEID 0x08
#define VIRT_MMIO_VENDERID 0x0c
#define VIRT_MMIO_HOST_FEATURES 0x10
#define VIRT_MMIO_HOST_FEATURES_SEL 0x14
#define VIRT_MMIO_GUEST_FEATURES 0x20
#define VIRT_MMIO_GUEST_FEATURES_SEL 0x24
#define VIRT_MMIO_GUEST_PAGE_SIZE 0x28
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

#define VIRT_MMIO_STATUS_ACKNOWLEDGE (1<<0)
#define VIRT_MMIO_STATUS_DRIVER (1<<1)
#define VIRT_MMIO_STATUS_DRIVER_OK (1<<2)
#define VIRT_MMIO_STATUS_FEATURES_OK (1<<3)
#define VIRT_MMIO_STATUS_DEVICE_NEEDS_RESET (1<<6)
#define VIRT_MMIO_STATUS_FAILED (1<<7)

struct VRingDesc {
  uint64_t addr;
  uint32_t len;

#define VIRTQ_DESC_F_NEXT (1<<0)
#define VIRTQ_DESC_F_WRITE (1<<1)
#define VIRTQ_DESC_F_INDIRECT (1<<2)

  uint16_t flags;
  uint16_t next;
};

struct VRingAvail {
  uint16_t flags;
  uint16_t idx;
  uint16_t ring[VIRTQ_ENTRY_NUM];
};

struct VRingUsedElem {
  uint32_t id;
  uint32_t len;
};

struct VRingUsed {
  uint16_t flags;
  uint16_t idx;
  struct VRingUsedElem ring[VIRTQ_ENTRY_NUM];
};

struct VRing {
  struct VRingDesc desc[VIRTQ_ENTRY_NUM];
  struct VRingAvail avail;
  struct VRingUsed used;
  uint32_t num;
  uint32_t num_default;
  uint32_t align;
};

struct VirtQueue {
  struct VRing vring;
  int desc_idx;
  int top_desc_idx;
  unsigned last_used_idx;

  unsigned vq_idx;
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
  uint32_t type;
  uint32_t reserved;
  uint64_t sector;
  uint8_t data[512];
  uint8_t status;
};

struct virtio_blk_discard_write_zeroes {
  uint64_t sector;
  uint32_t num_sectors;
  uint32_t flags;
};

struct virtio_blk_config {
  uint64_t capacity;
  uint32_t size_max;
  uint32_t seg_max;
  struct virtio_blk_geometry {
    uint16_t cylinders;
    uint8_t heads;
    uint8_t sectors;
  } geometry;
  uint32_t blk_size;
  struct virtio_blk_topology {
    uint8_t physical_block_exp;
    uint8_t alignment_offset;
    uint16_t min_io_size;
    uint32_t opt_io_size;
  } topology;
  uint8_t writeback;
  uint8_t unused0[3];
  uint32_t max_discard_sectors;
  uint32_t max_discard_seg;
  uint32_t discard_sector_alignment;
  uint32_t max_write_zeroes_sectors;
  uint32_t max_write_zeroes_seg;
  uint8_t write_zeroes_may_unmap;
  uint8_t unused1[3];
};

static inline uint32_t get_virt_mmio(unsigned offset)
{
  return *(uint32_t*)(VIRT_DISK_MMIO + offset);
}

static inline void set_virt_mmio(unsigned offset, uint32_t value)
{
	*((volatile uint32_t*)(VIRT_DISK_MMIO + offset)) = value;
}

struct VirtQueue *init_virt_mmio(int index);
struct virtio_blk_req *new_blk_request(int sector, void *buf, int is_write);
void add_single_desc(struct VirtQueue *queue, struct VRingDesc desc);
void notify_to_device(struct VirtQueue *queue);
bool is_queue_available(int index);
