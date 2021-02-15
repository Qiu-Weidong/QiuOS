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
    u16 base;
    u32 offset;
}GATE;


#endif // QIUOS_TYPE_H_