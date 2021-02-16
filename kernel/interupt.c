/*
 * 中断相关处理函数
*/

#include "asm.h"
#include "const.h"
#include "proto.h"

extern u64 idt[IDT_SIZE];

PRIVATE void init_8259A()
{
    out_byte(INT_MASTER_CONTROLLER, 0x11);
    nop();
    nop();

    out_byte(INT_SLAVE_CONTROLLER, 0x11);
    nop();
    nop();

    out_byte(INT_MASTER_MASK, INT_VECTOR_IRQ0);
    nop();
    nop();

    out_byte(INT_SLAVE_MASK, INT_VECTOR_IRQ8);
    nop();
    nop();

    out_byte(INT_MASTER_MASK, 0x04);
    nop();
    nop(); 

    out_byte(INT_SLAVE_MASK, 0x02);
    nop();
    nop(); 

    out_byte(INT_MASTER_MASK, 0x01);
    nop();
    nop(); 

    out_byte(INT_SLAVE_MASK, 0x01);
    nop();
    nop(); 

    out_byte(INT_MASTER_MASK, 0xfc);
    nop();
    nop();

    out_byte(INT_SLAVE_MASK, 0xff);
}

PUBLIC void init_interupt_vector()
{
    // 先初始化8259A
    init_8259A();
}