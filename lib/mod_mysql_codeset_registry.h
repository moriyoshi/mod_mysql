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
#ifndef _MOD_MYSQL_CODESET_REGISTRY_H
#define _MOD_MYSQL_CODESET_REGISTRY_H

#include "mod_mysql_codeset.h"

class mod_mysql_codeset_registry
{
public:
    virtual mod_mysql_codeset* find(const char* name) = 0;
    virtual mod_mysql_codeset* find(mod_mysql_codeset::code_e code) = 0;
    virtual void add(mod_mysql_codeset* cset) = 0;
    virtual int count() = 0;
    mod_mysql_codeset* operator[](const char* name);
    mod_mysql_codeset* operator[](mod_mysql_codeset::code_e code);
};

// {{{ mod_mysql_codeset_registry::operator[]
inline mod_mysql_codeset* mod_mysql_codeset_registry::operator[](
        const char* name)
{
    return find(name);
}

inline mod_mysql_codeset* mod_mysql_codeset_registry::operator[](
        mod_mysql_codeset::code_e code)
{
    return find(code);
}
// }}}

#endif /* _MOD_MYSQL_CODESET_REGISTRY_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
