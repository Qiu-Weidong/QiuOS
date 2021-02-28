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

process proc1,proc2,proc3;
process * current_proc;
task_state_segment tss;

private
void kernel_init();

int kernel_main()
{
    set_color(HIGHLIGHT | FG_YELLOW | BG_BLACK);
    kernel_init();
    puts("Welcome to QiuOS World!\n");
    dis_color = HIGHLIGHT | FG_BLUE;
    ud2();
    shutdown();
}

private
void kernel_init()
{
    gdt_init();
    idt_init();
    ltr(SELECTOR_KERNEL_TSS);
}
