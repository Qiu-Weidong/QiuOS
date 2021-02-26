extern default_handler
extern irq_table
extern excp_table

extern tss
extern start_process
; 外中断
extern task_schedule
extern keyboard_handler

[section .text]
global default_handler_stub
global divide_error_stub
global single_step_exception_stub
global nmi_stub
global breakpoint_exception_stub
global overflow_stub
global bounds_check_stub
global invalid_opcode_stub
global coproc_not_available_stub
global double_fault_stub
global coproc_seg_overrun_stub
global invalid_tss_stub
global segment_not_present_stub
global stack_exception_stub
global general_protection_stub

global page_fault_stub
global coproc_error_stub
global align_check_stub
global machine_check_stub
global simd_exception_stub

; 8259A外中断
global clock_intr_stub
global keyboard_intr_stub

; fault会重新执行产生异常的指令，而trap不会
default_handler_stub:
    call default_handler
    iret
divide_error_stub:
    ; call divide_error
    call [excp_table + 0]
    iret
single_step_exception_stub:
    ; call single_step_exception
    call [excp_table + 4]
    iret
nmi_stub:
    ; call nmi
    call [excp_table + 8]
    iret
breakpoint_exception_stub:
    ; call breakpoint_exception
    call [excp_table + 12]
    iret
overflow_stub:
    ; call overflow
    call [excp_table + 16]
    iret
bounds_check_stub:
    ; call bounds_check
    call [excp_table + 20]
    iret
invalid_opcode_stub:
    ; call invalid_opcode
    call [excp_table + 24]
    iret
coproc_not_available_stub:
    ; call coproc_not_available
    call [excp_table + 28]
    iret
double_fault_stub:
    ; call double_fault
    call [excp_table + 32]
    add esp, 4
    iret                        ; abort不会返回，可以没有iret指令
coproc_seg_overrun_stub:
    ; call coproc_seg_overrun
    call [excp_table + 36]
    iret
invalid_tss_stub:
    ; call invalid_tss
    call [excp_table+40]
    add esp, 4
    iret
segment_not_present_stub:
    ; call segment_not_present
    call [excp_table+44]
    add esp, 4
    iret
stack_exception_stub:
    ; call stack_exception
    call [excp_table+48]
    add esp, 4
    iret
general_protection_stub:
    ; call general_protection
    call [excp_table+52]
    add esp, 4
    iret
page_fault_stub:
    ; call page_fault
    call [excp_table + 56]
    add esp, 4
    iret
coproc_error_stub:
    ; call coproc_error
    call [excp_table+64]
    iret
align_check_stub:
    ; call align_check
    call [excp_table+68]
    add esp, 4
    iret
machine_check_stub:
    ; call machine_check
    call [excp_table+72]
    iret                    ; abort不会返回，可以没有iret指令
simd_exception_stub:
    ; call simd_exception
    call [excp_table + 76]
    iret
clock_intr_stub:
    ; 保存现场
    call save
    ; esp指向内核栈

    ; 屏蔽当前中断，这里是时钟中断
    in al, 0x21                     ; '.
    or al, 0x01                     ;   |
    out 0x21, al                    ;   |
                                    ;   | 可以利用宏来简化
    ; 开启中断                       ;   |
    mov al, 0x20                    ;   |
    out 0x20, al                    ;  /

    call task_schedule

    ; 恢复当前中断
    in	al, 0x21	
	and	al, ~1		
	out	0x21, al	    
    ; 启动进程，方法，压栈要启动的进程的pcb指针，然后call start_process函数
    push eax
    call start_process

keyboard_intr_stub:
    call keyboard_handler
    mov al, 0x20
    out 0x20, al
    iret


save:
    ; esp -> register的最末尾
    pushad
    push ds 
    push es 
    push fs 
    push gs 

    ; 此时esp -> pcb开始处
    mov eax, esp

    ; 修改段寄存器
    mov dx, ss 
    mov es, dx
    mov fs, dx
    mov ds, dx

    ; 将esp指向内核栈，暂时为0x7e00
    mov esp, 0x7e00
    ; 返回
    jmp [eax + 48]
