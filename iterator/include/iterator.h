#pragma once

typedef struct iterator iterator;
typedef struct iterator_vmt iterator_vmt;

typedef void (*set_FnPtr)(iterator *this, void *value);
typedef iterator *(*iter_FnPtr)(iterator *this);
typedef void *(*get_FnPtr)(iterator *this);
typedef void (*dtor_FnPtr)(iterator *this);

struct iterator {
        iterator_vmt *vmt;
};

void iterator_create(iterator *this, get_FnPtr get, set_FnPtr set,
                     iter_FnPtr next, iter_FnPtr prev, dtor_FnPtr dtor);
void iterator_destroy(iterator *this);

void iterator_set(iterator *this, void *value);
void *iterator_get(iterator *this);
iterator *iterator_next(iterator *this);
iterator *iterator_prev(iterator *this);
