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

uint8_t stack[1024];
uint8_t stack2[1024];
uint8_t stack3[1024];
uint64_t ldt[3];

process proc1,proc2,proc3;
process * current_proc;
task_state_segment tss;

private
void kernel_init();

void start_process(process *proc) NO_RETURN;

private
int test();

private 
int test2();

private 
int test3();

private
void proc_exit();

int kernel_main()
{
    set_color(HIGHLIGHT | FG_YELLOW | BG_BLACK);
    kernel_init();
    puts("Welcome to QiuOS World!\n");
    dis_color = HIGHLIGHT | FG_BLUE;
    // 长跳转，定义一个6字节的空间，偏移占前4个字节，选择子占后两个字节，最后ljmp跳转
    // *((uint32_t volatile *)fptr) = (uint32_t)dest_func;
    // *((uint16_t volatile *)(fptr+2)) = (5 << 3);
    // ljmp(fptr);
    // sti();
    // for (;;);
    ud2();

    ldt[0] = make_seg_desc(0, 0xfffff, DA_32 | DA_CR | DA_DPL3 | DA_LIMIT_4K); // 用户进程的代码段描述符
    ldt[1] = make_seg_desc(0, 0xfffff, DA_32 | DA_DRW | DA_DPL3 | DA_LIMIT_4K);
    ldt[2] = make_seg_desc(0xb8000, 0xffff, DA_32 | DA_DRW | DA_DPL3);

    proc1.pid = 0;
    proc1.priority = 0;
    proc1.sel_ldt = gdt_push_back(gdt, make_ldt_desc((uint32_t)ldt, sizeof(ldt) - 1, DA_32 | DA_DPL0));

    proc1.registers.cs = (0 << 3) + SA_RPL3 + SA_TIL;
    proc1.registers.ds = proc1.registers.es = proc1.registers.ss = proc1.registers.fs = (1 << 3) + SA_RPL3 + SA_TIL;
    proc1.registers.gs = (2 << 3) + SA_RPL3 + SA_TIL;
    proc1.registers.eflags = 0x1202;

    proc1.registers.esp = (uint32_t)stack + 1024;
    proc1.registers.eip = (uint32_t)test;
    // *((uint32_t *)(stack + 1020)) = (uint32_t)proc_exit;
    proc2 = proc1;
    proc2.pid = 1;
    proc2.registers.esp = (uint32_t)stack2 + 1024;
    proc2.registers.eip = (uint32_t)test2;

    proc3 = proc1;
    proc3.pid = 2;
    proc3.registers.esp = (uint32_t)stack3 + 1024;
    proc3.registers.eip = (uint32_t)test3;

    current_proc = &proc1;
    dis_pos = 0;
    tss.ss0 = SELECTOR_KERNEL_DS;
    tss.esp0 = (void *)current_proc+72;
    sti();
    start_process(&proc1); // 启动进程
    // putdec(sizeof(process));
    hlt();

    shutdown();
}

private
void kernel_init()
{
    gdt_init();
    idt_init();
    ltr(SELECTOR_KERNEL_TSS);
}

private
void delay() NO_OPTIMIZE;

private
void delay()
{
    for (int i = 0; i < 300; i++)
    {
        for (int j = 0; j < 300; j++)
        {
        }
    }
}

private
int test()
{
    dis_color = FG_MAGENTA|HIGHLIGHT;
    puts("\nprocess A start!\n");
    for (;;)
    {
        dis_color = FG_MAGENTA|HIGHLIGHT;
        puts("A");
        delay();
    }
    return 0;
}
private
int test2()
{
    dis_color = FG_BLUE|HIGHLIGHT;
    puts("\nprocess B start!\n");
    for(;;)
    {
        dis_color = FG_BLUE|HIGHLIGHT;
        puts("B");
        delay();
    }
}

private
int test3()
{
    dis_color = FG_CYAN|HIGHLIGHT;
    puts("\nprocess C start!\n");
    for(;;)
    {
        dis_color = FG_CYAN|HIGHLIGHT;
        puts("C");
        delay();
    }
}
private
void proc_exit()
{
    puts("process exit with:");
}