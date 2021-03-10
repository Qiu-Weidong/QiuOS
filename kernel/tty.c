#include "tty.h"
#include "io.h"
#include "global.h"
#include "../include/stdio.h"
// tty _tty;


public
void task_tty()
{
    keyboard_init();
    tty_init(&_tty);
    console_init(&_tty.csl,0x0, 0x4000);

    char msg[] = "Qiux";
    printf("hello %s world!\n",msg);
    
    while (true)
    {
        uint32_t key = keyboard_read();
        if(key == FLAG_NONE) continue;
        else if (!(key & FLAG_EXT))
            screen_putc(&_tty.csl, key);
        else
        {
            int raw_code = key & MASK_RAW;
            switch (raw_code)
            {
            case ENTER:
                screen_putc(&_tty.csl, '\n');
                break;
            case BACKSPACE:
                screen_putc(&_tty.csl, '\b');
                break;
            case UP:
                scroll_up(&_tty.csl, 1);
                break;
            case DOWN:
                scroll_down(&_tty.csl, 1);
                break;
            case F1:
            case F2:
            case F3:
            case F4:
            case F5:
            case F6:
            case F7:
            case F8:
            case F9:
            case F10:
            case F11:
            case F12:
                break;
            default:
                break;
            }
        }
    }
}

public
void tty_init(tty *tty)
{
    console_init(&tty->csl, 0x0, 0x4000);
    atomic_clear(&tty->lock);
}
