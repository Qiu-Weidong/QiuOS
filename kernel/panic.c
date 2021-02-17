#include "panic.h"
#include "io.h"
#include "asm.h"

public void panic(const char * msg)
{
    uint8_t old_color = set_color(HIGHLIGHT|FG_RED|BG_BLACK);
    puts("panic: ");
    set_color(HIGHLIGHT|FG_WHITE|BG_BLACK);
    puts(msg);
    set_color(old_color);
    hlt();
}

// public void assert(bool condition)
// {
//     if(!condition)
//         panic("assert fail!");
// }