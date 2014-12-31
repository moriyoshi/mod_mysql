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
#ifndef _MOD_MYSQL_POOL_H
#define _MOD_MYSQL_POOL_H

#include <apr_pools.h>
#include <apr_strings.h>
#include <httpd.h>
#include <http_log.h>
#include "mod_mysql_base.h"
#include "mod_mysql_error.h"
#include "mod_mysql_pool_factory.h"
#include "mod_mysql_allocator.h"
#include "safety.h"

class mod_mysql_pool
        : public mod_mysql_base, public mod_mysql_pool_factory,
          public mod_mysql_allocator
{
private:
    static const char __dgt_key[];
    bool is_being_destroyed;
    apr_pool_t* pool;

public:
    static mod_mysql_pool* wrap(apr_pool_t* pool);
    static apr_status_t _unregister(mod_mysql_pool* self);
    static apr_status_t _cleanup(mod_mysql_pool* self);
    static apr_status_t _cleanup_child(mod_mysql_pool* self);

private:
    void register_myself();
    void* operator new(size_t sz, apr_pool_t* pool);
    mod_mysql_pool(apr_pool_t* pool);

public:
    mod_mysql_pool(mod_mysql_pool* parent, apr_allocator_t* ator = 0,
            apr_abortfunc_t abort_fn = 0);
    virtual ~mod_mysql_pool();

    virtual mod_mysql_pool* create_pool(apr_allocator_t* ator = 0,
            apr_abortfunc_t abort_fn = 0);

    static mod_mysql_pool* create_root_pool(apr_allocator_t* ator = 0,
            apr_abortfunc_t abort_fn = 0);

    bool is_ancestor(mod_mysql_pool* that);

    mod_mysql_pool* get_parent();
    operator apr_pool_t*();

    virtual void* alloc(size_t size);
    virtual void release(void* ptr);
    virtual void* realloc(void* old, size_t size);
    template<typename _T> _T* dup(const _T* p, apr_size_t nelems);
    void clear();
    void destroy();
    void tag(const char* name);

    void operator delete(void* p);
};

// {{{ mod_mysql_pool::mod_mysql_pool()
inline mod_mysql_pool::mod_mysql_pool(apr_pool_t* _pool)
        : pool(_pool), is_being_destroyed(false)
{
    register_myself();
}

inline mod_mysql_pool::mod_mysql_pool(mod_mysql_pool* parent,
        apr_allocator_t* ator, apr_abortfunc_t abort_fn)
        : is_being_destroyed(false)
{
    apr_status_t s;

    if ((s = apr_pool_create_ex(&pool, parent->pool, abort_fn, ator))) {
        throw mod_mysql_error(s, APLOG_MARK);
    }

    if ((s = apr_pool_userdata_set(this, __dgt_key, NULL, pool))) {
        throw mod_mysql_error(s, APLOG_MARK);
    }

    // we won't register any cleanup function because the pool
    // should be transient.
}
// }}}

// {{{ mod_mysql_pool::register_myself()
inline void mod_mysql_pool::register_myself()
{
    apr_status_t s;

    if ((s = apr_pool_userdata_set(this, __dgt_key,
            reinterpret_cast<apr_status_t(*)(void*)>(
                &mod_mysql_pool::_unregister), pool))) {
        throw mod_mysql_error(s, APLOG_MARK);
    }

    apr_pool_cleanup_register(pool, this,
            reinterpret_cast<apr_status_t(*)(void*)>(
                &mod_mysql_pool::_cleanup),
            reinterpret_cast<apr_status_t(*)(void*)>(
                &mod_mysql_pool::_cleanup_child));
}
// }}}

// {{{ mod_mysql_pool::is_ancestor()
inline bool mod_mysql_pool::is_ancestor(mod_mysql_pool* that)
{
    return apr_pool_is_ancestor(pool, that->pool);
}
// }}}

// {{{ mod_mysql_pool::get_parent()
inline mod_mysql_pool* mod_mysql_pool::get_parent()
{
    apr_pool_t* pp = apr_pool_parent_get(pool);

    mod_mysql_pool* retval;
    apr_status_t s;
    if ((s = apr_pool_userdata_get(reinterpret_cast<void**>(&retval),
            __dgt_key, pp))) {
        throw mod_mysql_error(s, APLOG_MARK);
    }

    if (!retval) {
        retval = new(pp) mod_mysql_pool(pp);
    }

    return retval;
}
// }}}

// {{{ mod_mysql_pool::operator apr_pool_t*()
inline mod_mysql_pool::operator apr_pool_t*()
{
    return pool;
}
// }}}

// {{{ mod_mysql_pool::dup<>()
template<typename _T>
inline _T* mod_mysql_pool::dup(const _T* p, apr_size_t nelems)
{
    return reinterpret_cast<_T*>(apr_pstrmemdup(pool,
            reinterpret_cast<const char*>(p),
            safe_size_t_multiply(sizeof(_T), nelems)));
}
// }}}

// {{{ mod_mysql_pool::clear()
inline void mod_mysql_pool::clear()
{
    apr_pool_clear(pool); 
}
// }}}

// {{{ mod_mysql_pool::destroy()
inline void mod_mysql_pool::destroy()
{
    apr_pool_destroy(pool);
}
// }}}

// {{{ mod_mysql_pool::tag()
inline void mod_mysql_pool::tag(const char* name)
{
    apr_pool_tag(pool, name);
}
// }}}

// {{{ mod_mysql_pool::operator new()
inline void* mod_mysql_pool::operator new(size_t sz, apr_pool_t* pool)
{
    return apr_palloc(pool, sz);
}
// }}}

// {{{ mod_mysql_pool::operator delete()
inline void mod_mysql_pool::operator delete(void* p)
{
    // do nothing
}
// }}}

// {{{ mod_mysql_pool::wrap()
inline mod_mysql_pool* mod_mysql_pool::wrap(apr_pool_t* pool)
{
    return new(pool) mod_mysql_pool(pool);
}
// }}}

#endif /* _MOD_MYSQL_POOL_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
