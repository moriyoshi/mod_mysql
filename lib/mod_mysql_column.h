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
#ifndef _MOD_MYSQL_COLUMN_H
#define _MOD_MYSQL_COLUMN_H

#include "mod_mysql_field_info.h"

#ifndef MOD_MYSQL_COLUMN_T_DEFINED
typedef struct mod_mysql_column_t mod_mysql_column_t;
#define MOD_MYSQL_COLUMN_T_DEFINED
#endif /* MOD_MYSQL_COLUMN_T_DEFINED */

struct mod_mysql_column_t
{
    mod_mysql_field_info_t *field_info;
    const char *repr;
};

#ifdef __cplusplus
class mod_mysql_column
        : public mod_mysql_base, public mod_mysql_column_t
{
public:
    mod_mysql_column(mod_mysql_field_info_t* finfo, const char* repr);
    mod_mysql_field_info* get_field_info() const;
    const char* as_string() const;
};

// {{{ mod_mysql_column::mod_mysql_column()
inline mod_mysql_column::mod_mysql_column(mod_mysql_field_info_t* _finfo,
        const char* _repr)
{
    field_info = _finfo;
    repr = _repr;
}
// }}}

// {{{ mod_mysql_column::get_field_info()
inline mod_mysql_field_info* mod_mysql_column::get_field_info() const
{
    return static_cast<mod_mysql_field_info*>(field_info);
}
// }}}

// {{{ mod_mysql_column::to_string()
inline const char* mod_mysql_column::as_string() const
{
    return repr;
}
// }}}

#endif

#endif /* _MOD_MYSQL_COLUMN_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
