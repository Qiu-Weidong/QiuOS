#ifndef QIUOS_TTY_H_
#define QIUOS_TTY_H_
#include "console.h"
#include "keyboard.h"

#define INPUT_BUFFER_SIZE 256

typedef struct s_tty
{
    char input_buffer[INPUT_BUFFER_SIZE];   // 输入缓冲区
    console * p_console;                    // 控制台
}tty;

public 
void task_tty();

public 
void tty_init();


#endif // QIUOS_TTY_H_