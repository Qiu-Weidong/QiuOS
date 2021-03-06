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

; 参数会依次按照ebx、ecx、edx、esi、edi、ebp传递
halt:
    mov eax, _NR_halt
    int intr_syscall
    ret

; void exit(int status);
exit:
    push ebx
    mov eax, _NR_exit
    mov ebx, [esp+8]
    int intr_syscall
    pop ebx
    ret

getpid:
    mov eax, _NR_getpid
    int intr_syscall
    ret

write:
    push ebx
    mov eax, _NR_write
    mov ebx, [esp+8]
    mov ecx, [esp+12]
    mov edx, [esp+16]
    int intr_syscall
    pop ebx
    ret

; int sendrec(int function, int src_dest, message * msg);
sendrec:
    push ebx
    mov eax, _NR_sendrec
    mov ebx, [esp+8]        ; function
    mov ecx, [esp+12]        ; src_dest
    mov edx, [esp+16]       ; msg
    int intr_syscall
    pop ebx
    ret

