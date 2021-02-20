#ifndef QIUOS_PROCESS_H_
#define QIUOS_PROCESS_H_
#include "type.h"

#define PROCESS_MAX 128
typedef enum
{
    created,
    ready,
    running,
    blocked,
    dead 
}process_status;

// Processing Control Block
typedef struct 
{
    pid_t pid;                          // 进程id
    process_status status;                 // 状态
    uint32_t priority;                  // 优先级
    uint32_t exit_code;                 // 退出码
    selector_t tss_desc;                // tss描述符，页表、ldt、寄存器快照等信息都在其中
    
}process_control_block;

public 
pid_t process_create(process_entry pstart);

#endif // QIUOS_PROCESS_H_