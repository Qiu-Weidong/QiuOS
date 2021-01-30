%ifndef LIB32_INC_
%define LIB32_INC_
; =============== 保护模式下的常用函数 ================

; int puts(char * str, int row, int col)
puts:
    push ebp
    mov ebp, esp
    mov ebx, [ebp+12]                       ; ebx = row
    mov eax, ebx                            ; eax = ebx = row
    shl ebx, 2                              ; ebx = 4*row
    add ebx, eax                            ; ebx = 5*row
    shl ebx, 4                              ; ebx = 80*row
    mov eax, [ebp+16]                       ; eax = col
    add ebx, eax                            ; ebx = 80*row+col
    shl ebx, 1                              ; ebx = 160*row+2*col
    mov edi, ebx                            ; edi指向显示位置

    mov esi, [ebp+8]                        ; esi指向str
    mov ax, 0x0700                          ; 初始化ah=0x7
    xor ebx, ebx
    cld

puts_loop:
    lodsb                                   ; al = [esi++]
    test al, al
    jz puts_end
    mov word [gs:edi], ax
    add edi, 2
    inc ebx
    jmp puts_loop

puts_end:
    mov eax, ebx
    pop ebp
    ret

; 打印al中的数字(16进制)
; gs:edi为显存位置，打印后edi+=4
disAl:
    mov bl, al
    mov bh, 0x7 
    shr bl, 4           

    cmp bl, 10
    jb Less_10
    add bl, 'A' - 10
    jmp print
Less_10:
    add bl, '0'
print:
    mov word [gs:edi], bx
    add edi, 2

    mov bl, al
    and bl, 0x0f
    cmp bl, 9
    ja  greater_9
    add bl, '0'
    jmp print2
greater_9:
    add bl, 'A'-10
print2:
    mov word [gs:edi], bx
    add edi, 2
    ret
; 打印ax中的数字，16进制
; edi指向显存打印位置，打印完成后，edi+=8
disAx:
    mov dx, ax                  ; 将ax保存在dx中
    shr ax, 8
    call disAl

    mov ax, dx
    and ax, 0x00ff
    call disAl
    ret

disEax:
    mov edx, eax
    shr eax, 24
    call disAl

    mov eax, edx
    shr eax, 16
    and eax, 0xff
    call disAl

    mov eax, edx
    shr eax, 8
    and eax, 0xff
    call disAl

    mov eax, edx
    and eax, 0xff
    call disAl
    ret

putint:
    push ebp
    mov ebp, esp
    mov ebx, [ebp+12]                       ; ebx = row
    mov eax, ebx                            ; eax = ebx = row
    shl ebx, 2                              ; ebx = 4*row
    add ebx, eax                            ; ebx = 5*row
    shl ebx, 4                              ; ebx = 80*row
    mov eax, [ebp+16]                       ; eax = col
    add ebx, eax                            ; ebx = 80*row+col
    shl ebx, 1                              ; ebx = 160*row+2*col
    mov edi, ebx                            ; edi指向显示位置

    mov bh, 0x7 
    mov bl, '0'
    mov word [gs:edi], bx
    add edi, 2
    mov bl, 'x'
    mov word [gs:edi], bx
    add edi, 2

    mov eax, [esp+8]
    call disEax

    pop ebp
    ret
; 打印内存大小函数
; int disMemInfo(void * MemChkBuf, int MCRCount)
; {
;     for(int i=0;i<MCRCount;i++)
;     {
;         for(int j=0;j<20;j+=4)
;         {
;            putInt(MemChkBuf[j]);
;            putspace()
;         }
;         putchar('\n')
;         if(type == AddressRangeMemory)
;         {
;           if(BaseAddrLow + LengthLow > MemSize)
;               MemSize = BaseAddrLow + LengthLow
;         }
;     }
; }

disMemInfo:
    push ebp
    mov ebp, esp
    sub esp, 4                                          ; 为MemSize分配空间
    mov dword [esp], 0                                   ; 将MemSize初始化为0

    mov esi, [ebp+8]                                    ; esi = MemChkBuf
    mov edi, 160*5                                      ; edi -> 第5行起始位置
    mov ecx, esi
    jmp Check_MCRCount
loop1:
    push edi
    add ecx, 20                                     ; ecx = esi+20
loop2:
    mov bh, 0x7
    mov bl, '0'
    mov word [gs:edi], bx
    add edi, 2
    mov bl, 'x'
    mov word [gs:edi], bx
    add edi, 2
    lodsd                                           ; eax = [esi]; esi+=4
    call disEax                                     ; putint()
    add edi, 2                                      ; putspace()
    cmp esi, ecx
    jl loop2
    ; esi 指向type后面  [esi-4] == type [esi-8] == LengthHigh [esi-12] == LengthLow
    ;                  [esi-16] == BaseAddrH [esi-20] == BaseAddrL   
    pop edi
    add edi, 160                                    ; 换行
    ; eax -> type
    cmp eax, 0x1                                    ; type == AddressRangeMemory
    jne TypeNotAddressRangeMemory
    mov eax, [esi-20]                               ; eax = BaseAddrLow
    add eax, [esi-12]                               ; eax = BaseAddrLow + LengthLow
    cmp eax, [esp]                                  ; eax > MemSize
    jb TypeNotAddressRangeMemory 
    mov dword [esp], eax

TypeNotAddressRangeMemory:
    dec dword [ebp+12]

Check_MCRCount:
    cmp dword [ebp+12], 0
    jg loop1

    ; 外层循环结束
    mov eax, dword [esp]
    add esp, 4
    pop ebp
    ret


%endif ; LIB32_INC_