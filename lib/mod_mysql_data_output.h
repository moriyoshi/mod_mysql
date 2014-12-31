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
#ifndef _MOD_MYSQL_DATA_OUTPUT_H
#define _MOD_MYSQL_DATA_OUTPUT_H

#include <apr.h>

class mod_mysql_data_output
{
public:
    virtual void write(const mod_mysql_ranged_ref<const apr_byte_t>& buf) = 0;
    void write(apr_byte_t c);
    void writele(apr_uint16_t v);
    void writele(apr_uint32_t v);
    void writele(apr_uint64_t v);
    void writebe(apr_uint16_t v);
    void writebe(apr_uint32_t v);
    void writebe(apr_uint64_t v);
    void pad(apr_byte_t c, apr_size_t len);
};

// {{{ mod_mysql_data_output::write()
inline void mod_mysql_data_output::write(apr_byte_t c)
{
    write(mod_mysql_ranged_ref<const apr_byte_t>(&c, 1));
}
// }}}

// {{{ mod_mysql_data_output::writele()
inline void mod_mysql_data_output::writele(apr_uint16_t v)
{
    const apr_byte_t buf[2] = {
        v & 0xff,
        (v >> 8) & 0xff
    };
    write(mod_mysql_ranged_ref<const apr_byte_t>(
            buf, sizeof(buf) / sizeof(buf[0])));
}

inline void mod_mysql_data_output::writele(apr_uint32_t v)
{
    const apr_byte_t buf[4] = {
        v & 0xff,
        (v >> 8) & 0xff,
        (v >> 16) & 0xff,
        (v >> 24) & 0xff
    };
    write(mod_mysql_ranged_ref<const apr_byte_t>(
            buf, sizeof(buf) / sizeof(buf[0])));
}

inline void mod_mysql_data_output::writele(apr_uint64_t v)
{
    const apr_byte_t buf[8] = {
        v & 0xff,
        (v >> 8) & 0xff,
        (v >> 16) & 0xff,
        (v >> 24) & 0xff,
        (v >> 32) & 0xff,
        (v >> 40) & 0xff,
        (v >> 48) & 0xff,
        (v >> 56) & 0xff
    };
    write(mod_mysql_ranged_ref<const apr_byte_t>(
            buf, sizeof(buf) / sizeof(buf[0])));
}
// }}}

// {{{ mod_mysql_data_output::writebe()
inline void mod_mysql_data_output::writebe(apr_uint16_t v)
{
    const apr_byte_t buf[2] = {
        (v >> 8) & 0xff,
        v & 0xff
    };
    write(mod_mysql_ranged_ref<const apr_byte_t>(
            buf, sizeof(buf) / sizeof(buf[0])));
}

inline void mod_mysql_data_output::writebe(apr_uint32_t v)
{
    const apr_byte_t buf[4] = {
        (v >> 24) & 0xff,
        (v >> 16) & 0xff,
        (v >> 8) & 0xff,
        v & 0xff
    };
    write(mod_mysql_ranged_ref<const apr_byte_t>(
            buf, sizeof(buf) / sizeof(buf[0])));
}

inline void mod_mysql_data_output::writebe(apr_uint64_t v)
{
    const apr_byte_t buf[8] = {
        (v >> 56) & 0xff,
        (v >> 48) & 0xff,
        (v >> 40) & 0xff,
        (v >> 32) & 0xff,
        (v >> 24) & 0xff,
        (v >> 16) & 0xff,
        (v >> 8) & 0xff,
        v & 0xff
    };
    write(mod_mysql_ranged_ref<const apr_byte_t>(buf,
            sizeof(buf) / sizeof(buf[0])));
}
// }}}

// {{{ mod_mysql_data_output::pad()
inline void mod_mysql_data_output::pad(apr_byte_t c, apr_size_t sz)
{
    const apr_byte_t nilbytes[16] = {
        c, c, c, c, c, c, c, c, c, c, c, c, c, c, c, c
    };

    for (; sz > sizeof(nilbytes); sz -= sizeof(nilbytes)) {
        write(mod_mysql_ranged_ref<const apr_byte_t>(
            nilbytes, sizeof(nilbytes)));
    }

    write(mod_mysql_ranged_ref<const apr_byte_t>(nilbytes, sz));
}
// }}}
#endif /* _MOD_MYSQL_DATA_OUTPUT_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
