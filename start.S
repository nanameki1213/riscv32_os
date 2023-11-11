.text
.globl _start
.type _start, @function

_start:
  li sp, 0x80ffffff
  call main
loop:
  j loop

#define RHR 0                 // receive holding register (for input bytes)
#define THR 0                 // transmit holding register (for output bytes)
#define LSR 5                 // line status register
#define LSR_RX_READY (1<<0)   // input is waiting to be read from RHR
#define LSR_TX_IDLE (1<<5)    // THR can accept another character to send

.globl getchar
.type getchar, @function

getchar:
  li a5, 0x10000000
  // ラインステータスレジスタ（LSR）にLSR_RX_READYフラグが立っていればデータ受信済み
  lb a4, LSR(a5)
  andi a4, a4, LSR_RX_READY
  beqz a4, getc_empty
  // データ受信済みの場合
  // データ受信レジスタ(RHR)からシリアルへ入力された値を読み込んで返す
  lb a0, RHR(a5)
  ret
getc_empty:
  // データ未受信の場合
  // -1を返す
  li a0, -1
  ret
