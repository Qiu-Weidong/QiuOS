#ifndef QIUX_GLOBAL_H_
#define QIUX_GLOBAL_H_

#include "type.h"
#include "tss.h"
#include "proc.h"

extern uint32_t volatile dis_pos ; 
extern uint8_t volatile dis_color ; 
extern uint64_t gdt[];
extern uint64_t idt[];
extern uint64_t ldt[];
extern process volatile *current_proc;
extern task_state_segment tss;
extern uint8_t stack[][1024];
extern process tasks[];

extern tty _tty;
extern int32_t k_reenter;    


#endif // QIUX_GLOBAL_H_