#include "type.h"
#include "const.h"
#include "asm.h"
#include "io.h"
#include "shutdown.h"
#include "panic.h"


uint32_t dis_pos = 2400;             // 从第15行开始显示
uint8_t  dis_color = 0xf;            // 默认颜色为白色高亮
uint64_t gdt[GDT_SIZE];              // gdt表
uint64_t idt[IDT_SIZE];              // idt表

int kernel_main()
{
    set_color(HIGHLIGHT|FG_YELLOW|BG_BLACK);
    puts("Welcome to QiuOS World!\n");
    // shutdown();
    assert(1==2);
    hlt();
}