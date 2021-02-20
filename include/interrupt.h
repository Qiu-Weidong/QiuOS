#ifndef QIUOS_INTERRUPT_H_
#define QIUOS_INTERRUPT_H_
#include "type.h"
#define IDT_SIZE 256


public 
void idt_init();

public
bool_t is_intr_on();

#endif // QIUOS_INTERRUPT_H_