#pragma once

#include <sys/types.h>

typedef struct list list;

list *_list_new(u_int64_t size, char *type);
#ifndef list_new
#define list_new(type) _list_new(sizeof(type), #type)
#endif
void list_delete(list *this);

u_int64_t list_nelem(list *this);
u_int64_t list_size(list *this);

void list_add(list *this, u_int64_t pos, const void *val);
void *list_remove(list *this, u_int64_t pos);

void list_push_front(list *this, const void *val);
void *list_pop_front(list *this);
void list_push_back(list *this, const void *val);
void *list_pop_back(list *this);

void *list_set(list *this, u_int64_t pos, const void *val);
void *list_get(list *this, u_int64_t pos);

#ifndef list_get_first
#define list_get_first(this) list_get(this, 0)
#endif

#ifndef list_get_last
#define list_get_last(this) list_get(this, list_nelem(this) - 1)
#endif
