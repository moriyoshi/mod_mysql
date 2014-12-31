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
#include "mod_mysql_statement_params.h"
#include "mod_mysql_statement_param.h"

// {{{ mod_mysql_statement_params::~mod_mysql_statement_params()
mod_mysql_statement_params::~mod_mysql_statement_params()
{
}
// }}}

// {{{ mod_mysql_statement_params::count()
int mod_mysql_statement_params::count() const
{
    return mod_mysql_statement_params_count(dgt);
}

int mod_mysql_statement_params::count(mod_mysql_statement_param* const& v) const
{
    int retval = 0;

    for (int i = count(); --i >= 0;) {
        mod_mysql_statement_param* f = const_cast<mod_mysql_statement_params*>(this)->get(i);
        if (*f == *v) {
            retval++;
        }
    }

    return retval;
}
// }}}

// {{{ mod_mysql_statement_params::add()
bool mod_mysql_statement_params::add(mod_mysql_statement_param* const& v)
{
    mod_mysql_statement_params_add(dgt, v);
    return true;
}
// }}}

// {{{ mod_mysql_statement_params::refer()
mod_mysql_statement_param* const& mod_mysql_statement_params::refer(int idx) const
{
    return mod_mysql_statement_params_refer(dgt, idx);
}

mod_mysql_statement_param*& mod_mysql_statement_params::refer(int idx)
{
    throw mod_mysql_exception("Operation is not supported");
}
// }}}

// {{{ mod_mysql_statement_params::get()
mod_mysql_statement_param* mod_mysql_statement_params::get(int idx)
{
    return mod_mysql_statement_params_get(dgt, idx);
}
// }}}

// {{{ mod_mysql_statement_params::set()
void mod_mysql_statement_params::set(int idx, mod_mysql_statement_param* const& v)
{
    mod_mysql_statement_params_set(dgt, idx, v);
}
// }}}

// {{{ mod_mysql_statement_params::remove()
int mod_mysql_statement_params::remove(int idx)
{
    return mod_mysql_statement_params_remove(dgt, idx);
}

int mod_mysql_statement_params::remove(const mod_mysql_statement_param*& v)
{
    int retval = 0;

    for (int i = count(); --i >= 0;) {
        mod_mysql_statement_param* f = get(i);
        if (*f == *v) {
            remove(i);
            retval++;
        }
    }

    return retval;
}
// }}}

// {{{ mod_mysql_statement_params::index_of()
int mod_mysql_statement_params::index_of(mod_mysql_statement_param* const& v) 
{
    for (int i = 0, l = count(); i < l; i++) {
        mod_mysql_statement_param* f = get(i);
        if (*f == *v) {
            return i;
        }
    }

    return -1;
}
// }}}

// {{{ mod_mysql_statement_params::last_index_of()
int mod_mysql_statement_params::last_index_of(mod_mysql_statement_param* const& v)
{
    for (int i = count(); --i >= 0;) {
        mod_mysql_statement_param* f = get(i);
        if (*f == *v) {
            return i;
        }
    }

    return -1;
}
// }}}

// vim: sts=4 sw=4 ts=4 et fdm=marker
