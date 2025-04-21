#pragma once

#include <sys/types.h>

typedef struct list list;

list *_list_new(u_int64_t size, char *type);
#ifndef list_new
#define list_new(type) _list_new(sizeof(type), #type)
#endif
void list_delete(list *this);

void list_add(list *this, const void *val);
void *list_remove(list *this, u_int64_t pos);

void list_push_front(list *this, const void *val);
void *list_pop_front(list *this);
void list_push_back(list *this, const void *val);
void *list_pop_back(list *this);

void *list_set(list *this, u_int64_t pos, const void *val);
void *list_get(list *this, u_int64_t pos);
