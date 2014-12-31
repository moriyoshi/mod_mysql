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
#ifndef _MOD_MYSQL_STATEMENT_PARAM_H
#define _MOD_MYSQL_STATEMENT_PARAM_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MOD_MYSQL_STATEMENT_PARAM_T_DEFINED
typedef struct mod_mysql_statement_param_t mod_mysql_statement_param_t;
#define MOD_MYSQL_STATEMENT_PARAM_T_DEFINED
#endif /* MOD_MYSQL_STATEMENT_PARAM_T_DEFINED */

#ifndef MOD_MYSQL_FIELD_VALUE_T_DEFINED
typedef struct mod_mysql_field_value_t mod_mysql_field_value_t;
#define MOD_MYSQL_FIELD_VALUE_T_DEFINED
#endif /* MOD_MYSQL_FIELD_VALUE_T_DEFINED */

struct mod_mysql_statement_param_t
{
    int ord;
    mod_mysql_field_value_t* val;
};

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
#include "mod_mysql_base.h"
#include "mod_mysql_field_value.h"

#ifdef HAVE_STRING_H
#include <string.h>
#endif /* HAVE_STRING_H */

class mod_mysql_statement_param
        : public mod_mysql_base, public mod_mysql_statement_param_t
{
public:
    mod_mysql_statement_param(int ord, mod_mysql_field_value* val);
    int get_ordinal() const;

    bool operator ==(const mod_mysql_statement_param& that) const;
};

// {{{ mod_mysql_statement_param::mod_mysql_statement_param()
inline mod_mysql_statement_param::mod_mysql_statement_param(int _ord,
        mod_mysql_field_value* _val)
{
    ord = _ord;
    val = static_cast<mod_mysql_field_value_t *>(_val);
}
// }}}

// {{{ mod_mysql_statement_param::get_ordinal()
inline int mod_mysql_statement_param::get_ordinal() const
{
    return ord;
}
// }}}

// {{{ mod_mysql_statement_param::operator ==()
inline bool
mod_mysql_statement_param::operator ==(
        const mod_mysql_statement_param& that) const
{
    return ord == that.ord && mod_mysql_field_value_equals(val, that.val);
}
// }}}
#endif

#endif /* _MOD_MYSQL_STATEMENT_PARAM_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
