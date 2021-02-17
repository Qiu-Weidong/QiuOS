#include "shutdown.h"
#include "asm.h"


public void shutdown()
{
    const char msg[] = "Shutdown";
    puts("Power off...\n");
    // ACPI power-off;
    out_word(0xb004,0x2000);
    
    for(const char * p = msg; *p != '\0'; p++)
        out_byte(0x8900, *p);
    
    out_byte(0x501, 0x31);
    cli();
    hlt();
    puts("Never Reach...\n");
    while (1){}
    
}

public void reboot()
{
    puts("Rebooting...\n");
    hlt();
}