#ifndef QIUOS_GLOBAL_H_
#define QIUOS_GLOBAL_H_

#include "type.h"
#include "tss.h"
#include "proc.h"

extern uint64_t gdt[];
extern task_state_segment tss;
extern uint64_t idt[];
extern intr_stub intr_stubs[];
extern uint32_t volatile dis_pos = 2400; // 从第15行开始显示
extern uint8_t volatile dis_color = 0xf; // 默认颜色为白色高亮
extern uint64_t ldt[];
extern process volatile *current_proc;
extern process proc;


#endif // QIUOS_GLOBAL_H_