#include "type.h"
#include "const.h"
#include "asm.h"
#include "io.h"

uint32_t dis_pos = 2400;             // 从第15行开始显示
uint8_t  dis_color = 0xf;            // 默认颜色为白色高亮
uint64_t gdt[GDT_SIZE];              // gdt表
uint64_t idt[IDT_SIZE];              // idt表

int kernel_main()
{
    dis_color = HIGHLIGHT|FG_MAGENTA|BG_BLACK;
    putchar('7');
    putchar('\n');
    putchar('&');
    putchar('\n');
    putdec(123);
    puthex(0x123fff);
    putoct(16);
    hlt();
}