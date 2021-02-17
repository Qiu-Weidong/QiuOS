#ifndef QIUOS_EXCEPTION_H_
#define QIUOS_EXCEPTION_H_
#include "const.h"
#include "type.h"

public void panic(const char * msg);
// public void assert(bool condition);
#define assert(condition)                          \
    if(!(condition)){                               \
        panic(#condition);                          \
    }                                               

#endif // QIUOS_EXCEPTION_H_