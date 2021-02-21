#include "process.h"
#include "const.h"
#include "tss.h"

extern process_control_block processes[PROCESS_MAX]; // 进程控制块
extern task_state_segment tasks[TSS_MAX_SIZE];

public 
pid_t process_create(process_entry pstart)
{
    static uint32_t next_pid = 0;
    int pcb = -1;
    for(int i=0;i<PROCESS_MAX;i++) if(processes[i].status == dead) pcb = i;
    if(pcb == -1) return pcb;
    processes[pcb].pid = next_pid++;
    processes[pcb].status = created;
    processes[pcb].priority = 0;
    // 初始化tss
    tasks[pcb].back_link = 0;
    tasks[pcb].cs = 0x08;
    // tasks[pcb].
    return processes[pcb].pid;
}