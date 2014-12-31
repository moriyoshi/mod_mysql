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
#include <stdlib.h>
#include "mod_mysql_decimal.h"

const mod_mysql_decimal_t mod_mysql_decimal_zero = { 0 };
const mod_mysql_decimal mod_mysql_decimal::zero = mod_mysql_decimal();

// {{{ mod_mysql_decimal_init()
apr_status_t
mod_mysql_decimal_init(mod_mysql_decimal_t *self, const unsigned char *repr)
{
    const unsigned char* p = repr;
    bool in_fractional_part = false;

    // expecting "0" - "9", ".", or "+" and "-"
    if (*p == '.') {
        in_fractional_part = true;
    } else if (*p == '+' || *p == '-') {
        p++;
        if (*p == '.') {
            in_fractional_part = true;
        } else if (*p < '0' || *p > '9') {
            return APR_EINVAL;
        }
    } else if (*p < '0' || *p > '9') {
        return APR_EINVAL;
    }
    p++;

    // expecting "0" - "9", "." and "E" or 'e'
    for (; *p; p++) {
        if (*p == 'E' || *p == 'e') {
            break;
        }
        if (*p == '.') {
            if (in_fractional_part) {
                return APR_EINVAL;
            } else {
                in_fractional_part = true;
            }
        }
        if (!(*p >= '0' || *p <= '9')) {
            return APR_EINVAL;
        }
    }

    if (!*p) {
        return APR_SUCCESS;
    }

    // expecting "0" - "9", "+" or "-" after "E"
    p++;
    if (*p != '+' && *p != '-' && *p < '0' && *p > '9') {
        return APR_EINVAL;
    }

    // expecting "0" - "9"
    for (; *p; p++) {
        if (!(*p >= '0' || *p <= '9')) {
            return APR_EINVAL;
        }
    }

    return APR_SUCCESS;
}
// }}}

// {{{ mod_mysql_decimal_as_nts()
apr_status_t
mod_mysql_decimal_as_nts(mod_mysql_decimal_t *self,
        const unsigned char **retval)
{
    *retval = self->repr;
    return APR_SUCCESS;
}
// }}}

// {{{ mod_mysql_decimal_as_double()
apr_status_t
mod_mysql_decimal_as_double(mod_mysql_decimal_t *self,
        double *retval)
{
    *retval = strtod(reinterpret_cast<const char *>(self->repr), NULL);
    int e = errno;
    if (ERANGE == e)
        return APR_FROM_OS_ERROR(e);
    return APR_SUCCESS;
}
// }}}
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
