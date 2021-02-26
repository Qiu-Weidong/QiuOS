extern tss
extern current_proc

[section .text]
global current_process
global start_process

; void start_process(process * proc);
start_process:
    mov esp, [esp+4]
    lea eax, [esp+72]
    mov dword [tss+4], eax
    lldt [esp+76]


    pop gs
    pop fs
    pop es
    pop ds
    popad 
    add esp, 4

    iret

; process * current_process();
current_process:
    mov eax,current_proc
    ret



