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
#ifndef _MOD_MYSQL_COMMAND_HANDLER_H
#define _MOD_MYSQL_COMMAND_HANDLER_H

#include <apr_pools.h>

#include "mod_mysql_base.h"
#include "mod_mysql_result.h"

#ifndef MOD_MYSQL_COMMAND_HANDLER_T_DEFINED
#define MOD_MYSQL_COMMAND_HANDLER_T_DEFINED
typedef struct mod_mysql_command_handler_t mod_mysql_command_handler_t;
#endif /* MOD_MYSQL_COMMAND_HANDLER_T_DEFINED */

#ifndef MOD_MYSQL_STATEMENT_PARAMS_T_DEFINED
#define MOD_MYSQL_STATEMENT_PARAMS_T_DEFINED
typedef struct mod_mysql_statement_params_t mod_mysql_statement_params_t;
#endif /* MOD_MYSQL_STATEMENT_PARAMS_T_DEFINED */

typedef struct mod_mysql_command_handler_vtbl_t
{
    void (*destroy_fn)(mod_mysql_command_handler_t *self);
    /* MOD_MYSQL_CMD_SLEEP */
    mod_mysql_result_t *(*handle_sleep_fn)(
            mod_mysql_command_handler_t *self, apr_pool_t *pool);
    /* MOD_MYSQL_CMD_QUIT */
    mod_mysql_result_t *(*handle_quit_fn)(
            mod_mysql_command_handler_t *self, apr_pool_t *pool);
    /* MOD_MYSQL_CMD_INIT_DB */
    mod_mysql_result_t *(*handle_init_db_fn)(
            mod_mysql_command_handler_t *self, apr_pool_t *pool,
            const apr_byte_t *db_name);
    /* MOD_MYSQL_CMD_QUERY */
    mod_mysql_result_t *(*handle_query_fn)(mod_mysql_command_handler_t *self,
            apr_pool_t *pool, const apr_byte_t *query);
    /* MOD_MYSQL_FIELD_LIST */
    mod_mysql_result_t *(*handle_field_list_fn)(
            mod_mysql_command_handler_t *self, apr_pool_t *pool,
            const apr_byte_t *table_name);
    /* MOD_MYSQL_CREATE_DB */
    mod_mysql_result_t *(*handle_create_db_fn)(
            mod_mysql_command_handler_t *self, apr_pool_t *pool,
            const apr_byte_t *db_name);
    /* MOD_MYSQL_DROP_DB */
    mod_mysql_result_t *(*handle_drop_db_fn)(
            mod_mysql_command_handler_t *self, apr_pool_t *pool,
            const apr_byte_t *db_name);
    /* MOD_MYSQL_REFRESH */
    mod_mysql_result_t *(*handle_refresh_fn)(
            mod_mysql_command_handler_t *self, apr_pool_t *pool,
            unsigned int options);
    /* MOD_MYSQL_SHUTDOWN */
    mod_mysql_result_t *(*handle_shutdown_fn)(
            mod_mysql_command_handler_t *self, apr_pool_t *pool,
            int level);
    /* MOD_MYSQL_STATISTICS */
    mod_mysql_result_t *(*handle_stat_fn)(
            mod_mysql_command_handler_t *self, apr_pool_t *pool);
    /* MOD_MYSQL_PROCESS_INFO */
    mod_mysql_result_t *(*handle_proc_info_fn)(
            mod_mysql_command_handler_t *self, apr_pool_t *pool);
    /* MOD_MYSQL_CONNECT */
    mod_mysql_result_t *(*handle_connect_fn)(
            mod_mysql_command_handler_t *self, apr_pool_t *pool);
    /* MOD_MYSQL_PROCESS_KILL */
    mod_mysql_result_t *(*handle_proc_kill_fn)(
            mod_mysql_command_handler_t *self, apr_pool_t *pool,
            int thread_id);
    /* MOD_MYSQL_DEBUG */
    mod_mysql_result_t *(*handle_debug_fn)(
            mod_mysql_command_handler_t *self, apr_pool_t *pool);
    /* MOD_MYSQL_PING */
    mod_mysql_result_t *(*handle_ping_fn)(
            mod_mysql_command_handler_t *self, apr_pool_t *pool);
    /* MOD_MYSQL_TIME */
    mod_mysql_result_t *(*handle_time_fn)(
            mod_mysql_command_handler_t *self, apr_pool_t *pool);
    /* MOD_MYSQL_DELAYED_INSERT */
    mod_mysql_result_t *(*handle_delayed_insert_fn)(
            mod_mysql_command_handler_t *self, apr_pool_t *pool);
    /* MOD_MYSQL_CHANGE_USER */
    mod_mysql_result_t *(*handle_change_user_fn)(
            mod_mysql_command_handler_t *self, apr_pool_t *pool,
            const apr_byte_t *user);
    /* MOD_MYSQL_BINLOG_DUMP */
    mod_mysql_result_t *(*handle_binlog_dump_fn)(
            mod_mysql_command_handler_t *self, apr_pool_t *pool);
    /* MOD_MYSQL_TABLE_DUMP */
    mod_mysql_result_t *(*handle_table_dump_fn)(
            mod_mysql_command_handler_t *self, apr_pool_t *pool);
    /* MOD_MYSQL_REGISTER_SLAVE */
    mod_mysql_result_t *(*handle_register_slave_fn)(
            mod_mysql_command_handler_t *self, apr_pool_t *pool,
            apr_uint32_t server_id, const apr_byte_t *host,
            const apr_byte_t *user, const apr_byte_t *password, int port,
            apr_uint32_t recovery_rank);
    /* MOD_MYSQL_PREPARE */
    mod_mysql_result_t *(*handle_prepare_stmt_fn)(
            mod_mysql_command_handler_t *self, apr_pool_t *pool,
            const apr_byte_t *query);
    /* MOD_MYSQL_EXECUTE */
    mod_mysql_result_t *(*handle_execute_stmt_fn)(
            mod_mysql_command_handler_t *self, apr_pool_t *pool,
            apr_uint32_t stmt, mod_mysql_statement_params_t *params);
    /* MOD_MYSQL_CLOSE_STMT */
    mod_mysql_result_t *(*handle_close_stmt_fn)(
            mod_mysql_command_handler_t *self, apr_pool_t *pool,
            apr_uint32_t stmt);
    /* MOD_MYSQL_RESET_STMT */
    mod_mysql_result_t *(*handle_reset_stmt_fn)(
            mod_mysql_command_handler_t *self, apr_pool_t *pool,
            apr_uint32_t stmt);
    /* MOD_MYSQL_SET_OPTION */
    mod_mysql_result_t *(*handle_set_option_fn)(
            mod_mysql_command_handler_t *self, apr_pool_t *pool,
            int flags);
} mod_mysql_command_handler_vtbl_t;

struct mod_mysql_command_handler_t
{
    mod_mysql_command_handler_vtbl_t *_vtbl;
};

#define mod_mysql_command_handler_destroy(self) \
        ((mod_mysql_command_handler_t *)(self))->_vtbl->destroy_fn( \
            (mod_mysql_command_handler_t *)(self))

#define mod_mysql_command_handler_handle_sleep(self, pool) \
        ((mod_mysql_command_handler_t *)(self))->_vtbl->handle_sleep_fn( \
            (mod_mysql_command_handler_t *)(self), \
            pool)

#define mod_mysql_command_handler_handle_quit(self, pool) \
        ((mod_mysql_command_handler_t *)(self))->_vtbl->handle_quit_fn( \
            (mod_mysql_command_handler_t *)(self), \
            pool)

#define mod_mysql_command_handler_handle_init_db(self, pool, db_name) \
        ((mod_mysql_command_handler_t *)(self))->_vtbl->handle_init_db_fn( \
            (mod_mysql_command_handler_t *)(self), \
            pool, db_name)

#define mod_mysql_command_handler_handle_query(self, pool, command) \
        ((mod_mysql_command_handler_t *)(self))->_vtbl->handle_query_fn( \
            (mod_mysql_command_handler_t *)(self), \
            pool, command)

#define mod_mysql_command_handler_handle_field_list(self, pool, table_name) \
        ((mod_mysql_command_handler_t *)(self))->_vtbl->handle_field_list_fn( \
            (mod_mysql_command_handler_t *)(self), \
            pool, table_name)

#define mod_mysql_command_handler_handle_create_db(self, pool, db_name) \
        ((mod_mysql_command_handler_t *)(self))->_vtbl->handle_create_db_fn( \
            (mod_mysql_command_handler_t *)(self), \
            pool, db_name)

#define mod_mysql_command_handler_handle_drop_db(self, pool, db_name) \
        ((mod_mysql_command_handler_t *)(self))->_vtbl->handle_drop_db_fn( \
            (mod_mysql_command_handler_t *)(self), \
            pool, db_name)

#define mod_mysql_command_handler_handle_refresh(self, pool, opts) \
        ((mod_mysql_command_handler_t *)(self))->_vtbl->handle_refresh_fn( \
            (mod_mysql_command_handler_t *)(self), \
            pool, opts)

#define mod_mysql_command_handler_handle_shutdown(self, pool, level) \
        ((mod_mysql_command_handler_t *)(self))->_vtbl->handle_shutdown_fn( \
            (mod_mysql_command_handler_t *)(self), \
            pool, level)

#define mod_mysql_command_handler_handle_stat(self, pool) \
        ((mod_mysql_command_handler_t *)(self))->_vtbl->handle_stat_fn( \
            (mod_mysql_command_handler_t *)(self), \
            pool)

#define mod_mysql_command_handler_handle_proc_info(self, pool) \
        ((mod_mysql_command_handler_t *)(self))->_vtbl->handle_proc_info_fn( \
            (mod_mysql_command_handler_t *)(self), \
            pool)

#define mod_mysql_command_handler_handle_connect(self, pool) \
        ((mod_mysql_command_handler_t *)(self))->_vtbl->handle_connect_fn( \
            (mod_mysql_command_handler_t *)(self), \
            pool)

#define mod_mysql_command_handler_handle_proc_kill(self, pool, tid) \
        ((mod_mysql_command_handler_t *)(self))->_vtbl->handle_proc_kill_fn( \
            (mod_mysql_command_handler_t *)(self), \
            pool, tid)

#define mod_mysql_command_handler_handle_debug(self, pool) \
        ((mod_mysql_command_handler_t *)(self))->_vtbl->handle_debug_fn( \
            (mod_mysql_command_handler_t *)(self), \
            pool)

#define mod_mysql_command_handler_handle_ping(self, pool) \
        ((mod_mysql_command_handler_t *)(self))->_vtbl->handle_ping_fn( \
            (mod_mysql_command_handler_t *)(self), \
            pool)

#define mod_mysql_command_handler_handle_time(self, pool) \
        ((mod_mysql_command_handler_t *)(self))->_vtbl->handle_time_fn( \
            (mod_mysql_command_handler_t *)(self), \
            pool)

#define mod_mysql_command_handler_handle_delayed_insert(self, pool) \
        ((mod_mysql_command_handler_t *)(self))->_vtbl \
            ->handle_delayed_insert_fn( \
            (mod_mysql_command_handler_t *)(self), \
            pool)

#define mod_mysql_command_handler_handle_change_user(self, pool, user) \
        ((mod_mysql_command_handler_t *)(self))->_vtbl \
            ->handle_change_user_fn( \
            (mod_mysql_command_handler_t *)(self), \
            pool, user)

#define mod_mysql_command_handler_handle_binlog_dump(self, pool) \
        ((mod_mysql_command_handler_t *)(self))->_vtbl->handle_binlog_dump_fn( \
            (mod_mysql_command_handler_t *)(self), \
            pool)

#define mod_mysql_command_handler_handle_table_dump(self, pool) \
        ((mod_mysql_command_handler_t *)(self))->_vtbl->handle_table_dump_fn( \
            (mod_mysql_command_handler_t *)(self), \
            pool)

#define mod_mysql_command_handler_handle_register_slave(self, pool, server_id, host, user, password, port, recovery_rank) \
        ((mod_mysql_command_handler_t *)(self))->_vtbl \
            ->handle_register_slave_fn( \
            (mod_mysql_command_handler_t *)(self), \
            pool, server_id, host, user, password, port, recovery_rank)

#define mod_mysql_command_handler_handle_prepare_stmt(self, pool, query) \
        ((mod_mysql_command_handler_t *)(self))->_vtbl \
            ->handle_prepare_stmt_fn( \
            (mod_mysql_command_handler_t *)(self), \
            pool, query)

#define mod_mysql_command_handler_handle_execute_stmt(self, pool, stmt, params)\
        ((mod_mysql_command_handler_t *)(self))->_vtbl \
            ->handle_execute_stmt_fn( \
            (mod_mysql_command_handler_t *)(self), \
            pool, stmt, params)

#define mod_mysql_command_handler_handle_close_stmt(self, pool, stmt) \
        ((mod_mysql_command_handler_t *)(self))->_vtbl \
            ->handle_close_stmt_fn( \
            (mod_mysql_command_handler_t *)(self), \
            pool, stmt)

#define mod_mysql_command_handler_handle_reset_stmt(self, pool, stmt) \
        ((mod_mysql_command_handler_t *)(self))->_vtbl \
            ->handle_reset_stmt_fn( \
            (mod_mysql_command_handler_t *)(self), \
            pool, stmt)

#define mod_mysql_command_handler_handle_set_option(self, pool, flags) \
        ((mod_mysql_command_handler_t *)(self))->_vtbl->handle_set_option_fn( \
            (mod_mysql_command_handler_t *)(self), \
            pool, flags)


#ifdef __cplusplus
class mod_mysql_statement_params;

class mod_mysql_command_handler
{
public:
    virtual ~mod_mysql_command_handler();

    virtual mod_mysql_result* handle_sleep(mod_mysql_pool* pool) = 0;
    virtual mod_mysql_result* handle_quit(mod_mysql_pool* pool) = 0;
    virtual mod_mysql_result* handle_init_db(mod_mysql_pool* pool,
            const apr_byte_t* db_name) = 0;
    virtual mod_mysql_result* handle_query(mod_mysql_pool* pool,
            const apr_byte_t* query) = 0;
    virtual mod_mysql_result* handle_field_list(mod_mysql_pool* pool,
            const apr_byte_t* table_name) = 0;
    virtual mod_mysql_result* handle_create_db(mod_mysql_pool* pool,
            const apr_byte_t* db_name) = 0;
    virtual mod_mysql_result* handle_drop_db(mod_mysql_pool* pool,
            const apr_byte_t* db_name) = 0;
    virtual mod_mysql_result* handle_refresh(mod_mysql_pool* pool,
            unsigned int options) = 0;
    virtual mod_mysql_result* handle_shutdown(mod_mysql_pool* pool,
            int level) = 0;
    virtual mod_mysql_result* handle_stat(mod_mysql_pool* pool) = 0;
    virtual mod_mysql_result* handle_proc_info(mod_mysql_pool* pool) = 0;
    virtual mod_mysql_result* handle_connect(mod_mysql_pool* pool) = 0;
    virtual mod_mysql_result* handle_proc_kill(mod_mysql_pool* pool,
            int thread_id) = 0;
    virtual mod_mysql_result* handle_debug(mod_mysql_pool* pool) = 0;
    virtual mod_mysql_result* handle_ping(mod_mysql_pool* pool) = 0;
    virtual mod_mysql_result* handle_time(mod_mysql_pool* pool) = 0;
    virtual mod_mysql_result* handle_delayed_insert(mod_mysql_pool* pool) = 0;
    virtual mod_mysql_result* handle_change_user(mod_mysql_pool* pool,
            const apr_byte_t *user) = 0;
    virtual mod_mysql_result* handle_binlog_dump(mod_mysql_pool* pool) = 0;
    virtual mod_mysql_result* handle_table_dump(mod_mysql_pool* pool) = 0;
    virtual mod_mysql_result* handle_register_slave(mod_mysql_pool* pool,
            apr_uint32_t server_id, const apr_byte_t *host,
            const apr_byte_t *user, const apr_byte_t *password, int port,
            apr_uint32_t recovery_rank) = 0;
    virtual mod_mysql_result* handle_prepare(mod_mysql_pool* pool,
            const apr_byte_t *query) = 0;
    virtual mod_mysql_result* handle_execute(mod_mysql_pool* pool,
            apr_uint32_t stmt, mod_mysql_statement_params* params) = 0;
    virtual mod_mysql_result* handle_close_stmt(mod_mysql_pool* pool,
            apr_uint32_t stmt) = 0;
    virtual mod_mysql_result* handle_reset_stmt(mod_mysql_pool* pool,
            apr_uint32_t stmt) = 0;
    virtual mod_mysql_result* handle_set_option(mod_mysql_pool* pool,
            int flags) = 0;
};
#endif

#endif /* _MOD_MYSQL_COMMAND_HANDLER_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
