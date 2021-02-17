; 导入全局变量
extern dis_pos
extern dis_color

[section .text]
global putchar
global puts

; gs指向显存
; void putchar(int c)
putchar:
    mov edi, [dis_pos]
    mov ah, [dis_color]
    mov al, [esp+4]
    mov [gs:edi], ax
    add byte [dis_pos], 2
    ret

; gs指向显存
; int puts(const char * str)
puts:
    xor ecx, ecx
    mov edi, [dis_pos]
    mov esi, [esp+4]
    mov ax, gs
    mov es, ax
    mov ah, [dis_color]
L2:
    lodsb 
    test al, al                         ; 如果是'\0'，则输出结束
    jz L1
    cmp al, 10                          ; 如果是\n,则换行
    jne L3
    add edi, 160                        ; 换行
    mov eax, edi
    xor edx, edx
    mov ebx, 160
    div ebx                             ; edx -> 余数，eax -> 商
    sub edi, edx
    mov ah, [dis_color]
    inc ecx
    jmp L2
L3:
    stosw
    inc ecx
    jmp L2
L1:
    mov ax, ds
    mov es, ax
    mov [dis_pos], edi
    mov eax, ecx
    ret