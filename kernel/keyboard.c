#include "type.h"
#include "interrupt.h"
#include "asm.h"
#include "io.h"
#include "keyboard.h"
#include "tty.h"

private
keyboard_buffer kb_buf;

public
void keyboard_handler(const intr_frame *frame UNUSED)
{
    uint8_t scan_code = in_byte(0x60);
    // 读出扫描码，并放入活跃的tty的键盘缓冲区
    if (kb_buf.head != (kb_buf.tail+1) % KEYBOARD_BUFFER_SIZE)
    {
        kb_buf.buffer[kb_buf.tail] = scan_code;
        kb_buf.tail = (kb_buf.tail+1) % KEYBOARD_BUFFER_SIZE;
    }
}

public
void keyboard_init()
{
    kb_buf.head = kb_buf.tail = 0;
    enable_irq(1);
}
