%ifndef LOADER_INC_
%define LOADER_INC_

; 关闭软驱马达函数
; void killMotor();
killMotor:
    mov dx, 0x03f2                  ; '.
    xor al, al                      ;  | 关闭软驱马达，方法：向0x03f2端口写入0
    out dx, al                      ;  /
    ret

; void disTime(int row, int col);
;   al -> row bl -> col
disTime:
    xor ah, ah
    mov dx, ax
    shl ax, 2
    add ax, dx
    shl ax, 4
    xor bh, bh
    add ax, bx
    shl ax, 1
    mov di, ax

    mov ax, 0xb800
    mov es, ax

    mov al, 0x9
    out 0x70, al
    in al, 0x71 
    call disBCD
    mov al, '/'
    stosw

    mov al, 0x8
    out 0x70, al
    in al, 0x71 
    call disBCD
    mov al, '/'
    stosw

    mov al, 0x7
    out 0x70, al
    in al, 0x71 
    call disBCD

    add di, 2

    mov al, 0x4
    out 0x70, al
    in al, 0x71 
    call disBCD
    mov al, ':'
    stosw

    mov al, 0x2
    out 0x70, al
    in al, 0x71 
    call disBCD
    mov al, ':'
    stosw

    mov al, 0x0
    out 0x70, al
    in al, 0x71 
    call disBCD

    ret
; void disBCD(short BCD);
; al中为要显示的BCD，es:di指向显示位置
disBCD:
    mov ah, 0x7
    mov bl, al
    mov cl, 4
    shr al, cl
    and al, 0xf
    or  al, 0x30

    stosw                           ; 将ax放入es:di

    mov al, bl
    and al, 0xf
    or  al, 0x30            
    stosw
    ret

; short memchk(void * buffer)
; 将内存信息保存在buffer中，返回ARDS的个数
; es:di指向buffer
memchk:
    ; 通过0x15中断获取内存信息
    ; es:di指向地址范围描述符结构ARDS
    push bp
    mov bp, sp 
    sub esp, 2

    xor ebx, ebx                   ; 
    mov word [bp-2], 0
Mem_chk:
    mov eax, 0xe820
    mov ecx, 20
    mov edx, 0534D4150h
    int 0x15
    jc Mem_chk_fail                             ; 出错了
    add di, 20
    inc word [bp-2]
    test ebx, ebx
    jnz Mem_chk                                 ; 如果ebx不为零，继续
    jmp Mem_chk_success 
Mem_chk_fail:
    mov word [bp-2], 0


Mem_chk_success:
    mov ax, word [bp-2]
    add esp, 2
    pop bp
    ret


%endif ; LOADER_INC_