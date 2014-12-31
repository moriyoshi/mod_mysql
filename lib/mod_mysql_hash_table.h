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
#ifndef _MOD_MYSQL_HASH_TABLE_H
#define _MOD_MYSQL_HASH_TABLE_H

#include <apr.h>
#include <apr_hash.h>

#include "mod_mysql_base.h"
#include "mod_mysql_pool.h"
#include "mod_mysql_map.h"
#include "mod_mysql_ranged_ref.h"
#include "mod_mysql_illegal_state_exception.h"

template <typename _Tkey, typename _Telem>
class mod_mysql_hash_table;

template <typename _Tkey, typename _Telem> class mod_mysql_hash_table
        : public mod_mysql_base
{
public:
    class entry
            : public mod_mysql_pair<const mod_mysql_ranged_ref<_Tkey>, _Telem>,
              public mod_mysql_base
    {
    protected:
        friend class mod_mysql_hash_table;
        mod_mysql_ranged_ref<_Tkey> key;
        _Telem value;
    public:
        virtual const mod_mysql_ranged_ref<_Tkey>& get_first();
        virtual _Telem& get_last();
    };

    class iterator
            : public mod_mysql_iterator<entry&>,
              public mod_mysql_base
    {
    private:
        apr_hash_t* impl;
        apr_hash_index_t* idx;
        bool removed;
        entry cur;

    public:
        const mod_mysql_pool* pool;

    public:
        iterator(mod_mysql_pool* pool, apr_hash_t* impl);
        virtual entry& current();
        virtual bool valid();
        virtual void next();
        virtual void remove();
    };

private:
    apr_hash_t* impl;

public:
    mod_mysql_hash_table(mod_mysql_pool* pool,
            apr_hashfunc_t _hash_func = apr_hashfunc_default);
    mod_mysql_hash_table(const mod_mysql_hash_table& that);
    mod_mysql_hash_table(mod_mysql_pool* pool, const mod_mysql_hash_table& that);
    void set(const _Tkey* key, apr_ssize_t key_len, const _Telem& val);
    void set(const _Tkey& key, const _Telem& val);
    _Telem get(const _Tkey* key, apr_ssize_t key_len) const;
    _Telem get(const _Tkey& key);
    virtual int count() const;
    mod_mysql_iterator<entry>* create_iterator(mod_mysql_pool* pool);
    _Telem operator[](const _Tkey& key) const;
};

// {{{ mod_mysql_hash_table<>::entry::get_first()
template <typename _Tkey, typename _Telem>
const mod_mysql_ranged_ref<_Tkey>&
mod_mysql_hash_table<_Tkey, _Telem>::entry::get_first()
{
    return key;
}
// }}}

// {{{ mod_mysql_hash_table<>::entry::get_last()
template <typename _Tkey, typename _Telem>
_Telem& mod_mysql_hash_table<_Tkey, _Telem>::entry::get_last()
{
    return value;
}
// }}}

// {{{ mod_mysql_hash_table<>::iterator::iterator()
template <typename _Tkey, typename _Telem>
inline mod_mysql_hash_table<_Tkey, _Telem>::iterator::iterator(
        mod_mysql_pool* _pool, apr_hash_t* _impl)
        : pool(_pool), impl(_impl), idx(apr_hash_first(*pool, impl)),
          removed(false)
{
}
// }}}

// {{{ mod_mysql_hash_table<>::iterator::current()
template <typename _Tkey, typename _Telem>
typename mod_mysql_hash_table<_Tkey, _Telem>::entry&
mod_mysql_hash_table<_Tkey, _Telem>::iterator::current()
{
    if (!idx)
        throw mod_mysql_illegal_state_exception("The last element is already enumerated");

    if (removed)
        throw mod_mysql_illegal_state_exception("Element is already removed");

    apr_ssize_t sz;
    apr_hash_this(idx,
            const_cast<const void**>(
                    reinterpret_cast<void**>(&cur.key.iov_base)), &sz,
            reinterpret_cast<void**>(&cur.value));
    cur.key.iov_len = sz;
    return cur;
}
// }}}

// {{{ mod_mysql_hash_table<>::iterator::valid()
template <typename _Tkey, typename _Telem>
bool mod_mysql_hash_table<_Tkey, _Telem>::iterator::valid()
{
    return static_cast<bool>(idx);
}
// }}}

// {{{ mod_mysql_hash_table<>::iterator::next()
template <typename _Tkey, typename _Telem>
void mod_mysql_hash_table<_Tkey, _Telem>::iterator::next()
{
    if (idx) {
        idx = apr_hash_next(idx);
        removed = false;
    }
}
// }}}

// {{{ mod_mysql_hash_table<>::iterator::remove()
template <typename _Tkey, typename _Telem>
void mod_mysql_hash_table<_Tkey, _Telem>::iterator::remove()
{
    if (!idx)
        return false;

    if (removed)
        throw mod_mysql_illegal_state_exception("Element is already removed");

    const _Tkey* key;
    apr_ssize_t sz;
    apr_hash_this(idx,
            const_cast<const void**>(reinterpret_cast<void**>(&key)), &sz,
            NULL);
    apr_hash_set(impl, key, NULL);

    removed = true;
}
// }}}

// {{{ mod_mysql_hash_table<>::mod_mysql_hash_table()
template <typename _Tkey, typename _Telem>
inline mod_mysql_hash_table<_Tkey, _Telem>::mod_mysql_hash_table(
        mod_mysql_pool *pool, apr_hashfunc_t _hash_func)
        : impl(apr_hash_make_custom(*pool, _hash_func))
{
}

template <typename _Tkey, typename _Telem>
inline mod_mysql_hash_table<_Tkey, _Telem>::mod_mysql_hash_table(
        const mod_mysql_hash_table<_Tkey, _Telem>& that)
        : impl(apr_hash_copy(apr_hash_pool_get(that->impl), that.impl))
{
}

template <typename _Tkey, typename _Telem>
inline mod_mysql_hash_table<_Tkey, _Telem>::mod_mysql_hash_table(
        mod_mysql_pool* _pool, const mod_mysql_hash_table<_Tkey, _Telem>& that)
        : impl(apr_hash_copy(*_pool, that.impl))
{
}
// }}}

// {{{ mod_mysql_hash_table<>::set()
template <typename _Tkey, typename _Telem>
inline void mod_mysql_hash_table<_Tkey, _Telem>::set(const _Tkey* key,
        apr_ssize_t key_len, const _Telem& val)
{
    apr_hash_set(impl, key, sizeof(_Tkey) * key_len,
            reinterpret_cast<void*>(val));
}

template <typename _Tkey, typename _Telem>
inline void mod_mysql_hash_table<_Tkey, _Telem>::set(const _Tkey& key,
        const _Telem& val)
{
    apr_hash_set(impl, &key, sizeof(_Tkey), val);
}
// }}}

// {{{ mod_mysql_hash_table<>::get()
template <typename _Tkey, typename _Telem>
inline _Telem mod_mysql_hash_table<_Tkey, _Telem>::get(const _Tkey* key,
        apr_ssize_t key_len) const
{
    return reinterpret_cast<_Telem>(
            apr_hash_get(impl, key, sizeof(_Tkey) * key_len));
}

template <typename _Tkey, typename _Telem>
inline _Telem mod_mysql_hash_table<_Tkey, _Telem>::get(const _Tkey& key)
{
    return reinterpret_cast<_Telem>(
            apr_hash_get(impl, &key, sizeof(_Tkey)));
}
// }}}

// {{{ mod_mysql_hash_table<>::count()
template <typename _Tkey, typename _Telem>
int mod_mysql_hash_table<_Tkey, _Telem>::count() const
{
    return static_cast<int>(apr_hash_count(const_cast<apr_hash_t*>(impl)));
}
// }}}

// {{{ mod_mysql_hash_table<>::create_iterator
template <typename _Tkey, typename _Telem>
mod_mysql_iterator<typename mod_mysql_hash_table<_Tkey, _Telem>::entry>*
mod_mysql_hash_table<_Tkey, _Telem>::create_iterator(mod_mysql_pool* pool)
{
    return new(*pool) iterator(pool, impl);
}
// }}}
    
// {{{ mod_mysql_hash_table<>::operator[]
template <typename _Tkey, typename _Telem>
inline _Telem mod_mysql_hash_table<_Tkey, _Telem>::operator[](
        const _Tkey& key) const
{
    return get(key);
}
// }}}

#endif /* _MOD_MYSQL_HASH_TABLE_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
