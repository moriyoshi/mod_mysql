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
#include "mod_mysql_result.h"
#include "mod_mysql_fields.h"
#include "mod_mysql_pool.h"

// {{{ mod_mysql_result::mod_mysql_result()
mod_mysql_result::mod_mysql_result(
        mod_mysql_pool* _pool, code_t _code, mod_mysql_fields* _fields,
        int _num_affected_rows, int _num_warnings, int _last_insert_id)
        : pool(_pool), dgt(0), fields(_fields)
{
    dgt = _pool->alloc_obj<mod_mysql_result_t>(1);

    dgt->code              = _code;
    dgt->fields            = _fields ? _fields->dgt: 0;
    dgt->num_affected_rows = _num_affected_rows,
    dgt->num_warnings      = _num_warnings;
    dgt->last_insert_id    = _last_insert_id;
}

mod_mysql_result::mod_mysql_result(mod_mysql_pool* _pool,
        mod_mysql_result_t* _dgt)
        : pool(_pool), dgt(_dgt)
{
}
// }}}

// {{{ mod_mysql_result::get_fields()
mod_mysql_fields* mod_mysql_result::get_fields() const
{
    if (!fields && dgt->fields) {
        const_cast<mod_mysql_result*>(this)->fields =
                new(pool) mod_mysql_fields(dgt->fields);
    }
    
    return fields;
}
// }}}

// vim: sts=4 sw=4 ts=4 et fdm=marker
