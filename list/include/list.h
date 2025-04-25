#pragma once

#include <sys/types.h>

typedef struct list list;

list *_list_new(u_int64_t size, char *type);
#ifndef list_new
#define list_new(type) _list_new(sizeof(type), #type)
#endif
void list_delete(list *this);

u_int64_t list_len(list *this);
u_int64_t list_size(list *this);
const char *list_type(list *this);

void list_insert_at(list *this, const void *val, u_int64_t pos);
void list_insert_first(list *this, const void *val);
void list_insert_last(list *this, const void *val);

void *list_remove_at(list *this, u_int64_t pos);
void *list_remove_first(list *this);
void *list_remove_last(list *this);

void *list_peek_at(list *this, u_int64_t pos);
void *list_peek_first(list *this);
void *list_peek_last(list *this);

void list_set_at(list *this, const void *val, u_int64_t pos);
void list_set_first(list *this, const void *val);
void list_set_last(list *this, const void *val);
