#include "type.h"
#include "interrupt.h"
#include "asm.h"
#include "io.h"
#include "keyboard.h"

public
void keyboard_handler(const intr_frame * frame UNUSED)
{
    uint8_t scan_code = in_byte(0x60);
    // 读出扫描码，并放入活跃的tty的键盘缓冲区
}


