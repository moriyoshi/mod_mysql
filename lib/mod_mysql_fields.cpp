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
#include "mod_mysql_fields.h"
#include "mod_mysql_field_info.h"

// {{{ mod_mysql_fields::~mod_mysql_fields()
mod_mysql_fields::~mod_mysql_fields()
{
}
// }}}

// {{{ mod_mysql_fields::count()
int mod_mysql_fields::count() const
{
    return mod_mysql_fields_count(dgt);
}

int mod_mysql_fields::count(mod_mysql_field_info* const& v) const
{
    int retval = 0;

    for (int i = count(); --i >= 0;) {
        mod_mysql_field_info* f = const_cast<mod_mysql_fields*>(this)->get(i);
        if (*f == *v) {
            retval++;
        }
    }

    return retval;
}
// }}}

// {{{ mod_mysql_fields::add()
bool mod_mysql_fields::add(mod_mysql_field_info* const& v)
{
    mod_mysql_fields_add(dgt, v);
    return true;
}
// }}}

// {{{ mod_mysql_fields::refer()
mod_mysql_field_info* const& mod_mysql_fields::refer(int idx) const
{
    throw mod_mysql_exception("Operation is not supported");
}

mod_mysql_field_info*& mod_mysql_fields::refer(int idx)
{
    throw mod_mysql_exception("Operation is not supported");
}
// }}}

// {{{ mod_mysql_fields::get()
mod_mysql_field_info* mod_mysql_fields::get(int idx)
{
    return static_cast<mod_mysql_field_info*>(mod_mysql_fields_get(dgt, idx));
}
// }}}

// {{{ mod_mysql_fields::set()
void mod_mysql_fields::set(int idx, mod_mysql_field_info* const& v)
{
    mod_mysql_fields_set(dgt, idx, v);
}
// }}}

// {{{ mod_mysql_fields::remove()
int mod_mysql_fields::remove(int idx)
{
    return mod_mysql_fields_remove(dgt, idx);
}

int mod_mysql_fields::remove(const mod_mysql_field_info*& v)
{
    int retval = 0;

    for (int i = count(); --i >= 0;) {
        mod_mysql_field_info* f = get(i);
        if (*f == *v) {
            remove(i);
            retval++;
        }
    }

    return retval;
}
// }}}

// {{{ mod_mysql_fields::index_of()
int mod_mysql_fields::index_of(mod_mysql_field_info* const& v) 
{
    for (int i = 0, l = count(); i < l; i++) {
        mod_mysql_field_info* f = get(i);
        if (*f == *v) {
            return i;
        }
    }

    return -1;
}
// }}}

// {{{ mod_mysql_fields::last_index_of()
int mod_mysql_fields::last_index_of(mod_mysql_field_info* const& v)
{
    for (int i = count(); --i >= 0;) {
        mod_mysql_field_info* f = get(i);
        if (*f == *v) {
            return i;
        }
    }

    return -1;
}
// }}}

// vim: sts=4 sw=4 ts=4 et fdm=marker
