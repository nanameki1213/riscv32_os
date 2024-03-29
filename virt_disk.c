#include "memlayout.h"
#include "virt.h"
#include "disk.h"
#include "lib.h"
#include "defines.h"
#include <stddef.h>
#include <stdint.h>

extern struct virtio_blk_req *blk_req;
extern int blk_req_idx;
extern struct VirtQueue *common_virt_queue;

unsigned blk_capacity;
struct virtio_blk_config *blk_config;

int init_virt_disk()
{
  if(get_virt_mmio(VIRT_MMIO_MAGIC) != 0x74726976) {
    panic("MAGIC value is ilegal\n");
  }
  if(get_virt_mmio(VIRT_MMIO_DEVICEID) != VIRT_BLK_DEVICEID) {
    panic("Device ID is ilegal\n");
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
  // 7. Perform device-specific setup, including discovery of virtqueues for the device

  // MMIOのconfig領域を紐づけ
  blk_config = (struct virtio_blk_config*)((intptr_t)VIRT_DISK_MMIO + VIRT_MMIO_CONFIG);
  // ディスクの容量を取得
  blk_capacity = blk_config->capacity * SECTOR_SIZE;
  printf("ディスクの容量: %d bytes\n", blk_capacity);

  // 8. Set the DRIVER_OK status bit. At this point the device is "live".
  set_virt_mmio(VIRT_MMIO_STATUS, get_virt_mmio(VIRT_MMIO_STATUS) | VIRT_MMIO_STATUS_DRIVER_OK);

  return 0;
}

void init_disk()
{
  init_virt_disk();
  common_virt_queue = init_virt_mmio(VIRT_DISK_DEFAULT_QUEUE);
	if(common_virt_queue == NULL) {
		return;
	}
  printf("address of queue: 0x%x\n", common_virt_queue);
}

/// @brief ディスクの読み書きAPI
/// @param buf 読み込んだデータまたは書き込むデータ
/// @param sector 操作対象のセクタ番号
/// @param is_write 1: 書き込み，0: 読み込み
void read_write_disk(void *buf, unsigned sector, int is_write)
{
  // リクエストを構築
  struct virtio_blk_req *req = blk_req;
	req->sector = sector;
	req->type = is_write ? VIRTIO_BLK_T_IN : VIRTIO_BLK_T_OUT;
	if(is_write) {
		memcpy(req->data, buf, SECTOR_SIZE);
	}
	// ログを表示
	printf("sector: %d\n", req->sector);
	printf("type: %d\n", req->type);
  // ディスクリプタを構築
  // printf("request structure addr: 0x%x\n", (intptr_t)req);

  struct VRingDesc *desc = common_virt_queue->vring.desc;
  desc[0].addr = (intptr_t)req;
  desc[0].len = sizeof(uint32_t) * 2 + sizeof(uint64_t);
  desc[0].flags = VIRTQ_DESC_F_NEXT;
  desc[0].next = 1;
	
  desc[1].addr = (intptr_t)req + desc[0].len;
  desc[1].len = SECTOR_SIZE;
  desc[1].flags = VIRTQ_DESC_F_NEXT;
	if(!is_write) {
		desc[1].flags |= VIRTQ_DESC_F_WRITE;
	}
  desc[1].next = 2;
	
  desc[2].addr = (intptr_t)req + desc[0].len + desc[1].len;
  desc[2].len = sizeof(uint8_t);
  desc[2].flags = VIRTQ_DESC_F_WRITE;
  desc[2].next = 0;

  // ログを表示
  // printf("ディスクリプタテーブルの状態:\n");
  /* for(int i = 0; i < 3; i++) {
    printf("ring[%d]\n", i);
    printf("len: %d\n", common_virt_queue->vring.desc[i].len);
    printf("addr: 0x%x\n", (uint32_t)common_virt_queue->vring.desc[i].addr);
    printf("next: %d\n", common_virt_queue->vring.desc[i].next);
  }
	*/ 

  notify_to_device(common_virt_queue, common_virt_queue->queue_index);
	common_virt_queue->queue_index = 3;

  // ログを表示
  /* printf("使用可能リングの状態:\n");
  printf("avail idx: %d\n", common_virt_queue->vring.avail.idx);
  for(int i = 0; i < common_virt_queue->vring.avail.idx; i++) {
    printf("ring[%d]: %d\n", i, common_virt_queue->vring.avail.ring[i]);
  }
  printf("使用済みリングの状態:\n");
  printf("used idx: %d\n", common_virt_queue->vring.used.idx);
  // 割り込みステータスの状態
  printf("割り込みステータスの状態: 0x%x\n", get_virt_mmio(VIRT_MMIO_INTR_STATUS));
	*/ 
  // デバイスがリクエストを処理するまで待機
  while(common_virt_queue->last_used_index != common_virt_queue->vring.used.idx) {
		;
	}
  // printf("処理完了: %d\n", common_virt_queue->vring.used.idx);

  if(req->status == VIRTIO_BLK_S_IOERR ||
     req->status == VIRTIO_BLK_S_UNSUPP) {
		panic("ディスク: エラー発生\n");
  }

  if(is_write == 0) {
    memcpy(buf, req->data, SECTOR_SIZE);
  }

}
