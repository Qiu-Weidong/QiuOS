; [extern current_proc]
; [extern tss]
; [extern k_reenter]

; [section .text]
; [global start_process]
; [global restart_current_process]
; [global re_enter]

; ; void start_process(process * proc);
; start_process:
;     mov eax, dword [esp+4]
;     mov dword [current_proc], eax

; ; void restart_current_process();
; restart_current_process:
;     mov esp, dword [current_proc]
;     lldt [esp + 80]
;     lea eax, [esp+76]
;     mov dword [tss+4], eax
; re_enter:
;     lock dec dword [k_reenter]
;     pop gs 
;     pop fs 
;     pop es 
;     pop ds 
;     popad

;     add esp, 8
;     iretd
