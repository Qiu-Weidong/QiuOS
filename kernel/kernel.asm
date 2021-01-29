; $ nasm -f elf kernel.asm -o kernel.o
; ld -s -m elf_i386 kernel.o -o kernel.bin

[section .text] ;代码在此
global _start   ; 导出_start函数

_start:
    mov ah, 0xf ; 黑底白字
    mov al, 'K'
    mov [gs:((80*1+39)*2)],ax
    jmp $

    