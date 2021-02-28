#ifndef QIUOS_TSS_H_
#define QIUOS_TSS_H_
#include "type.h"
#define TSS_MAX_SIZE 128
// task state segment(tss) 任务状态段
typedef struct s_task_state_segment
{
    uint16_t back_link,:16;                      // 上一个任务链接
    uint32_t esp0;                           // 0特权级的栈指针
    uint16_t ss0, :16;                            // 0特权级的栈段描述符
    uint32_t esp1;
    uint16_t ss1, :16;
    uint32_t esp2;
    uint16_t ss2, :16;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax, ecx, edx, ebx;
    uint32_t esp, ebp, esi, edi;
    uint16_t es, :16;
    uint16_t cs, :16;
    uint16_t ss, :16;
    uint16_t ds, :16;
    uint16_t fs, :16;
    uint16_t gs, :16;
    uint16_t ldt, :16;
    uint16_t trace, bitmap;
}task_state_segment;

struct s_eflag
{
    
};

#endif // QIUOS_TSS_H_