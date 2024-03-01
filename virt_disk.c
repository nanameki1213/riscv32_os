#include "memlayout.h"
#include "defines.h"
#include "virt.h"
#include "disk.h"
#include "lib.h"

extern struct virtio_blk_req *blk_req;
extern int blk_req_idx;
extern struct VirtQueue *common_virt_queue;

unsigned blk_capacity;
struct virtio_blk_config *blk_config;

void init_disk()
{
  init_virt_disk();
  init_virt_mmio();
  printf("address of queue: 0x%x\n", common_virt_queue);
}

int init_virt_disk()
{
  if(get_virt_mmio(VIRT_MMIO_MAGIC) != 0x74726976) {
    printf("MAGIC value is ilegal\n");
    return 1;
  }
  if(get_virt_mmio(VIRT_MMIO_DEVICEID) != VIRT_BLK_DEVICEID) {
    printf("Device ID is ilegal: %d\n", get_virt_mmio(VIRT_MMIO_DEVICEID));
    return 1;
  }
  printf("デバイスID: %d\n", get_virt_mmio(VIRT_MMIO_DEVICEID));
  // 以下はデバイスの初期化

  // 1. Reset the device.
  set_virt_mmio(VIRT_MMIO_STATUS, 0x00);
  // 2. Set the ACKNOWLEDGE status bit: the guest OS has noticed the device.
  set_virt_mmio(VIRT_MMIO_STATUS, get_virt_mmio(VIRT_MMIO_STATUS) | VIRT_MMIO_STATUS_ACKNOWLEDGE);
  // 3. Set the DRIVER status bit: the guest OS knows how to drive the device.
  set_virt_mmio(VIRT_MMIO_STATUS, get_virt_mmio(VIRT_MMIO_STATUS) | VIRT_MMIO_STATUS_DRIVER);
  // 4. Read device feature bits,
  // and write the subset of feature bits understood by the OS and driver to the device.

  // 5. Set the FEATURES_OK status bit. The driver MUST NOT accept new feature bits after this step.
  set_virt_mmio(VIRT_MMIO_STATUS, get_virt_mmio(VIRT_MMIO_STATUS) | VIRT_MMIO_STATUS_FEATURES_OK);
  // 6. Re-read device status to ensure the FEATURES_OK bit is still set
  if(!(get_virt_mmio(VIRT_MMIO_STATUS) | VIRT_MMIO_STATUS_FEATURES_OK)) {
    printf("デバイスが使用できません\n");
    return 1;
  }
  // 7. Perform device-specific setup, including discovery of virtqueues for the device,
  // optional per-bus setup, reading and possibly writing the device's virtio configuration space, and population of virtqueues.

  // MMIOのconfig領域を紐づけ
  blk_config = (struct virtio_blk_config*)((intptr_t)VIRT_DISC_MMIO + VIRT_MMIO_CONFIG);
  // ディスクの容量を取得
  blk_capacity = blk_config->capacity * SECTOR_SIZE;
  printf("ディスクの容量: %d bytes\n", blk_capacity);

  // 8. Set the DRIVER_OK status bit. At this point the device is "live".
  set_virt_mmio(VIRT_MMIO_STATUS, get_virt_mmio(VIRT_MMIO_STATUS) | VIRT_MMIO_STATUS_DRIVER_OK);

  return 0;
}

/// @brief ディスクの読み書きAPI
/// @param buf 読み込んだデータまたは書き込むデータ
/// @param sector 操作対象のセクタ番号
/// @param is_write 1: 書き込み，0: 読み込み
void read_write_disk(void *buf, unsigned sector, int is_write)
{
  // リクエストを構築
  struct virtio_blk_req *req = new_blk_request(sector, buf, is_write);
  // ディスクリプタを構築
  printf("request structure addr: 0x%x\n", (intptr_t)req);
  struct VRingDesc desc[3] = {0};
  desc[0].addr = (uint64)req;
  desc[0].len = sizeof(uint32) * 2 + sizeof(uint64);
  desc[0].flags = VIRTQ_DESC_F_NEXT;
  desc[0].next = 1;
  desc[1].addr = (uint64)req + desc[0].len;
  desc[1].len = SECTOR_SIZE;
  desc[1].flags = VIRTQ_DESC_F_NEXT;
  desc[1].next = 2;
  desc[2].addr = (uint64)req + desc[0].len + desc[1].len;
  desc[2].len = sizeof(uint8);
  desc[2].flags = VIRTQ_DESC_F_WRITE;
  desc[2].next = 0;
  // ディスクリプタを登録
  memcpy(common_virt_queue->vring.desc, desc, sizeof(struct VRingDesc) * 3);
  common_virt_queue->top_desc_idx = 0;
  common_virt_queue->desc_idx = 3;
  // ログを表示
  printf("ディスクリプタテーブルの状態:\n");
  for(int i = 0; i < common_virt_queue->desc_idx; i++) {
    printf("ring[%d]\n", i);
    printf("len: %d\n", common_virt_queue->vring.desc[i].len);
    printf("addr: 0x%x\n", (uint32)common_virt_queue->vring.desc[i].addr);
    printf("next: %d\n", common_virt_queue->vring.desc[i].next);
  }
  printf("desc idx: %d\n", common_virt_queue->desc_idx);
  notify_to_device(common_virt_queue);
  // ログを表示
  printf("使用可能リングの状態:\n");
  printf("avail idx: %d\n", common_virt_queue->vring.avail.idx);
  for(int i = 0; i < common_virt_queue->vring.avail.idx; i++) {
    printf("ring[%d]: %d\n", i, common_virt_queue->vring.avail.ring[i]);
  }
  printf("使用済みリングの状態:\n");
  printf("used idx: %d\n", common_virt_queue->vring.used.idx);
  // 割り込みステータスの状態
  printf("割り込みステータスの状態: 0x%x\n", get_virt_mmio(VIRT_MMIO_INTR_STATUS));
  // デバイスがリクエストを処理するまで待機
  while(common_virt_queue->last_used_idx != common_virt_queue->vring.used.idx) {
    ;
  }
  // while(get_virt_mmio(VIRT_MMIO_INTR_STATUS) != 0x1) {
  //   ;
  // }
  printf("処理完了: %d\n", common_virt_queue->vring.used.idx);

  if(req->status == VIRTIO_BLK_S_IOERR ||
     req->status == VIRTIO_BLK_S_UNSUPP) {
    printf("エラー発生\n");
  }

  if(is_write == 0) {
    memcpy(buf, req->data, SECTOR_SIZE);
  }

}