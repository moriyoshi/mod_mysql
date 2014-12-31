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
#ifndef _MOD_MYSQL_RECOVERABLE_EXCEPTION_H
#define _MOD_MYSQL_RECOVERABLE_EXCEPTION_H

#include "mod_mysql_server_message.h"
#include "mod_mysql_exception.h"

class mod_mysql_recoverable_exception: public mod_mysql_exception
{
private:
    mod_mysql_server_message::code_t code;
    const char* msg;

public:
    mod_mysql_recoverable_exception(
            mod_mysql_server_message::code_t code,
            const char* msg);

    mod_mysql_server_message::code_t get_code() const;
    const char* get_message() const;
};

// {{{ mod_mysql_recoverable_exception::mod_mysql_recoverable_exception()
inline mod_mysql_recoverable_exception::mod_mysql_recoverable_exception(
        mod_mysql_server_message::code_t _code, const char* _msg)
        : code(_code), msg(_msg)
{
}
// }}}

// {{{ mod_mysql_recoverable_exception::get_code()
inline mod_mysql_server_message::code_t
mod_mysql_recoverable_exception::get_code() const
{
    return code;
}
// }}}

// {{{ mod_mysql_recoverable_exception::get_message()
inline const char* mod_mysql_recoverable_exception::get_message() const
{
    return msg;
}
// }}}

#endif /* _MOD_MYSQL_RECOVERABLE_EXCEPTION_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
