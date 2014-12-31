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
#ifndef _COMMON_H
#define _COMMON_H

#ifndef MOD_MYSQL_DEBUG
#ifdef DEBUG
#define MOD_MYSQL_DEBUG 1
#else
#define MOD_MYSQL_DEBUG 0
#endif
#endif

#define DBG(expr) if (MOD_MYSQL_DEBUG) { expr; }

#ifdef __cplusplus
inline int MOD_MYSQL_MAX(const int a, const int b)
{
    return a >= b ? a: b;
}

inline int MOD_MYSQL_MIN(const int a, const int b)
{
    return a <= b ? a: b;
}

inline int MOD_MYSQL_MAX(const unsigned int a, const unsigned int b)
{
    return a >= b ? a: b;
}

inline int MOD_MYSQL_MIN(const unsigned int a, const unsigned int b)
{
    return a <= b ? a: b;
}
#else
#define MOD_MYSQL_MAX(a, b) \
    ((a) >= (b) ? (a): (b))
#define MOD_MYSQL_MIN(a, b) \
    ((a) <= (b) ? (a): (b))
#endif

#endif /* _COMMON_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
