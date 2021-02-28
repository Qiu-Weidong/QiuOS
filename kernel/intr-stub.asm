; 一个假的常量
has_err_code    equ 0

; 导入中断处理函数列表
extern intr_handlers
; 导入当前进程指针
extern current_process

; 有错误码的异常桩
; exception_stub(int vec_no, has_err_code);
%macro exception_stub 2
    push %1     ; push vec_no

    pushad 
    push ds 
    push es 
    push fs 
    push gs 

    mov eax, esp                ; '.
    mov esp, 0x7e00             ;  | 切换堆栈并设置参数
    push eax                    ;  /

    call [intr_handlers+4*%1]
    add esp, 4

    ; mov esp, [current_process]

    pop gs 
    pop fs 
    pop es 
    pop ds 
    popad

    add esp, 8
    iret
%endmacro

; 没有错误码的异常桩
; exception_stub(int vec_no);
%macro exception_stub 1
    sub esp, 4
    push %1

    pushad 
    push ds 
    push es 
    push fs 
    push gs 

    push esp
    call [intr_handlers+4*%1]
    add esp, 4

    pop gs 
    pop fs 
    pop es 
    pop ds 
    popad

    add esp, 8
    iret
%endmacro

[section .data]
global intr_stubs
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
    dd 0                            ;15 intel保留为使用
    dd coproc_error_stub            ;16
    dd align_check_stub             ;17
    dd machine_check_stub           ;18
    dd simd_exception_stub          ;19
    
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




; 保存现场
; 重新设置esp，并push之前的esp
save:
    ret