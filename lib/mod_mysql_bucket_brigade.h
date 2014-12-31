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
#ifndef _MOD_MYSQL_BUCKET_BRIGADE_H
#define _MOD_MYSQL_BUCKET_BRIGADE_H

#include <apr_buckets.h>
#include "mod_mysql_base.h"
#include "mod_mysql_iterator.h"
#include "mod_mysql_error.h"
#include "mod_mysql_bucket.h"
#include "mod_mysql_instantiable.h"
#include "safety.h"

class mod_mysql_bucket_brigade
        : public mod_mysql_base,
          public mod_mysql_instantiable
{
public:
    class iterator
            : public mod_mysql_iterator<mod_mysql_bucket*>,
              public mod_mysql_instantiable,
              public mod_mysql_base
    {
    private:
        mod_mysql_bucket* cur;
        mod_mysql_bucket* nxt;
        mod_mysql_bucket* sentinel;
        bool removed;
    public:
        iterator(const mod_mysql_bucket_brigade* bb);
        virtual mod_mysql_bucket*& current();
        virtual bool valid();
        virtual void next();
        virtual void remove();
    };

protected:
    apr_brigade_flush brigade_flusher;
    apr_bucket_brigade* bb;

public:
    mod_mysql_bucket_brigade(apr_bucket_brigade* bb);
    ~mod_mysql_bucket_brigade();

    bool is_empty() const;
    mod_mysql_bucket* get_first();
    mod_mysql_bucket* get_last();
    apr_off_t get_length(int read_all = 1);

    void prepend(mod_mysql_bucket* e);
    void prepend(mod_mysql_bucket_brigade& that);
    void append(mod_mysql_bucket* e);
    void append(mod_mysql_bucket_brigade& that);
    void remove(mod_mysql_bucket* e);
    void cleanup();
    apr_size_t flatten(char *buf, apr_size_t buf_sz);
    mod_mysql_ranged_ref<apr_byte_t> flatten(
            const mod_mysql_ranged_ref<apr_byte_t>& ref);
    mod_mysql_bucket_brigade* split(const mod_mysql_bucket* e);
    mod_mysql_bucket_brigade* split(apr_off_t off);
    mod_mysql_bucket* partition(apr_off_t off);
    virtual mod_mysql_iterator<mod_mysql_bucket*>* create_iterator(
            mod_mysql_pool* pool) const;
    iterator create_iterator() const;
    void write(const char* str, apr_size_t nbyte);
    void write(const mod_mysql_ranged_ref<const apr_byte_t>& ref);
    void write(const char* str);
    void write(const unsigned char c);
    void writele(const unsigned short v);
    void writele(const unsigned long v);
    void writestrs(const char* str, ...);
    void printf(const char* fmt, ...);
};

// {{{ mod_mysql_bucket_brigade::mod_mysql_bucket_brigade()
inline mod_mysql_bucket_brigade::mod_mysql_bucket_brigade(
        apr_bucket_brigade *_bb)
        : bb(_bb), brigade_flusher(0)
{
}
// }}}

// {{{ mod_mysql_bucket_brigade::~mod_mysql_bucket_brigade()
inline mod_mysql_bucket_brigade::~mod_mysql_bucket_brigade()
{
    apr_brigade_destroy(bb);
}
// }}}

// {{{ mod_mysql_bucket_brigade::is_empty()
inline bool mod_mysql_bucket_brigade::is_empty() const
{
    return APR_BRIGADE_EMPTY(bb);
}
// }}}

// {{{ mod_mysql_bucket_brigade::get_first()
inline mod_mysql_bucket* mod_mysql_bucket_brigade::get_first()
{
    return new(APR_BRIGADE_FIRST(bb)) mod_mysql_bucket();
}
// }}}

// {{{ mod_mysql_bucket_brigade::get_last()
inline mod_mysql_bucket* mod_mysql_bucket_brigade::get_last()
{
    return new(APR_BRIGADE_FIRST(bb)) mod_mysql_bucket();
}
// }}}

// {{{ mod_mysql_bucket_brigade::prepend()
inline void mod_mysql_bucket_brigade::prepend(mod_mysql_bucket* e)
{
    APR_BRIGADE_INSERT_HEAD(bb, static_cast<apr_bucket*>(e));
}

inline void mod_mysql_bucket_brigade::prepend(mod_mysql_bucket_brigade& that)
{
    APR_BRIGADE_PREPEND(bb, that.bb);
}
// }}}

// {{{ mod_mysql_bucket_brigade::append()
inline void mod_mysql_bucket_brigade::append(mod_mysql_bucket* e)
{
    APR_BRIGADE_INSERT_TAIL(bb, static_cast<apr_bucket*>(e));
}

inline void mod_mysql_bucket_brigade::append(mod_mysql_bucket_brigade& that)
{
    APR_BRIGADE_PREPEND(bb, that.bb);
}
// }}}

// {{{ mod_mysql_bucket_brigade::remove()
inline void mod_mysql_bucket_brigade::remove(mod_mysql_bucket* e)
{
    apr_bucket_delete(e);
}
// }}}

// {{{ mod_mysql_bucket_brigade::cleanup()
inline void mod_mysql_bucket_brigade::cleanup()
{
    apr_status_t s;

    if ((s = apr_brigade_cleanup(bb))) {
        throw mod_mysql_error(s, APLOG_MARK);
    }
}
// }}}

// {{{ mod_mysql_bucket_brigade::flatten()
inline apr_size_t mod_mysql_bucket_brigade::flatten(char *buf,
        apr_size_t buf_sz)
{
    apr_status_t s;

    if ((s = apr_brigade_flatten(bb, buf, &buf_sz))) {
        throw mod_mysql_error(s, APLOG_MARK);
    }

    return buf_sz;
}

inline mod_mysql_ranged_ref<apr_byte_t> mod_mysql_bucket_brigade::flatten(
        const mod_mysql_ranged_ref<apr_byte_t>& ref)
{
    return mod_mysql_ranged_ref<apr_byte_t>(
            ref.get_referent(),
            flatten(reinterpret_cast<char *>(ref.get_referent()),
                ref.get_size()));
}
// }}}

// {{{ mod_mysql_bucket_brigade::partition()
inline mod_mysql_bucket* mod_mysql_bucket_brigade::partition(
        apr_off_t off)
{
    apr_bucket* new_bucket;
    apr_status_t s;

    if ((s = apr_brigade_partition(bb, off, &new_bucket))) {
        throw mod_mysql_error(s, APLOG_MARK);
    }

    return static_cast<mod_mysql_bucket*>(new_bucket);
}
// }}}

// {{{ mod_mysql_bucket_brigade::split()
inline mod_mysql_bucket_brigade* mod_mysql_bucket_brigade::split(
        const mod_mysql_bucket* e)
{
    return new(mod_mysql_pool::wrap(bb->p)) mod_mysql_bucket_brigade(
            apr_brigade_split(bb, const_cast<apr_bucket*>(
            static_cast<const apr_bucket*>(e))));
}

inline mod_mysql_bucket_brigade* mod_mysql_bucket_brigade::split(
        apr_off_t off)
{
    return split(partition(off));
}
// }}}

// {{{ mod_mysql_bucket_brigade::get_length()
inline apr_off_t mod_mysql_bucket_brigade::get_length(int read_all)
{
    apr_status_t s;
    apr_off_t len;

    if ((s = apr_brigade_length(bb, read_all, &len))) {
        throw mod_mysql_error(s, APLOG_MARK);
    }

    return len;
}
// }}}

// {{{ mod_mysql_bucket_brigade::write()
inline void mod_mysql_bucket_brigade::write(const char *str, apr_size_t nbyte)
{
    apr_status_t s;

    if ((s = apr_brigade_write(bb, brigade_flusher, this, str, nbyte))) {
        throw mod_mysql_error(s, APLOG_MARK);
    }
}

inline void mod_mysql_bucket_brigade::write(
        const mod_mysql_ranged_ref<const apr_byte_t>& ref)
{
    write(reinterpret_cast<const char*>(ref.get_referent()),
            ref.get_size());
}

inline void mod_mysql_bucket_brigade::write(const char *str)
{
    apr_status_t s;

    if ((s = apr_brigade_puts(bb, brigade_flusher, this, str))) {
        throw mod_mysql_error(s, APLOG_MARK);
    }
}

inline void mod_mysql_bucket_brigade::write(const unsigned char c)
{
    apr_status_t s;

    if ((s = apr_brigade_putc(bb, brigade_flusher, this, c))) {
        throw mod_mysql_error(s, APLOG_MARK);
    }
}
// }}}

// {{{ mod_mysql_bucket_brigade::writele()
inline void mod_mysql_bucket_brigade::writele(const unsigned short v)
{
    const unsigned char buf[2] = { v, v >> 8 };
    write(reinterpret_cast<const char *>(buf), sizeof(buf));
}

inline void mod_mysql_bucket_brigade::writele(const unsigned long v)
{
    const unsigned char buf[4] = { v, v >> 8, v >> 16, v >> 24 };
    write(reinterpret_cast<const char *>(buf), sizeof(buf));
}
// }}}

// {{{ mod_mysql_bucket_brigade::writestrs()
inline void mod_mysql_bucket_brigade::writestrs(const char *str, ...)
{
    apr_status_t s;
    va_list args;

    if (!str) {
        return;
    }

    this->write(str);

    va_start(args, str);

    s = apr_brigade_vputstrs(bb, brigade_flusher, this, args);
    va_end(args);

    if (s) {
        throw mod_mysql_error(s, APLOG_MARK);
    }
}
// }}}

// {{{ mod_mysql_bucket_brigade::printf()
inline void mod_mysql_bucket_brigade::printf(const char *fmt, ...)
{
    apr_status_t s;
    va_list args;
    va_start(args, fmt);

    s = apr_brigade_vprintf(bb, brigade_flusher, this, fmt, args);
    va_end(args);

    if (s) {
        throw mod_mysql_error(s, APLOG_MARK);
    }
}
// }}}

// {{{ mod_mysql_bucket_brigade::create_iterator()
inline mod_mysql_bucket_brigade::iterator
mod_mysql_bucket_brigade::create_iterator() const
{
    return iterator(this); 
}
// }}}

#endif /* _MOD_MYSQL_BUCKET_BRIGADE_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
