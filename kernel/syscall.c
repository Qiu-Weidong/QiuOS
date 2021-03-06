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

private 
size_t sys_write(filedesc_t fd, const void * buffer, size_t size)
{
    tty * p_tty = current_proc->p_tty;
    while(test_and_set(&p_tty->lock))
        ;
    for(int i=0;i<size;i++) screen_putc(&p_tty->csl,((char *)buffer)[i]);

    atomic_clear(&p_tty->lock);
    return size;
}
private
int sys_sendrec(int function, int src_dest, message * msg, process* p)
{
    sys_write(1, "sys_sendrec",11);
    return 0;
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
    case _NR_write:
        frame->eax = sys_write(frame->ebx, (const void *)frame->ecx, frame->edx);
        break;
    
    case _NR_sendrec:
        frame->eax = sys_sendrec(frame->ebx, frame->ecx, frame->edx,(process *)frame);
        break;
    default:
        break;
    }
}

