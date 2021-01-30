; =============== 实模式下的常用函数 ================
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
    sub esp, 2                      ; 开辟两个字节的空间

    mov byte [bp-2], cl             ; 在栈上保存cl
    mov cx, BPB_SecPerTrk
    div cl                          ; cl保存着扇区号18，ah=R，al=Q
    mov ch, al
    shr ch, 1                       ; 柱面号 = Q >> 1
    mov cl, ah   
    inc cl                          ; 起始扇区号 = R+1
    mov dl, BS_DrvNum             ; 驱动器号
    mov dh, al
    and dh, 0x1                     ; 磁头号 Q & 1
.readagain:
    mov ah, 0x2
    mov al, byte [bp-2]
    int 0x13
    jc .readagain

    add esp, 2
    pop bp
    ret

; KillMotor只在loader中使用，因此在boot中不编译
%ifdef LOADER
KillMotor:
	push	dx
	mov	dx, 03F2h
	mov	al, 0
	out	dx, al
	pop	dx
	ret
%endif ; LOADER

