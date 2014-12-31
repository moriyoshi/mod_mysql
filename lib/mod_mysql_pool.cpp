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
#include "mod_mysql_pool.h"
#include "mod_mysql_unsupported_exception.h"

const char mod_mysql_pool::__dgt_key[] =__FILE__;

// {{{ mod_mysql_pool::create_pool
mod_mysql_pool* mod_mysql_pool::create_pool(apr_allocator_t* ator,
        apr_abortfunc_t abort_fn)
{
    apr_status_t s;
    apr_pool_t* new_pool;

    if ((s = apr_pool_create_ex(&new_pool, pool, abort_fn, ator))) {
        throw mod_mysql_error(s, APLOG_MARK);
    }

    return new(new_pool) mod_mysql_pool(new_pool);
}
// }}}

// {{{ mod_mysql_pool::create_root_pool
mod_mysql_pool* mod_mysql_pool::create_root_pool(apr_allocator_t* ator,
        apr_abortfunc_t abort_fn)
{
    apr_status_t s;
    apr_pool_t* new_pool;

    if ((s = apr_pool_create_ex(&new_pool, 0, abort_fn, ator))) {
        throw mod_mysql_error(s, APLOG_MARK);
    }

    return new(new_pool) mod_mysql_pool(new_pool);
}
// }}}

// {{{ mod_mysql_pool::~mod_mysql_pool()
mod_mysql_pool::~mod_mysql_pool()
{
    if (!this->is_being_destroyed) {
        this->is_being_destroyed = true;
        destroy();
    }
}
// }}}

// {{{ mod_mysql_pool::_unregister()
apr_status_t mod_mysql_pool::_unregister(mod_mysql_pool* self)
{
    if (!self->is_being_destroyed) {
        self->is_being_destroyed = true;
        delete self;
    }
}
// }}}

// {{{ mod_mysql_pool::_cleanup()
apr_status_t mod_mysql_pool::_cleanup(mod_mysql_pool* self)
{
    if (!self->is_being_destroyed) {
        self->is_being_destroyed = true;
        delete self;
    }
}
// }}}

// {{{ mod_mysql_pool::_cleanup_child()
apr_status_t mod_mysql_pool::_cleanup_child(mod_mysql_pool* self)
{
    // do nothing for now
}
// }}}

// {{{ mod_mysql_pool::alloc()
void* mod_mysql_pool::alloc(size_t size)
{
    return apr_palloc(pool, size);
}
// }}}

// {{{ mod_mysql_pool::realloc()
void* mod_mysql_pool::realloc(void* old, size_t size)
{
    throw mod_mysql_unsupported_exception("Not implemented");
}
// }}}

// {{{ mod_mysql_pool::release()
void mod_mysql_pool::release(void* ptr)
{
    // do nothing
}
// }}}

// vim: sts=4 sw=4 ts=4 et fdm=marker
