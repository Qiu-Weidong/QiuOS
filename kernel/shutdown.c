#include "shutdown.h"
#include "asm.h"


public 
void ACPI_power_off()
{
    // ACPI power-off;这条指令执行后直接就关机了
    out_word(0xb004, 0x2000);
}

public
void request_shutdown_port()
{
    // bochs和qemu的关机方式，但物理机不支持
    const char * s = "Shutdown";
    for(const char * p = s; *p!='\0';p++)
        out_byte(0x8900, *p);
    
}

public
void shutdown()
{
    puts("Power off...\n");

    ACPI_power_off();

    request_shutdown_port();
    
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

