#include "virt.h"
#include "lib.h"
// ブロックデバイスのリクエスト構造体(数はVIRTQ_REQ_MAX_NUMで定義)
struct virtio_blk_req *blk_req;
int blk_req_idx = 0;
// 共通virtキュー(TODO: 複数のキューを扱えるようにする)
struct VirtQueue *common_virt_queue;

int init_virtqueue(uint32 *base, struct VirtQueue **queue_addr)
{
  // Select the queue writing its index to QueueSel.
  set_virt_mmio(base, VIRT_MMIO_QUEUE_SEL, 0);
  // Check if the queue is not already in use
  if(get_virt_mmio(base, VIRT_MMIO_QUEUE_READY) == 0x1) {
    return 1;
  }
  // Read maxium queue size (number of elements) from QueueNumMax.
  int max_size = get_virt_mmio(base, VIRT_MMIO_QUEUE_MAX);
  if(max_size == 0x0) {
    printf("キューの最大数が0です\n");
    return 1;
  }
  // Allocate and zero the queue memory, making sure the memory is physically contiguous.
  struct VirtQueue *queue = (struct VirtQueue*)alloc_page();
  memset(queue, 0, sizeof(struct VirtQueue));
  // Notify the device about the queue size by writing the size to QueueNum
  set_virt_mmio(base, VIRT_MMIO_QUEUE_NUM, VIRTQ_ENTRY_NUM);
  // Write physical addresses of the queue's Descriptor Area, DriverArea and Device Area
  set_virt_mmio(base, VIRT_MMIO_QUEUE_DESC_LOW, (uint32)&(queue->vring.desc));
  set_virt_mmio(base, VIRT_MMIO_QUEUE_DRIVER_LOW, (uint32)&(queue->vring.avail));
  set_virt_mmio(base, VIRT_MMIO_QUEUE_DEVICE_LOW, (uint32)&(queue->vring.used));
  // Write 0x1 to QueueReady
  set_virt_mmio(base, VIRT_MMIO_QUEUE_READY, 0x1);

  *queue_addr = queue;
  // printf("address of queue: 0x%x\n", *queue_addr);
  // printf("desc address: 0x%x\n", (uint32)&(queue->vring.desc));
  // printf("avail address: 0x%x\n", (uint32)&(queue->vring.avail));
  // printf("used address: 0x%x\n", (uint32)&(queue->vring.used));

  // リクエスト用の構造体の領域を割り当て
  blk_req = (struct virtio_blk_req*)alloc_page();

  return 0; 
}

void init_disk(uint32 *base)
{
  init_virt_disk(base);
  init_virtqueue(base, &common_virt_queue);
  printf("address of queue: 0x%x\n", common_virt_queue);
}

// 引数をもとに新たなリクエストを作成
struct virtio_blk_req *new_blk_request(int sector, void *buf, int is_write)
{
  blk_req[blk_req_idx].sector = sector;
  blk_req[blk_req_idx].type = (is_write == 1) ? VIRTIO_BLK_T_IN : VIRTIO_BLK_T_OUT;
  if(is_write) {
    memcpy(blk_req[blk_req_idx].data, buf, SECTOR_SIZE);
  }

  return &blk_req[blk_req_idx++];
}

void add_single_desc(struct VirtQueue *queue, struct VRingDesc desc)
{
  queue->top_desc_idx = queue->desc_idx;
  queue->vring.desc[queue->desc_idx++] = desc;
}

void notify_to_device(uint32 *base, struct VirtQueue *queue)
{
  // 使用可能リングを更新
  queue->vring.avail.ring[queue->vring.avail.idx++] = queue->top_desc_idx;
  // デバイスに通知
  set_virt_mmio(base, VIRT_MMIO_QUEUE_NOTIFY, queue->top_desc_idx);
}