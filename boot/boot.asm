%include "const.inc" 

    org OffsetOfBoot
TopOfStack             equ OffsetOfBoot             ; 栈基址

BS_jmpBoot:         jmp short _start                ; 跳转指令，长度3，jmp short只有两个字节，nop占位
                    nop                             ; nop占位
_BS_OEMName:        db 'Qiuboot',0                  ; 生产厂商名，长度8
_BPB_BytesPerSec:   dw  BPB_BytesPerSec             ; 每扇区字节数
_BPB_SecPerClus:    db  BPB_SecPerClus              ; 每簇扇区数
_BPB_RsvdSecCnt:    dw  BPB_RsvdSecCnt              ; 保留扇区数，包含了引导扇区
_BPB_NumFATs:       db  BPB_NumFATs                 ; FAT表的个数，建议为2
_BPB_RootEntCnt:    dw  BPB_RootEntCnt              ; 根目录可以容纳的目录项数
_BPB_TotSec16:      dw  BPB_TotSec16                ; 总扇区数
_BPB_MEdia:         db  BPB_MEdia                   ; 介质描述符
_BPB_FATSz16:       dw  BPB_FATSz16                 ; 每个FAT表所占扇区数
_BPB_SecPerTrk:     dw  BPB_SecPerTrk               ; 每磁道扇区数
_BPB_NumHeads:      dw  BPB_NumHeads                ; 磁头数
_BPB_HiddSec:       dd  BPB_HiddSec                 ; 隐藏扇区数
_BPB_TotSec32:      dd  BPB_TotSec32                ; 如果BPB_TotSec16为0，则使用这个值
_BS_DrvNum:         db  BS_DrvNum                   ; int 0x13 的驱动器号
_BS_Reservedl:      db  BS_Reservedl                ; 未使用
_BS_BootSig:        db  BS_BootSig                  ; 扩展引导标记
_BS_VolID:          dd  BS_VolID                    ; 卷序列号
_BS_VolLab:         db  'boot loader'               ; 卷标，系统显示的磁盘名称
_BS_FileSysType:    db  'FAT12   '                  ; 文件系统类型

; 引导代码
_start: 
    mov ax,cs
    mov ds,ax
    mov es,ax
    mov ss,ax
    mov sp,TopOfStack

    ; 清屏
    mov ax, 0x0600                  ; ah=6, al=0, 0表示清屏
    mov bx, 0x0700                  ; bl=0x7,黑底白字
    mov cx, 0x0000                  ; (ch,cl) = (0,0) 左上角
    mov dx, 0x184f                  ; 右下角:(80,50)
    int 0x10                        ; 0x10号中断

    ; 显示字符串,地址在es:bp中
    mov ax, 0x1301                  ; ah=0x13表示输出字符串，al=0x01表示输出后光标位于字符串后面
    mov bx, 0x000d                  ; bh=0x0表示显示第0页，bl=0x04表示红色
    mov dx, 0x0000                  ; dh=0x11表示显示在第17行，dl=0x0表示显示在第0列
    mov bp, BootMessage             ; es:bp指向带显示字符串地址
    mov cx, strLen                  ; cx表示字符串长度
    int 0x10

    ; 重置软盘
    xor ah, ah
    xor dl, dl
    int 0x13
    
    ; 从这里开始查找loader.bin文件
    mov word [SectorCurrent], SectorNumOfRootDirStart ; 初始化SectorCurrent
    mov ax, BaseOfLoader            ; '.
    mov es, ax                      ;  | 设置es:bx指向BaseOfLoader:TopOfStack
    mov bx, OffsetOfLoader          ;  /
Search_for_loader:

    mov ax, word [SectorCurrent]    ; '.
    mov cl, 1                       ;  | 读取SectorCurrent所在的扇区
    call ReadSector                 ;  /

    mov si, LoaderFileName
    mov di, OffsetOfLoader     
    xor dx, dx                      ; 每个扇区16个条目，此处用dx来循环
Search_in_one_Sector:
    mov cx, LoaderFileNameLen       ; 设置字符串长度11
    call strcmp
    cmp al, 0                       ; al=0表示两个字符串相同
    je  LOADER_FOUND

    and di, 0xffe0
    add di, 0x0020
    mov si, LoaderFileName
    inc dx
    cmp dx, 0x10
    jl  Search_in_one_Sector  

    inc word [SectorCurrent]
    cmp word [SectorCurrent], RootDirSectors+SectorNumOfRootDirStart
    jl  Search_for_loader
    
    ; 这里表示没有找到loader.bin
LOADER_NOT_FOUND:
    mov ax, cs
    mov es, ax
    mov ax, 0x1301                  ; ah=0x13表示输出字符串，al=0x01表示输出后光标位于字符串后面
    mov bx, 0x0004                  ; bh=0x0表示显示第0页，bl=0x04表示红色
    mov dx, 0x0100                  ; dh=0x11表示显示在第17行，dl=0x0表示显示在第0列
    mov bp, LoaderNotFoundMessage   ; es:bp指向带显示字符串地址
    mov cx, LoaderNotFoundMessageLen    ; cx表示字符串长度
    int 0x10

    jmp $

; loader.bin文件找到了
LOADER_FOUND:
    ; 显示字符串,地址在es:bp中
    mov ax, cs
    mov es, ax
    mov ax, 0x1301                  ; ah=0x13表示输出字符串，al=0x01表示输出后光标位于字符串后面
    mov bx, 0x0002                  ; bh=0x0表示显示第0页，bl=0x04表示红色
    mov dx, 0x0100                  ; dh=0x11表示显示在第1行，dl=0x0表示显示在第0列
    mov bp, LoaderFoundMessage      ; es:bp指向带显示字符串地址
    mov cx, LoaderFoundMessageLen   ; cx表示字符串长度
    int 0x10

    ; 从这里开始加载loader.bin
    mov ax, BaseOfLoader            ; '.
    mov es, ax                      ;  | 初始化es:bx
    mov bx, OffsetOfLoader          ;  /
    and di, 0xffe0                  ; '.
    add di, 0x001a                  ;   此时di指向起始簇号
    mov ax, word [es:di]            ; 将ClusNo的值放入ax中
    mov word [ClusNo], ax           ; 将起始簇号保存到ClusNo中

LOAD_START:
    mov ax, word [ClusNo]
    ; 簇号+31得到扇区号
    add ax, RootDirSectors+SectorNumOfRootDirStart-2  
    mov cl, 1
    call ReadSector
    add bx, BPB_BytesPerSec

    push bx

    mov dx, word [ClusNo]
    mov ax, dx                      ; ax = dx = CLusNo
    shl ax, 1                       ; ax = 2*ClusNo, dx = ClusNo
    add ax, dx                      ; ax = 3*ClusNo
    shr ax, 1                       ; ax = 3*ClusNo / 2 = bios
    xor dx, dx                      ; dx:ax = bios     

    mov bx, BPB_BytesPerSec 
    div bx                          ; ax = bios / 512 , dx = bios % 512
    add ax, 1                       ; ax = bios / 512 + 1
    
    ; 设置es:bx->fat表缓冲区
    mov bx, ax
    mov ax, BaseOfFAT
    mov es, ax
    mov ax, bx
    mov bx, OffsetOfFAT
    
    cmp ax, word [curFatSector]
    jz CONTINUE
    push dx
    mov word [curFatSector], ax
    mov cl, 2
    call ReadSector   
    pop dx
CONTINUE:
    add bx, dx                      ; es:bx -> fat+bios
    mov cx, word [es:bx]            ; cx -> item
    mov ax, [ClusNo]
    and ax, 0x1                     ; ax = CLusNo的最后一位
    jz EVEN
    shr cx, 4 
EVEN:
    and cx, 0x0fff

    mov ah, 0x0e
    mov al, '.'
    mov bl, 0x0f
    int 0x10

    pop bx
    mov ax, BaseOfLoader       
    mov es, ax   
    mov word [ClusNo], cx
    cmp cx, 0x0ff8
    jl LOAD_START

LOAD_SUCCESS:
    ; 来到这里表示加载完成
    jmp BaseOfLoader:OffsetOfLoader

%include "lib16.inc"


BootMessage: db "Welcome to QiuOS World!"
strLen equ $ - BootMessage

LoaderFileName: db "LOADER  BIN" 
LoaderFileNameLen equ $ - LoaderFileName

LoaderFoundMessage: db "booting"
LoaderFoundMessageLen equ $ - LoaderFoundMessage

LoaderNotFoundMessage: db "no loader"
LoaderNotFoundMessageLen equ $ - LoaderNotFoundMessage

SectorCurrent: dw 0                             ; 当前扇区号

ClusNo:        dw 0                             ; 簇号
curFatSector:  dw -1                            ; 当前加载的fat表扇区号


times 510 - ($-$$) db 0
dw 0xaa55


; 查找loader.bin文件伪代码

; for(sectorCurrent=19;sectorCurrent<33;sectorCurrent++)
;       ReadSector
;       for i=0; i < 16 ; i++
;           strcmp
;           j al==0 LOADER_FOUND
;           di &= 0xfe00
;           di += 0x20
; NOT_FOUND
