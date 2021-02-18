extern default_handler

[section .text]
global intr_stub:
; 异常会重复执行产生异常的指令，而中断会返回到下一条指令
intr_stub:
    call default_handler
    iret