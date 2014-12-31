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
#ifndef _MOD_MYSQL_BUCKET_H
#define _MOD_MYSQL_BUCKET_H

#include <httpd.h>
#include <http_log.h>
#include <apr_buckets.h>
#include "mod_mysql_base.h"
#include "mod_mysql_error.h"
#include "safety.h"

class mod_mysql_bucket: public apr_bucket
{
public:
    void* operator new(size_t, apr_bucket* b);
    mod_mysql_bucket();
    mod_mysql_bucket(const apr_bucket& that);
    ~mod_mysql_bucket();

    const char* get_type_name() const;
    bool is_metadata() const;
    bool is_eos() const;
    bool is_flush() const;
    bool is_pipe() const;
    bool is_file() const;
    bool is_socket() const;
    bool is_heap() const;
    bool is_transient() const;
    bool is_immortal() const;
    bool is_pool() const;
    bool is_mmap() const;

    mod_mysql_bucket* get_next() const;
    mod_mysql_bucket* get_prev() const;
    void unlink();
    void read(const char** str, apr_size_t* len, apr_read_type_e to_block);
    void setaside(apr_pool_t* p);
    void split(apr_size_t off);
    mod_mysql_bucket* copy();
    void destroy();
};

// {{{ mod_mysql_bucket::operator new()
inline void* mod_mysql_bucket::operator new(size_t sz, apr_bucket* b)
{
    return b;
}
// }}}

// {{{ mod_mysql_bucket::mod_mysql_bucket()
inline mod_mysql_bucket::mod_mysql_bucket()
{
}

inline mod_mysql_bucket::mod_mysql_bucket(const apr_bucket& that)
{
    *this = that; 
}
// }}}

// {{{ mod_mysql_bucket::~mod_mysql_bucket()
inline mod_mysql_bucket::~mod_mysql_bucket()
{
    // do nothing
}
// }}}

// {{{ mod_mysql_bucket::get_type_name()
inline const char* mod_mysql_bucket::get_type_name() const
{
    return type->name;
}
// }}}

// {{{ mod_mysql_bucket::is_metadata()
inline bool mod_mysql_bucket::is_metadata() const
{
    return APR_BUCKET_IS_METADATA(this);
}
// }}}

// {{{ mod_mysql_bucket::is_flush()
inline bool mod_mysql_bucket::is_flush() const
{
    return APR_BUCKET_IS_FLUSH(this);
}
// }}}

// {{{ mod_mysql_bucket::is_eos()
inline bool mod_mysql_bucket::is_eos() const
{
    return APR_BUCKET_IS_EOS(this);
}
// }}}

// {{{ mod_mysql_bucket::is_file()
inline bool mod_mysql_bucket::is_file() const
{
    return APR_BUCKET_IS_FILE(this);
}
// }}}

// {{{ mod_mysql_bucket::is_pipe()
inline bool mod_mysql_bucket::is_pipe() const 
{
    return APR_BUCKET_IS_PIPE(this);
}
// }}}

// {{{ mod_mysql_bucket::is_socket()
inline bool mod_mysql_bucket::is_socket() const
{
    return APR_BUCKET_IS_SOCKET(this);
}
// }}}

// {{{ mod_mysql_bucket::is_heap()
inline bool mod_mysql_bucket::is_heap() const
{
    return APR_BUCKET_IS_HEAP(this);
}
// }}}

// {{{ mod_mysql_bucket::is_transient()
inline bool mod_mysql_bucket::is_transient() const
{
    return APR_BUCKET_IS_TRANSIENT(this);
}
// }}}

// {{{ mod_mysql_bucket::is_immortal()
inline bool mod_mysql_bucket::is_immortal() const
{
    return APR_BUCKET_IS_IMMORTAL(this);
}
// }}}

// {{{ mod_mysql_bucket::is_pool()
inline bool mod_mysql_bucket::is_pool() const
{
    return APR_BUCKET_IS_POOL(this);
}
// }}}

// {{{ mod_mysql_bucket::is_mmap()
inline bool mod_mysql_bucket::is_mmap() const
{
#if APR_HAS_MMAP
    return APR_BUCKET_IS_MMAP(this);
#else
    return false;
#endif
}
// }}}

// {{{ mod_mysql_bucket::destroy()
inline void mod_mysql_bucket::destroy()
{
    apr_bucket_destroy(this);
}
// }}}

// {{{ mod_mysql_bucket::read()
inline void mod_mysql_bucket::read(const char** str, apr_size_t* len,
        apr_read_type_e to_block)
{
    apr_status_t s;

    if ((s = apr_bucket_read(this, str, len, to_block))) {
        throw mod_mysql_error(s, APLOG_MARK);
    }
}
// }}}

// {{{ mod_mysql_bucket::setaside()
inline void mod_mysql_bucket::setaside(apr_pool_t* p)
{
    apr_status_t s;

    if ((s = apr_bucket_setaside(this, p))) {
        throw mod_mysql_error(s, APLOG_MARK);
    }
}
// }}}

// {{{ mod_mysql_bucket::split()
inline void mod_mysql_bucket::split(apr_size_t off)
{
    apr_status_t s;

    if ((s = apr_bucket_split(this, off))) {
        throw mod_mysql_error(s, APLOG_MARK);
    }
}
// }}}

// {{{ mod_mysql_bucket::copy()
inline mod_mysql_bucket* mod_mysql_bucket::copy()
{
    apr_status_t s;
    apr_bucket* new_one;

    if ((s = apr_bucket_copy(this, &new_one))) {
        throw mod_mysql_error(s, APLOG_MARK);
    }

    return new(new_one) mod_mysql_bucket(); 
}
// }}}

// {{{ mod_mysql_bucket::get_next()
inline mod_mysql_bucket* mod_mysql_bucket::get_next() const
{
    return static_cast<mod_mysql_bucket*>(APR_BUCKET_NEXT(this));
}
// }}}

// {{{ mod_mysql_bucket::get_prev()
inline mod_mysql_bucket* mod_mysql_bucket::get_prev() const
{
    return static_cast<mod_mysql_bucket*>(APR_BUCKET_PREV(this));
}
// }}}

// {{{ mod_mysql_bucket::unlink()
inline void mod_mysql_bucket::unlink()
{
    APR_BUCKET_REMOVE(this);
}
// }}}

#endif /* _MOD_MYSQL_BUCKET_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */

