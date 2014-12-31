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
#ifndef _MOD_MYSQL_LIST_H
#define _MOD_MYSQL_LIST_H

#include "mod_mysql_collection.h"
#include "mod_mysql_unsupported_exception.h"
#include "mod_mysql_illegal_state_exception.h"
#include "mod_mysql_list_iterator.h"

class mod_mysql_pool;

template<typename _T> class mod_mysql_list
        : public mod_mysql_collection<_T>
{
public:
    typedef mod_mysql_list<_T> this_t;
    typedef _T item_t;

public:
    virtual int count() const = 0;
    virtual int count(const item_t& v) const;
    virtual bool add(const item_t& v) = 0;
    virtual item_t& refer(int idx) = 0;
    virtual const item_t& refer(int idx) const = 0;
    virtual item_t get(int idx) const;
    virtual void set(int idx, const item_t& v);
    virtual int remove(int idx) = 0;
    virtual int remove(const item_t& v);
    virtual int index_of(const item_t& v) = 0;
    virtual int last_index_of(const item_t& v) = 0;
    virtual bool contains(const item_t& v);
    virtual mod_mysql_iterator<item_t>* create_iterator(
            mod_mysql_pool* pool) const;
    virtual mod_mysql_ordinal_iterator<item_t>* create_iterator(
            mod_mysql_pool* pool);
    item_t& operator[](int idx);
    const item_t& operator[](int idx) const;
};

// {{{ mod_mysql_list<>::count()
template<typename _T>
int mod_mysql_list<_T>::count(const _T& v) const
{
    int c = 0;

    for (mod_mysql_list_iterator<const this_t> i(*this); i.valid(); i.next()) {
        _T const& elem = i.current();
        if (elem == v)
            c++;
    }

    return c;
}
// }}}

// {{{ mod_mysql_list<>::remove()
template<typename _T>
int mod_mysql_list<_T>::remove(const _T& v)
{
    int c = 0;

    for (mod_mysql_list_iterator<const this_t> i(*this); i.valid(); i.next()) {
        if (i.current() == v) {
            i.remove();
            c++;
        }
    }

    return c;
}
// }}}

// {{{ mod_mysql_list<>::contains()
template<typename _T>
bool mod_mysql_list<_T>::contains(const _T& v)
{
    return index_of(v) >= 0;
}
// }}}

// {{{ mod_mysql_list<>::operator[]
template<typename _T>
inline typename mod_mysql_list<_T>::item_t&
mod_mysql_list<_T>::operator [](int idx)
{
    return refer(idx);
}

template<typename _T>
inline typename mod_mysql_list<_T>::item_t const&
mod_mysql_list<_T>::operator [](int idx) const
{
    return refer(idx);
}
// }}}

// {{{ mod_mysql_list<>::get()
template<typename _T>
typename mod_mysql_list<_T>::item_t
mod_mysql_list<_T>::get(int idx) const
{
    return refer(idx);
}
// }}}

// {{{ mod_mysql_list<>::set()
template<typename _T> void mod_mysql_list<_T>::set(int idx, const _T& v)
{
    refer(idx) = v;
}
// }}}

// {{{ mod_mysql_list<>::create_iterator()
template<typename _T>
mod_mysql_iterator<_T>* mod_mysql_list<_T>::create_iterator(
        mod_mysql_pool* pool) const
{
    return new(pool) mod_mysql_list_iterator<const this_t>(*this);
}

template<typename _T>
mod_mysql_ordinal_iterator<_T>* mod_mysql_list<_T>::create_iterator(
        mod_mysql_pool* pool)
{
    return new(pool) mod_mysql_list_iterator<this_t>(*this);
}
// }}}

#endif /* _MOD_MYSQL_LIST_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
