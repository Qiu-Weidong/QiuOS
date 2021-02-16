#ifndef QIUOS_TYPE_H_
#define QIUOS_TYPE_H_


typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

// 段描述符
typedef struct 
{
    u32 base;   // 段基址
    u32 limit;  // 段界限，只用到低20位，高12位没有使用
    u16 attr;   // G D/B L AVL 0000 P DPL S type
}DESCRIPTOR;

// 门描述符
typedef struct 
{
    u8  dcount;
    u8  attr;  
    u16 selector;
    u32 offset;
}GATE;

typedef struct 
{
    u16 back_link;                      // 上一个任务链接
    u32 esp0;                           // 0特权级的栈指针
    u16 ss0;                            // 0特权级的栈段描述符
    u32 esp1;
    u16 ss1;
    u32 esp2;
    u16 ss2;
    u32 cr3;
    u32 eip;
    u32 eflags;
    u32 eax, ecx, edx, ebx;
    u32 esp, ebp, esi, edi;
    u16 es;
    u16 cs;
    u16 ss;
    u16 ds;
    u16 fs;
    u16 gs;
    u16 ldt;
    u16 trace, bitmap;
}TSS;

#endif // QIUOS_TYPE_H_