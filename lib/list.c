#include "list.h"

public 
void list_init(list * list)
{
    if(list == NULL) return; // 暂时先这样，等assert完善后再修改
    list->head.next = &list->tail;
    list->head.prex = NULL;
    list->tail.next = NULL;
    list->tail.prex = &list->head;
}


