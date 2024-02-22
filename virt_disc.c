#include "memlayout.h"
#include "defines.h"
#include "virt.h"
#include "lib.h"

int init_virt_disk(uint32 *base)
{
  printf("virt mmio base addr: 0x%x\n", base);
  if(get_virt_mmio(base, VIRT_MMIO_MAGIC) != 0x74726976) {
    printf("MAGIC value is ilegal\n");
    return 1;
  }
  if(get_virt_mmio(base, VIRT_MMIO_DEVICEID) != VIRT_BLK_DEVICEID) {
    printf("Device ID is ilegal: %d\n", get_virt_mmio(base, VIRT_MMIO_DEVICEID));
    return 1;
  }
  // Reset the device.
  set_virt_mmio(base, VIRT_MMIO_STAT, 0x00);
  // Set the ACKNOWLEDGE status bit: the guest OS has noticed the device.
  set_virt_mmio(base, VIRT_MMIO_STAT, get_virt_mmio(base, VIRT_MMIO_STAT) | VIRT_MMIO_STAT_ACKNOWLEDGE);
  // Set the DRIVER status bit: the guest OS knows how to drive the device.
  set_virt_mmio(base, VIRT_MMIO_STAT, get_virt_mmio(base, VIRT_MMIO_STAT) | VIRT_MMIO_STAT_DRIVER);
  if(!(get_virt_mmio(base, VIRT_MMIO_STAT) | VIRT_MMIO_STAT_DRIVER_OK)) {
    return 1;
  }
  // Set the FEATURES_OK status bit. The driver MUST NOT accept new feature bits after this step.
  set_virt_mmio(base, VIRT_MMIO_STAT, VIRT_MMIO_STAT_FEATURES_OK);
  // Set the DRIVER_OK status bit. At this point the device is "live".
  set_virt_mmio(base, VIRT_MMIO_STAT, VIRT_MMIO_STAT_DRIVER_OK);

  return 0;
}

int init_virtqueue(uint32 *base)
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
  queue = (struct VirtQueue*)alloc_page();
  memset(queue, 0, sizeof(struct VirtQueue));
  // Notify the device about the queue size by writing the size to QueueNum
  set_virt_mmio(base, VIRT_MMIO_QUEUE_NUM, VIRTQ_ENTRY_NUM);
  // Write physical addresses of the queue's Descriptor Area, DriverArea and Device Area
  set_virt_mmio(base, VIRT_MMIO_QUEUE_DESC_LOW, (uint32)&(queue->vring.desc));
  set_virt_mmio(base, VIRT_MMIO_QUEUE_DRIVER_LOW, (uint32)&(queue->vring.avail));
  set_virt_mmio(base, VIRT_MMIO_QUEUE_DEVICE_LOW, (uint32)&(queue->vring.used));
  // Write 0x1 to QueueReady
  set_virt_mmio(base, VIRT_MMIO_QUEUE_READY, 0x1);

  // リクエスト用の構造体の領域を割り当て
  virt_req = (struct virtio_blk_req*)alloc_page();

  return 0; 
}

void init_disk(uint32 *base)
{
  init_virt_disk(base);
  init_virtqueue(base);
}

void add_desc(struct VirtQueue *queue, struct VRingDesc desc)
{
  queue->vring.desc[queue->desc_idx++] = desc;
}

void notify_to_device(uint32 *base, struct VirtQueue *queue)
{
  set_virt_mmio(base, VIRT_MMIO_QUEUE_NOTIFY, queue->desc_idx);
}

/// @brief ディスクの読み書きAPI
/// @param buf 読み込んだデータまたは書き込むデータ
/// @param sector 操作対象のセクタ番号
/// @param is_write 1: 書き込み，0: 読み込み
void read_write_disc(void *buf, unsigned sector, int is_write)
{
  // リクエストを構築
  virt_req->type = (is_write == 1) ? VIRTIO_BLK_T_IN : VIRTIO_BLK_T_OUT;
  virt_req->sector = sector;
  if(is_write) {
    memcpy(virt_req->data, buf, SECTOR_SIZE);
  }

  // ディスクリプタを構築
  struct VRingDesc desc;
  desc.addr = (uint64*)virt_req;
  desc.len = sizeof(struct virtio_blk_req);
  desc.flags &= ~(VIRTQ_DESC_F_WRITE); // デバイスからはread-onlyにする
  desc.next = 0;
  // ディスクリプタを登録
  add_desc(queue, desc);
  // 使用可能リングの構築
  struct VRingAvail avail;
  avail.ring[0] = 0;
  avail.idx = 1;
  // 使用可能リングに新たなエントリを登録
  queue->vring.avail = avail;
  // デバイスに通知
  notify_to_device(VIRT_MMIO, queue);
  // デバイスがリクエストを処理するまで待機
  while(queue->vring.used.idx == 0) {
    ;
  }

  if(virt_req->status == VIRTIO_BLK_S_IOERR ||
     virt_req->status == VIRTIO_BLK_S_UNSUPP) {
    printf("エラー発生\n");
  }

  if(is_write == 0) {
    memcpy(buf, virt_req->data, SECTOR_SIZE);
  }
}