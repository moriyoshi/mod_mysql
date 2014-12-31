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
#ifndef _MOD_MYSQL_COLLATION_H
#define _MOD_MYSQL_COLLATION_H

#include "mod_mysql_comparator.h"
#include "mod_mysql_opaque_cp.h"

#ifndef MOD_MYSQL_CHARSET_T_DEFINED
typedef struct mod_mysql_charset mod_mysql_charset_t;
#define MOD_MYSQL_CHARSET_T_DEFINED
#endif /* MOD_MYSQL_CHARSET_T_DEFINED */

#ifndef MOD_MYSQL_COLLATION_T_DEFINED
typedef struct mod_mysql_collation mod_mysql_collation_t;
#define MOD_MYSQL_COLLATION_T_DEFINED
#endif /* MOD_MYSQL_COLLATION_T_DEFINED */

/* {{{ C API */
#ifdef __cplusplus
extern "C" {
#endif

const char* mod_mysql_collation_get_name(mod_mysql_collation_t *self);
mod_mysql_charset_t* mod_mysql_collation_get_target_charset(
        mod_mysql_collation_t *self);
int mod_mysql_collation_compare(mod_mysql_collation_t* self,
        mod_mysql_opaque_cp_t first, mod_mysql_opaque_cp_t secon);

#ifdef __cplusplus
}
#endif
/* }}} */

#ifdef __cplusplus
class mod_mysql_charset;

class mod_mysql_collation: public mod_mysql_comparator<mod_mysql_opaque_cp_t>
{
public:
    virtual const char* get_name() = 0;
    virtual mod_mysql_charset* get_target_charset() = 0;
    virtual int compare(mod_mysql_opaque_cp_t first,
            mod_mysql_opaque_cp_t second) = 0;
};

#endif

#endif /* _MOD_MYSQL_COLLATION_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
