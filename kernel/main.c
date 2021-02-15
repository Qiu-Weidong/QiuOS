#include "type.h"
#include "const.h"
#include "proto.h"

u32 dis_pos = 2400;             // 从第15行开始显示
u8  dis_color = 0xf;            // 白色高亮
u64 gdt[GDT_SIZE];              // gdt表
u64 idt[IDT_SIZE];              // idt表

int kernel_main()
{
    setColor(HIGHLIGHT|FG_MAGENTA|BG_BLUE);
    puts("Welcome to QiuOS World!\nWelcome to QiuOS World!");
    // 重新设置gdt表
    // 设置idt表

    __asm__ volatile("hlt");
}