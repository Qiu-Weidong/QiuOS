; ================= FAT12 磁盘头的一些常量 =======================
BS_OEMName       db 'Qiuboot',0         ; 生产厂商名，长度8
BPB_BytesPerSec  dw  512                ; 每扇区字节数
BPB_SecPerClus   db  1                  ; 每簇扇区数
BPB_RsvdSecCnt   dw  1                  ; 保留扇区数，包含了引导扇区
BPB_NumFATs      db  2                  ; FAT表的个数，建议为2
BPB_RootEntCnt   dw  224                ; 根目录可以容纳的目录项数
BPB_TotSec16     dw  2880               ; 总扇区数
BPB_MEdia        db  0xf0               ; 介质描述符
BPB_FATSz16      dw  9                  ; 每个FAT表所占扇区数
BPB_SecPerTrk    dw  18                 ; 每磁道扇区数
BPB_NumHeads     dw  2                  ; 磁头数
BPB_HiddSec      dd  0                  ; 隐藏扇区数
BPB_TotSec32     dd  0                  ; 如果BPB_TotSec16为0，则使用这个值
BS_DrvNum        db  0                  ; int 0x13 的驱动器号
BS_Reservedl     db  0                  ; 未使用
BS_BootSig       db  0x29               ; 扩展引导标记
BS_VolID         dd  0                  ; 卷序列号
BS_VolLab        db  'boot loader'      ; 卷标，系统显示的磁盘名称
BS_FileSysType   db  'FAT12   '         ; 文件系统类型
; ================= FAT12 磁盘头常量到此结束 =====================