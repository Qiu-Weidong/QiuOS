#include "type.h"
#include "const.h"
#include "io.h"
#include "shutdown.h"
#include "proto.h"
#include "gdt.h"
#include "interrupt.h"
#include "asm.h"
#include "tss.h"
#include "process.h"

uint32_t dis_pos = 2400; // 从第15行开始显示
uint8_t dis_color = 0xf; // 默认颜色为白色高亮
extern uint64_t gdt[GDT_SIZE];  // gdt表
extern uint64_t idt[IDT_SIZE];  // idt表
task_state_segment kernel_tss;  // 内核的任务状态段

task_state_segment tasks[TSS_MAX_SIZE]; // 内核执行进程的tss，该系统最多支持TSS_MAX_SIZE个进程同时运行
process_control_block processes[PROCESS_MAX]; // 进程控制块

private
void kernel_init();


int kernel_main()
{
    set_color(HIGHLIGHT | FG_YELLOW | BG_BLACK);
    kernel_init();
    puts("Welcome to QiuOS World!\n");
    set_color(HIGHLIGHT | FG_RED);
    // ud2();
    int3();
    shutdown();
}

private
void kernel_init()
{
    gdt_init();
    idt_init();
    // tss_init();
    // 加载tss
    ltr(SELECTOR_KERNEL_TSS);
}