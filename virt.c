#include "virt.h"
#include "lib.h"
// ブロックデバイスのリクエスト構造体(数はVIRTQ_REQ_MAX_NUMで定義)
struct virtio_blk_req *blk_req;
int blk_req_idx = 0;
// 共通virtキュー(TODO: 複数のキューを扱えるようにする)
struct VirtQueue *common_virt_queue;

int init_virt_mmio()
{
  // 1. Select the queue writing its index to QueueSel.
  set_virt_mmio(VIRT_MMIO_QUEUE_SEL, 0);
  // 2. Check if the queue is not already in use
  if(get_virt_mmio(VIRT_MMIO_QUEUE_READY) != 0) {
    printf("キューが使用中です\n");
    return 1;
  }
  // 3. Read maxium queue size (number of elements) from QueueNumMax.
  int max_size = get_virt_mmio(VIRT_MMIO_QUEUE_MAX);
  if(max_size == 0) { // キューが無効
    return 1;
  }
  printf("キューの最大数: %d\n", max_size);
  // 4. Allocate and zero the queue memory
  struct VirtQueue *queue = (struct VirtQueue*)alloc_page();
  memset(queue, 0, sizeof(struct VirtQueue));
  // 5. Notify the device about the queue size by writing the size to QueueNum.
  set_virt_mmio(VIRT_MMIO_QUEUE_NUM, VIRTQ_ENTRY_NUM);
  // 6. Write physical addresses of the queue's Descriptor Area, DriverArea and Device Area
  set_virt_mmio(VIRT_MMIO_QUEUE_DESC_LOW, (intptr_t)&queue->vring.desc & 0xffff);
  set_virt_mmio(VIRT_MMIO_QUEUE_DESC_HIGH, (intptr_t)&queue->vring.desc >> 32);
  set_virt_mmio(VIRT_MMIO_QUEUE_DRIVER_LOW, (intptr_t)&queue->vring.avail & 0xffff);
  set_virt_mmio(VIRT_MMIO_QUEUE_DRIVER_HIGH, (intptr_t)&queue->vring.avail >> 32);
  set_virt_mmio(VIRT_MMIO_QUEUE_DEVICE_LOW, (intptr_t)&queue->vring.used & 0xffff);
  set_virt_mmio(VIRT_MMIO_QUEUE_DEVICE_HIGH, (intptr_t)&queue->vring.used >> 32);
  // 7. Write 0x1 to QueueReady
  set_virt_mmio(VIRT_MMIO_QUEUE_READY, 0x1);

  common_virt_queue = queue;
  // リクエスト用の構造体の領域を割り当て
  blk_req = (struct virtio_blk_req*)alloc_page();

  return 0;
}

void init_disk()
{
  init_virt_disk();
  init_virt_mmio();
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

void notify_to_device(struct VirtQueue *queue)
{
  // 使用可能リングを更新
  queue->vring.avail.ring[queue->vring.avail.idx++] = queue->top_desc_idx;
  // デバイスに通知
  set_virt_mmio(VIRT_MMIO_QUEUE_NOTIFY, queue->top_desc_idx);
  queue->last_used_idx++;
}