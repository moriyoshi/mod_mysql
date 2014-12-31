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
#ifndef _MOD_MYSQL_CODESET_H
#define _MOD_MYSQL_CODESET_H

#include "mod_mysql_charset.h"
#include "mod_mysql_encoding.h"

#ifndef MOD_MYSQL_CODESET_T_DEFINED
typedef struct mod_mysql_codeset_t mod_mysql_codeset_t;
#define MOD_MYSQL_CODESET_T_DEFINED
#endif /* MOD_MYSQL_CODESET_T_DEFINED */

struct mod_mysql_codeset_t
{
    const char* name;
    const char** aliases;
    mod_mysql_charset_t* cset;
    mod_mysql_encoding_t* enc;
};

#ifdef __cplusplus
#include "mod_mysql_base.h"

class mod_mysql_codeset: public mod_mysql_base, public mod_mysql_codeset_t
{
public:
    mod_mysql_codeset(mod_mysql_charset_t* cset, mod_mysql_encoding_t* enc,
            const char* name, const char* aliases[] = 0);

    mod_mysql_charset_t* get_charset();
    mod_mysql_encoding_t* get_encoding();
    const char* get_name();
    const char** get_aliases();
};

// {{{ mod_mysql_codeset::mod_mysql_codeset()
inline mod_mysql_codeset::mod_mysql_codeset(
        mod_mysql_charset_t* _cset, mod_mysql_encoding_t* _enc,
        const char* _name, const char* _aliases[])
{
    cset    = _cset;
    enc     = _enc;
    name    = _name;
    aliases = _aliases;
}
// }}}

// {{{ mod_mysql_codeset::get_charset()
inline mod_mysql_charset_t* mod_mysql_codeset::get_charset()
{
    return cset;
}
// }}}

// {{{ mod_mysql_codeset::get_encoding()
inline mod_mysql_encoding_t* mod_mysql_codeset::get_encoding()
{
    return enc;
}
// }}}

// {{{ mod_mysql_codeset::get_name()
inline const char* mod_mysql_codeset::get_name()
{
    return name;
}
// }}}

// {{{ mod_mysql_codeset::get_aliases()
inline const char** mod_mysql_codeset::get_aliases()
{
    return aliases;
}
// }}}
#endif

#endif /* _MOD_MYSQL_CODESET_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
