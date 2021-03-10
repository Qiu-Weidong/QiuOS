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
#include "atomic.h"
#include "../include/syscall.h"
#include "../include/stdio.h"
#include "tty.h"

public uint32_t volatile dis_pos = 2400; // 从第15行开始显示
public uint8_t volatile dis_color = 0xf; // 默认颜色为白色高亮

public uint64_t gdt[GDT_SIZE];    // gdt表
public uint64_t idt[IDT_SIZE];    // idt表
public uint64_t ldt[3];
public process volatile *current_proc;
public int32_t k_reenter;           // 中断重入标志
public task_state_segment tss;

public uint8_t stack[16][1024];
public process tasks[16];
tty _tty;

private 
int usrprogA(int argc, char ** argv)
{
    // write(1, "hello world!",10);
    sendrec(64,32,nullptr);
    for(;;);
}


public
int kernel_main()
{
    gdt_init();
    idt_init();

    // console_init(&csl,0x2A00, 0x2600);

    dis_color = HIGHLIGHT | FG_YELLOW | BG_BLACK;

    puts("Welcome to Qiux World!\n");

    dis_color = HIGHLIGHT | FG_BLUE;
    
    ldt[0] = make_seg_desc(0, 0xfffff, DA_32 | DA_CR | DA_DPL3 | DA_LIMIT_4K); // 用户进程的代码段描述符
    ldt[1] = make_seg_desc(0, 0xfffff, DA_32 | DA_DRW | DA_DPL3 | DA_LIMIT_4K);
    ldt[2] = make_seg_desc(0xb8000, 0xffff, DA_32 | DA_DRW | DA_DPL3);

    process * proc = create_process(usrprogA);
    // create_process(usrprogB);
    // create_process(usrprogC);
    // create_process(usrprogD);
    
    tss.ss0 = SEL_KERNEL_DS;
    ltr(SEL_TSS);

    tty_init(&_tty);
    console_init(&_tty.csl,0x0, 0x4000);

    k_reenter = 0;

    start_process(proc);
    shutdown();
}
