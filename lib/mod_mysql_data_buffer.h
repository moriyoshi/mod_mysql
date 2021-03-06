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
#ifndef _MOD_MYSQL_DATA_BUFFER_H
#define _MOD_MYSQL_DATA_BUFFER_H

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif

#include <apr.h>

#include "mod_mysql_data_output.h"
#include "mod_mysql_invalid_argument_exception.h"

class mod_mysql_data_buffer: public mod_mysql_data_output
{
private:
    mod_mysql_ranged_ref<apr_byte_t> tgt;
    int idx;

public:
    mod_mysql_data_buffer(const mod_mysql_ranged_ref<apr_byte_t>& tgt);
    virtual void write(const mod_mysql_ranged_ref<const apr_byte_t>& buf);
    void write(apr_byte_t c) { mod_mysql_data_output::write(c); }
    operator mod_mysql_ranged_ref<apr_byte_t>();
    operator mod_mysql_ranged_ref<const apr_byte_t>();
};

// {{{ mod_mysql_data_buffer::mod_mysql_data_buffer()
inline mod_mysql_data_buffer::mod_mysql_data_buffer(
        const mod_mysql_ranged_ref<apr_byte_t>& _tgt)
        : tgt(_tgt), idx(0)
{
}
// }}}

// {{{ mod_mysql_data_buffer::write()
inline void mod_mysql_data_buffer::write(
        const mod_mysql_ranged_ref<const apr_byte_t>& buf)
{
    const int cnt = buf.count();
    if (idx + cnt >= tgt.count()) {
        throw mod_mysql_invalid_argument_exception("The container have not enough capacity to append the octets");
    }

    memmove(static_cast<apr_byte_t*>(tgt) + idx,
            static_cast<const apr_byte_t*>(buf), buf.get_size());

    idx += cnt;
}
// }}}

// {{{ mod_mysql_data_buffer::operator mod_mysql_ranged_ref<apr_byte_t>
inline mod_mysql_data_buffer::operator mod_mysql_ranged_ref<apr_byte_t>()
{
    return mod_mysql_ranged_ref<apr_byte_t>(tgt, idx);
}
// }}}

// {{{ mod_mysql_data_buffer::operator mod_mysql_ranged_ref<const apr_byte_t>
inline mod_mysql_data_buffer::operator mod_mysql_ranged_ref<const apr_byte_t>()
{
    return mod_mysql_ranged_ref<const apr_byte_t>(tgt, idx);
}
// }}}

#endif /* _MOD_MYSQL_DATA_BUFFER_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
