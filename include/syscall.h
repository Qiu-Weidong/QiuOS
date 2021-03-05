#ifndef QIUOS_SYSCALL_H_
#define QIUOS_SYSCALL_H_
#include "interrupt.h"

#define _NR_halt 0
#define _NR_exit 1
#define _NR_exec 2
#define _NR_wait 3
#define _NR_getpid 4
#define _NR_write  5

// 定义在sys-call.asm中，供用户使用
public
void halt(void);

public
void exit(int status);

public
pid_t getpid();

public
size_t write(filedesc_t,const void *,size_t);

public 
void syscall_handler(intr_frame * frame);

#endif // QIUOS_SYSCALL_H_