#ifndef QIUNIX_PROC_H_
#define QIUNIX_PROC_H_
#include "type.h"
#include "interrupt.h"
#include "tty.h"

typedef int (*process_func)(int, char **);
typedef unsigned int pid_t;
typedef unsigned int priority_t;

typedef struct process_control_block
{
    intr_frame registers; // 寄存器快照
    pid_t pid;            // 进程id 76
    selector_t sel_ldt;   // ldt的选择子 80
    priority_t priority;  // 优先级 84
    tty *p_tty;           // 指向终端的指针 88
} process;

public
void start_process(process *proc) NO_RETURN;

public
void restart_current_process() NO_RETURN;

public
void task_schedule(const intr_frame *frame UNUSED);

public
process *create_process(process_func proc_main);

#endif // QIUNIX_PROC_H_