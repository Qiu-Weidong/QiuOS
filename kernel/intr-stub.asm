; 一个假的常量
has_err_code    equ 0

; 中断处理过程使用的栈顶
kernel_esp      equ 0x7e00

; 与8259A相关的一些常量
INT_M_CTL	    equ	0x20	
INT_M_CTLMASK	equ	0x21	
INT_S_CTL	    equ	0xA0	
INT_S_CTLMASK	equ	0xA1	
EOI		        equ	0x20
; 与中断相关的常量
intr_syscall    equ 0x90


; 全局变量
[extern intr_handlers   ]
[extern current_proc    ]
[extern tss             ]
[extern k_reenter       ]

[global start_process]

; 有错误码的异常桩
; exception_stub(int vec_no, has_err_code);
%macro exception_stub 2
    call save

    sti 
    call [intr_handlers+4*%1]
    add esp, 4
    cli

    ret 
%endmacro

; 没有错误码的异常桩
; exception_stub(int vec_no);
%macro exception_stub 1
    push %1                     ; 将中断号压栈
    call save
    
    sti
    call [intr_handlers+4*%1]
    ; mov esp, [esp]              ; 将esp指向要调度进程intr_frame的起始位置, 这里还是原来的进程
    add esp, 4
    cli
    
    ret 
%endmacro

; 外中断都没有错误码
; intr_stub(int irq)
%macro intr_stub 1
    push %1
    call save
    
    in al, INT_M_CTLMASK
    or al, (1<<%1)
    out INT_M_CTLMASK, al

    mov al, EOI
    out INT_M_CTL, al

    sti 
    call [intr_handlers+4*%1+32*4]
    add esp, 4
    cli 

    in al, INT_M_CTLMASK
    and al, ~(1<<%1)
    out INT_M_CTLMASK, al
    ret 
%endmacro

[section .data]
[global intr_stubs]
intr_stubs:
    dd divide_error_stub            ; 0
    dd single_step_exception_stub   ; 1
    dd nmi_stub                     ; 2
    dd breakpoint_exception_stub    ; 3
    dd overflow_stub                ; 4
    dd bounds_check_stub            ; 5
    dd invalid_opcode_stub          ; 6
    dd coproc_not_available_stub    ; 7
    dd double_fault_stub            ; 8
    dd coproc_seg_overrun_stub      ; 9
    dd invalid_tss_stub             ;10
    dd segment_not_present_stub     ;11
    dd stack_exception_stub         ;12
    dd general_protection_stub      ;13
    dd page_fault_stub              ;14
    dd 0                            ;15 intel保留未使用
    dd coproc_error_stub            ;16
    dd align_check_stub             ;17
    dd machine_check_stub           ;18
    dd simd_exception_stub          ;19

    ; 20~31 intel保留未使用
    times 31-20+1 dd 0

    ; 接下来是8259A外中断
    dd clock_stub                   ;32 时钟中断 
    dd keyboard_stub                ;33 键盘中断

    times 0x90-33-1 dd 0
    
    dd syscall_stub                 ;0x90 系统调用

    times 256*4 - ($-intr_stubs) dd 0   ; 全部置为0

[section .text]

; 异常桩
divide_error_stub:          exception_stub 0
single_step_exception_stub: exception_stub 1
nmi_stub:                   exception_stub 2
breakpoint_exception_stub:  exception_stub 3
overflow_stub:              exception_stub 4
bounds_check_stub:          exception_stub 5
invalid_opcode_stub:        exception_stub 6
coproc_not_available_stub:  exception_stub 7
double_fault_stub:          exception_stub 8, has_err_code
coproc_seg_overrun_stub:    exception_stub 9
invalid_tss_stub:           exception_stub 10, has_err_code
segment_not_present_stub:   exception_stub 11, has_err_code
stack_exception_stub:       exception_stub 12, has_err_code
general_protection_stub:    exception_stub 13, has_err_code
page_fault_stub:            exception_stub 14, has_err_code
coproc_error_stub:          exception_stub 16
align_check_stub:           exception_stub 17, has_err_code
machine_check_stub:         exception_stub 18
simd_exception_stub:        exception_stub 19

; 中断桩
clock_stub:                 intr_stub 0
keyboard_stub:              intr_stub 1



; 系统调用
syscall_stub:               exception_stub intr_syscall



offset_of_retaddr   equ 48
; 保存现场
; 重新设置esp，并push之前的esp
save:
    pushad
    push ds 
    push es 
    push fs 
    push gs 

    mov ax, ss 
    mov ds, ax 
    mov es, ax 
    mov fs, ax 

    mov esi, esp

    lock inc dword [k_reenter]           ; 如果没有重入，则为-1，inc后为0
    jnz set_restart1

    mov esp, kernel_esp
    push restart_current_process
    push esi
    xor ebp, ebp
    jmp [esi+offset_of_retaddr]
set_restart1:
    push re_enter
    jmp [esi+offset_of_retaddr]


; void start_process(process * proc);
start_process:
    mov eax, dword [esp+4]
    mov dword [current_proc], eax

; void restart_current_process();
restart_current_process:
    mov esp, dword [current_proc]
    lldt [esp + 80]
    lea eax, [esp+76]
    mov dword [tss+4], eax
re_enter:
    lock dec dword [k_reenter]
    pop gs 
    pop fs 
    pop es 
    pop ds 
    popad

    add esp, 8
    iretd
    