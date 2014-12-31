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
#ifndef _MOD_MYSQL_COLLECTION_H
#define _MOD_MYSQL_COLLECTION_H

#include "mod_mysql_pool.h"
#include "mod_mysql_iterator.h"
#include "mod_mysql_unsupported_exception.h"

template<typename _T> class mod_mysql_collection
{
public:
    typedef _T item_t;
public:
    bool is_empty();
    virtual int count() const = 0;
    virtual int count(const _T& v) const = 0;
    virtual bool add(const _T& v) = 0;
    virtual int add(const mod_mysql_collection& that);
    virtual int remove(const _T& v) = 0;
    virtual int remove(const mod_mysql_collection& that);
    virtual bool contains(const _T& v) = 0;
    virtual bool contains(const mod_mysql_collection& that);
    virtual mod_mysql_iterator<_T>*
        create_iterator(mod_mysql_pool* p) const = 0;
    mod_mysql_collection& operator+=(const mod_mysql_collection& that);
    mod_mysql_collection& operator+=(const _T& v);
    mod_mysql_collection& operator-=(const mod_mysql_collection& that);
    mod_mysql_collection& operator-=(const _T& v);
    bool operator>(const mod_mysql_collection& v);
    bool operator>(const _T& v);
    bool operator<(const mod_mysql_collection& v);
    bool operator<(const _T& v);
};
// {{{ mod_mysql_collection<>::is_empty()
template<typename _T>
inline bool mod_mysql_collection<_T>::is_empty()
{
    return count() == 0;
}
// }}}

// {{{ mod_mysql_collection<>::add()
template<typename _T>
int mod_mysql_collection<_T>::add(const mod_mysql_collection<_T>& that)
{
    int c = 0;

    mod_mysql_pool* pool = mod_mysql_pool::create_root_pool();
    mod_mysql_iterator<_T>* i = that.create_iterator(pool);
    if (!i)
        throw mod_mysql_unsupported_exception("Given collection is not iterable");

    for (;i->valid(); i->next()) {
        c += add(i->current());
    }

    delete i;
    delete pool;

    return c;
}
// }}}

// {{{ mod_mysql_collection<>::remove()
template<typename _T>
int mod_mysql_collection<_T>::remove(const mod_mysql_collection<_T>& that)
{
    int c = 0;

    mod_mysql_pool* pool = mod_mysql_pool::create_root_pool();
    mod_mysql_iterator<_T>* i = that.create_iterator(pool);
    if (!i)
        throw mod_mysql_unsupported_exception("Given collection is not iterable");

    for (; i->valid(); i->next()) {
        c += remove(i->current());
    }

    delete i;
    delete pool;

    return c;
}
// }}}

// {{{ mod_mysql_collection<>::contains()
template<typename _T>
bool mod_mysql_collection<_T>::contains(const mod_mysql_collection<_T>& that)
{
    bool retval = true;

    mod_mysql_pool* pool = mod_mysql_pool::create_root_pool();
    mod_mysql_iterator<_T>* i = that.create_iterator(pool);

    for (; i->valid(); i->next()) {
        if (!contains(i->current())) {
            retval = false;
            break;
        }
    }

    delete i;
    delete pool;

    return retval;
}
// }}}

// {{{ mod_mysql_collection<>::operator +=()
template<typename _T>
inline mod_mysql_collection<_T>& mod_mysql_collection<_T>::operator+=(
        const mod_mysql_collection<_T>& that)
{
    add(that);
    return *this;
}

template<typename _T>
inline mod_mysql_collection<_T>& mod_mysql_collection<_T>::operator+=(
        const _T& v)
{
    add(v);
    return *this;
}
// }}}

// {{{ mod_mysql_collection<>::operator -=()
template<typename _T>
inline mod_mysql_collection<_T>& mod_mysql_collection<_T>::operator-=(
        const mod_mysql_collection<_T>& that)
{
    remove(that);
    return *this;
}

template<typename _T>
inline mod_mysql_collection<_T>& mod_mysql_collection<_T>::operator-=(
        const _T& v)
{
    remove(v);
    return *this;
}
// }}}

// {{{ mod_mysql_collection<>::operator >()
template<typename _T> inline bool mod_mysql_collection<_T>::operator>(
        const mod_mysql_collection& that)
{
    return contains(that);
}

template<typename _T> inline bool mod_mysql_collection<_T>::operator>(
        const _T& v)
{
    return contains(v);
}
// }}}

// {{{ mod_mysql_collection<>::operator <()
template<typename _T> inline bool mod_mysql_collection<_T>::operator<(
        const mod_mysql_collection& that)
{
    return !contains(that);
}

template<typename _T> inline bool mod_mysql_collection<_T>::operator<(
        const _T& v)
{
    return !contains(v);
}
// }}}

#endif /* _MOD_MYSQL_COLLECTION_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
