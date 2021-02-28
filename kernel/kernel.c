#include "type.h"
#include "const.h"
#include "io.h"
#include "shutdown.h"
#include "proto.h"
#include "gdt.h"
#include "interrupt.h"
#include "asm.h"
#include "tss.h"
#include "proc.h"

uint32_t volatile dis_pos = 2400; // 从第15行开始显示
uint8_t volatile dis_color = 0xf; // 默认颜色为白色高亮
extern uint64_t gdt[GDT_SIZE];    // gdt表
extern uint64_t idt[IDT_SIZE];    // idt表

uint64_t ldt[3];

process volatile *current_proc;
process proc;
task_state_segment tss;
uint8_t stack[1024];

private
void kernel_init();

public 
void start_process(process * proc);

private 
void test()
{
    // dis_pos = 0;
    puts("Hello QiuOS World!");
    for(;;);
}

int kernel_main()
{
    set_color(HIGHLIGHT | FG_YELLOW | BG_BLACK);
    kernel_init();
    puts("Welcome to QiuOS World!\n");
    dis_color = HIGHLIGHT | FG_BLUE;

    ldt[0] = make_seg_desc(0, 0xfffff, DA_32 | DA_CR | DA_DPL3 | DA_LIMIT_4K); // 用户进程的代码段描述符
    ldt[1] = make_seg_desc(0, 0xfffff, DA_32 | DA_DRW | DA_DPL3 | DA_LIMIT_4K);
    ldt[2] = make_seg_desc(0xb8000, 0xffff, DA_32 | DA_DRW | DA_DPL3);

    proc.pid = 0;
    proc.priority = 0;
    proc.sel_ldt = gdt_push_back(gdt, make_ldt_desc((uint32_t)ldt, sizeof(ldt) - 1, DA_32 | DA_DPL0));

    proc.registers.cs = (0 << 3) + SA_RPL3 + SA_TIL;
    proc.registers.ds = proc.registers.es = proc.registers.ss = proc.registers.fs = (1 << 3) + SA_RPL3 + SA_TIL;
    proc.registers.gs = (2 << 3) + SA_RPL3 + SA_TIL;
    // if 9     0001 0010 0000 0010 开中断 eflags |= 0x200
    proc.registers.eflags = 0x1002;

    proc.registers.esp = (uint32_t)stack + 1024;
    proc.registers.eip = (uint32_t)test;
    start_process(&proc);
    
    shutdown();
}

private
void kernel_init()
{
    gdt_init();
    idt_init();

    current_proc = &proc;

    // tss只需要初始化ss0和esp0就可以了
    tss.ss0 = SELECTOR_KERNEL_DS;
    tss.esp0 = (uint32_t)(&current_proc->pid);

    ltr(SELECTOR_KERNEL_TSS);
}
