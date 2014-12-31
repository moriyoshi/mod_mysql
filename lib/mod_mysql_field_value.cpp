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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_ASSERT_H
#include <assert.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <stdlib.h>

#include <apr_strings.h>

#include "mod_mysql_field_value.h"
#include "mod_mysql_datetime.h"
#include "mod_mysql_static_opaque_data.h"

extern "C" {

// {{{ mod_mysql_field_value_datetime_to_int()
static apr_status_t
mod_mysql_field_value_datetime_to_int(const apr_time_exp_t* dt,
        apr_int64_t *retval)
{
    if (dt->tm_hour < 0) { // Y-M-D only
        *retval = (dt->tm_year + 1900)* 10000
                + (dt->tm_mon + 1) * 100
                + (dt->tm_mday);
    } else { // Y-M-D H:M:S
        *retval = (dt->tm_year + 1900)* 10000000000ll
                + (dt->tm_mon + 1) * 100000000ll
                + (dt->tm_mday) * 1000000
                + (dt->tm_hour) * 10000
                + (dt->tm_min) * 100;
                + (dt->tm_sec);
    }

    return APR_SUCCESS;
}
// }}}

// {{{ mod_mysql_field_value_as_int()
apr_status_t
mod_mysql_field_value_as_int(mod_mysql_field_value_t* self,
        apr_int64_t* retval, apr_pool_t* pool)
{
    apr_status_t err = APR_SUCCESS;

    switch (self->type) {
    case MOD_MYSQL_FIELD_VALUE_TYPE_NULL:
        *retval = 0;
        break;
    case MOD_MYSQL_FIELD_VALUE_TYPE_INT:
        *retval = self->as.i;
        break;
    case MOD_MYSQL_FIELD_VALUE_TYPE_REAL:
        *retval = static_cast<apr_int64_t>(self->as.r);
        break;
    case MOD_MYSQL_FIELD_VALUE_TYPE_DECIMAL:
        {
            double tmp = 0;
            err = mod_mysql_decimal_as_double(&self->as.d, &tmp);
            *retval = static_cast<apr_int64_t>(tmp);
        }
        break;
    case MOD_MYSQL_FIELD_VALUE_TYPE_DATETIME:
        *retval = 0;
        err = mod_mysql_field_value_datetime_to_int(&self->as.dt, retval);
        break;
    case MOD_MYSQL_FIELD_VALUE_TYPE_STRING:
        {
            apr_pool_t *cpool;

            if ((err = apr_pool_create(&cpool, pool)))
                break;

            do {
                char *p;
                if ((err = mod_mysql_opaque_data_map(self->as.str,
                        reinterpret_cast<void**>(&p), cpool)))
                    break;
#ifdef HAVE_STRTOLL
                *retval = strtoll(p, NULL, 10);
#else
                // XXX: will probably result in floating-point weirdness.
                *retval = static_cast<apr_int64_t>(strtod(p, NULL));
#endif
                if ((err = mod_mysql_opaque_data_unmap(self->as.str, p)))
                    break;
            } while (0);

            apr_pool_destroy(cpool);
        }
        break;
    }

    return err;
}
// }}}

// {{{ mod_mysql_field_value_as_real()
apr_status_t
mod_mysql_field_value_as_real(mod_mysql_field_value_t* self,
        double* retval, apr_pool_t *pool)
{
    apr_status_t err = APR_SUCCESS;

    switch (self->type) {
    case MOD_MYSQL_FIELD_VALUE_TYPE_NULL:
        *retval = 0;
        break;
    case MOD_MYSQL_FIELD_VALUE_TYPE_INT:
        *retval = self->as.i;
        break;
    case MOD_MYSQL_FIELD_VALUE_TYPE_REAL:
        *retval = self->as.r;
        break;
    case MOD_MYSQL_FIELD_VALUE_TYPE_DECIMAL:
        err = mod_mysql_decimal_as_double(&self->as.d, retval);
        break;
    case MOD_MYSQL_FIELD_VALUE_TYPE_DATETIME:
        {
            apr_int64_t tmp = 0;
            err = mod_mysql_field_value_datetime_to_int(&self->as.dt, &tmp);
            *retval = tmp;
        }
        break;
    case MOD_MYSQL_FIELD_VALUE_TYPE_STRING:
        {
            apr_pool_t *cpool;

            if ((err = apr_pool_create(&cpool, pool)))
                break;

            do {
                char *p;
                if ((err = mod_mysql_opaque_data_map(self->as.str,
                        reinterpret_cast<void**>(&p), cpool)))
                    break;

                *retval = static_cast<apr_int64_t>(strtod(p, NULL));

                if ((err = mod_mysql_opaque_data_unmap(self->as.str, p)))
                    break;
            } while (0);

            apr_pool_destroy(cpool);
        }
        break;
    }

    return err;
}
// }}}

// {{{ mod_mysql_field_value_as_decimal()
apr_status_t
mod_mysql_field_value_as_decimal(mod_mysql_field_value_t* self,
        mod_mysql_decimal_t* retval, apr_pool_t *pool)
{
    apr_status_t err = APR_SUCCESS;

    switch (self->type) {
    case MOD_MYSQL_FIELD_VALUE_TYPE_NULL:
        *retval = mod_mysql_decimal_zero;
        break;
    case MOD_MYSQL_FIELD_VALUE_TYPE_INT:
        err = mod_mysql_decimal_init_int(retval, self->as.i);
        break;
    case MOD_MYSQL_FIELD_VALUE_TYPE_REAL:
        err = mod_mysql_decimal_init_double(retval, self->as.i);
        break;
    case MOD_MYSQL_FIELD_VALUE_TYPE_DECIMAL:
        *retval = self->as.d;
        break;
    case MOD_MYSQL_FIELD_VALUE_TYPE_DATETIME:
        {
            apr_int64_t tmp = 0;
            if (err = mod_mysql_field_value_datetime_to_int(&self->as.dt, &tmp))
                break;
            err = mod_mysql_decimal_init_int(retval, tmp);
        }
        break;
    case MOD_MYSQL_FIELD_VALUE_TYPE_STRING:
        {
            apr_pool_t *cpool;

            if ((err = apr_pool_create(&cpool, pool)))
                break;

            do {
                unsigned char *p;
                if ((err = mod_mysql_opaque_data_map(self->as.str,
                        reinterpret_cast<void**>(&p), cpool))) {
                    break;
                }
                if ((err = mod_mysql_decimal_init(retval, p))) {
                    break;
                }
                if ((err = mod_mysql_opaque_data_unmap(self->as.str, p))) {
                    break;
                }
            } while (0);

            apr_pool_destroy(cpool);
        }
        break;
    }

    return err;
}
// }}}

// {{{ mod_mysql_field_value_as_datetime()
apr_status_t
mod_mysql_field_value_as_datetime(mod_mysql_field_value_t* self,
        apr_time_exp_t* retval, apr_pool_t *pool)
{
    apr_status_t err = APR_SUCCESS;

    switch (self->type) {
    case MOD_MYSQL_FIELD_VALUE_TYPE_DATETIME:
        *retval = self->as.dt;
        break;
    default:
        apr_pool_t *cpool;
        mod_mysql_opaque_data_t *data;

        if ((err = apr_pool_create(&cpool, pool)))
            break;

        if ((err = mod_mysql_field_value_as_string(self, &data, cpool)))
            break;

        do {
            unsigned char *p;
            if ((err = mod_mysql_opaque_data_map(data,
                    reinterpret_cast<void**>(&p), cpool)))
                break;

            do {
                if ((err = mod_mysql_datetime_parse_sql(retval, p)))
                    break;
            } while (0);

            assert(!mod_mysql_opaque_data_unmap(data, p));
        } while (0);

        mod_mysql_referent_dispose(data);
        apr_pool_destroy(cpool);
        break;
    }

    return err;
}
// }}}

// {{{ mod_mysql_field_value_as_string()
apr_status_t
mod_mysql_field_value_as_string(mod_mysql_field_value_t* self,
        mod_mysql_opaque_data_t** retval, apr_pool_t *pool)
{
    apr_status_t err;

    switch (self->type) {
    case MOD_MYSQL_FIELD_VALUE_TYPE_STRING:
        mod_mysql_referent_add_ref(self->as.str);
        *retval = self->as.str;
        break;
    case MOD_MYSQL_FIELD_VALUE_TYPE_NULL:
        *retval = 0;
        break;
    case MOD_MYSQL_FIELD_VALUE_TYPE_INT:
        {
            char *p = apr_psprintf(pool, "%lld", self->as.i);
            mod_mysql_opaque_data_t *_retval =
                    static_cast<mod_mysql_opaque_data_t*>(apr_palloc(pool,
                    sizeof(mod_mysql_opaque_data_t)));
            mod_mysql_static_opaque_data_init(_retval, p, strlen(p));
            *retval = _retval;
        }
        break;
    case MOD_MYSQL_FIELD_VALUE_TYPE_REAL:
        {
            char *p = apr_psprintf(pool, "%f", self->as.r);
            mod_mysql_opaque_data_t *_retval =
                    static_cast<mod_mysql_opaque_data_t*>(apr_palloc(pool,
                    sizeof(mod_mysql_opaque_data_t)));
            mod_mysql_static_opaque_data_init(_retval, p, strlen(p));
            *retval = _retval;
        }
        break;
    case MOD_MYSQL_FIELD_VALUE_TYPE_DECIMAL:
        {
            unsigned char *p;
            {
                const unsigned char *_p;
                if ((err = mod_mysql_decimal_as_nts(&self->as.d, &_p)))
                    break;

                p = reinterpret_cast<unsigned char *>(
                        apr_pstrdup(pool, reinterpret_cast<const char *>(_p)));
            }

            mod_mysql_opaque_data_t *_retval =
                    static_cast<mod_mysql_opaque_data_t*>(apr_palloc(pool,
                    sizeof(mod_mysql_opaque_data_t)));
            mod_mysql_static_opaque_data_init(_retval, p,
                    strlen(reinterpret_cast<char*>(p)));
            *retval = _retval;
        }
        break;
    case MOD_MYSQL_FIELD_VALUE_TYPE_DATETIME:
        {
            unsigned char *p;
            if ((err = mod_mysql_datetime_as_sql(&self->as.dt, &p, pool)))
                break;

            mod_mysql_opaque_data_t *_retval =
                    static_cast<mod_mysql_opaque_data_t*>(apr_palloc(pool,
                    sizeof(mod_mysql_opaque_data_t)));
            mod_mysql_static_opaque_data_init(_retval, p,
                    strlen(reinterpret_cast<char*>(p)));
            *retval = _retval;
        }
        break;
    }
}
// }}}

} // extern "C"

// vim: sts=4 sw=4 ts=4 et fdm=marker
