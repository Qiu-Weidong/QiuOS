#include "type.h"
#include "const.h"
#include "asm.h"
#include "io.h"
#include "shutdown.h"
#include "panic.h"
#include "proto.h"


uint32_t dis_pos = 2400;             // 从第15行开始显示
uint8_t  dis_color = 0xf;            // 默认颜色为白色高亮
uint64_t gdt[GDT_SIZE];              // gdt表
uint64_t idt[IDT_SIZE];              // idt表

int kernel_main()
{
    set_color(HIGHLIGHT|FG_YELLOW|BG_BLACK);
    init_kernel();
    puts("Welcome to QiuOS World!\n");
    if(!is_intr_on()) sti();
    int(32);
    hlt();
}