#include "type.h"
#include "const.h"
#include "proto.h"
#include "asm.h"

u32 dis_pos = 2400;             // 从第15行开始显示
u8  dis_color = 0xf;            // 默认颜色为白色高亮
u64 gdt[GDT_SIZE];              // gdt表
u64 idt[IDT_SIZE];              // idt表

int kernel_main()
{
    dis_color = HIGHLIGHT|FG_MAGENTA|BG_BLACK;
    puts("Welcome to QiuOS World!\n");
    init_kernel();
    hlt();
}