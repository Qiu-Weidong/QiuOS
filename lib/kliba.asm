%include "intr.inc"

[section .text]
global enable_irq
global disable_irq

; void enable_irq(int irq)
enable_irq:
    mov ecx, [esp+4]
    pushf
    cli 
    mov al, 0xfe
    rol al, cl 
    cmp cl, 8
    jae enable_irq_slave
enable_irq_master:
    mov dx, INT_M_CTLMASK
    jmp enable_irq_exit
enable_irq_slave:
    mov dx, INT_S_CTLMASK

enable_irq_exit:
    in al, dx
    and al, ah 
    out dx , al
    popf
    ret 

; void disable_irq(int irq)
disable_irq:
    mov ecx, [esp+4]
    pushf
    cli
    mov al, 0x1
    rol al, cl
    cmp cl, 8
    jae disable_irq_slave
disable_irq_master:
    mov dx, INT_M_CTLMASK
    jmp disable_irq_exit
disable_irq_slave:
    mov dx, INT_S_CTLMASK
disable_irq_exit:
    in al, dx
    or al, ah
    out dx, al
    popf
    ret
