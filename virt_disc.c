#include "memlayout.h"
#include "defines.h"
#include "virt.h"
#include "lib.h"

extern struct virtio_blk_req *blk_req;
extern int blk_req_idx;
extern struct VirtQueue *common_virt_queue;

unsigned blk_capacity;
struct virtio_blk_config *blk_config;

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

  // MMIOのconfig領域を紐づけ
  blk_config = (struct virtio_blk_config*)((intptr_t)base + VIRT_MMIO_CONFIG);
  // ディスクの容量を取得
  blk_capacity = blk_config->capacity * SECTOR_SIZE;
  printf("ディスクの容量: %d bytes\n", blk_capacity);

  return 0;
}

/// @brief ディスクの読み書きAPI
/// @param buf 読み込んだデータまたは書き込むデータ
/// @param sector 操作対象のセクタ番号
/// @param is_write 1: 書き込み，0: 読み込み
void read_write_disc(void *buf, unsigned sector, int is_write)
{
  // リクエストを構築
  struct virtio_blk_req *req = new_blk_request(sector, buf, is_write);
  // ディスクリプタを構築
  struct VRingDesc desc;
  desc.addr = (uint64*)req;
  desc.len = sizeof(struct virtio_blk_req);
  desc.flags |= VIRTQ_DESC_F_WRITE; // デバイスからはread-onlyにする
  desc.next = 0;
  // ディスクリプタを登録
  add_single_desc(common_virt_queue, desc);
  // ログを表示
  printf("desc idx: %d\n", common_virt_queue->desc_idx);
  for(int i = 0; i < common_virt_queue->desc_idx; i++) {
    printf("len: %d\n", common_virt_queue->vring.desc[i].len);
  }
  // デバイスに通知
  notify_to_device(VIRT_DISC_MMIO, common_virt_queue);
  // ログを表示
  printf("idx: %d\n", common_virt_queue->vring.avail.idx);
  for(int i = 0; i < common_virt_queue->vring.avail.idx; i++) {
    printf("ring[%d]: %d\n", i, common_virt_queue->vring.avail.ring[i]);
  }
  printf("used idx: %d\n", common_virt_queue->vring.used.idx);
  // デバイスがリクエストを処理するまで待機
  while(common_virt_queue->vring.used.idx == 0) {
    ;
  }

  if(req->status == VIRTIO_BLK_S_IOERR ||
     req->status == VIRTIO_BLK_S_UNSUPP) {
    printf("エラー発生\n");
  }

  if(is_write == 0) {
    memcpy(buf, req->data, SECTOR_SIZE);
  }

  blk_req_idx--;
}