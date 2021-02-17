; 导入全局变量
extern dis_pos
extern dis_color

[section .text]
global putchar
global puts
global putdec
global puthex
global putoct
global putln
global getchar
global getline
global set_color

; gs指向显存
; void putchar(int c)
putchar:
    push edi
    push eax

    mov edi, [dis_pos]
    mov ah, [dis_color]
    mov al, [esp+12]
    cmp al, 10
    jnz L6
    call putln
    jmp L7
L6:
    mov [gs:edi], ax
    add byte [dis_pos], 2
L7:
    pop eax
    pop edi
    ret

; gs指向显存
; int puts(const char * str)
puts:
    push ecx
    push edi
    push esi
    push es

    xor ecx, ecx
    mov edi, [dis_pos]
    mov esi, [esp+20]
    mov ax, gs
    mov es, ax
    mov ah, [dis_color]
L2:
    lodsb 
    test al, al                         ; 如果是'\0'，则输出结束
    jz L1
    cmp al, 10                          ; 如果是\n,则换行
    jne L3
    mov [dis_pos], edi
    call putln
    mov edi, [dis_pos]
    inc ecx
    jmp L2
L3:
    stosw
    inc ecx
    jmp L2
L1:
    mov [dis_pos], edi
    mov eax, ecx

    pop es
    pop esi
    pop edi
    pop ecx
    ret


; void putdec(int n);
; char buffer[10];
; int cnt = 0;
; do
; {
;   buffer[cnt++] = n % 10 + '0';
;   n /= 10;
; }while(n);
; do
; {
;    cnt--;
;    putchar(buffer[cnt]);
; }
putdec:
    push eax
    push ebx
    mov eax, [esp+12]
    mov ebx, 10
    call putnum

    pop ebx
    pop eax
    ret

puthex:
    push eax
    push ebx

    push '0'
    call putchar
    push 'x'
    call putchar
    add esp, 8

    mov eax, [esp+12]
    mov ebx, 0x10
    call putnum

    pop ebx
    pop eax
    ret

putoct:
    push eax
    push ebx
    mov eax, [esp+12]
    mov ebx, 0x8
    call putnum

    pop ebx
    pop eax
    ret

putln:
    push eax
    push ebx
    push edx

    add dword [dis_pos], 160
    mov eax, dword [dis_pos]
    xor edx, edx
    mov ebx, 160
    div ebx                             ; edx -> 余数，eax -> 商
    sub dword [dis_pos], edx

    pop edx
    pop ebx
    pop eax
    ret

getchar:
    ret
getline:
    ret

; eax -> 数字，ebx -> 进制
putnum:
    push ecx
    push edx
    push esi

    sub esp, 0x10               ; 在栈上分配0x10的空间(int的最大值为2147483647，16字节的空间足够了)
    mov esi, esp
    xor ecx, ecx
L4: 
    xor edx, edx
    div ebx                     ; eax -> 商，edx -> 余数
    cmp edx, 10
    jl L8
    add edx, 'a'-10
    jmp L9
L8:
    add edx, '0'                ; edx -> n % 10 + 0
L9:
    mov [esi+ecx], dl 
    inc ecx
    test eax, eax
    jnz L4
L5:
    dec ecx
    movzx eax, byte [esi+ecx]
    push eax
    call putchar
    add esp, 4
    test ecx, ecx
    jnz L5

    add esp, 0x10
    pop esi
    pop edx
    pop ecx
    ret

set_color:
    mov al, byte [esp+4]
    mov byte [dis_color],al
    ret
