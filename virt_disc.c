#include "memlayout.h"
#include "defines.h"
#include "virt.h"
#include "lib.h"

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