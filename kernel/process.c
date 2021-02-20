#include "process.h"
#include "const.h"

extern process_control_block processes[PROCESS_MAX]; // 进程控制块

public 
pid_t process_create(process_entry pstart)
{
    static uint32_t next_pid = 0;
    process_control_block * pcb = NULL;
    for(int i=0;i<PROCESS_MAX;i++)
        if(processes[i].status == dead) pcb = processes+i;
    pcb->pid = next_pid++;
    pcb->status = created;
    pcb->priority = 0;
    return pcb->pid;
}