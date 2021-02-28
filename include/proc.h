#ifndef QIUOS_PROC_H_
#define QIUOS_PROC_H_
#include "type.h"
#include "interrupt.h"


typedef unsigned int pid_t;
typedef unsigned int priority_t;

typedef struct process_control_block
{
    intr_frame registers;       // 寄存器快照
    pid_t pid;                  // 进程id 76
    selector_t sel_ldt;         // ldt的选择子 80
    priority_t priority;        // 优先级 84
}process;



#endif // QIUOS_PROC_H_