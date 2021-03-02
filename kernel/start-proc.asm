[extern current_proc]
[extern tss]

[section .text]
[global start_process]
[global restart_current_process]

; void start_process(process * proc);
start_process:
    mov esp, [esp+4]
    lldt [esp + 80]
    lea eax, [esp+76]
    mov dword [tss+4], eax
    
    pop gs 
    pop fs 
    pop es 
    pop ds 
    popad

    add esp, 8
    iretd

restart_current_process:
    push dword [current_proc]
    call start_process

