extern tss 

[section .text]
global start_process

start_process:
    mov esp, [esp+4]    ; esp -> proc
    lldt [esp + 76]     ; sel_ldt
    pop gs
    pop fs
    pop es
    pop ds

    popad
    add esp, 4
    iretd
