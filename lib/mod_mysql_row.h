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
#ifndef _MOD_MYSQL_ROW_H
#define _MOD_MYSQL_ROW_H

#include "mod_mysql_base.h"
#include "mod_mysql_list.h"
#include "mod_mysql_iterator.h"

#ifndef MOD_MYSQL_ROW_T_DEFINED
typedef struct mod_mysql_row_t mod_mysql_row_t;
#define MOD_MYSQL_ROW_T_DEFINED
#endif /* MOD_MYSQL_ROW_T_DEFINED */

#ifndef MOD_MYSQL_COLUMN_T_DEFINED
typedef struct mod_mysql_column_t mod_mysql_column_t;
#define MOD_MYSQL_COLUMN_T_DEFINED
#endif /* MOD_MYSQL_COLUMN_T_DEFINED */

typedef struct mod_mysql_row_vtbl_t
{
    int (*count_fn)(const mod_mysql_row_t*);
    mod_mysql_column_t* (*get_fn)(mod_mysql_row_t*, int);
    mod_mysql_column_t* (*set_fn)(mod_mysql_row_t*, int,
            const mod_mysql_column_t*);
    int (*remove_fn)(mod_mysql_row_t*, int);
} mod_mysql_row_vtbl_t;

struct mod_mysql_row_t {
    mod_mysql_row_vtbl_t *_vtbl;
};

#define mod_mysql_row_get_num_of_columns(self) \
    (*((mod_mysql_row_t *)(self))->_vtbl->count_fn)(self)
#define mod_mysql_row_get_column(self, idx) \
    (*((mod_mysql_row_t *)(self))->_vtbl->get_fn)(self, idx)
#define mod_mysql_row_set_column(self, idx, item) \
    (*((mod_mysql_row_t *)(self))->_vtbl->set_fn)(self, idx, item)
#define mod_mysql_row_remove_column(self, idx) \
    (*((mod_mysql_row_t *)(self))->_vtbl->remove_fn)(self, idx)


#ifdef __cplusplus
class mod_mysql_column;

class mod_mysql_row
        : public mod_mysql_base,
          public mod_mysql_list<mod_mysql_column*>
{
private:
    mod_mysql_row_t* dgt;

public:
    mod_mysql_row(mod_mysql_row_t* dgt);

    virtual int count();
    virtual bool add(const mod_mysql_column*& v);
    virtual mod_mysql_column*& refer(int idx);
    virtual mod_mysql_column* const& refer(int idx) const;
    virtual mod_mysql_column* get(int idx) const;
    virtual void set(int idx, const mod_mysql_column*& v);
    virtual int remove(int idx);
    virtual int index_of(const mod_mysql_column*& v);
    virtual int last_index_of(const mod_mysql_column*& v);
};

// {{{ mod_mysql_row::mod_mysql_row()
inline mod_mysql_row::mod_mysql_row(mod_mysql_row_t* _dgt)
        : dgt(_dgt)
{
}
// }}}
#endif

#endif /* _MOD_MYSQL_ROW_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
