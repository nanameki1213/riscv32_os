#include "memlayout.h"
#include "defines.h"
#include "virt.h"
#include "lib.h"

extern struct virtio_blk_req *blk_req;
extern struct VirtQueue *common_virt_queue;

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
  desc.flags &= ~(VIRTQ_DESC_F_WRITE); // デバイスからはread-onlyにする
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
}