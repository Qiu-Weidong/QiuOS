[section .text]

[global halt  ]
[global exit  ]
[global getpid]
[global write]
[global sendrec]


intr_syscall    equ 0x90

_NR_halt        equ 0
_NR_exit        equ 1
_NR_exec        equ 2
_NR_wait        equ 3
_NR_getpid      equ 4
_NR_write       equ 5
_NR_sendrec     equ 6

; ebx、esp为被调用者保存寄存器

; 参数会依次按照ecx、edx、esi、edi传递
halt:
    mov eax, _NR_halt
    int intr_syscall
    ret

; void exit(int status);
exit:
    mov eax, _NR_exit
    mov ecx, [esp+4]
    int intr_syscall
    ret

getpid:
    mov eax, _NR_getpid
    int intr_syscall
    ret

write:
    mov eax, _NR_write
    mov ecx, [esp+4]
    mov edx, [esp+8]
    mov esi, [esp+12]
    int intr_syscall
    ret

; int sendrec(int function, int src_dest, message * msg);
sendrec:
    mov eax, _NR_sendrec
    mov ecx, [esp+4]        ; function
    mov edx, [esp+8]        ; src_dest
    mov esi, [esp+12]       ; msg
    int intr_syscall
    ret

