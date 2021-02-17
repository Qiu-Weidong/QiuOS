/*
 * 中断相关处理函数
*/

#include "asm.h"
#include "const.h"
#include "proto.h"

extern u64 idt[IDT_SIZE];

private void init_8259A()
{

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

    out_byte(INT_MASTER_MASK, 0xfc);
    nop();
    nop();

    out_byte(INT_SLAVE_MASK, 0xff);
}


                                        // 向量号 助记符 描述      类型         出错码  源 
private void divide_error();            // 0      #DE   除法错     Fault       无     div和idiv指令
private void single_step_exception();   // 1      #DB   调试异常   Fault/Trap  无     任何代码和数据的访问
private void nmi();                     // 2      -     非屏蔽中断 Interrupt   无     非屏蔽外部中断
private void breakpoint_exception();    // 3      #DP   调试断点   Trap        无     指令int 3
private void overflow();                // 4      #OF   溢出       Trap        无     指令into
private void bounds_check();            // 5      #BR   越界       Fault       无     指令bound
private void invalid_opcode();          // 6      #UD   无效操作码  Fault       无     指令ud2或无效指令 
private void coproc_not_available();    // 7      #NM   设备不可用  Fault       无     浮点wait/fwait指令
private void double_fault();            // 8      #DF   双重错误    Abort       有(0)  所有能产生异常或NMI或INTR的指令
private void coproc_seg_overrun();      // 9            协处理器段越界 Fault     无     浮点指令
private void invalid_tss();             // 10     #TS  无效TSS      Fault       有     任务切换或访问TSS时
private void segment_not_present();     // 11     #NP  段不存在     Fault       有      加载段寄存器或访问系统段时
private void stack_exception();         // 12     #SS  堆栈段错误   Fault       有      堆栈操作或加载SS
private void general_protection();      // 13     #GP  一般保护错误 Fault       有       内存或其他保护检验
private void page_fault();              // 14     #PF  页错误      Fault       有        内存访问
                                        // 15     intel保留未使用
private void coproc_error();            // 16     #MF  x87FPU浮点错 Fault       无       x87FPU浮点指令或wait/fwait指令
private void align_check();             // 17     #AC  对齐检查     Fault       有(0)    内存中的数据访问
private void machine_check();           // 18     #MC  Machine Check Abort     无  
private void simd_exception();          // 19     #XF  SIMD浮点异常  Fault      无
private void default_handler();         // 一个默认的异常处理程序


public void init_idt()
{
    // 先初始化8259A
    init_8259A();
    for(int i=0;i<IDT_SIZE;i++)
    {
        // 将selector设置为代码段选择子，将offset设置为函数地址
        set_gate(idt+i,(u32)default_handler,(1 << 3) + SA_RPL0 + SA_TIG,0,DA_386IGate);
    }

    u8 idt_ptr[6];
    *((u16 *)idt_ptr) = sizeof(idt) - 1;
    *((u32 *)(idt_ptr+2)) = (u32)idt;
    lidt(idt_ptr);
}

private void default_handler()
{
    puts("exception!\n");
}