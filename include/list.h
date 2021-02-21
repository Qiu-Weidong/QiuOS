#ifndef QIUOS_LIST_H_
#define QIUOS_LIST_H_
#include "type.h"

typedef struct s_list_elem
{
    struct s_list_elem *prex;
    struct s_list_elem *next;
} list_elem;

typedef struct
{
    list_elem head;
    list_elem tail;
} list;

#define list_entry(LIST_ELEM, STRUCT, MEMBER) \
    ((STRUCT *)((void *)&(LIST_ELEM) - __builtin_offsetof(STRUCT, MEMBER)))

public
void list_init(list * list);

public 
list_elem * list_begin(list * list);

public 
list_elem * list_end(list * list);

public 
list_elem * list_next(list * list);

public 
list_elem * list_rbegin(list * list);
public 
list_elem * list_rend(list * list);
public 
list_elem * list_prev(list * list);

public 
list_elem * list_head(list * list);
public 
list_elem * list_tail(list * list);

public 
list_elem * list_insert(list_elem * before, list_elem * elem);
public 
list_elem * list_push_back(list * list, list_elem * elem);
public 
list_elem * list_push_front(list * list, list_elem * elem);
public
list_elem * list_remove(list * list, list_elem * elem);

public 
list_elem * list_pop_front(list * list);
public 
list_elem * list_pop_back(list * list);
public 
size_t list_size(list * list);

#endif // QIUOS_LIST_H_