#ifndef QIUOS_KEYBOARD_H_
#define QIUOS_KEYBOARD_H_
#include "type.h"

#define KEYBOARD_BUFFER_SIZE 16

typedef struct s_keyboard_buffer
{

    uint32_t head;
    uint32_t tail;
    uint8_t buffer[KEYBOARD_BUFFER_SIZE];

}keyboard_buffer;

public 
void keyboard_init();

public
void keyboard_handler(const intr_frame *frame UNUSED);

#endif // QIUOS_KEYBOARD_H_