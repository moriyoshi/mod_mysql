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
#include "mod_mysql_row.h"
#include "mod_mysql_column.h"
#include "mod_mysql_unsupported_exception.h"

// {{{ mod_mysql_row::count()
int mod_mysql_row::count()
{
    return mod_mysql_row_get_num_of_columns(dgt);
}
// }}}

// {{{ mod_mysql_row::add()
bool mod_mysql_row::add(const mod_mysql_column*& v)
{
    throw mod_mysql_unsupported_exception("Operation is not supported.");
}
// }}}

// {{{ mod_mysql_row::refer()
mod_mysql_column*& mod_mysql_row::refer(int idx)
{
    throw mod_mysql_unsupported_exception("Operation is not supported.");
}

mod_mysql_column* const & mod_mysql_row::refer(int idx) const
{
    throw mod_mysql_unsupported_exception("Operation is not supported.");
}
// }}}

// {{{ mod_mysql_row::get()
mod_mysql_column* mod_mysql_row::get(int idx) const
{
    throw mod_mysql_unsupported_exception("Operation is not supported.");
}
// }}}

// {{{ mod_mysql_row::set()
void mod_mysql_row::set(int idx, const mod_mysql_column*& v)
{
    mod_mysql_row_set_column(dgt, idx, v);
}
// }}}

// {{{ mod_mysql_row::remove()
int mod_mysql_row::remove(int idx)
{
    return mod_mysql_row_remove_column(dgt, idx);
}
// }}}

// {{{ mod_mysql_row::index_of()
int mod_mysql_row::index_of(const mod_mysql_column*& v) 
{
    throw mod_mysql_unsupported_exception("Operation is not supported.");
}
// }}}

// {{{ mod_mysql_row::last_index_of()
int mod_mysql_row::last_index_of(const mod_mysql_column*& v)
{
    throw mod_mysql_unsupported_exception("Operation is not supported.");
}
// }}}

// vim: sts=4 sw=4 ts=4 et fdm=marker
