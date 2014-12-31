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
#ifndef _MOD_MYSQL_ROW_SET_H
#define _MOD_MYSQL_ROW_SET_H

#include "mod_mysql_base.h"
#include "mod_mysql_list.h"
#include "mod_mysql_iterator.h"
#include "mod_mysql_instantiable.h"
#include "mod_mysql_fields.h"
#include "mod_mysql_row.h"

#ifndef MOD_MYSQL_ROW_SET_T_DEFINED
typedef struct mod_mysql_row_set_t mod_mysql_row_set_t;
#define MOD_MYSQL_ROW_SET_T_DEFINED
#endif /* MOD_MYSQL_ROW_SET_T_DEFINED */

#ifndef MOD_MYSQL_ROW_ITERATOR_T_DEFINED
typedef struct mod_mysql_row_iterator_t mod_mysql_row_iterator_t;
#define MOD_MYSQL_ROW_ITERATOR_T_DEFINED
#endif /* MOD_MYSQL_ROW_ITERATOR_T_DEFINED */

typedef struct mod_mysql_row_set_vtbl_t
{
    mod_mysql_fields_t *(*get_fields)(mod_mysql_row_set_t *);
    int (*count_fn)(mod_mysql_row_set_t *);
    mod_mysql_row_iterator_t *(*create_iterator_fn)(
            mod_mysql_row_set_t *, apr_pool_t *pool);
} mod_mysql_row_set_vtbl_t;

struct mod_mysql_row_set_t
{
    mod_mysql_row_set_vtbl_t *_vtbl;
};

#define mod_mysql_row_set_get_fields(self) \
        (*((mod_mysql_row_set_t *)(self))->_vtbl->get_fields)(self)
#define mod_mysql_row_set_count(self) \
        (*((mod_mysql_row_set_t *)(self))->_vtbl->count)(self)
#define mod_mysql_row_set_create_iterator(self, pool) \
        (*((mod_mysql_row_set_t *)(self))->_vtbl->create_iterator)(self, pool)

#ifdef __cplusplus

class mod_mysql_row;

class mod_mysql_row_set
        : public mod_mysql_base,
          public mod_mysql_instantiable
{
private:
    mod_mysql_row_set_t* dgt;
    mod_mysql_fields* fields;
    mod_mysql_pool* pool;

public:
    mod_mysql_row_set(mod_mysql_pool* pool, mod_mysql_row_set_t* dgt);
    mod_mysql_fields* get_fields(); 
    int count();
    virtual mod_mysql_iterator<mod_mysql_row*>* create_iterator(
            mod_mysql_pool* pool) const;
};

// {{{ mod_mysql_row_set::mod_mysql_row_set()
inline mod_mysql_row_set::mod_mysql_row_set(mod_mysql_pool* _pool,
        mod_mysql_row_set_t* _dgt)
        : pool(_pool), dgt(_dgt)
{
}
// }}}

// {{{ mod_mysql_row_set::get_fields()
inline mod_mysql_fields* mod_mysql_row_set::get_fields()
{
    return new(pool) mod_mysql_fields(mod_mysql_row_set_get_fields(dgt));
}
// }}}

// {{{ mod_mysql_row_set::count()
inline int mod_mysql_row_set::count()
{
    return mod_mysql_row_set_count(dgt);
}
// }}}

// {{{ mod_mysql_row_set::create_iterator()
mod_mysql_iterator<mod_mysql_row*>* mod_mysql_row_set::create_iterator(
            mod_mysql_pool* pool) const
{
}
// }}}
#endif

#endif /* _MOD_MYSQL_ROW_SET_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
