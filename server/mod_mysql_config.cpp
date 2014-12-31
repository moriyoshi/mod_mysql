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
extern "C" {
#include <apr_strings.h>
#include <ap_provider.h>
}
#include "mod_mysql.h"
#include "mod_mysql_config.h"
#include "mod_mysql_service.h"
#include "mod_mysql_pool.h"
#include "mod_mysql_command_handler.h"
#include "mod_mysql_statement_params.h"
#include "mod_mysql_client_info.h"
#include "mod_mysql_instantiable.h"
#include "common.h"

// {{{ mod_mysql_command_handler_impl
class mod_mysql_command_handler_impl
        : public mod_mysql_command_handler, public mod_mysql_base,
          public mod_mysql_instantiable
{
protected:
    mod_mysql_command_handler_t* dgt;

public:
    mod_mysql_command_handler_impl(mod_mysql_command_handler_t* dgt);

    virtual ~mod_mysql_command_handler_impl();
    virtual mod_mysql_result* handle_sleep(mod_mysql_pool* pool);
    virtual mod_mysql_result* handle_quit(mod_mysql_pool* pool);
    virtual mod_mysql_result* handle_init_db(mod_mysql_pool* pool,
            const apr_byte_t* db_name);
    virtual mod_mysql_result* handle_query(mod_mysql_pool* pool,
            const apr_byte_t* query);
    virtual mod_mysql_result* handle_field_list(mod_mysql_pool* pool,
            const apr_byte_t* table_name);
    virtual mod_mysql_result* handle_create_db(mod_mysql_pool* pool,
            const apr_byte_t* db_name);
    virtual mod_mysql_result* handle_drop_db(mod_mysql_pool* pool,
            const apr_byte_t* db_name);
    virtual mod_mysql_result* handle_refresh(mod_mysql_pool* pool,
            unsigned int options);
    virtual mod_mysql_result* handle_shutdown(mod_mysql_pool* pool,
            int level);
    virtual mod_mysql_result* handle_stat(mod_mysql_pool* pool);
    virtual mod_mysql_result* handle_proc_info(mod_mysql_pool* pool);
    virtual mod_mysql_result* handle_connect(mod_mysql_pool* pool);
    virtual mod_mysql_result* handle_proc_kill(mod_mysql_pool* pool,
            int thread_id);
    virtual mod_mysql_result* handle_debug(mod_mysql_pool* pool);
    virtual mod_mysql_result* handle_ping(mod_mysql_pool* pool);
    virtual mod_mysql_result* handle_time(mod_mysql_pool* pool);
    virtual mod_mysql_result* handle_delayed_insert(mod_mysql_pool* pool);
    virtual mod_mysql_result* handle_change_user(mod_mysql_pool* pool,
            const apr_byte_t *user);
    virtual mod_mysql_result* handle_binlog_dump(mod_mysql_pool* pool);
    virtual mod_mysql_result* handle_table_dump(mod_mysql_pool* pool);
    virtual mod_mysql_result* handle_register_slave(mod_mysql_pool* pool,
            apr_uint32_t server_id, const apr_byte_t *host,
            const apr_byte_t *user, const apr_byte_t *password, int port,
            apr_uint32_t recovery_rank);
    virtual mod_mysql_result* handle_prepare(mod_mysql_pool* pool,
            const apr_byte_t *query);
    virtual mod_mysql_result* handle_execute(mod_mysql_pool* pool,
            apr_uint32_t stmt, mod_mysql_statement_params* params);
    virtual mod_mysql_result* handle_close_stmt(mod_mysql_pool* pool,
            apr_uint32_t stmt);
    virtual mod_mysql_result* handle_reset_stmt(mod_mysql_pool* pool,
            apr_uint32_t stmt);
    virtual mod_mysql_result* handle_set_option(mod_mysql_pool* pool,
            int flags);
};

// {{{ mod_mysql_command_handler_impl::mod_mysql_command_handler_impl()
mod_mysql_command_handler_impl::mod_mysql_command_handler_impl(
        mod_mysql_command_handler_t* _dgt)
        : dgt(_dgt)
{
}
// }}}

// {{{ mod_mysql_command_handler_impl::~mod_mysql_command_handler_impl()
mod_mysql_command_handler_impl::~mod_mysql_command_handler_impl()
{
    mod_mysql_command_handler_destroy(dgt);
}
// }}}
// }}}

// {{{ mod_mysql_command_handler_impl::handle_sleep()
mod_mysql_result*
mod_mysql_command_handler_impl::handle_sleep(mod_mysql_pool* pool)
{
    mod_mysql_result_t* result = mod_mysql_command_handler_handle_sleep(
            dgt, *pool);

    if (result) {
        mod_mysql_pool *sub_pool = pool->create_pool();
        return new(sub_pool) mod_mysql_result(sub_pool, result);
    }

    return 0;
}
// }}}

// {{{ mod_mysql_command_handler_impl::handle_quit()
mod_mysql_result*
mod_mysql_command_handler_impl::handle_quit(mod_mysql_pool* pool)
{
    mod_mysql_result_t* result = mod_mysql_command_handler_handle_quit(
            dgt, *pool);

    if (result) {
        mod_mysql_pool *sub_pool = pool->create_pool();
        return new(sub_pool) mod_mysql_result(sub_pool, result);
    }

    return 0;
}
// }}}

// {{{ mod_mysql_command_handler_impl::handle_init_db()
mod_mysql_result*
mod_mysql_command_handler_impl::handle_init_db(mod_mysql_pool* pool,
        const apr_byte_t* db_name)
{
    mod_mysql_result_t* result = mod_mysql_command_handler_handle_init_db(
            dgt, *pool, db_name);

    if (result) {
        mod_mysql_pool *sub_pool = pool->create_pool();
        return new(sub_pool) mod_mysql_result(sub_pool, result);
    }

    return 0;
}
// }}}

// {{{ mod_mysql_command_handler_impl::handle_query()
mod_mysql_result*
mod_mysql_command_handler_impl::handle_query(mod_mysql_pool* pool,
        const apr_byte_t* query)
{
    mod_mysql_result_t* result = mod_mysql_command_handler_handle_query(
            dgt, *pool, query);

    if (result) {
        mod_mysql_pool *sub_pool = pool->create_pool();
        return new(sub_pool) mod_mysql_result(sub_pool, result);
    }

    return 0;
}
// }}}

// {{{ mod_mysql_command_handler_impl::handle_field_list()
mod_mysql_result*
mod_mysql_command_handler_impl::handle_field_list(mod_mysql_pool* pool,
        const apr_byte_t* table_name)
{
    mod_mysql_result_t* result = mod_mysql_command_handler_handle_field_list(
            dgt, *pool, table_name);

    if (result) {
        mod_mysql_pool *sub_pool = pool->create_pool();
        return new(sub_pool) mod_mysql_result(sub_pool, result);
    }

    return 0;
}
// }}}

// {{{ mod_mysql_command_handler_impl::handle_create_db()
mod_mysql_result*
mod_mysql_command_handler_impl::handle_create_db(mod_mysql_pool* pool,
        const apr_byte_t* db_name)
{
    mod_mysql_result_t* result = mod_mysql_command_handler_handle_create_db(
            dgt, *pool, db_name);

    if (result) {
        mod_mysql_pool *sub_pool = pool->create_pool();
        return new(sub_pool) mod_mysql_result(sub_pool, result);
    }

    return 0;
}
// }}}

// {{{ mod_mysql_command_handler_impl::handle_drop_db()
mod_mysql_result*
mod_mysql_command_handler_impl::handle_drop_db(mod_mysql_pool* pool,
        const apr_byte_t* db_name)
{
    mod_mysql_result_t* result = mod_mysql_command_handler_handle_create_db(
            dgt, *pool, db_name);

    if (result) {
        mod_mysql_pool *sub_pool = pool->create_pool();
        return new(sub_pool) mod_mysql_result(sub_pool, result);
    }

    return 0;
}
// }}}

// {{{ mod_mysql_command_handler_impl::handle_refresh()
mod_mysql_result*
mod_mysql_command_handler_impl::handle_refresh(mod_mysql_pool* pool,
        unsigned int options)
{
    mod_mysql_result_t* result = mod_mysql_command_handler_handle_refresh(
            dgt, *pool, options);

    if (result) {
        mod_mysql_pool *sub_pool = pool->create_pool();
        return new(sub_pool) mod_mysql_result(sub_pool, result);
    }

    return 0;
}
// }}}

// {{{ mod_mysql_command_handler_impl::handle_shutdown()
mod_mysql_result*
mod_mysql_command_handler_impl::handle_shutdown(mod_mysql_pool* pool,
        int level)
{
    mod_mysql_result_t* result = mod_mysql_command_handler_handle_shutdown(
            dgt, *pool, level);

    if (result) {
        mod_mysql_pool *sub_pool = pool->create_pool();
        return new(sub_pool) mod_mysql_result(sub_pool, result);
    }

    return 0;
}
// }}}

// {{{ mod_mysql_command_handler_impl::handle_stat()
mod_mysql_result*
mod_mysql_command_handler_impl::handle_stat(mod_mysql_pool* pool)
{
    mod_mysql_result_t* result = mod_mysql_command_handler_handle_stat(
            dgt, *pool);

    if (result) {
        mod_mysql_pool *sub_pool = pool->create_pool();
        return new(sub_pool) mod_mysql_result(sub_pool, result);
    }

    return 0;
}
// }}}

// {{{ mod_mysql_command_handler_impl::handle_proc_info()
mod_mysql_result*
mod_mysql_command_handler_impl::handle_proc_info(mod_mysql_pool* pool)
{
    mod_mysql_result_t* result = mod_mysql_command_handler_handle_proc_info(
            dgt, *pool);

    if (result) {
        mod_mysql_pool *sub_pool = pool->create_pool();
        return new(sub_pool) mod_mysql_result(sub_pool, result);
    }

    return 0;
}
// }}}

// {{{ mod_mysql_command_handler_impl::handle_connect()
mod_mysql_result*
mod_mysql_command_handler_impl::handle_connect(mod_mysql_pool* pool)
{
    mod_mysql_result_t* result = mod_mysql_command_handler_handle_connect(
            dgt, *pool);

    if (result) {
        mod_mysql_pool *sub_pool = pool->create_pool();
        return new(sub_pool) mod_mysql_result(sub_pool, result);
    }

    return 0;
}
// }}}

// {{{ mod_mysql_command_handler_impl::handle_proc_kill()
mod_mysql_result*
mod_mysql_command_handler_impl::handle_proc_kill(mod_mysql_pool* pool,
        int thread_id)
{
    mod_mysql_result_t* result = mod_mysql_command_handler_handle_proc_kill(
            dgt, *pool, thread_id);

    if (result) {
        mod_mysql_pool *sub_pool = pool->create_pool();
        return new(sub_pool) mod_mysql_result(sub_pool, result);
    }

    return 0;
}
// }}}

// {{{ mod_mysql_command_handler_impl::handle_debug()
mod_mysql_result*
mod_mysql_command_handler_impl::handle_debug(mod_mysql_pool* pool)
{
    mod_mysql_result_t* result = mod_mysql_command_handler_handle_debug(
            dgt, *pool);

    if (result) {
        mod_mysql_pool *sub_pool = pool->create_pool();
        return new(sub_pool) mod_mysql_result(sub_pool, result);
    }

    return 0;
}
// }}}

// {{{ mod_mysql_command_handler_impl::handle_ping()
mod_mysql_result*
mod_mysql_command_handler_impl::handle_ping(mod_mysql_pool* pool)
{
    mod_mysql_result_t* result = mod_mysql_command_handler_handle_debug(
            dgt, *pool);

    if (result) {
        mod_mysql_pool *sub_pool = pool->create_pool();
        return new(sub_pool) mod_mysql_result(sub_pool, result);
    }

    return 0;
}
// }}}

// {{{ mod_mysql_command_handler_impl::handle_time()
mod_mysql_result*
mod_mysql_command_handler_impl::handle_time(mod_mysql_pool* pool)
{
    mod_mysql_result_t* result = mod_mysql_command_handler_handle_time(
            dgt, *pool);

    if (result) {
        mod_mysql_pool *sub_pool = pool->create_pool();
        return new(sub_pool) mod_mysql_result(sub_pool, result);
    }

    return 0;
}
// }}}

// {{{ mod_mysql_command_handler_impl::handle_delayed_insert()
mod_mysql_result*
mod_mysql_command_handler_impl::handle_delayed_insert(mod_mysql_pool* pool)
{
    mod_mysql_result_t* result =
            mod_mysql_command_handler_handle_delayed_insert(dgt, *pool);

    if (result) {
        mod_mysql_pool *sub_pool = pool->create_pool();
        return new(sub_pool) mod_mysql_result(sub_pool, result);
    }

    return 0;
}
// }}}

// {{{ mod_mysql_command_handler_impl::handle_change_user()
mod_mysql_result*
mod_mysql_command_handler_impl::handle_change_user(mod_mysql_pool* pool,
        const apr_byte_t *user)
{
    mod_mysql_result_t* result = mod_mysql_command_handler_handle_change_user(
            dgt, *pool, user);

    if (result) {
        mod_mysql_pool *sub_pool = pool->create_pool();
        return new(sub_pool) mod_mysql_result(sub_pool, result);
    }

    return 0;
}
// }}}

// {{{ mod_mysql_command_handler_impl::handle_binlog_dump()
mod_mysql_result*
mod_mysql_command_handler_impl::handle_binlog_dump(mod_mysql_pool* pool)
{
    mod_mysql_result_t* result = mod_mysql_command_handler_handle_binlog_dump(
            dgt, *pool);

    if (result) {
        mod_mysql_pool *sub_pool = pool->create_pool();
        return new(sub_pool) mod_mysql_result(sub_pool, result);
    }

    return 0;
}
// }}}

// {{{ mod_mysql_command_handler_impl::handle_table_dump()
mod_mysql_result*
mod_mysql_command_handler_impl::handle_table_dump(mod_mysql_pool* pool)
{
    mod_mysql_result_t* result = mod_mysql_command_handler_handle_table_dump(
            dgt, *pool);

    if (result) {
        mod_mysql_pool *sub_pool = pool->create_pool();
        return new(sub_pool) mod_mysql_result(sub_pool, result);
    }

    return 0;
}
// }}}

// {{{ mod_mysql_command_handler_impl::handle_register_slave()
mod_mysql_result*
mod_mysql_command_handler_impl::handle_register_slave(mod_mysql_pool* pool,
            apr_uint32_t server_id, const apr_byte_t *host,
            const apr_byte_t *user, const apr_byte_t *password, int port,
            apr_uint32_t recovery_rank)
{
    mod_mysql_result_t* result =
            mod_mysql_command_handler_handle_register_slave(dgt, *pool,
                server_id, host, user, password, port, recovery_rank);

    if (result) {
        mod_mysql_pool *sub_pool = pool->create_pool();
        return new(sub_pool) mod_mysql_result(sub_pool, result);
    }

    return 0;
}
// }}}

// {{{ mod_mysql_command_handler_impl::handle_prepare()
mod_mysql_result* mod_mysql_command_handler_impl::handle_prepare(
        mod_mysql_pool* pool, const apr_byte_t *query)
{
    mod_mysql_result_t* result =
            mod_mysql_command_handler_handle_prepare_stmt(dgt, *pool, query);

    if (result) {
        mod_mysql_pool *sub_pool = pool->create_pool();
        return new(sub_pool) mod_mysql_result(sub_pool, result);
    }

    return 0;
}
// }}}

// {{{ mod_mysql_command_handler_impl::handle_execute()
mod_mysql_result*
mod_mysql_command_handler_impl::handle_execute(mod_mysql_pool* pool,
        apr_uint32_t stmt, mod_mysql_statement_params* params)
{
    mod_mysql_result_t* result =
            mod_mysql_command_handler_handle_execute_stmt(
                dgt, *pool, stmt, (mod_mysql_statement_params_t*)*params);

    if (result) {
        mod_mysql_pool *sub_pool = pool->create_pool();
        return new(sub_pool) mod_mysql_result(sub_pool, result);
    }

    return 0;
}
// }}}

// {{{ mod_mysql_command_handler_impl::handle_close_stmt()
mod_mysql_result*
mod_mysql_command_handler_impl::handle_close_stmt(mod_mysql_pool* pool,
        apr_uint32_t stmt)
{
    mod_mysql_result_t* result = mod_mysql_command_handler_handle_close_stmt(
            dgt, *pool, stmt);

    if (result) {
        mod_mysql_pool *sub_pool = pool->create_pool();
        return new(sub_pool) mod_mysql_result(sub_pool, result);
    }

    return 0;
}
// }}}

// {{{ mod_mysql_command_handler_impl::handle_reset_stmt()
mod_mysql_result*
mod_mysql_command_handler_impl::handle_reset_stmt(mod_mysql_pool* pool,
        apr_uint32_t stmt)
{
    mod_mysql_result_t* result = mod_mysql_command_handler_handle_reset_stmt(
            dgt, *pool, stmt);

    if (result) {
        mod_mysql_pool *sub_pool = pool->create_pool();
        return new(sub_pool) mod_mysql_result(sub_pool, result);
    }

    return 0;
}
// }}}

// {{{ mod_mysql_command_handler_impl::handle_set_option()
mod_mysql_result*
mod_mysql_command_handler_impl::handle_set_option(mod_mysql_pool* pool,
        int flags)
{
    mod_mysql_result_t* result = mod_mysql_command_handler_handle_set_option(
            dgt, *pool, flags);

    if (result) {
        mod_mysql_pool *sub_pool = pool->create_pool();
        return new(sub_pool) mod_mysql_result(sub_pool, result);
    }

    return 0;
}
// }}}
// }}}

// {{{ mod_mysql_service_impl
class mod_mysql_service_impl
        : public mod_mysql_service, public mod_mysql_base,
          public mod_mysql_instantiable
{
protected:
    mod_mysql_service_t* dgt;

public:
    mod_mysql_service_impl(mod_mysql_service_t* dgt);

    virtual mod_mysql_command_handler* create_command_handler(
            mod_mysql_pool* pool, mod_mysql_client_info* cinfo);
};

// {{{ mod_mysql_service_impl::mod_mysql_service_impl()
mod_mysql_service_impl::mod_mysql_service_impl(mod_mysql_service_t* _dgt)
        : dgt(_dgt)
{
}
// }}}

// {{{ mod_mysql_service_impl::create_command_handler()
mod_mysql_command_handler* mod_mysql_service_impl::create_command_handler(
        mod_mysql_pool* pool, mod_mysql_client_info* cinfo)
{
    mod_mysql_command_handler_t* hdlr = mod_mysql_service_create_command_handler(
            dgt, *pool, cinfo);
    if (!hdlr) {
        throw mod_mysql_exception("Cannot create query handler");
    }

    return new(pool) mod_mysql_command_handler_impl(hdlr);
}
// }}}
// }}}

// {{{ mod_mysql_config::mod_mysql_config()
mod_mysql_config::mod_mysql_config(mod_mysql_pool* _pool,
        const char* _provider_namespace, const char *_provider_version)
    : pool(_pool), provider_namespace(_provider_namespace),
      provider_version(_provider_version), srv(0),
      function_enabled(UNSET),
      max_allowed_packet(131072)
{
}

mod_mysql_config::mod_mysql_config(mod_mysql_pool* _pool,
        const mod_mysql_config& that)
    : pool(_pool), provider_namespace(that.provider_namespace),
      provider_version(that.provider_version), srv(0),
      function_enabled(that.function_enabled),
      max_allowed_packet(that.max_allowed_packet)
{
}
// }}}

// {{{ mod_mysql_config::merge()
mod_mysql_config* mod_mysql_config::merge(mod_mysql_pool* pool,
        const mod_mysql_config& child)
{
    mod_mysql_config *retval;

    retval = new(pool) mod_mysql_config(pool, *this);

    if (child.function_enabled != UNSET) {
        retval->function_enabled = child.function_enabled;
    }

    retval->max_allowed_packet = retval->max_allowed_packet == 131072 ?
            child.max_allowed_packet: 131072;
    retval->provider_namespace = child.provider_namespace;
    retval->provider_version = child.provider_version;

    return retval;
}
// }}}

// {{{ mod_mysql_config::is_function_enabled
bool mod_mysql_config::is_function_enabled() const
{
    return function_enabled == ON;
}
// }}}

// {{{ mod_mysql_config::set_function_enabled
void mod_mysql_config::set_function_enabled(bool flag)
{
    function_enabled = flag ? ON: OFF;
}
// }}}

// {{{ mod_mysql_config::get_max_allowed_packet
unsigned long mod_mysql_config::get_max_allowed_packet() const
{
    return max_allowed_packet;
}
// }}}

// {{{ mod_mysql_config::set_max_allowed_packet
void mod_mysql_config::set_max_allowed_packet(unsigned long len)
{
    max_allowed_packet = len;
}
// }}}

// {{{ mod_mysql_config::get_provider_name
const char* mod_mysql_config::get_provider_name() const
{
    return provider_name;
}
// }}}

// {{{ mod_mysql_config::set_provider_name
void mod_mysql_config::set_provider_name(const char* name)
{
    provider_name = name;
}
// }}}

// {{{ mod_mysql_config::get_service()
mod_mysql_service* mod_mysql_config::get_service()
{
    if (!srv) {
        mod_mysql_service_provider_t* srv_provider =
                reinterpret_cast<mod_mysql_service_provider_t*>(
                    ap_lookup_provider(provider_namespace,
                            provider_name, provider_version));
        if (!srv_provider) {
            throw mod_mysql_exception("Provider is not given");
        }
        srv = new(pool) mod_mysql_service_impl(
                mod_mysql_service_provider_create_service(
                    srv_provider, *pool));
    }

    return srv;
}
// }}}

// vim: sts=4 sw=4 ts=4 et fdm=marker
