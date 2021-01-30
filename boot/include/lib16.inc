; =============== 实模式下的常用函数 ================
%ifndef LIB16_INC_
%define LIB16_INC_

%include "const.inc"

; 字符串比较函数, 测试通过
; ds:[si] -> str1, es:[di] -> str2
; 相同则al为0，否则al非0
strcmp:
    cld
    add cx, di
st: lodsb
    sub al, [es:di]
    jnz ed
    inc di
    cmp di, cx
    jl  st
ed: ret


; 读取软盘函数, 测试通过
; ax = 要读取的扇区号
; cl = 要读取的扇区数
; es:bx -> 数据缓冲区
ReadSector:
    ; 转换公式如下
    ;                                              / 柱面号 Q >> 1 
    ;             扇区号                   /  商 Q |
    ; ———————————————————————————————— =  |        \ 磁头号 Q & 1
    ; 每磁道扇区数(这里是BPB_SecPerTrk)     \ 余数 R -> 起始扇区号 R+1
    push bp
    mov bp, sp 
    sub esp, 2                                  ; 开辟两个字节的空间

    mov byte [bp-2], cl                         ; 在栈上保存cl, 必须两字节对齐
    mov cx, BPB_SecPerTrk
    div cl                                      ; cl保存着扇区号18，ah=R，al=Q
    mov ch, al
    shr ch, 1                                   ; 柱面号 = Q >> 1
    mov cl, ah   
    inc cl                                      ; 起始扇区号 = R+1
    mov dl, BS_DrvNum                           ; 驱动器号
    mov dh, al
    and dh, 0x1                                 ; 磁头号 Q & 1
.readagain:
    mov ah, 0x2
    mov al, byte [bp-2]
    int 0x13
    jc .readagain

    add esp, 2
    pop bp
    ret

; 查找文件函数
; short searchFile(char * filename)
;       filename -> [sp+2]
; \return ax -> 文件的起始扇区号，如果文件不存在则为0
searchFile:
    push bp
    mov bp, sp
    sub esp, 2                                  ; 为SectorCurrent分配空间
    
    mov word [bp-2], SectorNumOfRootDirStart    ; 初始化SectorCurrent
    mov ax, BaseOfFAT                           ; '.
    mov es, ax                                  ;  | 设置es:bx指向BaseOfFAT:OffsetOfFAT
    mov bx, OffsetOfFAT                         ;  /
Search_for_file:                                ; 外层循环起始处
    
    mov ax, word [bp-2]                         ; '.
    mov cl, 1                                   ;  | 读取SectorCurrent所在扇区 
    call ReadSector                             ;  /

    mov si, word [bp+4]                         ; ds:si -> filename
    mov di, OffsetOfFAT                         ; es:di -> BaseOfFAT:OffsetOfFAT
    xor dx, dx

Search_in_one_Sector:
    mov cx, FileNameLen
    call strcmp
    cmp al, 0
    je File_found

    and di, 0xffe0
    add di, 0x0020
    mov si, word [bp+4]
    inc dx
    cmp dx, 0x10
    jl Search_in_one_Sector

    inc word [bp-2]
    cmp word [bp-2], RootDirSectors+SectorNumOfRootDirStart
    jl Search_for_file
File_not_found:
    xor ax, ax
    jmp Search_end
File_found:
    and di, 0xffe0
    add di, 0x001a
    mov ax, word [es:di]

Search_end:
    add esp, 2
    pop bp
    ret

; 加载文件函数
; void loadFile(short ClusNo, short base, short offset)
; \param ClusNo -> 文件的起始扇区簇号
loadFile:
    push bp
    mov bp, sp
    sub esp, 2
    mov ax, word [bp + 6]
    mov es, ax
    mov bx, word [bp + 8]
    mov word [bp-2], -1

Load_start:
    mov ax, word [bp+4]
    add ax, RootDirSectors+SectorNumOfRootDirStart-2
    mov cl, 1
    call ReadSector
    add bx, BPB_BytesPerSec

    push bx

    mov dx, word [bp+4]
    mov ax, dx
    shl ax, 1
    add ax, dx
    shr ax, 1
    xor dx, dx

    mov bx, BPB_BytesPerSec
    div bx
    add ax, 1

    mov bx, BaseOfFAT
    mov es, bx
    mov bx, OffsetOfFAT

    cmp ax, word [bp-2]
    jz CONTINUE
    push dx
    mov word [bp-2], ax
    mov cl, 2
    call ReadSector
    pop dx
CONTINUE:
    add bx, dx
    mov cx, word [es:bx]
    mov ax, [bp+4]
    and ax, 0x1 
    jz EVEN
    shr cx, 4
EVEN:
    and cx, 0x0fff

    mov ah, 0x0e
    mov al, '.'
    mov bl, 0x0f
    int 0x10 

    pop bx
    mov ax, word [bp+6]
    mov es, ax
    mov word [bp+4], cx
    cmp cx, 0x0ff8
    jl Load_start

Load_end:
    add esp, 2
    pop bp
    ret


%endif ; LIB16_INC_
