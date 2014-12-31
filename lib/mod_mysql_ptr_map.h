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
#ifndef _MOD_MYSQL_PTR_MAP_H
#define _MOD_MYSQL_PTR_MAP_H

#include "mod_mysql_map.h"
#include "mod_mysql_instantiable.h"
#include "mod_mysql_base.h"
#include "mod_mysql_array.h"

template<typename _Telem> class mod_mysql_ptr_map
          : public mod_mysql_map<void*, _Telem>,
            public mod_mysql_instantiable,
            public mod_mysql_base
{
public:
    typedef size_t ptrval_t;

private:
    mod_mysql_pool* pool;
    mod_mysql_array<entry>** root[256];
    int cnt;

public:
    mod_mysql_ptr_map(mod_mysql_pool* pool);
    virtual ~mod_mysql_ptr_map();

    virtual void set(const void*& key, const _Telem& val);
    virtual _Telem* get(const void*& key);
    virtual void remove(const void*& key);
    virtual int count();
    virtual mod_mysql_iterator<entry>* create_iterator(mod_mysql_pool* p);
};

// {{{ mod_mysql_ptr_map<>::mod_mysql_ptr_map()
template<typename _Telem>
inline mod_mysql_ptr_map<_Telem>::mod_mysql_ptr_map(mod_mysql_pool* _pool)
        : pool(_pool), cnt(0)
{
    for (mod_mysql_array<entry>** p = root,
            l = root + sizeof(root) / sizeof(*root); p < l; p++) {
        *p = 0;
    }
}
// }}}

// {{{ mod_mysql_ptr_map<>::~mod_mysql_ptr_map()
template<typename _Telem>
inline mod_mysql_ptr_map<_Telem>::~mod_mysql_ptr_map()
{
    for (mod_mysql_array<entry>*** p1 = root,
            l1 = root + sizeof(root) / sizeof(*root); p1 < l1; p1++) {
        if (*p1) {
            for (mod_mysql_array<entry>** p2 = *p1, l2 = root + 256;
                    p2 < l2; p2++) {
                delete *p2;
            }
            delete mod_mysql_memblock<mod_mysql_array<entry>*>::
                    get_container(*p1);
        }
    }
}
// }}}

// {{{ mod_mysql_ptr_map<>::set()
template<typename _Telem>
inline void mod_mysql_ptr_map<_Telem>::set(const void*& key, const _Telem& val)
{
    ptrval_t ptr = reinterpret_cast<ptrval_t>(key);

    mod_mysql_array<entry>**& n = root[(ptr >> 24) & 0xff];

    if (!n)
        n = *pool->alloc_block<mod_mysql_array<_Telem>*>(256);

    mod_mysql_array<entry>*& m = n[(ptr >> 16) & 0xff];

    if (!m)
        m = new(pool) mod_mysql_array(*pool);

    for (int i = m->count(); --i >= 0; ) {
        if ((*m)[i].get_key() == key) {
            return;
        }
    }

    m->push(entry(key, val));
    cnt++;
}
// }}}

// {{{ mod_mysql_ptr_map<>::get()
template<typename _Telem>
inline _Telem* mod_mysql_ptr_map<_Telem>::get(const void*& key)
{
    ptrval_t ptr = reinterpret_cast<ptrval_t>(key);

    mod_mysql_array<_Telem>**& n = root[(ptr >> 24) & 0xff];

    if (!n)
        return 0;

    mod_mysql_array<_Telem>*& m = n[(ptr >> 16) & 0xff];

    if (!m)
        return 0;

    for (int i = m->count(); --i >= 0; ) {
        if ((*m)[i].get_key() == key) {
            return &(*m)[i].get_value();
        }
    }

    return 0;
}
// }}}

// {{{ mod_mysql_ptr_map<>::remove()
template<typename _Telem>
inline void mod_mysql_ptr_map<_Telem>::remove(const void*& key)
{
    ptrval_t ptr = reinterpret_cast<ptrval_t>(key);

    mod_mysql_array<_Telem>**& n = root[(ptr >> 24) & 0xff];

    if (!n)
        return 0;

    mod_mysql_array<_Telem>*& m = n[(ptr >> 16) & 0xff];

    if (!m)
        return 0;

    for (int i = m->count(); --i >= 0; ) {
        if ((*m)[i].get_key() == key) {
            m->remove(i);
            cnt--;
        }
    }
}
// }}}

// {{{ mod_mysql_ptr_map<>::count()
template<typename _Telem>
inline int mod_mysql_ptr_map<_Telem>::count()
{
    return cnt;
}
// }}}

// {{{ mod_mysql_ptr_map<>::create_iterator()
template<typename _Telem>
inline mod_mysql_iterator<entry>*
mod_mysql_ptr_map<_Telem>::create_iterator(mod_mysql_pool* p)
{
    throw mod_mysql_unsupported_exception("TBD");
}
// }}}

#endif /* _MOD_MYSQL_PTR_MAP_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
