#ifndef QIUOS_TYPE_H_
#define QIUOS_TYPE_H_

#define public
#define private static
#define NULL ((void *)0)

#define true 1
#define false 0

typedef unsigned char       uint8_t;
typedef signed char         int8_t;
typedef unsigned short      uint16_t;
typedef signed short        int16_t;
typedef unsigned int        uint32_t;
typedef signed int          int32_t;
typedef unsigned long long  uint64_t;
typedef signed long long    int64_t;

typedef unsigned int        bool_t;
typedef unsigned int        size_t;
typedef unsigned short      selector_t;
typedef unsigned int        pid_t;
typedef unsigned int        tid_t;

typedef	void (*intr_stub)	();
typedef int (*compare) (const void *,const void *);
typedef int (*process_entry) (int argc, char ** argv);

#endif // QIUOS_TYPE_H_