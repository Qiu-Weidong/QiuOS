#include "shutdown.h"
#include "asm.h"

public
void shutdown()
{
    puts("Power off...\n");
    // ACPI power-off;这条指令执行后直接就关机了
    out_word(0xb004, 0x2000);

    // 如果关机失败
    puts("shutdown fail...");
    cli();
    hlt();
}

public
void reboot()
{
    puts("Rebooting...\n");
    hlt();
}