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
#ifndef _MOD_MYSQL_DECIMAL_H
#define _MOD_MYSQL_DECIMAL_H

#include <apr_errno.h>

#include "mod_mysql_field_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MOD_MYSQL_DECIMAL_T_DEFINED
typedef struct mod_mysql_decimal_t mod_mysql_decimal_t;
#define MOD_MYSQL_DECIMAL_T_DEFINED
#endif /* MOD_MYSQL_DECIMAL_T_DEFINED */

struct mod_mysql_decimal_t
{
    unsigned char repr[256];
};

extern const mod_mysql_decimal_t mod_mysql_decimal_zero;

apr_status_t
mod_mysql_decimal_init(mod_mysql_decimal_t *, const unsigned char *repr);

apr_status_t
mod_mysql_decimal_init_int(mod_mysql_decimal_t *, apr_int64_t v);

apr_status_t
mod_mysql_decimal_init_uint(mod_mysql_decimal_t *, apr_int64_t v);

apr_status_t
mod_mysql_decimal_init_double(mod_mysql_decimal_t *, double v);

apr_status_t
mod_mysql_decimal_as_nts(mod_mysql_decimal_t *, const unsigned char **retval);
apr_status_t
mod_mysql_decimal_as_double(mod_mysql_decimal_t *, double *retval);

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
#include "mod_mysql_base.h"
#include "mod_mysql_invalid_argument_exception.h"

class mod_mysql_decimal: public mod_mysql_decimal_t, public mod_mysql_base
{
public:
    static const mod_mysql_decimal zero;
public:
    mod_mysql_decimal();
    mod_mysql_decimal(const unsigned char* repr);
    mod_mysql_decimal(apr_int64_t v);
    mod_mysql_decimal(apr_uint64_t v);
    mod_mysql_decimal(double v);
    const unsigned char* as_nts();
    double as_double();
};

// {{{ mod_mysql_decimal::mod_mysql_decimal()
inline mod_mysql_decimal::mod_mysql_decimal()
{
    repr[0] = '\0';
}

inline mod_mysql_decimal::mod_mysql_decimal(const unsigned char* repr)
{
    switch (mod_mysql_decimal_init(this, repr)) {
    case APR_SUCCESS:
        break;
    default:
        throw mod_mysql_invalid_argument_exception("Wrong decimal format");
    }
}

inline mod_mysql_decimal::mod_mysql_decimal(apr_int64_t v)
{
    mod_mysql_decimal_init_int(this, v); // shouldn't fail
}

inline mod_mysql_decimal::mod_mysql_decimal(apr_uint64_t v)
{
    mod_mysql_decimal_init_uint(this, v); // shouldn't fail
}

inline mod_mysql_decimal::mod_mysql_decimal(double v)
{
    mod_mysql_decimal_init_double(this, v); // shouldn't fail
}
// }}}

// {{{ mod_mysql_decimal::as_nts()
inline const unsigned char* mod_mysql_decimal::as_nts()
{
    const unsigned char* retval;
    mod_mysql_decimal_as_nts(this, &retval);
    return retval;
}
// }}}

// {{{ mod_mysql_decimal::as_double()
inline double mod_mysql_decimal::as_double()
{
    double retval;
    mod_mysql_decimal_as_double(this, &retval);
    return retval;
}
// }}}

#endif

#endif /* _MOD_MYSQL_DECIMAL_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
