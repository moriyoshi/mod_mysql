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
#ifndef _MOD_MYSQL_ARRAY_LIST_H
#define _MOD_MYSQL_ARRAY_LIST_H

#include "mod_mysql_base.h"
#include "mod_mysql_array.h"
#include "mod_mysql_iterator.h"

template<typename _Telem> class mod_mysql_array_list: public mod_mysql_base
{
public:
    class iterator: public mod_mysql_iterator<_Telem> {
    public:
        typedef _Telem item_t;

    private:
        int cnt;
        int idx;
        const item_t* array;

    public:
        iterator(const item_t* array = 0, int cnt = 0);

        virtual bool current(item_t& item);

        virtual void next();
    };

private:
    mod_mysql_array<_Telem> array;

public:
    mod_mysql_array_list(apr_pool_t* pool);

    mod_mysql_array_list(const mod_mysql_array_list& that);

    mod_mysql_array_list(apr_pool_t* pool, const mod_mysql_array_list& that);

    int count() const;

    void add(const _Telem& elem);

    void add(int idx, const _Telem& elem);

    const _Telem& get(int idx) const;

    _Telem set(int idx, const _Telem& elem);

    _Telem remove(int idx);

    bool remove(const _Telem& elem);

    int index_of(const _Telem& elem) const;

    int last_index_of(const _Telem& elem) const;

    iterator create_iterator() const;

    _Telem& operator[](int idx);

    const _Telem& operator[](int idx) const;

    operator mod_mysql_array<_Telem>() const;
};

// {{{ template<> inline mod_mysql_array_list<>::mod_mysql_array_list
template<typename _Telem> inline mod_mysql_array_list<_Telem>::mod_mysql_array_list(
        apr_pool_t* _pool): array(_pool)
{
}

template<typename _Telem> inline mod_mysql_array_list<_Telem>::mod_mysql_array_list(
        const mod_mysql_array_list& that): array(that.array)
{
}

template<typename _Telem> inline mod_mysql_array_list<_Telem>::mod_mysql_array_list(
        apr_pool_t* _pool, const mod_mysql_array_list& that)
        : array(_pool, that.array)
{
}
// }}}

// {{{ template<> inline int mod_mysql_array_list<>::count()
template<typename _Telem> inline int mod_mysql_array_list<_Telem>::count() const
{
    return array.count();
}
// }}}

// {{{ template<> inline void mod_mysql_array_list<>::add
template<typename _Telem> inline void mod_mysql_array_list<_Telem>::add(
        const _Telem& elem)
{
    array.push() = elem;
}

template<typename _Telem> inline void mod_mysql_array_list<_Telem>::add(int idx,
        const _Telem& elem)
{
    int cnt = array.count();

    array.push();

    _Telem* off = &static_cast<_Telem*>(array)[idx];

    memmove(off + 1, off, (cnt - idx) * sizeof(_Telem));

    *off = elem;
}
// }}}

// {{{ template<> inline _Telem mod_mysql_array_list<>::index_of
template<typename _Telem> inline int mod_mysql_array_list<_Telem>::index_of(
        const _Telem& elem) const
{
    int cnt = array.count();

    for (int i = 0; i < cnt; i++) {
        if (elem == array[i]) {
            return i;
        }
    }

    return cnt;
}
// }}}

// {{{ template<> inline _Telem mod_mysql_array_list<>::last_index_of
template<typename _Telem> inline int mod_mysql_array_list<_Telem>::last_index_of(
        const _Telem& elem) const
{
    for (int i = array.count(); --i >= 0;) {
        if (elem == array[i]) {
            return i;
        }
    }

    return -1;
}
// }}}

// {{{ template<> inline _Telem mod_mysql_array_list<>::remove
template<typename _Telem> inline _Telem mod_mysql_array_list<_Telem>::remove(int idx)
{
    int cnt = array.count() - 1;
    _Telem* off = &static_cast<_Telem*>(array)[idx];
    _Telem retval = *off;

    memmove(off, off + 1, (cnt - idx) * sizeof(_Telem));

    array.pop();

    return retval;
}

template<typename _Telem> inline bool mod_mysql_array_list<_Telem>::remove(
        const _Telem& elem)
{
    int idx = index_of(elem);

    if (idx == array.count()) {
        return false;
    }

    remove(idx);

    return true;
}
// }}}

//  {{{ template<> inline _Telem& mod_mysql_array_list<>::get
template<typename _Telem> inline const _Telem& mod_mysql_array_list<_Telem>::get(int idx) const
{
    return array[idx];
}
// }}}

// {{{ template <> inline _Telem mod_mysql_array_list<>::set
template <typename _Telem> inline _Telem mod_mysql_array_list<_Telem>::set(
        int idx, const _Telem& elem)
{
    _Telem retval = array[idx];
    array[idx] = elem;

    return retval;
}
// }}}

// {{{ template <> inline mod_mysql_array_list<_Telem>::iterator mod_mysql_array_list<>::create_iterator
template <typename _Telem> inline typename mod_mysql_array_list<_Telem>::iterator
mod_mysql_array_list<_Telem>::create_iterator() const
{
    return iterator(array, array.count());
}
// }}}

// {{{ template <> _Telem& mod_mysql_array_list<>::operator[]
template <typename _Telem> inline _Telem& mod_mysql_array_list<_Telem>::operator[](
        int idx)
{
    return array[idx];
}
// }}}

// {{{ template <> const _Telem& mod_mysql_array_list<>::operator[] const
template <typename _Telem> inline const _Telem& mod_mysql_array_list<_Telem>::operator[] (
        int idx) const
{
    return get(idx);
}
// }}}

// {{{ template <> mod_mysql_array_list<>::operator mod_mysql_array<>
template <typename _Telem> inline mod_mysql_array_list<_Telem>::operator mod_mysql_array<_Telem>() const
{
    return array;
}
// }}}

// {{{ template <> mod_mysql_array_list<>::iterator::iterator
template <typename _Telem> inline mod_mysql_array_list<_Telem>::iterator::iterator(
        const item_t* _array, int _cnt)
        : array(_array), cnt(_cnt), idx(0)
{
}
// }}}

// {{{ template <> bool mod_mysql_array_list<>::iterator::current
template <typename _Telem> inline bool mod_mysql_array_list<_Telem>::iterator::current(
        item_t& item)
{
    if (idx < 0 || idx >= cnt) {
        return false;
    }

    item = array[idx];

    return true;
}
// }}}

// {{{ template <> bool mod_mysql_array_list<>::iterator::current
template <typename _Telem> inline void mod_mysql_array_list<_Telem>::iterator::next()
{
    if (++idx >= cnt) {
        idx = -1;
    }
}
// }}}

#endif /* _MOD_MYSQL_ARRAY_LIST_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
