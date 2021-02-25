/*
 * 中断相关处理函数
*/

#include "proto.h"
#include "io.h"
#include "interrupt.h"
#include "asm.h"
#include "const.h"

uint64_t idt[IDT_SIZE];

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

// 向量号 助记符 描述      类型         出错码  源
void divide_error(uint32_t eip, uint16_t cs, uint32_t eflags); // 0      #DE   除法错     Fault       无     div和idiv指令
void single_step_exception(uint32_t eip, uint16_t cs, uint32_t eflags); // 1      #DB   调试异常   Fault/Trap  无     任何代码和数据的访问
void nmi(uint32_t eip, uint16_t cs, uint32_t eflags); // 2      -     非屏蔽中断 Interrupt   无     非屏蔽外部中断
void breakpoint_exception(uint32_t eip, uint16_t cs, uint32_t eflags); // 3      #DP   调试断点   Trap        无     指令int 3
void overflow(uint32_t eip, uint16_t cs, uint32_t eflags); // 4      #OF   溢出       Trap        无     指令into
void bounds_check(uint32_t eip, uint16_t cs, uint32_t eflags); // 5      #BR   越界       Fault       无     指令bound
void invalid_opcode(uint32_t eip, uint16_t cs, uint32_t eflags); // 6      #UD   无效操作码  Fault       无     指令ud2或无效指令
void coproc_not_available(uint32_t eip, uint16_t cs, uint32_t eflags); // 7      #NM   设备不可用  Fault       无     浮点wait/fwait指令
void double_fault(uint32_t err_code, uint32_t eip, uint16_t cs, uint32_t eflags); // 8      #DF   双重错误    Abort       有(0)  所有能产生异常或NMI或INTR的指令
void coproc_seg_overrun(uint32_t eip, uint16_t cs, uint32_t eflags); // 9            协处理器段越界 Fault     无     浮点指令
void invalid_tss(uint32_t err_code, uint32_t eip, uint16_t cs, uint32_t eflags); // 10     #TS  无效TSS      Fault       有     任务切换或访问TSS时
void segment_not_present(uint32_t err_code, uint32_t eip, uint16_t cs, uint32_t eflags); // 11     #NP  段不存在     Fault       有      加载段寄存器或访问系统段时
void stack_exception(uint32_t err_code, uint32_t eip, uint16_t cs, uint32_t eflags); // 12     #SS  堆栈段错误   Fault       有      堆栈操作或加载SS
void general_protection(uint32_t err_code, uint32_t eip, uint16_t cs, uint32_t eflags); // 13     #GP  一般保护错误 Fault       有       内存或其他保护检验
void page_fault(uint32_t err_code, uint32_t eip, uint16_t cs, uint32_t eflags); // 14     #PF  页错误      Fault       有        内存访问
                   // 15     intel保留未使用
void coproc_error(uint32_t eip, uint16_t cs, uint32_t eflags); // 16     #MF  x87FPU浮点错 Fault       无       x87FPU浮点指令或wait/fwait指令
void align_check(uint32_t err_code, uint32_t eip, uint16_t cs, uint32_t eflags); // 17     #AC  对齐检查     Fault       有(0)    内存中的数据访问
void machine_check(uint32_t eip, uint16_t cs, uint32_t eflags); // 18     #MC  Machine Check Abort     无
void simd_exception(uint32_t eip, uint16_t cs, uint32_t eflags); // 19     #XF  SIMD浮点异常  Fault      无
void default_handler(uint32_t eip, uint16_t cs, uint32_t eflags); // 一个默认的异常处理程序

void clock_handler(uint32_t eip, uint16_t cs, uint32_t eflags);
void keyboard_handler(uint32_t eip, uint16_t cs, uint32_t eflags);
// 中断桩
void default_handler_stub();
void divide_error_stub();
void single_step_exception_stub();
void nmi_stub();
void breakpoint_exception_stub();
void overflow_stub();
void bounds_check_stub();
void invalid_opcode_stub();
void coproc_not_available_stub();
void double_fault_stub();
void coproc_seg_overrun_stub();
void invalid_tss_stub();
void segment_not_present_stub();
void stack_exception_stub();
void general_protection_stub();
void page_fault_stub();
void coproc_error_stub();
void align_check_stub();
void machine_check_stub();
void simd_exception_stub();

void clock_intr_stub();
void keyboard_intr_stub();

public
void idt_init()
{
    // 先初始化8259A
    init_8259A();
    const selector_t cs_selector = (1 << 3) + SA_RPL0 + SA_TIG;

    for (int i = 0; i < IDT_SIZE; i++)
        // 将selector设置为代码段选择子，将offset设置为函数地址
        idt[i] = make_intr_gate(default_handler_stub, cs_selector, 0);

    idt[INT_VECTOR_ALIGN_CHECK] = make_trap_gate(align_check_stub, cs_selector, 0);
    idt[INT_VECTOR_BOUNDS_CHECK] = make_trap_gate(bounds_check_stub, cs_selector, 0);
    idt[INT_VECTOR_BREAKPOINT_ECEPTION] = make_trap_gate(breakpoint_exception_stub, cs_selector, 0);
    idt[INT_VECTOR_COPROC_ERROR] = make_trap_gate(coproc_error_stub, cs_selector, 0);
    idt[INT_VECTOR_COPROC_NOT_AVAILABLE] = make_trap_gate(coproc_not_available_stub, cs_selector, 0);
    idt[INT_VECTOR_COPROC_SEG_OVERRUN] = make_trap_gate(coproc_seg_overrun_stub, cs_selector, 0);
    idt[INT_VECTOR_DIVIDE_ERROR] = make_trap_gate(divide_error_stub, cs_selector, 0);
    idt[INT_VECTOR_DOUBLE_FAULT] = make_trap_gate(double_fault_stub, cs_selector, 0);
    idt[INT_VECTOR_GENERAL_PROTECTION] = make_trap_gate(general_protection_stub, cs_selector, 0);
    idt[INT_VECTOR_INVALID_OPCODE] = make_trap_gate(invalid_opcode_stub, cs_selector, 0);
    idt[INT_VECTOR_INVALID_TSS] = make_trap_gate(invalid_tss_stub, cs_selector, 0);
    idt[INT_VECTOR_MACHINE_CHECK] = make_trap_gate(machine_check_stub, cs_selector, 0);
    idt[INT_VECTOR_NMI] = make_intr_gate(nmi_stub, cs_selector, 0);
    idt[INT_VECTOR_OVERFLOW] = make_trap_gate(overflow_stub, cs_selector, 0);
    idt[INT_VECTOR_PAGE_FAULT] = make_trap_gate(page_fault_stub, cs_selector, 0);
    idt[INT_VECTOR_SEG_NOT_PRESENT] = make_trap_gate(segment_not_present_stub, cs_selector, 0);
    idt[INT_VECTOR_SIMD_EXCEPTION] = make_trap_gate(simd_exception_stub, cs_selector, 0);
    idt[INT_VECTOR_SINGLE_STEP_EXCEPTION] = make_trap_gate(single_step_exception_stub, cs_selector, 0);
    idt[INT_VECTOR_STACK_EXCEPTION] = make_trap_gate(stack_exception_stub, cs_selector, 0);

    idt[INT_VECTOR_IRQ0] = make_intr_gate(clock_intr_stub,cs_selector,0);
    idt[INT_VECTOR_IRQ0+1] = make_intr_gate(keyboard_intr_stub,cs_selector,0);

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

void default_handler(uint32_t eip, uint16_t cs, uint32_t eflags)
{
    puts("Unknown Exception\n\n");
    puts("EFLAGS:");
    puthex(eflags);
    putln();
    puts("CS:");
    puthex(cs);
    putln();
    puts("EIP:");
    puthex(eip);
    putln();
}
void divide_error(uint32_t eip, uint16_t cs, uint32_t eflags)
{
    // 触发方式，除以0
    puts("#DE Divide Error\n\n");
    puts("EFLAGS:");
    puthex(eflags);
    putln();
    puts("CS:");
    puthex(cs);
    putln();
    puts("EIP:");
    puthex(eip);
    putln();
    // fault, 会再次执行除法指令, 没有再次执行出错指令???,应该是执行了，但相应寄存器的内容已经改变
}
void single_step_exception(uint32_t eip, uint16_t cs, uint32_t eflags)
{
    // 触发方式，TF置为1 , fault/trap
    puts("#DB Single Step\n\n");
    puts("EFLAGS:");
    puthex(eflags);
    putln();
    puts("CS:");
    puthex(cs);
    putln();
    puts("EIP:");
    puthex(eip);
    putln();
    // hlt();
    // 将TF置为1后会一直触发该中断 fault或trap
}
void nmi(uint32_t eip, uint16_t cs, uint32_t eflags)
{
    // 触发方式，外部中断
    puts("- NMI Interrupt\n\n");
    puts("EFLAGS:");
    puthex(eflags);
    putln();
    puts("CS:");
    puthex(cs);
    putln();
    puts("EIP:");
    puthex(eip);
    putln();
    // hlt();
}
void breakpoint_exception(uint32_t eip, uint16_t cs, uint32_t eflags)
{
    // 触发方式 int3或int 3
    puts("#BP Breakpoint\n\n");
    puts("EFLAGS:");
    puthex(eflags);
    putln();
    puts("CS:");
    puthex(cs);
    putln();
    puts("EIP:");
    puthex(eip);
    putln();
    // hlt();
    // trap, 不会重复执行触发指令(int 3和int3都是)
}
void overflow(uint32_t eip, uint16_t cs, uint32_t eflags)
{
    // into指令，若OF被置位
    puts("#OF Overflow\n\n");
    puts("EFLAGS:");
    puthex(eflags);
    putln();
    puts("CS:");
    puthex(cs);
    putln();
    puts("EIP:");
    puthex(eip);
    putln();
    // hlt();
    // trap不会执行触发指令
}
void bounds_check(uint32_t eip, uint16_t cs, uint32_t eflags)
{
    // bound指令触发
    puts("#BR Bound Range Exceeded\n\n");
    puts("EFLAGS:");
    puthex(eflags);
    putln();
    puts("CS:");
    puthex(cs);
    putln();
    puts("EIP:");
    puthex(eip);
    putln();
    // hlt();
    // fault, 会再次执行
}
void invalid_opcode(uint32_t eip, uint16_t cs, uint32_t eflags)
{
    // ud2指令触发
    puts("#UD Invalid Opcode (Undefined Opcode)\n\n");
    puts("EFLAGS:");
    puthex(eflags);
    putln();
    puts("CS:");
    puthex(cs);
    putln();
    puts("EIP:");
    puthex(eip);
    putln();
    hlt();
    // fault 会再次执行
}
void coproc_not_available(uint32_t eip, uint16_t cs, uint32_t eflags)
{
    // fwait指令(未触发)
    puts("#NM Device Not Available (No Math Coprocessor)\n\n");
    puts("EFLAGS:");
    puthex(eflags);
    putln();
    puts("CS:");
    puthex(cs);
    putln();
    puts("EIP:");
    puthex(eip);
    putln();
    // hlt();
}
void double_fault(uint32_t err_code, uint32_t eip, uint16_t cs, uint32_t eflags)
{
    puts("#DF Double Fault\n\n");
    puts("ERROR CODE:");
    puthex(err_code);
    putln();
    puts("EFLAGS:");
    puthex(eflags);
    putln();
    puts("CS:");
    puthex(cs);
    putln();
    puts("EIP:");
    puthex(eip);
    putln();
    // hlt();abort不会返回
}
void coproc_seg_overrun(uint32_t eip, uint16_t cs, uint32_t eflags)
{
    puts("- Coprocessor Segment Overrun\n\n");
    puts("EFLAGS:");
    puthex(eflags);
    putln();
    puts("CS:");
    puthex(cs);
    putln();
    puts("EIP:");
    puthex(eip);
    putln();
    // hlt();
}
void invalid_tss(uint32_t err_code, uint32_t eip, uint16_t cs, uint32_t eflags)
{
    puts("#TS Invalid TSS\n\n");
    puts("ERROR CODE:");
    puthex(err_code);
    putln();
    puts("EFLAGS:");
    puthex(eflags);
    putln();
    puts("CS:");
    puthex(cs);
    putln();
    puts("EIP:");
    puthex(eip);
    putln();
    // hlt();
    // fault，会再次执行
}
void segment_not_present(uint32_t err_code, uint32_t eip, uint16_t cs, uint32_t eflags)
{
    puts("#NP Segment Not Present\n\n");
    puts("ERROR CODE:");
    puthex(err_code);
    putln();
    puts("EFLAGS:");
    puthex(eflags);
    putln();
    puts("CS:");
    puthex(cs);
    putln();
    puts("EIP:");
    puthex(eip);
    putln();
    hlt();
    // fault 会再次触发,但再次执行会触发一般保护错误，因为再次执行mov ds,ax指令，而ax的值已经改变
}
void stack_exception(uint32_t err_code, uint32_t eip, uint16_t cs, uint32_t eflags)
{
    puts("#SS Stack-Segment Fault\n\n");
    puts("ERROR CODE:");
    puthex(err_code);
    putln();
    puts("EFLAGS:");
    puthex(eflags);
    putln();
    puts("CS:");
    puthex(cs);
    putln();
    puts("EIP:");
    puthex(eip);
    putln();
    hlt();
}
void general_protection(uint32_t err_code, uint32_t eip, uint16_t cs, uint32_t eflags)
{
    puts("#GP General Protection\n\n");
    puts("ERROR CODE:");
    puthex(err_code);
    putln();
    puts("EFLAGS:");
    puthex(eflags);
    putln();
    puts("CS:");
    puthex(cs);
    putln();
    puts("EIP:");
    puthex(eip);
    putln();
    // hlt();
    // fault, 会再次执行
}
void page_fault(uint32_t err_code, uint32_t eip, uint16_t cs, uint32_t eflags)
{
    puts("#PF Page Fault\n\n");
    puts("ERROR CODE:");
    puthex(err_code);
    putln();
    puts("EFLAGS:");
    puthex(eflags);
    putln();
    puts("CS:");
    puthex(cs);
    putln();
    puts("EIP:");
    puthex(eip);
    putln();
    hlt();
}
// 15     intel保留未使用
void coproc_error(uint32_t eip, uint16_t cs, uint32_t eflags)
{
    puts("#MF x87 FPU Floating-Point Error (Math Fault)\n\n");
    puts("EFLAGS:");
    puthex(eflags);
    putln();
    puts("CS:");
    puthex(cs);
    putln();
    puts("EIP:");
    puthex(eip);
    putln();
    hlt();
}
void align_check(uint32_t err_code, uint32_t eip, uint16_t cs, uint32_t eflags)
{
    puts("#AC Alignment Check\n\n");
    puts("ERROR CODE:");
    puthex(err_code);
    putln();
    puts("EFLAGS:");
    puthex(eflags);
    putln();
    puts("CS:");
    puthex(cs);
    putln();
    puts("EIP:");
    puthex(eip);
    putln();
    hlt();
}
void machine_check(uint32_t eip, uint16_t cs, uint32_t eflags)
{
    puts("#MC Machine Check\n\n");
    puts("EFLAGS:");
    puthex(eflags);
    putln();
    puts("CS:");
    puthex(cs);
    putln();
    puts("EIP:");
    puthex(eip);
    putln();
    hlt();
}
void simd_exception(uint32_t eip, uint16_t cs, uint32_t eflags) 
{
    puts("#XF SIMD Floating-Point Exception\n\n");
    puts("EFLAGS:");
    puthex(eflags);
    putln();
    puts("CS:");
    puthex(cs);
    putln();
    puts("EIP:");
    puthex(eip);
    putln();
    hlt();
}


void clock_handler(uint32_t eip, uint16_t cs, uint32_t eflags)
{
    
}
void kerboard_handler(uint32_t eip, uint16_t cs, uint32_t eflags)
{
    uint8_t scan_code = in_byte(0x60);
    puthex(scan_code);
    putchar(' ');
}