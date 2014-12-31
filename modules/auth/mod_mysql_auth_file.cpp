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
#include "mod_mysql.h"
#include "mod_mysql_config_handler.h"
#include "mod_mysql_client_info.h"
#include "mod_mysql_authenticator.h"
#include "mod_mysql_std_logger.h"
#include "mod_mysql_pool.h"

extern "C" module AP_MODULE_DECLARE_DATA mysql_auth_file_module;

// {{{ class mod_mysql_auth_file_config
class mod_mysql_auth_file_config
        : public mod_mysql_base, public mod_mysql_instantiable
{
private:
    mod_mysql_pool* pool;

    const char* filename;

public:
    mod_mysql_auth_file_config(mod_mysql_pool* pool);
    mod_mysql_auth_file_config(mod_mysql_pool* pool,
            const mod_mysql_auth_file_config& override);
    mod_mysql_auth_file_config* merge(mod_mysql_pool* pool,
            const mod_mysql_auth_file_config& override);

    const char* get_filename() const;
    void set_filename(const char*);
};

// {{{ mod_mysql_auth_file_config::mod_mysql_auth_file_config()
inline mod_mysql_auth_file_config::mod_mysql_auth_file_config(
        mod_mysql_pool* _pool)
        : pool(_pool), filename(0)
{

}

inline mod_mysql_auth_file_config::mod_mysql_auth_file_config(
        mod_mysql_pool* _pool, const mod_mysql_auth_file_config& override)
        : pool(_pool), filename(override.filename)
{

}
// }}}

// {{{ mod_mysql_auth_file_config::merge()
mod_mysql_auth_file_config* mod_mysql_auth_file_config::merge(
        mod_mysql_pool* pool, const mod_mysql_auth_file_config& child)
{
    mod_mysql_auth_file_config *retval;

    retval = new(pool) mod_mysql_auth_file_config(pool, *this);

    retval->filename = child.filename;

    return retval;
}
// }}}

// {{{ mod_mysql_auth_file_config::get_filename()
inline const char* mod_mysql_auth_file_config::get_filename() const
{
    return filename;
}
// }}}

// {{{ mod_mysql_auth_file_config::set_filename()
void mod_mysql_auth_file_config::set_filename(const char* val)
{
    filename = val;
}
// }}}
// }}}

// {{{ mod_mysql_auth_file_authenticator_impl
class mod_mysql_auth_file_authenticator_impl
        : public mod_mysql_authenticator, public mod_mysql_base,
          public mod_mysql_instantiable
{
protected:
    mod_mysql_pool pool;
    mod_mysql_logger* log;

public:
    mod_mysql_auth_file_authenticator_impl(mod_mysql_pool* pool,
            mod_mysql_logger* log);
    virtual mod_mysql_server_message* authenticate(
            mod_mysql_pool* pool,
            const mod_mysql_client_info* cinfo,
            const mod_mysql_ranged_ref<const apr_byte_t>& pw_seed,
            const mod_mysql_ranged_ref<const apr_byte_t>& pw_hash);
};

// {{{ mod_mysql_auth_file_authenticator_impl::mod_mysql_auth_file_authenticator_impl()
mod_mysql_auth_file_authenticator_impl::mod_mysql_auth_file_authenticator_impl(
        mod_mysql_pool* _pool, mod_mysql_logger* _log)
        : pool(_pool), log(_log)
{
}
// }}}

// {{{ mod_mysql_auth_file_authenticator_impl::authenticate()
mod_mysql_server_message* mod_mysql_auth_file_authenticator_impl::authenticate(
        mod_mysql_pool* local_pool,
        const mod_mysql_client_info* cinfo,
        const mod_mysql_ranged_ref<const apr_byte_t>& pw_seed,
        const mod_mysql_ranged_ref<const apr_byte_t>& pw_hash)
{
    log->err(APLOG_MARK, 0, "user=%s, hostname=%s, pw_hash=%s", cinfo->user, cinfo->hostname, (const apr_byte_t*)pw_hash);
    return 0;
}
// }}}
// }}}

// {{{ class mod_mysql_auth_file
class mod_mysql_auth_file
        : public mod_mysql_config_traits<mod_mysql_auth_file_config>
{
private:
    static mod_mysql_pool* pool;
    static mod_mysql_logger* log;
    static mod_mysql_authenticator* auth;

public:
    static module* get_module_rec();
    static void *create_server_config(apr_pool_t *pool, server_rec *s);
    static int hook_authenticate(apr_pool_t* pool, apr_status_t* status,
            mod_mysql_server_message_t* msg,
            const mod_mysql_client_info_t* cinfo,
            const apr_byte_t* pw_seed, apr_size_t pw_seed_len,
            const apr_byte_t* pw_hash, apr_size_t pw_hash_len);
    static void register_hooks(apr_pool_t *p);
    static void init();
    static void init_authenticator();
};

mod_mysql_pool* mod_mysql_auth_file::pool = 0;
mod_mysql_logger* mod_mysql_auth_file::log = 0;
mod_mysql_authenticator* mod_mysql_auth_file::auth = 0;

// {{{ mod_mysql_auth_file::get_module_rec()
inline module* mod_mysql_auth_file::get_module_rec()
{
    return &mysql_auth_file_module;
}
// }}}

// {{{ mod_mysql_auth_file::create_server_config()
inline void *mod_mysql_auth_file::create_server_config(
        apr_pool_t *pool, server_rec *s)
{
    mod_mysql_pool* _pool = mod_mysql_pool::wrap(pool);

    return new(_pool) mod_mysql_auth_file_config(_pool);
}
// }}}

// {{{ mod_mysql_auth_file::hook_authenticate()
int mod_mysql_auth_file::hook_authenticate(apr_pool_t* _pool,
        apr_status_t* status,
        mod_mysql_server_message_t* msg,
        const mod_mysql_client_info_t* cinfo,
        const apr_byte_t* pw_seed, apr_size_t pw_seed_len,
        const apr_byte_t* pw_hash, apr_size_t pw_hash_len)
{
    mod_mysql_pool* pool = mod_mysql_pool::wrap(_pool);

    try {
        mod_mysql_server_message *m = auth->authenticate(pool,
                static_cast<const mod_mysql_client_info*>(cinfo),
                mod_mysql_ranged_ref<const apr_byte_t>(pw_seed, pw_seed_len),
                mod_mysql_ranged_ref<const apr_byte_t>(pw_hash, pw_hash_len));
        if (m) {
            *msg = *m;
            return DECLINED;
        }
    } catch (mod_mysql_error e) {
        *status = e.get_status();
        return DECLINED;
    }

    return OK;
}
// }}}

// {{{ mod_mysql_auth_file::register_hooks()
inline void mod_mysql_auth_file::register_hooks(apr_pool_t *pool)
{
    mod_mysql_hook_authenticate(&mod_mysql_auth_file::hook_authenticate,
            NULL, NULL, APR_HOOK_MIDDLE);

    try {
        mod_mysql_auth_file::init();
    } catch (mod_mysql_error e) {
        fprintf(stderr, "ERROR\n");
    }
}
// }}}

// {{{ mod_mysql_auth_file::init
void mod_mysql_auth_file::init()
{
    {
        apr_pool_t* _pool;
        apr_status_t s;
        if ((s = apr_pool_create(&_pool, NULL))) {
            throw mod_mysql_error(s, APLOG_MARK);
        }

        mod_mysql_auth_file::pool = mod_mysql_pool::wrap(_pool);
        mod_mysql_auth_file::log = new(pool) mod_mysql_std_logger(_pool);
    }

    init_authenticator();
}
// }}}

// {{{ mod_mysql_auth_file::init_authenticator()
void mod_mysql_auth_file::init_authenticator()
{
    mod_mysql_pool* pool = mod_mysql_auth_file::pool->create_pool();

    mod_mysql_auth_file::auth =
            new(pool) mod_mysql_auth_file_authenticator_impl(pool, log);
}
// }}}

// }}}

extern "C" {
static const command_rec commands[] = {
    mod_mysql_config_handler<mod_mysql_auth_file, TAKE1>::rec(
            "MySQLAuthFile", RSRC_CONF,
            "Usage: MySQLAuthFile (filename)",
            &mod_mysql_auth_file_config::set_filename),
    { NULL }
};

module AP_MODULE_DECLARE_DATA mysql_auth_file_module = {
    STANDARD20_MODULE_STUFF,
    // create per-directory config structure
    NULL,
    // merge per-directory(?) config structure
    NULL,
    // create per-server config structure
    &mod_mysql_auth_file::create_server_config,
    // merge per-server config ...
    NULL,
    /* command handlers */
    commands,
    /* hook registerar */
    &mod_mysql_auth_file::register_hooks
};
} // extern "C"
// vim: sts=4 sw=4 ts=4 et fdm=marker
