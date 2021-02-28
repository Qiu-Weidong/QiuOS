#include "shutdown.h"
#include "asm.h"

public
void shutdown()
{
    puts("Power off...\n");

    
    // ACPI power-off;这条指令执行后直接就关机了
    out_word(0xb004, 0x2000);

    // bochs和qemu的关机方式，但物理机不支持
    const char * s = "Shutdown";
    for(const char * p = s; *p!='\0';p++)
        out_byte(0x8900, *p);
    
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