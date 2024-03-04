#include "virt.h"
#include "lib.h"
#include "stdbool.h"
// ブロックデバイスのリクエスト構造体(数はVIRTQ_REQ_MAX_NUMで定義)
struct virtio_blk_req *blk_req;
int blk_req_idx = 0;
// 共通virtキュー(TODO: 複数のキューを扱えるようにする)
struct VirtQueue *common_virt_queue;

struct VirtQueue *init_virt_mmio(int index)
{
  // 1. Select the queue writing its index to QueueSel.
  set_virt_mmio(VIRT_MMIO_QUEUE_SEL, index);
  // 2. Check if the queue is not already in use
  if(get_virt_mmio(VIRT_MMIO_QUEUE_READY) != 0) {
    printf("キューが使用中です\n");
    return NULL;
  }
  // 3. Read maxium queue size (number of elements) from QueueNumMax.
  int max_size = get_virt_mmio(VIRT_MMIO_QUEUE_MAX);
  if(max_size == 0) { // キューが無効
		printf("キューが無効です\n");
    return NULL;
  }
  printf("リクエストが処理できる最大のキュー数: %d\n", max_size);
  // 4. Allocate and zero the queue memory
  struct VirtQueue *vq = (struct VirtQueue*)alloc_page();
  memset(vq, 0, sizeof(struct VirtQueue));
  vq->vq_idx = index;
  // 5. Notify the device about the queue size by writing the size to QueueNum.
  set_virt_mmio(VIRT_MMIO_QUEUE_NUM, VIRTQ_ENTRY_NUM);
  // 6. Write physical addresses of the queue's Descriptor Area, DriverArea and Device Area
  set_virt_mmio(VIRT_MMIO_QUEUE_DESC_LOW, (uint64)vq->vring.desc & 0xffffffff);
  set_virt_mmio(VIRT_MMIO_QUEUE_DESC_HIGH, 0);
  set_virt_mmio(VIRT_MMIO_QUEUE_DRIVER_LOW, (uint64)&vq->vring.avail & 0xffffffff);
  set_virt_mmio(VIRT_MMIO_QUEUE_DRIVER_HIGH, 0);
  set_virt_mmio(VIRT_MMIO_QUEUE_DEVICE_LOW, (uint64)&vq->vring.used & 0xffffffff);
  set_virt_mmio(VIRT_MMIO_QUEUE_DEVICE_HIGH, 0);
  // 7. Write 0x1 to QueueReady
  set_virt_mmio(VIRT_MMIO_QUEUE_READY, 0x1);

  // リクエスト用の構造体の領域を割り当て
  blk_req = (struct virtio_blk_req*)alloc_page();

  return vq;
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
  printf("デバイスに通知します\n");
  set_virt_mmio(VIRT_MMIO_QUEUE_NOTIFY, VIRT_DISK_DEFAULT_QUEUE);
  queue->last_used_idx++;
}

bool is_queue_available(int index)
{
	set_virt_mmio(VIRT_MMIO_QUEUE_SEL, index);
	return get_virt_mmio(VIRT_MMIO_QUEUE_MAX) != 0;
}
