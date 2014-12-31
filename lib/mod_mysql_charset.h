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
#ifndef _MOD_MYSQL_CHARSET_H
#define _MOD_MYSQL_CHARSET_H

#include "mod_mysql_opaque_cp.h"

#ifndef MOD_MYSQL_CHARSET_T_DEFINED
typedef struct mod_mysql_charset mod_mysql_charset_t;
#define MOD_MYSQL_CHARSET_T_DEFINED
#endif /* MOD_MYSQL_CHARSET_T_DEFINED */

#ifdef __cplusplus
extern "C" {
#endif
int mod_mysql_charset_contains(mod_mysql_charset_t *self,
        mod_mysql_opaque_cp_t c);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include "mod_mysql_collection.h"

class mod_mysql_charset
        : public mod_mysql_collection<mod_mysql_opaque_cp_t>
{
private:
    const char* name;

public:
    const char* get_name();
    virtual int count() const = 0;
    virtual int count(const mod_mysql_opaque_cp_t& v) const = 0;
    virtual bool add(const mod_mysql_opaque_cp_t& v) = 0;
    virtual int remove(const mod_mysql_opaque_cp_t& v) = 0;
    virtual bool contains(const mod_mysql_opaque_cp_t& v) = 0;
};

inline const char* mod_mysql_charset::get_name()
{
    return name;
}
#endif

#endif /* _MOD_MYSQL_CHARSET_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
