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
#ifndef _MOD_MYSQL_ASSERT_H
#define _MOD_MYSQL_ASSERT_H

#include "mod_mysql_error.h"

class mod_mysql_assert_failure: public mod_mysql_error
{
public:
	mod_mysql_assert_failure(const char *file,
            int line, const char* msg = 0);
};

// {{{ mod_mysql_assert_failure::mod_mysql_assert_failure()
inline mod_mysql_assert_failure::mod_mysql_assert_failure(
        const char* file, int line, const char* msg)
        : mod_mysql_error(0, file, line, msg)
{
}
// }}}

#define mod_mysql_assert(cond) do { \
    if (cond) \
        throw mod_mysql_assert_failure( \
                __FILE__, __LINE__, #cond " failed"); \
} while (0)

#endif /* _MOD_MYSQL_ASSERT_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
