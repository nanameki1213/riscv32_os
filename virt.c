#include "virt.h"
#include "lib.h"
// ブロックデバイスのリクエスト構造体(数はVIRTQ_REQ_MAX_NUMで定義)
struct virtio_blk_req *blk_req;
int blk_req_idx = 0;
// 共通virtキュー(TODO: 複数のキューを扱えるようにする)
struct VirtQueue *common_virt_queue;

int init_virt_mmio()
{
  // Select the queue writing its index to QueueSel.
  set_virt_mmio(VIRT_MMIO_QUEUE_SEL, 0);
  // Check if the queue is not already in use
  if(get_virt_mmio(VIRT_MMIO_QUEUE_PFN) != 0) {
    printf("このキューはすでに使われています\n");
    return 1;
  }
  // Read maxium queue size (number of elements) from QueueNumMax.
  int max_size = get_virt_mmio(VIRT_MMIO_QUEUE_MAX);
  if(max_size == 0) { // キューが無効
    return 1;
  }
  printf("キューの最大数: %d\n", max_size);
  // Allocate and zero the queue memory
  struct VirtQueue *queue = (struct VirtQueue*)alloc_page();
  memset(queue, 0, sizeof(struct VirtQueue));
  // Notify the device about the queue size by writing the size to QueueNum.
  set_virt_mmio(VIRT_MMIO_QUEUE_NUM, 0);
  // Notify the device about the used alignment by writing its value in bytes to QueueAlign.
  set_virt_mmio(VIRT_MMIO_QUEUE_ALIGN, 0);
  // Write the physical number of the first page of the queue to the QueuePFN register.
  set_virt_mmio(VIRT_MMIO_QUEUE_PFN, queue);

  common_virt_queue = queue;
  // printf("address of queue: 0x%x\n", *queue_addr);
  // printf("desc address: 0x%x\n", (uint32)&(queue->vring.desc));
  // printf("avail address: 0x%x\n", (uint32)&(queue->vring.avail));
  // printf("used address: 0x%x\n", (uint32)&(queue->vring.used));

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

void notify_to_device(uint32 *base, struct VirtQueue *queue)
{
  // 使用可能リングを更新
  queue->vring.avail.ring[queue->vring.avail.idx++] = queue->top_desc_idx;
  // デバイスに通知
  set_virt_mmio(VIRT_MMIO_QUEUE_NOTIFY, queue->top_desc_idx);
}