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
#ifndef _MOD_MYSQL_FIELD_VALUE_H
#define _MOD_MYSQL_FIELD_VALUE_H

#include <apr.h>
#include <apr_time.h>
#include <string.h>

#include "mod_mysql_field_type.h"
#include "mod_mysql_decimal.h"
#include "mod_mysql_opaque_data.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MOD_MYSQL_FIELD_VALUE_T_DEFINED
typedef struct mod_mysql_field_value_t mod_mysql_field_value_t;
#define MOD_MYSQL_FIELD_VALUE_T_DEFINED
#endif /* MOD_MYSQL_FIELD_VALUE_T_DEFINED */

typedef enum mod_mysql_field_value_type_t
{
    MOD_MYSQL_FIELD_VALUE_TYPE_NULL     = 0,
    MOD_MYSQL_FIELD_VALUE_TYPE_INT      = 1,
    MOD_MYSQL_FIELD_VALUE_TYPE_REAL     = 2,
    MOD_MYSQL_FIELD_VALUE_TYPE_DECIMAL  = 3,
    MOD_MYSQL_FIELD_VALUE_TYPE_DATETIME = 4,
    MOD_MYSQL_FIELD_VALUE_TYPE_STRING   = 5
} mod_mysql_field_value_type_t;

struct mod_mysql_field_value_t
{
    mod_mysql_field_value_type_t type;
    union {
        apr_int64_t              i;
        double                   r;
        mod_mysql_decimal_t      d;
        apr_time_exp_t           dt;
        mod_mysql_opaque_data_t  *str;
    } as;
};
 
#define mod_mysql_field_value_init_int(self, v) do { \
    (self)->type = MOD_MYSQL_FIELD_VALUE_TYPE_INT; \
    (self)->as.i = (v); \
} while (0)

#define mod_mysql_field_value_init_double(self, v) do {  \
    (self)->type = MOD_MYSQL_FIELD_VALUE_TYPE_REAL; \
    (self)->as.r = (v); \
} while (0)

#define mod_mysql_field_value_init_decimal(self, v) do { \
    (self)->type = MOD_MYSQL_FIELD_VALUE_TYPE_DECIMAL; \
    (self)->as.d = (*v); \
} while (0)

#define mod_mysql_field_value_init_datetime(self, v) do { \
    (self)->type = MOD_MYSQL_FIELD_VALUE_TYPE_DATETIME; \
    (self)->as.dt = (*v); \
} while (0)

#define mod_mysql_field_value_init_string(self, v) do { \
    mod_mysql_referent_add_ref(self); \
    (self)->type = MOD_MYSQL_FIELD_VALUE_TYPE_STRING; \
    (self)->as.str = v; \
} while (0)

#define mod_mysql_field_value_equals(self, that) ( \
        (self)->type == (that)->type && \
        memcmp(&(self)->as, &(that)->as, sizeof((self)->as) == 0))

apr_status_t
mod_mysql_field_value_as_int(mod_mysql_field_value_t*, apr_int64_t* retval,
        apr_pool_t *pool);
apr_status_t
mod_mysql_field_value_as_real(mod_mysql_field_value_t*, double* retval,
        apr_pool_t *pool);
apr_status_t
mod_mysql_field_value_as_decimal(mod_mysql_field_value_t*,
        mod_mysql_decimal_t* retval, apr_pool_t *pool);
apr_status_t
mod_mysql_field_value_as_datetime(mod_mysql_field_value_t*,
        apr_time_exp_t* retval, apr_pool_t *pool);
apr_status_t
mod_mysql_field_value_as_string(mod_mysql_field_value_t*,
        mod_mysql_opaque_data_t** retval, apr_pool_t *pool);

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
#include "mod_mysql_base.h"

class mod_mysql_field_value: public mod_mysql_field_value_t
{
public:
    mod_mysql_field_value(apr_int64_t);
    mod_mysql_field_value(double);
    mod_mysql_field_value(const mod_mysql_decimal_t&);
    mod_mysql_field_value(const apr_time_exp_t&);
    mod_mysql_field_value(mod_mysql_opaque_data_t*);

    apr_int64_t as_int(apr_pool_t* pool);
    double as_double(apr_pool_t* pool);
    mod_mysql_decimal as_decimal(apr_pool_t* pool);
    apr_time_exp_t as_datetime(apr_pool_t* pool);
    template<typename _T> mod_mysql_opaque_data<_T> as_string(apr_pool_t* pool);
};

// {{{ mod_mysql_field_value::mod_mysql_field_value()
inline mod_mysql_field_value::mod_mysql_field_value(apr_int64_t val)
{
    mod_mysql_field_value_init_int(this, val);
}

inline mod_mysql_field_value::mod_mysql_field_value(double val)
{
    mod_mysql_field_value_init_double(this, val);
}

inline
mod_mysql_field_value::mod_mysql_field_value(const mod_mysql_decimal_t& val)
{
    mod_mysql_field_value_init_decimal(this, &val);
}

inline
mod_mysql_field_value::mod_mysql_field_value(const apr_time_exp_t& val)
{
    mod_mysql_field_value_init_datetime(this, &val);
}

inline
mod_mysql_field_value::mod_mysql_field_value(mod_mysql_opaque_data_t* val)
{
    mod_mysql_field_value_init_string(this, val);
}
// }}}

// {{{ mod_mysql_field_value::as_int()
inline apr_int64_t mod_mysql_field_value::as_int(apr_pool_t* pool)
{
    apr_int64_t retval;
    apr_status_t res;

    if ((res = mod_mysql_field_value_as_int(this, &retval, pool)))
        throw mod_mysql_exception("umm..");

    return retval;
}
// }}}

// {{{ mod_mysql_field_value::as_double()
inline double mod_mysql_field_value::as_double(apr_pool_t* pool)
{
    double retval;
    apr_status_t res;

    if ((res = mod_mysql_field_value_as_real(this, &retval, pool)))
        throw mod_mysql_exception("umm..");

    return retval;
}
// }}}

// {{{ mod_mysql_field_value::as_decimal()
inline mod_mysql_decimal mod_mysql_field_value::as_decimal(apr_pool_t* pool)
{
    mod_mysql_decimal retval = mod_mysql_decimal::zero;
    apr_status_t res;

    if ((res = mod_mysql_field_value_as_decimal(this, &retval, pool)))
        throw mod_mysql_exception("umm..");

    return retval;
}
// }}}

// {{{ mod_mysql_field_value::as_datetime()
inline apr_time_exp_t mod_mysql_field_value::as_datetime(apr_pool_t* pool)
{
    apr_time_exp_t retval;
    apr_status_t res;

    if ((res = mod_mysql_field_value_as_datetime(this, &retval, pool)))
        throw mod_mysql_exception("umm..");

    return retval;
}
// }}}

// {{{ mod_mysql_field_value::as_string()
template<typename _T>
inline mod_mysql_opaque_data<_T> mod_mysql_field_value::as_string(
        apr_pool_t* pool)
{
    mod_mysql_opaque_data_t* _retval;
    apr_status_t res;

    if ((res = mod_mysql_field_value_as_string(this, &_retval, pool)))
        throw mod_mysql_exception("umm..");

    return mod_mysql_opaque_data<_T>(_retval);
}
// }}}

#endif

#endif /* _MOD_MYSQL_FIELD_VALUE_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
