#ifndef QIUX_TTY_H_
#define QIUX_TTY_H_
#include "console.h"
#include "keyboard.h"
#include "atomic.h"

#define INPUT_BUFFER_SIZE 256

typedef struct s_tty
{
    console csl;                            // 控制台
    atomic_t lock;                          // 控制台锁
    char input_buffer[INPUT_BUFFER_SIZE];   // 输入缓冲区
}tty;

public 
void task_tty();

public 
void tty_init(tty * tty);


#endif // QIUX_TTY_H_