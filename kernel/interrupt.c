/*
 * 中断相关处理函数
*/

#include "proto.h"
#include "io.h"
#include "interrupt.h"
#include "asm.h"
#include "const.h"
#include "global.h"
#include "keyboard.h"

intr_handler intr_handlers[IDT_SIZE];
extern intr_stub intr_stubs[IDT_SIZE];

// 异常处理函数
// 向量号 助记符 描述      类型         出错码  源
private void divide_error(const intr_frame *frame);          // 0      #DE   除法错     Fault       无     div和idiv指令
private void single_step_exception(const intr_frame *frame); // 1      #DB   调试异常   Fault/Trap  无     任何代码和数据的访问
private void nmi(const intr_frame *frame);                   // 2      -     非屏蔽中断 Interrupt   无     非屏蔽外部中断
private void breakpoint_exception(const intr_frame *frame);  // 3      #DP   调试断点   Trap        无     指令int 3
private void overflow(const intr_frame *frame);              // 4      #OF   溢出       Trap        无     指令into
private void bounds_check(const intr_frame *frame);          // 5      #BR   越界       Fault       无     指令bound
private void invalid_opcode(const intr_frame *frame);        // 6      #UD   无效操作码  Fault       无     指令ud2或无效指令
private void coproc_not_available(const intr_frame *frame);  // 7      #NM   设备不可用  Fault       无     浮点wait/fwait指令
private void double_fault(const intr_frame *frame);          // 8      #DF   双重错误    Abort       有(0)  所有能产生异常或NMI或INTR的指令
private void coproc_seg_overrun(const intr_frame *frame);    // 9            协处理器段越界 Fault     无     浮点指令
private void invalid_tss(const intr_frame *frame);           // 10     #TS  无效TSS      Fault       有     任务切换或访问TSS时
private void segment_not_present(const intr_frame *frame);   // 11     #NP  段不存在     Fault       有      加载段寄存器或访问系统段时
private void stack_exception(const intr_frame *frame);       // 12     #SS  堆栈段错误   Fault       有      堆栈操作或加载SS
private void general_protection(const intr_frame *frame);    // 13     #GP  一般保护错误 Fault       有       内存或其他保护检验
private void page_fault(const intr_frame *frame);            // 14     #PF  页错误      Fault       有        内存访问
                                                     // 15     intel保留未使用
private void coproc_error(const intr_frame *frame);          // 16     #MF  x87FPU浮点错 Fault       无       x87FPU浮点指令或wait/fwait指令
private void align_check(const intr_frame *frame);           // 17     #AC  对齐检查     Fault       有(0)    内存中的数据访问
private void machine_check(const intr_frame *frame);         // 18     #MC  Machine Check Abort     无
private void simd_exception(const intr_frame *frame);        // 19     #XF  SIMD浮点异常  Fault      无

private void default_handler(const intr_frame *frame); // 一个默认的异常处理程序


void task_schedule(const intr_frame *frame UNUSED);
void keyboard_handler(const intr_frame *frame UNUSED);
void syscall_handler(intr_frame * frame);

public
void idt_init()
{
    // 先初始化8259A
    init_8259a();

    // 开启键盘中断
    // enable_irq(0);
    // enable_irq(1);
    // keyboard_init();

    const selector_t cs_selector = (1 << 3) + SA_RPL0 + SA_TIG;

    // 0~19设置为陷阱门
    for (int i = 0; i < 20; i++)
        idt[i] = make_trap_gate(intr_stubs[i], cs_selector, 0);
    // 剩下的设置为中断门
    for (int i = 20; i < IDT_SIZE; i++)
        idt[i] = make_intr_gate(intr_stubs[i], cs_selector, 0);

    for (int i = 0; i < IDT_SIZE; i++)
        intr_handlers[i] = default_handler;
    
    intr_handlers[INT_VECTOR_IRQ0] = task_schedule;
    intr_handlers[INT_VECTOR_IRQ0+1] = keyboard_handler;

    idt[INT_VECTOR_SYSCALL] = make_intr_gate(intr_stubs[INT_VECTOR_SYSCALL], cs_selector, 3);
    intr_handlers[INT_VECTOR_SYSCALL] = (void (*)(const intr_frame *))syscall_handler;

    uint16_t idt_ptr[3];
    *((uint16_t volatile *)idt_ptr) = sizeof(uint64_t)*IDT_SIZE - 1;
    *((uint32_t volatile *)(idt_ptr + 1)) = (uint32_t)idt;
    lidt(idt_ptr);
}

public
bool_t is_intr_on()
{
    uint32_t flags = get_eflags();
    return flags & INTR_FLAG;
}

private const char *err_msg[] = {
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

private void default_handler(const intr_frame *frame)
{
    puts("Exception! ---> ");
    if (frame->no_code.vec_no >= 0 && frame->no_code.vec_no < 20)
        puts(err_msg[frame->no_code.vec_no]);
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
    // hlt();
    for(;;);
}


