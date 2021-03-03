#include "../include/syscall.h"
#include "shutdown.h"
#include "global.h"

private 
void sys_halt(void)
{
    shutdown();
}

private
void sys_exit(int status)
{
    for(;;);
}

private
pid_t sys_getpid()
{
    return current_proc->pid;
}

public 
void syscall_handler(intr_frame * frame)
{
    switch (frame->eax)
    {
    case _NR_halt:
        sys_halt();
        break;
    case _NR_exit:
        sys_exit(frame->ebx);
        break;
    case _NR_getpid:
        frame->eax = sys_getpid();
        break;
    default:
        break;
    }
}

