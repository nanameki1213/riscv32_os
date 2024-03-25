#include "virt.h"
#include "lib.h"
#include "memory.h"
#include <stdbool.h>
#include <stddef.h>

// ブロックデバイスのリクエスト構造体(数はVIRTQ_REQ_MAX_NUMで定義)
struct virtio_blk_req *blk_req;
// 共通virtキュー(TODO: 複数のキューを扱えるようにする)
struct VirtQueue *common_virt_queue;

struct VirtQueue *init_virt_mmio(int index)
{
	// バージョンを確認
	int version = get_virt_mmio(VIRT_MMIO_VERSION);
	if(version != VIRT_VERSION) {
		printf("virtioのバージョンが対応していません: %d\n", version);
		return NULL;
	}
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
  vq->queue_index = index;
  // 5. Notify the device about the queue size by writing the size to QueueNum.
  set_virt_mmio(VIRT_MMIO_QUEUE_NUM, VIRTQ_ENTRY_NUM);
  // 6. Write physical addresses of the queue's Descriptor Area, Driver Area and Device Area
	set_virt_mmio(VIRT_MMIO_DESC_LOW, (intptr_t)vq->vring.desc & 0xffffffff);
	set_virt_mmio(VIRT_MMIO_DESC_HIGH, 0);
	set_virt_mmio(VIRT_MMIO_DRIVER_LOW, (intptr_t)&vq->vring.avail & 0xffffffff);
	set_virt_mmio(VIRT_MMIO_DRIVER_HIGH, 0);
	set_virt_mmio(VIRT_MMIO_DEVICE_LOW, (intptr_t)&vq->vring.used & 0xffffffff);
	set_virt_mmio(VIRT_MMIO_DEVICE_HIGH, 0);

	// 7. Write 0x1 to QueueReady
	set_virt_mmio(VIRT_MMIO_QUEUE_READY, 0x1);

  // リクエスト用の構造体の領域を割り当て
  blk_req = (struct virtio_blk_req*)alloc_page();

  return vq;
}

void notify_to_device(struct VirtQueue *queue, int desc_idx)
{
  // 使用可能リングを更新
  queue->vring.avail.ring[queue->vring.avail.idx++] = desc_idx;
  // デバイスに通知
	set_virt_mmio(VIRT_MMIO_QUEUE_NOTIFY, VIRT_DISK_DEFAULT_QUEUE);
	queue->last_used_index++;
}

bool is_queue_available(int index)
{
	set_virt_mmio(VIRT_MMIO_QUEUE_SEL, index);
	return get_virt_mmio(VIRT_MMIO_QUEUE_MAX) != 0;
}
