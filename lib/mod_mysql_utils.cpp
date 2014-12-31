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
#include "mod_mysql_utils.h"

// {{{ mod_mysql_utils::uppercase_ascii()
void mod_mysql_utils::uppercase_ascii(char *str)
{
    for (char *p = str; *p; p++) {
        if (*p >= 0x61 && *p <= 0x7a) {
            p -= 0x20;
        }
    }
}
// }}}

// vim: sts=4 sw=4 ts=4 et fdm=marker
