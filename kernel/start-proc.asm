[extern current_proc]
[extern tss]

[section .text]
[global start_process]
[global restart_current_process]

; void start_process(process * proc);
start_process:
    mov eax, dword [esp+4]
    mov dword [current_proc], eax
    ; jmp restart_current_process

; void restart_current_process();
restart_current_process:
    mov esp, dword [current_proc]
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
