#include "../include/iterator.h"
#include <stdlib.h>

struct iterator_vmt {
        set_FnPtr _set;
        get_FnPtr _get;
        iter_FnPtr _next;
        iter_FnPtr _prev;
        dtor_FnPtr _dtor;
};

void iterator_create(iterator *this, get_FnPtr get, set_FnPtr set,
                     iter_FnPtr next, iter_FnPtr prev, dtor_FnPtr dtor)
{
        this->vmt = calloc(1, sizeof(*this->vmt));
        this->vmt->_get = get;
        this->vmt->_set = set;
        this->vmt->_next = next;
        this->vmt->_prev = prev;
        this->vmt->_dtor = dtor;
}

void iterator_destroy(iterator *this)
{
        iterator_vmt *vmt = this->vmt;
        vmt->_dtor(this);
        free(vmt);
}

void iterator_set(iterator *this, void *value)
{
        return this->vmt->_set(this, value);
}

void *iterator_get(iterator *this) { return this->vmt->_get(this); }

iterator *iterator_next(iterator *this) { return this->vmt->_next(this); }

iterator *iterator_prev(iterator *this) { return this->vmt->_prev(this); }
