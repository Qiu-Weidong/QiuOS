#include "console.h"
#include "asm.h"
#include "../include/string.h"

private
void set_cursor(unsigned int position)
{
    cli();
    out_byte(CRTC_ADDR_REG, CURSOR_H);
    out_byte(CRTC_DATA_REG, (position >> 8) & 0xff);
    out_byte(CRTC_ADDR_REG, CURSOR_L);
    out_byte(CRTC_DATA_REG, position & 0xff);
    sti();
}

private
void set_video_start_addr(uint32_t addr)
{
    cli();
    out_byte(CRTC_ADDR_REG, START_ADDR_H);
    out_byte(CRTC_DATA_REG, ((V_MEM_BASE + addr) >> 8) & 0xff);
    out_byte(CRTC_ADDR_REG, START_ADDR_L);
    out_byte(CRTC_DATA_REG, (V_MEM_BASE + addr) & 0xff);
    sti();
}

private
void flush(console *csl)
{
    set_cursor(csl->cursor);
    set_video_start_addr(csl->current_start_addr);
}

/*
 * 向上滚动n行
*/
public
void scroll_up(console *csl, int n)
{
    if(csl->current_start_addr >= SCREEN_WIDTH * n)
        csl->current_start_addr -= SCREEN_WIDTH * n;
    csl->current_start_addr = csl->current_start_addr > csl->original_addr ? csl->current_start_addr : csl->original_addr;
    flush(csl);
}

public
void scroll_down(console *csl, int n)
{
    csl->current_start_addr += SCREEN_WIDTH * n;
    csl->current_start_addr = csl->current_start_addr < csl->original_addr + csl->v_mem_limit ? csl->current_start_addr : csl->original_addr + csl->v_mem_limit;
    flush(csl);
}

public
void screen_clear(console *csl)
{
    // al ' ' ah 0x7f
    int v = ((' ' | 0xf00) << 16) | (' ' | 0xf00);
    memset((void *)(V_MEM_BASE + (csl->original_addr << 1)), v, csl->v_mem_limit << 1);
    csl->current_start_addr = csl->original_addr; // 从实际显存地址(current_start_addr-0xb8000)*2处开始显示
    csl->cursor = csl->current_start_addr;        // cursor从0xb8000开始算

    flush(csl);
}

/*
 * 不用考虑实际显存地址，而将显存看作一段0x4000的空间，从地址0开始。
 * 每打印一个字符光标增加1(注意不再是2)，每滚动一行current_start_addr改变80(注意不是160)
*/

public
void screen_putc(console *csl, char c)
{
    uint16_t *p_mem = (uint16_t *)(V_MEM_BASE + (csl->cursor << 1)); // 需要打印的位置

    if (c == '\n' && csl->cursor < csl->original_addr + csl->v_mem_limit - SCREEN_WIDTH)
    {
        // 直接将光标置为下一行开始
        csl->cursor = csl->cursor + SCREEN_WIDTH - csl->cursor % SCREEN_WIDTH;
    }
    else if (c == '\b' && csl->cursor > csl->original_addr)
    {

        p_mem--;
        *p_mem = (' ' | 0xf00);
        csl->cursor--;
    }
    else if (c != '\b' && c != '\n' && csl->cursor < csl->original_addr + csl->v_mem_limit - 1)
    {
        *p_mem = (c | 0xf00);
        csl->cursor++;
    }

    while (csl->cursor > SCREEN_SIZE)
        scroll_up(csl, 1);
    flush(csl);
}


public
void console_init(console *csl, uint32_t base, uint32_t limit)
{
    csl->current_start_addr = csl->cursor = csl->current_start_addr = base;
    csl->v_mem_limit = limit;
    screen_clear(csl);
    flush(csl);
}
