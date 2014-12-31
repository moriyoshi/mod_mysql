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
#ifndef _MOD_MYSQL_FIELDS_H
#define _MOD_MYSQL_FIELDS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MOD_MYSQL_FIELDS_T_DEFINED
typedef struct mod_mysql_fields_t mod_mysql_fields_t;
#define MOD_MYSQL_FIELDS_T_DEFINED
#endif /* MOD_MYSQL_FIELDS_T_DEFINED */

#ifndef MOD_MYSQL_FIELD_INFO_T_DEFINED
typedef struct mod_mysql_field_info_t mod_mysql_field_info_t;
#define MOD_MYSQL_FIELD_INFO_T_DEFINED
#endif /* MOD_MYSQL_FIELD_INFO_T_DEFINED */

typedef struct mod_mysql_fields_vtbl_t
{
    int (*count_fn)(const mod_mysql_fields_t*);
    void (*add_fn)(mod_mysql_fields_t*, mod_mysql_field_info_t *);
    mod_mysql_field_info_t* (*get_fn)(mod_mysql_fields_t*, int);
    mod_mysql_field_info_t* (*set_fn)(mod_mysql_fields_t*, int,
            mod_mysql_field_info_t*);
    int (*remove_fn)(mod_mysql_fields_t*, int);
} mod_mysql_fields_vtbl_t;

struct mod_mysql_fields_t
{
    mod_mysql_fields_vtbl_t *_vtbl;
};

#define mod_mysql_fields_count(self) \
    (*((mod_mysql_fields_t *)(self))->_vtbl->count_fn)(self)
#define mod_mysql_fields_add(self, item) \
    (*((mod_mysql_fields_t *)(self))->_vtbl->add_fn)(self, item)
#define mod_mysql_fields_get(self, idx) \
    (*((mod_mysql_fields_t *)(self))->_vtbl->get_fn)(self, idx)
#define mod_mysql_fields_set(self, idx, item) \
    (*((mod_mysql_fields_t *)(self))->_vtbl->set_fn)(self, idx, item)
#define mod_mysql_fields_remove(self, idx) \
    (*((mod_mysql_fields_t *)(self))->_vtbl->remove_fn)(self, idx)

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
#include "mod_mysql_base.h"
#include "mod_mysql_instantiable.h"
#include "mod_mysql_list.h"
#include "mod_mysql_result.h"

class mod_mysql_field_info;
class mod_mysql_pool;

class mod_mysql_fields
        : public mod_mysql_base,
          public mod_mysql_instantiable,
          public mod_mysql_list<mod_mysql_field_info*>
{
    friend mod_mysql_result::mod_mysql_result(mod_mysql_pool*,
            mod_mysql_result::code_t, mod_mysql_fields*, int,
            int, int);
private:
    mod_mysql_fields_t *dgt;

public:
    mod_mysql_fields(mod_mysql_fields_t*);
    ~mod_mysql_fields();
    virtual int count() const;
    virtual int count(mod_mysql_field_info* const& v) const;
    virtual bool add(mod_mysql_field_info* const& v);
    virtual mod_mysql_field_info*& refer(int idx);
    virtual mod_mysql_field_info* const & refer(int idx) const;
    virtual mod_mysql_field_info* get(int idx);
    virtual void set(int idx, mod_mysql_field_info* const& v);
    virtual int remove(int idx);
    virtual int remove(const mod_mysql_field_info*& v);
    virtual int index_of(mod_mysql_field_info* const& v);
    virtual int last_index_of(mod_mysql_field_info* const& v);
};

// {{{ mod_mysql_fields::mod_mysql_fields()
inline mod_mysql_fields::mod_mysql_fields(mod_mysql_fields_t* _dgt)
        : dgt(_dgt)
{
}
// }}}
#endif

#endif /* _MOD_MYSQL_FIELDS_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
