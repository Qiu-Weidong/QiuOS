#ifndef QIUOS_TSS_H_
#define QIUOS_TSS_H_
#include "type.h"
#define TSS_MAX_SIZE 128
// task state segment(tss) 任务状态段
typedef struct 
{
    uint16_t back_link;                      // 上一个任务链接
    uint32_t esp0;                           // 0特权级的栈指针
    uint16_t ss0;                            // 0特权级的栈段描述符
    uint32_t esp1;
    uint16_t ss1;
    uint32_t esp2;
    uint16_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax, ecx, edx, ebx;
    uint32_t esp, ebp, esi, edi;
    uint16_t es;
    uint16_t cs;
    uint16_t ss;
    uint16_t ds;
    uint16_t fs;
    uint16_t gs;
    uint16_t ldt;
    uint16_t trace, bitmap;
}task_state_segment;

// public 
// void tss_init();


#endif // QIUOS_TSS_H_