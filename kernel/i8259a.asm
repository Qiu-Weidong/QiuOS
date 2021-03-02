INT_M_CTL	    equ	0x20	
INT_M_CTLMASK	equ	0x21	
INT_S_CTL	    equ	0xA0	
INT_S_CTLMASK	equ	0xA1	
EOI		        equ	0x20

%macro delay 0
times 3 nop
%endmacro
[section .text]
[global enable_irq]
[global disable_irq]
[global init_8259a]


; void init_8259a();
init_8259a:
    mov al, 0xff
    out INT_M_CTLMASK, al
    delay

    out INT_S_CTLMASK, al 
    delay

    mov al, 0x11
    out INT_M_CTL, al
    delay

    out INT_S_CTL, al
    delay

    mov al, 0x20
    out INT_M_CTLMASK, al 
    delay

    mov al, 0x28
    out INT_S_CTLMASK, al 
    delay

    mov al, 0x04
    out INT_M_CTLMASK, al 
    delay

    mov al, 0x02
    out INT_S_CTLMASK, al 
    delay

    mov al, 0x01
    out INT_M_CTLMASK, al 
    delay

    out INT_S_CTLMASK, al 
    delay

    mov al, 0xff 
    out INT_M_CTLMASK, al
    delay 

    out INT_S_CTLMASK, al 
    ret

; void enable_irq(int irq)
enable_irq:
    mov ecx, [esp+4]
    pushf
    cli 
    mov ah, 0xfe
    rol ah, cl 
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
    mov ah, 0x1
    rol ah, cl
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
