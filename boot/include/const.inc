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

FATSz                   equ 9       ; FAT表所占扇区数，对FAT12来说是9

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