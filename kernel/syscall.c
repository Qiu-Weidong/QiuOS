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
int sys_sendrec(int function, int src_dest, message * msg, process* p);

public 
void syscall_handler(intr_frame * frame)
{
    switch (frame->eax)
    {
    case _NR_halt:
        sys_halt();
        break;
    case _NR_exit:
        sys_exit(frame->ecx);
        break;
    case _NR_getpid:
        frame->eax = sys_getpid();
        break;
    case _NR_write:
        frame->eax = sys_write(frame->ecx, (const void *)frame->edx, frame->esi);
        break;
    
    case _NR_sendrec:
        frame->eax = sys_sendrec(frame->ecx, frame->edx, frame->esi,(process *)frame);
        break;
    default:
        break;
    }
}

void * va2la(pid_t pid, void *p) { return p ; }
/*
 * <Ring 0 >
*/
private
int sys_sendrec(int function, int src_dest, message * msg, process* p)
{
    assert(k_reenter == 0); // 当处于ring0发生中断时，k_reenter会大于0
    pid_t caller = p->pid;

    int ret = 0;
    message * mla = (message *) va2la(caller, msg);
    mla->source = caller;
    

}
