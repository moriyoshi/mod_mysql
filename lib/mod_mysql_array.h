/*
 * mod_mysql
 *
 * Copyright (c) 2007 Moriyoshi Koizumi. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _MOD_MYSQL_ARRAY_H
#define _MOD_MYSQL_ARRAY_H

#include <apr_tables.h>
#include "mod_mysql_base.h"
#include "mod_mysql_list.h"
#include "mod_mysql_instantiable.h"

template<typename _T> class mod_mysql_array
        : public mod_mysql_base,
          public mod_mysql_instantiable,
          public mod_mysql_list<_T>
{
public:
    typedef _T element_t;

private:
    apr_array_header_t* impl;

protected:
    mod_mysql_array(apr_array_header_t* _impl);

public:
    mod_mysql_array(apr_pool_t* pool, int nelems = 0);

    mod_mysql_array(apr_pool_t* pool, const mod_mysql_array& that);

    mod_mysql_array(apr_pool_t* pool, const mod_mysql_array& first,
            const mod_mysql_array& second);

    mod_mysql_array clone();

    _T& push();
    _T& pop();
    mod_mysql_array concat(const mod_mysql_array& that);
    virtual int count() const;
    virtual void add(const _T& v);
    virtual int add(const mod_mysql_array& that);
    virtual void remove(int idx);
    virtual int remove(const _T& v);
    virtual int index_of(const _T& v);
    virtual int last_index_of(const _T& v);
    virtual _T& refer(int idx);
    mod_mysql_array operator+(const mod_mysql_array& that);
    operator _T*() const;
    _T& operator[](int idx);
    const _T& operator[](int idx) const;
};

// {{{ mod_mysql_array<>::mod_mysql_array
template<typename _T> inline mod_mysql_array<_T>::mod_mysql_array(
        apr_array_header_t* _impl)
        : impl(_impl)
{
}

template<typename _T> inline mod_mysql_array<_T>::mod_mysql_array(apr_pool_t* _pool,
        int nelems)
        : impl(apr_array_make(_pool, nelems, sizeof(_T)))
{
}

template<typename _T> inline mod_mysql_array<_T>::mod_mysql_array(apr_pool_t* _pool,
        const mod_mysql_array<_T>& that)
        : impl(apr_array_copy(_pool, that.impl))
{
}

template<typename _T> inline mod_mysql_array<_T>::mod_mysql_array(apr_pool_t* _pool,
        const mod_mysql_array<_T>& first, const mod_mysql_array<_T>& second)
        : impl(apr_array_append(_pool, first.impl, second.impl))
{
}
// }}}

// {{{ mod_mysql_array<>::clone
template<typename _T> inline mod_mysql_array<_T> mod_mysql_array<_T>::clone()
{
    return mod_mysql_array(impl->pool, *this);
}
// }}}

// {{{ mod_mysql_array<>::push
template<typename _T> inline _T& mod_mysql_array<_T>::push()
{
    return *reinterpret_cast<_T*>(apr_array_push(impl));
}
// }}}

// {{{ mod_mysql_array<>::pop
template<typename _T> inline _T& mod_mysql_array<_T>::pop()
{
    return *reinterpret_cast<_T*>(apr_array_pop(impl));
}
// }}}

// {{{ mod_mysql_array<>::count
template<typename _T> int mod_mysql_array<_T>::count() const
{
    return impl->nelts;
}
// }}}

// {{{ mod_mysql_array<>::add()
template<typename _T> void mod_mysql_array<_T>::add(const _T& elem)
{
    _T& new_elem = push();
    new_elem = elem;
}

template<typename _T>
int mod_mysql_array<_T>::add(const mod_mysql_array<_T>& that)
{
    apr_array_cat(impl, that.impl);
    return that.count();
}
// }}}

// {{{ mod_mysql_array<>::remove()
template<typename _T> void mod_mysql_array<_T>::remove(int idx)
{
    memmove(static_cast<_T*>(*this) + idx,
            static_cast<_T*>(*this) + idx + 1,
            impl->nelts - idx - 1);
    pop();
}

template<typename _T> int mod_mysql_array<_T>::remove(const _T& elem)
{
    int c = 0;
    const int nelts = count();

    for (int idx = nelts; --idx >= 0; c++) {
        if ((*this)[idx] == elem) {
            memmove(static_cast<_T*>(*this) + idx,
                    static_cast<_T*>(*this) + idx + 1,
                    nelts - idx - 1);
        }
    }

    for (int i = c; --i >= 0; ) {
        pop();
    }

    return c;
}
// }}}

// {{{ mod_mysql_array<>::concat()
template<typename _T>
inline mod_mysql_array<_T> mod_mysql_array<_T>::concat(
        const mod_mysql_array<_T>& that)
{
    return mod_mysql_array(impl->pool, *this, that);
}
// }}}

// {{{ mod_mysql_array<>::refer()
template<typename _T> inline _T& mod_mysql_array<_T>::refer(int idx)
{
    return reinterpret_cast<_T*>(impl->elts)[idx];
}
// }}}

// {{{ mod_mysql_array<>::index_of()
template<typename _T>
int mod_mysql_array<_T>::index_of(const _T& idx)
{
    const int nelts = count();

    for (int idx = 0; idx < nelts; idx++) {
        if ((*this)[idx] == idx) {
            return idx;
        }
    }

    return -1;
}
// }}}

// {{{ mod_mysql_array<>::last_index_of()
template<typename _T>
int mod_mysql_array<_T>::last_index_of(const _T& idx)
{
    for (int idx = count(); --idx >= 0; ) {
        if ((*this)[idx] == idx) {
            return idx;
        }
    }

    return -1;
}
// }}}

// {{{ mod_mysql_array<>::operator+
template<typename _T>
inline mod_mysql_array<_T> mod_mysql_array<_T>::operator+(
        const mod_mysql_array<_T>& that)
{
    return concat(that);
}
// }}}

// {{{ mod_mysql_array<>::operator _T*
template<typename _T> inline mod_mysql_array<_T>::operator _T*() const
{
    return reinterpret_cast<_T*>(impl->elts);
}
// }}}

// {{{ mod_mysql_array<>::operator []
template<typename _T> inline const _T& mod_mysql_array<_T>::operator [](int idx) const
{
    return reinterpret_cast<_T*>(impl->elts)[idx];
}
// }}}

#endif /* _MOD_MYSQL_ARRAY_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
