#ifndef QIUOS_CONST_H_

#define QIUOS_CONST_H_

#define PUBLIC
#define PRIVATE static

#define GDT_SIZE 128
#define IDT_SIZE 256

// 显示颜色相关
// BL R G B I R G B
#define HIGHLIGHT   0x08
#define BLINK       0x80
#define BG_BLACK    0x00
#define BG_BLUE     0x10
#define BG_GREEN    0x20
#define BG_CYAN     0x30
#define BG_RED      0x40
#define BG_MAGENTA  0x50
#define BG_YELLOW   0x60
#define BG_WHITE    0x70
#define FG_BLACK    0x00
#define FG_BLUE     0x01
#define FG_GREEN    0x02
#define FG_CYAN     0x03
#define FG_RED      0x04
#define FG_MAGENTA  0x05
#define FG_YELLOW   0x06
#define FG_WHITE    0x07

// 8259A相关
#define INT_MASTER_CONTROLLER   0x20
#define INT_MASTER_MASK         0x21
#define INT_SLAVE_CONTROLLER    0xA0
#define INT_SLAVE_MASK          0xA1

#define INT_VECTOR_IRQ0         0x20
#define INT_VECTOR_IRQ8         0x28

// 保护模式相关
// 							    G	D/B	L	AVL		P	DPL	S	XEWA	XCRA
#define DA_32       0x4000  //	0	1	0	0		0	00	0	0000			D/B位为1，表示32位段
#define DA_LIMIT_4K 0x8000  //	1	0	0	0		0	00	0	0000			G位为1，表示段界限单位为4K

#define DA_DPL0     0x00    //	0	0	0	0		0	00	0	0000
#define DA_DPL1     0x20    //	0	0	0	0		0	01	0	0000
#define DA_DPL2     0x40    //	0	0	0	0		0	10	0	0000
#define DA_DPL3     0x60    //	0	0	0	0		0	11	0	0000

#define DA_DR       0x90    //	0	0	0	0		1	00	1	0000			TYPE都为0，只读数据段
#define DA_DRW      0x92    //	0	0	0	0		1	00	1	0010			W为1，读写数据段
#define DA_DRWA     0x93    //	0	0	0	0		1	00	1	0011			W为1，A为1，已访问读写数据段
#define DA_C        0x98    //	0	0	0	0		1	00	1			1000	X为1，代码段
#define DA_CR       0x9A    //	0	0	0	0		1	00	1			1010	X、R为1，可读代码段
#define DA_CCO      0x9C    //	0	0	0	0		1	00	1			1100	X、C为1，一致代码段
#define DA_CCOR     0x9E    //	0	0	0	0		1	00	1			1110	X、C、R为1，可读一致代码段

#define DA_LDT      0x82    //	0	0	0	0		1	00	0	0010			TYPE=2，LDT
#define DA_TaskGate 0x85    //	0	0	0	0		1	00	0	0101			TYPE=5，任务门
#define DA_386TSS   0x89    //	0	0	0	0		1	00	0	1001			TYPE=9，386TSS
#define DA_386CGate 0x8C    //	0	0	0	0		1	00	0	1100			TYPE=C，386调用门
#define DA_386IGate 0x8E    //	0	0	0	0		1	00	0	1110			TYPE=E，386中断们
#define DA_386TGate 0x8F    //	0	0	0	0		1	00	0	1111			TYPE=F，386陷阱门

#define SA_RPL0     0x0     //	0	00
#define SA_RPL1     0x1     //	0	01
#define SA_RPL2     0x2     //	0	10
#define SA_RPL3     0x3     //	0	11

#define SA_TIG      0x0     //	0	00
#define SA_TIL      0x4     //	1	00

#endif // QIUOS_CONST_H_
