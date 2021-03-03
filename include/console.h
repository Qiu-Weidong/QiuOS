#ifndef QIUOS_CONSOLE_H_
#define QIUOS_CONSOLE_H_
#include "type.h"

// 显存一共32KB，8000=10 0000   00 0000 0000
// 每页80*25*2 = 4000B，共32*1024/4000 = 8.192页
// 3个终端，每个终端2.7页
// 每个终端占用内存为2.7*4000 = 10800B = 0x2A30 约为0x2A00
// 显存地址范围 0xb8000 ~ 0xbffff

/*
 * 因此，三个显存的起始地址和结束地址分别为
 * 
 * 起始地址     结束地址
 * 0xB8000      0xBAA00-1     0x2A00
 * 0xBAA00      0xBD400-1     0x2A00
 * 0xBD400      0xC0000-1     0x2C00
*/

typedef struct s_console
{
    uint32_t original_addr; // 占用的显存开始地址
    uint32_t v_mem_limit;   // 占用显存大小
    uint32_t current_start_addr; // 当前显示的开始地址
    uint32_t cursor;                // 当前光标位置    
} console;

#define SCREEN_WIDTH  80
#define SCREEN_HEIGHT 25
#define SCREEN_SIZE   4000

/*VGA相关端口*/
#define	CRTC_ADDR_REG	0x3D4	/* CRT Controller Registers - Addr Register */
#define	CRTC_DATA_REG	0x3D5	/* CRT Controller Registers - Data Register */
#define	START_ADDR_H	0xC	    /* reg index of video mem start addr (MSB) */
#define	START_ADDR_L	0xD	    /* reg index of video mem start addr (LSB) */
#define	CURSOR_H	    0xE	    /* reg index of cursor position (MSB) */
#define	CURSOR_L	    0xF	    /* reg index of cursor position (LSB) */
#define	V_MEM_BASE	    0xB8000	/* base of color video memory */
#define	V_MEM_SIZE	    0x8000	/* 32K: B8000H -> BFFFFH */

#endif // QIUOS_CONSOLE_H_