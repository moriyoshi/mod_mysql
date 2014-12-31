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
#ifndef _SAFETY_H
#define _SAFETY_H

#include "mod_mysql_error.h"

// {{{ safe_size_t_cast
#define safe_size_t_cast(type, val) _safe_size_t_cast<type>(val, __FILE__, __LINE__)

template<typename _T> inline _T _safe_size_t_cast(size_t val, const char* file,
        int line)
{
    _T retval = static_cast<_T>(val);

    if (static_cast<size_t>(retval) != val) {
        throw mod_mysql_error(0, file, line, "Integer overflow.");
    }

    return retval;
}
// }}}

// {{{ safe_size_t_multiply
#define safe_size_t_multiply(val, n) _safe_size_t_multiply(val, n, __FILE__, __LINE__)
inline size_t _safe_size_t_multiply(size_t val, size_t n, const char* file,
        int line)
{
    double dval = static_cast<double>(val) * n;

    if (static_cast<size_t>(dval) != val * n) {
        throw mod_mysql_error(0, file, line, "Integer overflow");
    }

    return val * n;
}
// }}}

#endif /* _SAFETY_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
