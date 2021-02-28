#ifndef QIUOS_PROC_H_
#define QIUOS_PROC_H_
#include "type.h"

typedef struct 
{
    // 段寄存器
    uint16_t gs,:16;    // 仅使用低16位 0
    uint16_t fs,:16;    // 4
    uint16_t es,:16;    // 8
    uint16_t ds,:16;    // 12

    // 寄存器
    uint32_t edi;   // 16
    uint32_t esi;   // 20
    uint32_t ebp;   // 24
    uint32_t esp_dummy;   // 28 pushad保存的esp，未使用
    uint32_t ebx;   // 32
    uint32_t edx;   // 36
    uint32_t ecx;   // 40
    uint32_t eax;   // 44

    uint32_t vec_no;    // 48
    uint32_t err_code;  // 52
    // CPU压栈
    uint32_t eip;   // 56
    uint16_t cs,:16;    // 60
    uint32_t eflags;    // 64
    uint32_t esp;   // 68
    uint16_t ss,:16;    // 72 
    
}register_frame;

typedef unsigned int pid_t;
typedef unsigned int priority_t;

typedef struct process_control_block
{
    register_frame registers;   // 寄存器快照
    pid_t pid;                  // 进程id 76
    selector_t sel_ldt;         // ldt的选择子 80
    priority_t priority;        // 优先级 84
}process;



#endif // QIUOS_PROC_H_