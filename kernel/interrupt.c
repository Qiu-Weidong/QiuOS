/*
 * 中断相关处理函数
*/

#include "proto.h"
#include "io.h"
#include "interrupt.h"
#include "asm.h"
#include "const.h"

uint64_t idt[IDT_SIZE];
extern intr_stub intr_stubs[IDT_SIZE];
intr_handler intr_handlers[IDT_SIZE];

private
void init_8259A()
{
    out_byte(INT_MASTER_MASK, 0xff);
    nop();
    nop();

    out_byte(INT_SLAVE_MASK, 0xff);
    nop();
    nop();

    out_byte(INT_MASTER_CONTROLLER, 0x11);
    nop();
    nop();

    out_byte(INT_SLAVE_CONTROLLER, 0x11);
    nop();
    nop();

    out_byte(INT_MASTER_MASK, INT_VECTOR_IRQ0);
    nop();
    nop();

    out_byte(INT_SLAVE_MASK, INT_VECTOR_IRQ8);
    nop();
    nop();

    out_byte(INT_MASTER_MASK, 0x04);
    nop();
    nop();

    out_byte(INT_SLAVE_MASK, 0x02);
    nop();
    nop();

    out_byte(INT_MASTER_MASK, 0x01);
    nop();
    nop();

    out_byte(INT_SLAVE_MASK, 0x01);
    nop();
    nop();

    out_byte(INT_MASTER_MASK, 0xfe);
    nop();
    nop();

    out_byte(INT_SLAVE_MASK, 0xff);
}

// 异常处理函数
// 向量号 助记符 描述      类型         出错码  源
void divide_error(intr_frame * frame);          // 0      #DE   除法错     Fault       无     div和idiv指令
void single_step_exception(intr_frame * frame); // 1      #DB   调试异常   Fault/Trap  无     任何代码和数据的访问
void nmi(intr_frame * frame);                   // 2      -     非屏蔽中断 Interrupt   无     非屏蔽外部中断
void breakpoint_exception(intr_frame * frame);  // 3      #DP   调试断点   Trap        无     指令int 3
void overflow(intr_frame * frame);              // 4      #OF   溢出       Trap        无     指令into
void bounds_check(intr_frame * frame);          // 5      #BR   越界       Fault       无     指令bound
void invalid_opcode(intr_frame * frame);        // 6      #UD   无效操作码  Fault       无     指令ud2或无效指令
void coproc_not_available(intr_frame * frame);  // 7      #NM   设备不可用  Fault       无     浮点wait/fwait指令
void double_fault(intr_frame * frame);                 // 8      #DF   双重错误    Abort       有(0)  所有能产生异常或NMI或INTR的指令
void coproc_seg_overrun(intr_frame * frame);           // 9            协处理器段越界 Fault     无     浮点指令
void invalid_tss(intr_frame * frame);                  // 10     #TS  无效TSS      Fault       有     任务切换或访问TSS时
void segment_not_present(intr_frame * frame);          // 11     #NP  段不存在     Fault       有      加载段寄存器或访问系统段时
void stack_exception(intr_frame * frame);              // 12     #SS  堆栈段错误   Fault       有      堆栈操作或加载SS
void general_protection(intr_frame * frame);           // 13     #GP  一般保护错误 Fault       有       内存或其他保护检验
void page_fault(intr_frame * frame);                   // 14     #PF  页错误      Fault       有        内存访问
                                                                                                                   // 15     intel保留未使用
void coproc_error(intr_frame * frame);          // 16     #MF  x87FPU浮点错 Fault       无       x87FPU浮点指令或wait/fwait指令
void align_check(intr_frame * frame);                  // 17     #AC  对齐检查     Fault       有(0)    内存中的数据访问
void machine_check(intr_frame * frame);         // 18     #MC  Machine Check Abort     无
void simd_exception(intr_frame * frame);        // 19     #XF  SIMD浮点异常  Fault      无

void default_handler(intr_frame * frame); // 一个默认的异常处理程序

void clock_handler(intr_frame * frame);
void keyboard_handler(intr_frame * frame);

public
void idt_init()
{
    // 先初始化8259A
    init_8259A();
    const selector_t cs_selector = (1 << 3) + SA_RPL0 + SA_TIG;

    // 0~19设置为陷阱门
    for (int i = 0; i < 19; i++)
        idt[i] = make_trap_gate(intr_stubs[i], cs_selector, 0);
    // 剩下的设置为中断门
    for (int i = 20; i < IDT_SIZE; i++)
        idt[i] = make_intr_gate(intr_stubs[i], cs_selector, 0);

    for(int i=0;i<IDT_SIZE;i++) intr_handlers[i] = default_handler;

    uint16_t idt_ptr[3];
    *((uint16_t volatile *)idt_ptr) = sizeof(idt) - 1;
    *((uint32_t volatile *)(idt_ptr + 1)) = (uint32_t)idt;
    lidt(idt_ptr);
}

public
bool_t is_intr_on()
{
    uint32_t flags = get_eflags();
    return flags & 0x20;
}

const char *err_msg[] = {
    "#DE Divide Error",
    "#DB RESERVED",
    "--  NMI Interrupt",
    "#BP Breakpoint",
    "#OF Overflow",
    "#BR BOUND Range Exceeded",
    "#UD Invalid Opcode (Undefined Opcode)",
    "#NM Device Not Available (No Math Coprocessor)",
    "#DF Double Fault",
    "    Coprocessor Segment Overrun (reserved)",
    "#TS Invalid TSS",
    "#NP Segment Not Present",
    "#SS Stack-Segment Fault",
    "#GP General Protection",
    "#PF Page Fault",
    "--  (Intel reserved. Do not use.)",
    "#MF x87 FPU Floating-Point Error (Math Fault)",
    "#AC Alignment Check",
    "#MC Machine Check",
    "#XF SIMD Floating-Point Exception"
};

void default_handler(intr_frame * frame)
{
    puts("Exception! ---> ");
    if(frame->vec_no >=0 && frame->vec_no < 20) puts(err_msg[frame->vec_no]);
    putln();
    puts("EIP:");
    puthex(frame->eip);
    putln();
    puts("CS:");
    puthex(frame->cs);
    putln();
    puts("EFLAGS:");
    puthex(frame->eflags);
    putln();
}
