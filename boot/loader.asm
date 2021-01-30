%include "const.inc"
%include "pm.inc"    
%define LOADER

    org OffsetOfLoader
TopOfStack     equ     OffsetOfLoader                                           ; 栈基址

    jmp _start

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
; 16位数据段
kernelFoundMessage:         db "loading"
kernelFoundMessageLen       equ $ - kernelFoundMessage

kernelFileName              db "KERNEL  BIN"
kernelFileNameLen           equ $ - kernelFileName

kernelNotFound              db "No Kernel"
kernelNotFoundLen           equ $ - kernelNotFound



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
    push kernelFileName
    call searchFile
    add esp, 2
    test ax, ax
    jnz KERNEL_FOUND

    ; 这里表示没有找到kernel.bin
    mov ax, cs                                  ; 0x7ede
    mov es, ax
    mov ax, 0x1301                              ; ah=0x13表示输出字符串，al=0x01表示输出后光标位于字符串后面
    mov bx, 0x008c                              ; bh=0x0表示显示第0页，bl=0x04表示红色
    mov dx, 0x0300                              ; dh=0x11表示显示在第17行，dl=0x0表示显示在第0列
    mov bp, kernelNotFound                      ; es:bp指向带显示字符串地址
    mov cx, kernelNotFoundLen                   ; cx表示字符串长度
    int 0x10

    jmp $

; kernel.bin文件找到了
KERNEL_FOUND:
    push OffsetOfKernel
    push BaseOfKernel
    push ax

    ; 显示字符串,地址在es:bp中
    mov ax, cs                                  ; 0x7ef5
    mov es, ax
    mov ax, 0x1301                              ; ah=0x13表示输出字符串，al=0x01表示输出后光标位于字符串后面
    mov bx, 0x000f                              ; bh=0x0表示显示第0页，bl=0x04表示红色
    mov dx, 0x0200                              ; dh=0x11表示显示在第1行，dl=0x0表示显示在第0列
    mov bp, kernelFoundMessage                  ; es:bp指向带显示字符串地址
    mov cx, kernelFoundMessageLen               ; cx表示字符串长度
    int 0x10                                    ; 0x7f08

    call loadFile
    add esp, 6

LOAD_SUCCESS:
    ; 来到这里表示加载完成
    mov	dx, 03F2h                               ; '.
	mov	al, 0                                   ;  | 关闭软驱马达
	out	dx, al                                  ;  /

    ; 通过0x15中断获取内存信息
    ; es:di指向地址范围描述符结构ARDS
    mov ax, ds
    mov es, ax
    mov ebx, 0                                  ; 先将ebx置为0
    mov di, _MemChkBuf                          ; 
Mem_chk:
    mov eax, 0xe820
    mov ecx, 20
    mov edx, 0534D4150h
    int 0x15
    jc Mem_chk_fail                             ; 出错了
    add di, 20
    inc dword [_MCRCount]                       ; 将MCR的数量加一
    test ebx, ebx
    jnz Mem_chk                                 ; 如果ebx不为零，继续
    jmp Mem_chk_success 
Mem_chk_fail:
    mov dword [_MCRCount], 0


Mem_chk_success:

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
%include "lib32.inc"

PM_START:
    mov ax, SelectorVideo
    mov gs, ax
    mov ax, SelectorFlatRW
    mov es, ax
    mov ss, ax
    mov ds, ax
    mov esp, TopOfStack


    ; 打印表头
    ; puts(_MemChkTitle, 4, 0)
    push 0
    push 4
    push MemChkTitle
    call puts
    add esp, 12

    push dword [MCRCount]
    push MemChkBuf
    call disMemInfo
    add esp, 8

    push eax

    push 0
    push 12
    push RAMSize
    call puts
    add esp, 12
    
    pop eax
    add edi, 4
    call disEax

    jmp $


; 一些保护模式下要用到的数据
[SECTION  .data] 
align 32
[BITS  32]
_MemChkTitle:       db " BaseAddrL  BaseAddrH  LengthLow LengthHigh       Type",0
MemChkTitleLen      equ $ - _MemChkTitle

_RAMSize:           db "RAM size:",0
RAMSizeLen          equ $ - _RAMSize

_MCRCount           dd 0        

_MemChkBuf:         times 256 db 0

; 保护模式下使用这些符号
MemChkTitle         equ _MemChkTitle    + BaseOfLoaderPhyAddr
RAMSize             equ _RAMSize        + BaseOfLoaderPhyAddr
MemChkBuf           equ _MemChkBuf      + BaseOfLoaderPhyAddr
MCRCount            equ _MCRCount       + BaseOfLoaderPhyAddr