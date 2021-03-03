[section .text]

[global halt  ]
[global exit  ]
[global getpid]


intr_syscall    equ 0x90

_NR_halt        equ 0
_NR_exit        equ 1
_NR_exec        equ 2
_NR_wait        equ 3
_NR_getpid      equ 4

; 参数会依次按照ebx、ecx、edx、esi、edi、ebp传递
halt:
    mov eax, _NR_halt
    int intr_syscall
    ret

; void exit(int status);
exit:
    mov eax, _NR_exit
    mov ebx, [esp+4]
    int intr_syscall
    ret

getpid:
    mov eax, _NR_getpid
    int intr_syscall
    ret

