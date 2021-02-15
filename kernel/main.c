#include "type.h"
#include "const.h"

u32 dis_pos = 2400;             // 从第15行开始显示
u8  dis_color = 0xf;            // 白色高亮

void setColor(u8 color) { dis_color = color; }
void putchar(int c);
int puts(const char * str);

int kernel_main()
{
    setColor(HIGHLIGHT|FG_MAGENTA|BG_BLUE);
    puts("Welcome to QiuOS World!\nWelcome to QiuOS World!");
    __asm__ volatile("hlt");
}