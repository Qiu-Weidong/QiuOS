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

public uint32_t volatile dis_pos = 2400; // 从第15行开始显示
public uint8_t volatile dis_color = 0xf; // 默认颜色为白色高亮
public uint64_t gdt[GDT_SIZE];    // gdt表
public uint64_t idt[IDT_SIZE];    // idt表
public uint64_t ldt[3];
public process volatile *current_proc;
public task_state_segment tss;

public uint8_t stack[16][1024];
public process tasks[16];

private 
void delay() NO_OPTIMIZE;
private
void delay()
{
    for(int i=0;i<1000;i++)
        for(int j=0;j<1000;j++);
    
}

int volatile cnt = 0;
int a = 0,b=0,c=0,d=0;
private 
int usrprogA(int argc, char ** argv)
{
    dis_color = HIGHLIGHT | FG_CYAN;
    puts("process A start!\n");
    for(int i=0;i<100000;i++)
    {
        cli();
        cnt++;
        sti();
    }
    dis_color = HIGHLIGHT | FG_CYAN;
    puts("process A end!\n");
    a = 1;
    if(a && b && c && d) putdec(cnt);
    for(;;);
}

private 
int usrprogB(int argc, char ** argv)
{
    dis_color = HIGHLIGHT | FG_MAGENTA;
    puts("process B start!\n");
    for(int i=0;i<100000;i++)
    {
        cli();
        cnt++;
        sti();
    }
    dis_color = HIGHLIGHT | FG_MAGENTA;
    puts("process B end!\n");
    b = 1;
    if(a && b && c && d) putdec(cnt);
    for(;;);
}
private
int usrprogC(int argc, char ** argv)
{
    dis_color = HIGHLIGHT | FG_YELLOW;
    puts("process C start!\n");
    for(int i=0;i<100000;i++)
    {
        cli();
        cnt++;
        sti();
    }
    dis_color = HIGHLIGHT | FG_YELLOW;
    puts("process C end!\n");
    c = 1;
    if(a && b && c && d) putdec(cnt);
    for(;;);
}
private
int usrprogD(int argc, char ** argv)
{
    dis_color = HIGHLIGHT | FG_GREEN;
    puts("process D start!\n");
    for(int i=0;i<100000;i++)
    {
        cli();
        cnt++;
        sti();
    }
    dis_color = HIGHLIGHT | FG_GREEN;
    puts("process D end!\n");
    d = 1;
    if(a && b && c && d) putdec(cnt);
    for(;;);
}

public
int kernel_main()
{
    gdt_init();
    idt_init();

    dis_color = HIGHLIGHT | FG_YELLOW | BG_BLACK;

    puts("Welcome to QiuOS World!\n");

    dis_color = HIGHLIGHT | FG_BLUE;
    
    ldt[0] = make_seg_desc(0, 0xfffff, DA_32 | DA_CR | DA_DPL3 | DA_LIMIT_4K); // 用户进程的代码段描述符
    ldt[1] = make_seg_desc(0, 0xfffff, DA_32 | DA_DRW | DA_DPL3 | DA_LIMIT_4K);
    ldt[2] = make_seg_desc(0xb8000, 0xffff, DA_32 | DA_DRW | DA_DPL3);

    process * proc = create_process(usrprogA);
    create_process(usrprogB);
    create_process(usrprogC);
    create_process(usrprogD);
    
    tss.ss0 = SEL_KERNEL_DS;
    ltr(SEL_TSS);

    dis_pos = 0;
    for(int i=0;i<25;i++)        
        puts("                                                                        \n");
    dis_pos = 0;
    start_process(proc);
    shutdown();
}
