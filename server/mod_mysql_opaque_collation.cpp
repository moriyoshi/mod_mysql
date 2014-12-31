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
#include "mod_mysql_opaque_collation.h"
#include "mod_mysql_exception.h"

// {{{ mod_mysql_opaque_collation::mod_mysql_opaque_collation()
mod_mysql_opaque_collation::mod_mysql_opaque_collation(const char* _name)
        : name(_name)
{
}
// }}}

// {{{ mod_mysql_opaque_collation::get_name()
const char* mod_mysql_opaque_collation::get_name()
{
    return name;
}
// }}}

// {{{ mod_mysql_opaque_collation::get_target_charset()
mod_mysql_charset* mod_mysql_opaque_collation::get_target_charset()
{
    return 0;
}
// }}}

// {{{ mod_mysql_opaque_collation::compare()
int mod_mysql_opaque_collation::compare(mod_mysql_opaque_cp_t first,
		mod_mysql_opaque_cp_t second)
{
    throw mod_mysql_exception("Not implemented");
}
// }}}

// vim: sts=4 sw=4 ts=4 et fdm=marker
