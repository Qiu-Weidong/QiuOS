#include "type.h"
#include "const.h"
#include "io.h"
#include "shutdown.h"
#include "proto.h"
#include "gdt.h"
#include "interrupt.h"
#include "asm.h"
#include "tss.h"

uint32_t volatile dis_pos = 2400; // 从第15行开始显示
uint8_t volatile dis_color = 0xf; // 默认颜色为白色高亮
extern uint64_t gdt[GDT_SIZE];  // gdt表
extern uint64_t idt[IDT_SIZE];  // idt表

private
void kernel_init();


int kernel_main()
{
    set_color(HIGHLIGHT | FG_YELLOW | BG_BLACK);
    kernel_init();
    puts("Welcome to QiuOS World!\n");
    dis_color = HIGHLIGHT|FG_BLUE;
    // 长跳转，定义一个6字节的空间，偏移占前4个字节，选择子占后两个字节，最后ljmp跳转
    // *((uint32_t volatile *)fptr) = (uint32_t)dest_func;
    // *((uint16_t volatile *)(fptr+2)) = (5 << 3);
    // ljmp(fptr);
    // sti();
    // for (;;);
    hlt();
    
    shutdown();
}

private
void kernel_init()
{
    gdt_init();
    idt_init();

}

