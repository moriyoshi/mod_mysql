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
#ifndef _MOD_MYSQL_OPAQUE_DATA_H
#define _MOD_MYSQL_OPAQUE_DATA_H

#include "mod_mysql_referent.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MOD_MYSQL_OPAQUE_DATA_DEFINED
typedef struct mod_mysql_opaque_data_t mod_mysql_opaque_data_t;
#define MOD_MYSQL_OPAQUE_DATA_DEFINED
#endif /* MOD_MYSQL_OPAQUE_DATA_DEFINED */

typedef struct mod_mysql_opaque_data_vtbl_t
{
    mod_mysql_referent_vtbl_t _super;
    apr_status_t (*map_fn)(mod_mysql_opaque_data_t *,
            void **retval, apr_pool_t* pool);
    apr_status_t (*unmap_fn)(mod_mysql_opaque_data_t *, void *blk);
    apr_status_t (*get_size_fn)(mod_mysql_opaque_data_t *, size_t *retval);
} mod_mysql_opaque_data_vtbl_t;

struct mod_mysql_opaque_data_t
{
    mod_mysql_referent_t _super;
};

#define MOD_MYSQL_OPAQUE_DATA_VTBL(self) \
    ((mod_mysql_opaque_data_vtbl_t *) \
    ((mod_mysql_opaque_data_t *)(self))->_super._vtbl)

#define mod_mysql_opaque_data_map(self, retval, pool) \
    (MOD_MYSQL_OPAQUE_DATA_VTBL(self)->map_fn)(self, retval, pool)
#define mod_mysql_opaque_data_unmap(self, blk) \
    (MOD_MYSQL_OPAQUE_DATA_VTBL(self)->unmap_fn)(self, blk)
#define mod_mysql_opaque_data_get_size(self, retval) \
    (MOD_MYSQL_OPAQUE_DATA_VTBL(self)->get_size_fn)(self, retval)

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
#include "mod_mysql_base.h"
#include "mod_mysql_mapper.h"
#include "mod_mysql_memblock.h"
#include "mod_mysql_instantiable.h"

template<typename _T> class mod_mysql_opaque_data
        : public mod_mysql_base, public mod_mysql_mapper
{
protected:
    mod_mysql_opaque_data_t* dgt;

public:
    virtual void release(void* ptr);
    mod_mysql_opaque_data(mod_mysql_opaque_data_t* dgt);
    mod_mysql_memblock<_T> map(apr_pool_t* pool);
    size_t get_size();
    int count();
};

// {{{ mod_mysql_opaque_data<>::release()
template<typename _T>
inline void mod_mysql_opaque_data<_T>::release(void* ptr)
{
    mod_mysql_opaque_data_unmap(dgt, ptr);
}
// }}}

// {{{ mod_mysql_opaque_data<>::mod_mysql_opaque_data()
template<typename _T>
inline mod_mysql_opaque_data<_T>::mod_mysql_opaque_data(
        mod_mysql_opaque_data_t* _dgt)
        : dgt(_dgt)
{
}
// }}}

// {{{ mod_mysql_opaque_data<>::map()
template<typename _T>
inline mod_mysql_memblock<_T> mod_mysql_opaque_data<_T>::map(
        apr_pool_t* pool)
{
    void* blk;
    size_t sz;

    if (mod_mysql_opaque_data_get_size(dgt, &sz)) {
        throw mod_mysql_exception("???");
    }

    if (mod_mysql_opaque_data_map(dgt, &blk, pool)) {
        throw mod_mysql_exception("???");
    }

    return mod_mysql_memblock<_T>(this, mod_mysql_ranged_ref<_T>(
            reinterpret_cast<_T*>(blk), sz));
}
// }}}

// {{{ mod_mysql_opaque_data<>::get_size()
template<typename _T>
inline size_t mod_mysql_opaque_data<_T>::get_size()
{
    size_t sz;

    if (mod_mysql_opaque_data_get_size(dgt, &sz)) {
        throw mod_mysql_exception("???");
    }

    return sz;
}
// }}}

// {{{ mod_mysql_opaque_data<>::count()
template<typename _T>
inline int mod_mysql_opaque_data<_T>::count()
{
    size_t const sz = get_size();
    mod_mysql_assert(sz % sizeof(_T) == 0);
    return sz / sizeof(_T);
}
// }}}

#endif

#endif /* _MOD_MYSQL_OPAQUE_DATA_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
