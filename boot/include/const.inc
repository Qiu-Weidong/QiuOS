%ifndef CONST_INC_
%define CONST_INC_
; ================= 一些常量的定义 ===========================
BaseOfBoot              equ 0x0                     ; boot存放的基地址
OffsetOfBoot            equ 0x7c00                  ; boot的偏移地址

BaseOfFAT               equ 0x0                     ; fat表放在这里
OffsetOfFAT             equ 0x500                   ; fat表的偏移地址

BaseOfLoader            equ 0x0                     ; loader的段地址
OffsetOfLoader          equ 0x7e00                  ; loader的偏移地址

BaseOfLoaderPhyAddr     equ BaseOfLoader*10h        ; loader加载后的段物理地址

BaseOfKernel            equ 0x8000                  ; kernel加载到的段地址
OffsetOfKernel          equ 0x0                     ; kernel的偏移地址

RootDirSectors          equ 14                      ; 根目录所占扇区数，每个条目32字节，
                                                    ; 根目录所占字节数BPB_RootEntCnt*32
                                                    ; 每个扇区512字节，因此
                                                    ; RootDirSectors = BPB_RootEntCnt * 32 / 512 
                                                    ; 注意向上取整

SectorNumOfRootDirStart equ 19                      ; 根目录的起始扇区号 1 + 9 * 2 = 19 
                                                    ; 1是保留扇区大小，9是fat大小，2是fat个数

FATSz                   equ 9                       ; FAT表所占扇区数，对FAT12来说是9

BPB_BytesPerSec         equ  512                    ; 每扇区字节数
BPB_SecPerClus          equ  1                      ; 每簇扇区数
BPB_RsvdSecCnt          equ  1                      ; 保留扇区数，包含了引导扇区
BPB_NumFATs             equ  2                      ; FAT表的个数，建议为2
BPB_RootEntCnt          equ  224                    ; 根目录可以容纳的目录项数
BPB_TotSec16            equ  2880                   ; 总扇区数
BPB_MEdia               equ  0xf0                   ; 介质描述符
BPB_FATSz16             equ  9                      ; 每个FAT表所占扇区数
BPB_SecPerTrk           equ  18                     ; 每磁道扇区数
BPB_NumHeads            equ  2                      ; 磁头数
BPB_HiddSec             equ  0                      ; 隐藏扇区数
BPB_TotSec32            equ  0                      ; 如果BPB_TotSec16为0，则使用这个值
BS_DrvNum               equ  0                      ; int 0x13 的驱动器号
BS_Reservedl            equ  0                      ; 未使用
BS_BootSig              equ  0x29                   ; 扩展引导标记
BS_VolID                equ  0                      ; 卷序列号

; ============== 内存使用情况 ===============
;          ___________
; 0xFFFFF | ROM-BIOS  | 
; 0xF0000 |___________|
;         |           |
;         |           |
;         |           |
;         |___________|
; 0xB8FFF |    显存   |
; 0xB8000 |___________|
;         |           |
; 0xA0000 |___________|
;         |           | 
; 0x9FC00 |___________|  
;         |           |  \  
;         |           |   | 
;         |           |   |
;         |           |   | 可以使用的内存
;         |           |   |
;         |           |   |
;         |           |   |
;   0x500 |___________|  /
;         |中断向量等  |
; 0x00000 |___________|
; ============================================
%endif ; CONST_INC_
