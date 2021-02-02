; $ nasm -f elf kernel.asm -o kernel.o
; ld -s -m elf_i386 kernel.o -o kernel.bin


[section .text] ;代码在此
global _start   ; 导出_start函数

_start:
    mov ah, 0xa

    mov al, 'W'
    mov [gs:((80*15+28)*2)],ax

    mov al, 'e'
    mov [gs:((80*15+28+1)*2)],ax

    mov al, 'l'
    mov [gs:((80*15+28+2)*2)],ax

    mov al, 'c'
    mov [gs:((80*15+28+3)*2)],ax

    mov al, 'o'
    mov [gs:((80*15+28+4)*2)],ax

    mov al, 'm'
    mov [gs:((80*15+28+5)*2)],ax

    mov al, 'e'
    mov [gs:((80*15+28+6)*2)],ax

    mov al, 't'
    mov [gs:((80*15+28+8)*2)],ax

    mov al, 'o'
    mov [gs:((80*15+28+9)*2)],ax

    mov al, 'Q'
    mov [gs:((80*15+28+11)*2)],ax

    mov al, 'i'
    mov [gs:((80*15+28+12)*2)],ax

    mov al, 'u'
    mov [gs:((80*15+28+13)*2)],ax
    
    mov al, 'O'
    mov [gs:((80*15+28+14)*2)],ax

    mov al, 'S'
    mov [gs:((80*15+28+15)*2)],ax

    mov al, 'W'
    mov [gs:((80*15+28+17)*2)],ax

    mov al, 'o'
    mov [gs:((80*15+28+18)*2)],ax

    mov al, 'r'
    mov [gs:((80*15+28+19)*2)],ax

    mov al, 'l'
    mov [gs:((80*15+28+20)*2)],ax

    mov al, 'd'
    mov [gs:((80*15+28+21)*2)],ax

    mov al, '!'
    mov [gs:((80*15+28+22)*2)],ax
    jmp $

    