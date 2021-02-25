#ifndef QIUOS_PROC_H_
#define QIUOS_PROC_H_
#include "type.h"

typedef struct 
{
    uint32_t gs;    // 仅使用低16位 0
    uint32_t fs;    // 4
    uint32_t es;    // 8
    uint32_t ds;    // 12
    uint32_t edi;   // 16
    uint32_t esi;   // 20
    uint32_t ebp;   // 24
    uint32_t kesp;   // 28
    uint32_t ebx;   // 32
    uint32_t edx;   // 36
    uint32_t ecx;   // 40
    uint32_t eax;   // 44
    uint32_t retaddr;   // 48
    uint32_t eip;   // 52
    uint32_t cs;    // 56
    uint32_t eflags;    // 60
    uint32_t esp;   // 64
    uint32_t ss;    // 68 
}register_frame;

typedef unsigned int pid_t;
typedef unsigned int priority_t;
typedef struct 
{
    register_frame registers;   // 寄存器快照
    pid_t pid;                  // 进程id 72
    selector_t sel_ldt;         // ldt的选择子 76
    priority_t priority;        // 优先级 80
}process;



#endif // QIUOS_PROC_H_