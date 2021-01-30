%include "const.inc"
%include "pm.inc"    
%define LOADER

    org OffsetOfLoader
TopOfStack     equ     OffsetOfLoader          ; 栈基址

    jmp _start
; %include "fat12hdr.inc"

; GDT
;                               段基址,     段界限, 属性
GDT_ENTRY:      Descriptor          0,          0,  0
DESC_FLAT_C:    Descriptor          0,    0xfffff,  DA_CR|DA_32|DA_LIMIT_4K
DESC_FLAT_RW:   Descriptor          0,    0xfffff,  DA_DRW|DA_32|DA_LIMIT_4K
DESC_VIDEO:     Descriptor    0xb8000,     0xffff,  DA_DRW|DA_DPL3              ; 注意video的特权级为3

GdtLen                      equ $ - GDT_ENTRY
GdtPtr                      dw GdtLen - 1
                            dd BaseOfLoaderPhyAddr + GDT_ENTRY

; 选择子
SelectorFlatC               equ DESC_FLAT_C - GDT_ENTRY
SelectorFlatRW              equ DESC_FLAT_RW - GDT_ENTRY
SelectorVideo               equ DESC_VIDEO - GDT_ENTRY + SA_RPL3

; 需要用到的字符串常量
kernelFoundMessage:         db "loading"
kernelFoundMessageLen       equ $ - kernelFoundMessage

kernelFileName              db "KERNEL  BIN"
kernelFileNameLen           equ $ - kernelFileName

kernelNotFound              db "No Kernel"
kernelNotFoundLen           equ $ - kernelNotFound

readyMessage                db "Ready!"
readyMessageLen             equ $ - readyMessage


; 变量
SectorCurrent:              dw 0   ; 当前扇区号

ClusNo:                     dw 0   ; 簇号
curFatSector:               dw -1  ; 当前加载的fat表扇区号

_start:
    mov ax, cs
    mov es, ax
    mov ds, ax
    mov ss, ax
    mov sp, TopOfStack

    ; 重置软盘
    xor ah, ah
    xor dl, dl
    int 0x13

    ; 从这里开始查找kernel.bin文件
    mov word [SectorCurrent], SectorNumOfRootDirStart ; 初始化SectorCurrent
    mov ax, BaseOfKernel                    ; '.
    mov es, ax                              ;  | 设置es:bx指向BaseOfKernel:OffsetOfKernel
    mov bx, OffsetOfKernel                  ;  /
Search_for_kernel:

    mov ax, word [SectorCurrent]            ; '.
    mov cl, 1                               ;  | 读取SectorCurrent所在的扇区
    call ReadSector                         ;  /

    mov si, kernelFileName
    mov di, OffsetOfKernel        
    xor dx, dx                              ; 每个扇区16个条目，此处用dx来循环
Search_in_one_Sector:
    mov cx, kernelFileNameLen               ; 设置字符串长度11
    call strcmp
    cmp al, 0                               ; al=0表示两个字符串相同
    je  KERNEL_FOUND

    and di, 0xffe0
    add di, 0x0020
    mov si, kernelFileName
    inc dx
    cmp dx, 0x10
    jl  Search_in_one_Sector  

    inc word [SectorCurrent]
    cmp word [SectorCurrent], RootDirSectors+SectorNumOfRootDirStart
    jl  Search_for_kernel
    
    ; 这里表示没有找到kernel.bin
    mov ax, cs                                  ; 0x7ede
    mov es, ax
    mov ax, 0x1301                              ; ah=0x13表示输出字符串，al=0x01表示输出后光标位于字符串后面
    mov bx, 0x0004                              ; bh=0x0表示显示第0页，bl=0x04表示红色
    mov dx, 0x0300                              ; dh=0x11表示显示在第17行，dl=0x0表示显示在第0列
    mov bp, kernelNotFound                      ; es:bp指向带显示字符串地址
    mov cx, kernelNotFoundLen                   ; cx表示字符串长度
    int 0x10

    jmp $

; kernel.bin文件找到了
KERNEL_FOUND:
    ; 显示字符串,地址在es:bp中
    mov ax, cs                                  ; 0x7ef5
    mov es, ax
    mov ax, 0x1301                              ; ah=0x13表示输出字符串，al=0x01表示输出后光标位于字符串后面
    mov bx, 0x0002                              ; bh=0x0表示显示第0页，bl=0x04表示红色
    mov dx, 0x0200                              ; dh=0x11表示显示在第1行，dl=0x0表示显示在第0列
    mov bp, kernelFoundMessage                  ; es:bp指向带显示字符串地址
    mov cx, kernelFoundMessageLen               ; cx表示字符串长度
    int 0x10                                    ; 0x7f08

    ; 从这里开始加载kernel.bin
    mov ax, BaseOfKernel            ; '.
    mov es, ax                      ;  | 初始化es:bx
    mov bx, OffsetOfKernel          ;  /
    and di, 0xffe0                  ; '.
    add di, 0x001a                  ;   此时di指向起始簇号
    mov ax, word [es:di]            ; 将ClusNo的值放入ax中
    mov word [ClusNo], ax

LOAD_START:
    mov ax, word [ClusNo]
    ; 簇号+31得到扇区号
    add ax, RootDirSectors+SectorNumOfRootDirStart-2  
    mov cl, 1
    call ReadSector
    add bx, BPB_BytesPerSec

    push bx

    mov dx, word [ClusNo]
    mov ax, dx        ; ax = dx = CLusNo
    shl ax, 1         ; ax = 2*ClusNo, dx = ClusNo
    add ax, dx        ; ax = 3*ClusNo
    shr ax, 1         ; ax = 3*ClusNo / 2 = bios
    xor dx, dx        ; dx:ax = bios     


    mov bx, BPB_BytesPerSec
    div bx            ; ax = bios / 512 , dx = bios % 512
    add ax, 1         ; ax = bios / 512 + 1
    
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
    add bx, dx          ; es:bx -> fat+bios
    mov cx, word [es:bx] ; cx -> item
    mov ax, [ClusNo]
    and ax, 0x1          ; ax = CLusNo的最后一位
    jz EVEN
    shr cx, 4 
EVEN:
    and cx, 0x0fff

    mov ah, 0x0e
    mov al, '.'
    mov bl, 0x0f
    int 0x10

    pop bx
    mov ax, BaseOfKernel       
    mov es, ax   
    mov word [ClusNo], cx
    cmp cx, 0x0ff8
    jl LOAD_START

LOAD_SUCCESS:
    ; 来到这里表示加载完成
    call KillMotor              ; 0x90265


    mov ax, cs
    mov es, ax
    mov ax, 0x1301              ; ah=0x13表示输出字符串，al=0x01表示输出后光标位于字符串后面
    mov bx, 0x0005              ; bh=0x0表示显示第0页，bl=0x04表示红色
    mov dx, 0x0300              ; dh=0x11表示显示在第1行，dl=0x0表示显示在第0列
    mov bp, readyMessage        ; es:bp指向带显示字符串地址
    mov cx, readyMessageLen     ; cx表示字符串长度
    int 0x10

    ; 加载gdtr
    lgdt [GdtPtr]
    ; 关中断
    cli

    ; 打开A20地址线
    in al, 0x92
    or al, 00000010b
    out 92h, al

    ; 将cr0最低位置为1
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    ; 进入保护模式
    jmp dword SelectorFlatC:(BaseOfLoaderPhyAddr+PM_START)

%include "lib16.inc"


[SECTION .s32]
align 32
[BITS 32]

PM_START:
    mov ax, SelectorVideo
    mov gs, ax
    mov ax, SelectorFlatRW
    mov es, ax
    mov ss, ax
    mov ds, ax
    mov esp, TopOfStack

    mov ah, 0xf
    mov al, 'P'
    mov [gs:((80*7+0)*2)], ax

    jmp $
