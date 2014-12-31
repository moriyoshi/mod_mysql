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
#ifndef _MOD_MYSQL_BUCKET_FACTORY_H
#define _MOD_MYSQL_BUCKET_FACTORY_H

#include <apr_buckets.h>
#include "mod_mysql_base.h"
#include "safety.h"

class mod_mysql_bucket_factory: public mod_mysql_base
{
private:
    apr_bucket_alloc_t* list;

public:
    mod_mysql_bucket_factory(apr_bucket_alloc_t* list);

#if APR_HAS_MMAP
    mod_mysql_bucket* create_mmap_bucket(apr_mmap_t*mm, apr_off_t start,
            apr_size_t len);
#endif
    mod_mysql_bucket* create_heap_bucket(const char* buf, apr_size_t len,
            void (*free_func)(void*));
    mod_mysql_bucket* create_eos_bucket();
    mod_mysql_bucket* create_flush_bucket();
    mod_mysql_bucket* create_immortal_bucket(const char* buf, apr_size_t len);
    mod_mysql_bucket* create_transient_bucket(const char* buf, apr_size_t len);
    mod_mysql_bucket* create_pool_bucket(const char* buf, apr_size_t len,
            apr_pool_t* pool);
    mod_mysql_bucket* create_socket_bucket(apr_socket_t* sock);
    mod_mysql_bucket* create_pipe_bucket(apr_file_t* pipe);
    mod_mysql_bucket* create_file_bucket(apr_file_t* fd, apr_off_t off,
            apr_size_t len, apr_pool_t* pool);
};

// {{{ mod_mysql_bucket_factory::mod_mysql_bucket_factory()
inline mod_mysql_bucket_factory::mod_mysql_bucket_factory(
        apr_bucket_alloc_t* _list)
        : list(_list)
{
}
// }}}

#if APR_HAS_MMAP
// {{{ mod_mysql_bucket_factory::create_mmap_bucket()
inline mod_mysql_bucket* mod_mysql_bucket_factory::create_mmap_bucket(
        apr_mmap_t* mm, apr_off_t start, apr_size_t len)
{
    return new(apr_bucket_mmap_create(mm, start, len, list))
            mod_mysql_bucket();
}
// }}}
#endif

// {{{ mod_mysql_bucket_factory::create_heap_bucket()
inline mod_mysql_bucket* mod_mysql_bucket_factory::create_heap_bucket(
        const char* buf, apr_size_t len, void (*free_func)(void*))
{
    return new(apr_bucket_heap_create(buf, len, free_func, list))
            mod_mysql_bucket();
}
// }}}

// {{{ mod_mysql_bucket_factory::create_eos_bucket()
inline mod_mysql_bucket* mod_mysql_bucket_factory::create_eos_bucket()
{
    return new(apr_bucket_eos_create(list)) mod_mysql_bucket();
}
// }}}

// {{{ mod_mysql_bucket_factory::create_flush_bucket()
inline mod_mysql_bucket* mod_mysql_bucket_factory::create_flush_bucket()
{
    return new(apr_bucket_flush_create(list))
            mod_mysql_bucket();
}
// }}}

// {{{ mod_mysql_bucket_factory::create_immortal_bucket()
inline mod_mysql_bucket* mod_mysql_bucket_factory::create_immortal_bucket(
        const char* buf, apr_size_t len)
{
    return new(apr_bucket_immortal_create(buf, len, list)) mod_mysql_bucket();
}
// }}}

// {{{ mod_mysql_bucket_factory::create_transient_bucket()
inline mod_mysql_bucket* mod_mysql_bucket_factory::create_transient_bucket(
        const char* buf, apr_size_t len)
{
    return new(apr_bucket_transient_create(buf, len, list)) mod_mysql_bucket();
}
// }}}

// {{{ mod_mysql_bucket_factory::create_pool_bucket()
inline mod_mysql_bucket* mod_mysql_bucket_factory::create_pool_bucket(
        const char* buf, apr_size_t len, apr_pool_t* pool)
{
    return new(apr_bucket_pool_create(buf, len, pool, list)) mod_mysql_bucket();
}
// }}}

// {{{ mod_mysql_bucket_factory::create_socket_bucket()
inline mod_mysql_bucket* mod_mysql_bucket_factory::create_socket_bucket(
        apr_socket_t* sock)
{
    return new(apr_bucket_socket_create(sock, list)) mod_mysql_bucket();
}
// }}}

// {{{ mod_mysql_bucket_factory::create_pipe_bucket()
inline mod_mysql_bucket* mod_mysql_bucket_factory::create_pipe_bucket(
        apr_file_t* pipe)
{
    return new(apr_bucket_pipe_create(pipe, list)) mod_mysql_bucket();
}
// }}}

// {{{ mod_mysql_bucket_factory::create_file_bucket()
inline mod_mysql_bucket* mod_mysql_bucket_factory::create_file_bucket(
        apr_file_t* fd, apr_off_t off, apr_size_t len, apr_pool_t* pool)
{
    return new(apr_bucket_file_create(fd, off, len, pool, list))
            mod_mysql_bucket();
}
// }}}

#endif /* _MOD_MYSQL_BUCKET_FACTORY_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */

