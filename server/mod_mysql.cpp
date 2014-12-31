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
#include <httpd.h>
#include <http_config.h>
#include <http_connection.h>
#include <http_protocol.h>
#include "common.h"
#include "mod_mysql.h"
#include "mod_mysql_authenticator.h"
#include "mod_mysql_service.h"
#include "mod_mysql_service_factory.h"
#include "mod_mysql_codeset.h"
#include "mod_mysql_collation.h"
#include "mod_mysql_config.h"
#include "mod_mysql_config_handler.h"
#include "mod_mysql_conn_logger.h"
#include "mod_mysql_connection.h"
#include "mod_mysql_error.h"
#include "mod_mysql_hash_table.h"
#include "mod_mysql_locale_registry.h"
#include "mod_mysql_opaque_collation.h"
#include "mod_mysql_pool.h"
#include "mod_mysql_logger.h"
#include "mod_mysql_std_logger.h"
#include "mod_mysql_utils.h"

extern "C" module AP_MODULE_DECLARE_DATA mysql_module;

APR_HOOK_STRUCT(
    APR_HOOK_LINK(authenticate)
);

APR_IMPLEMENT_EXTERNAL_HOOK_RUN_FIRST(
        mod_mysql, MOD_MYSQL, int, authenticate,
        (apr_pool_t* pool, apr_status_t* status,
            mod_mysql_server_message_t* msg,
            const mod_mysql_client_info_t* cinfo,
            const apr_byte_t* pw_seed, apr_size_t pw_seed_len,
            const apr_byte_t* pw_hash, apr_size_t pw_hash_len),
        (pool, status, msg, cinfo, pw_seed, pw_seed_len,
            pw_hash, pw_hash_len), OK);

mod_mysql_pool* mod_mysql::pool = 0;
mod_mysql_locale_registry* mod_mysql::locale_reg = 0;
mod_mysql_logger* mod_mysql::log = 0;
mod_mysql_authenticator* mod_mysql::auth = 0;
const char mod_mysql::provider_namespace[] =
        MOD_MYSQL_SERVICE_PROVIDER_NAMESPACE;
const char mod_mysql::provider_version[] =
        MOD_MYSQL_SERVICE_PROVIDER_VERSION;

// {{{ mod_mysql_locale_registry_impl
class mod_mysql_locale_registry_impl
        : public mod_mysql_locale_registry, public mod_mysql_base,
          public mod_mysql_instantiable
{
protected:
    typedef mod_mysql_hash_table<const char, mod_mysql_locale*>
            name_to_instance_map_t;
    typedef mod_mysql_hash_table<mod_mysql_locale::code_e,
            mod_mysql_locale*> code_to_instance_map_t;
protected:
    mod_mysql_pool pool;
    name_to_instance_map_t n_tbl;
    code_to_instance_map_t c_tbl;
public:
    mod_mysql_locale_registry_impl(mod_mysql_pool* pool);
    virtual mod_mysql_locale* find(const char* name);
    virtual mod_mysql_locale* find(mod_mysql_locale::code_e code);
    virtual void add(mod_mysql_locale* loc);
    virtual int count();
};

// {{{ mod_mysql_locale_registry_impl::mod_mysql_locale_registry_impl()
mod_mysql_locale_registry_impl::mod_mysql_locale_registry_impl(
        mod_mysql_pool* _pool)
        : pool(_pool), n_tbl(&pool), c_tbl(&pool)
{
}
// }}}

// {{{ mod_mysql_locale_registry_impl::find()
mod_mysql_locale* mod_mysql_locale_registry_impl::find(const char* name)
{
    char tmp[128];
    strncpy(tmp, name, sizeof(tmp) - 1);
    mod_mysql_utils::uppercase_ascii(tmp);
    return n_tbl.get(tmp, APR_HASH_KEY_STRING);
}

mod_mysql_locale* mod_mysql_locale_registry_impl::find(
        mod_mysql_locale::code_e code)
{
    return c_tbl.get(code);
}
// }}}

// {{{ mod_mysql_locale_registry_impl::add()
void mod_mysql_locale_registry_impl::add(mod_mysql_locale* loc)
{
    char* name = pool.dup<char>(
            loc->get_name(), strlen(loc->get_name()) + 1);
    mod_mysql_utils::uppercase_ascii(name);
    n_tbl.set(name, APR_HASH_KEY_STRING, loc);
    c_tbl.set(loc->get_code(), loc);
}
// }}}

// {{{ mod_mysql_locale_registry_impl::count()
int mod_mysql_locale_registry_impl::count()
{
    return n_tbl.count();
}
// }}}
// }}}

// {{{ mod_mysql_authenticator_impl
class mod_mysql_authenticator_impl
        : public mod_mysql_authenticator, public mod_mysql_base,
          public mod_mysql_instantiable
{
protected:
    mod_mysql_pool pool;

public:
    mod_mysql_authenticator_impl(mod_mysql_pool* pool);
    virtual mod_mysql_server_message* authenticate(
            mod_mysql_pool* pool,
            const mod_mysql_client_info* cinfo,
            const mod_mysql_ranged_ref<const apr_byte_t>& pw_seed,
            const mod_mysql_ranged_ref<const apr_byte_t>& pw_hash);
};

// {{{ mod_mysql_authenticator_impl::mod_mysql_authenticator_impl()
mod_mysql_authenticator_impl::mod_mysql_authenticator_impl(
        mod_mysql_pool* _pool)
        : pool(_pool)
{
}
// }}}

// {{{ mod_mysql_authenticator_impl::authenticate()
mod_mysql_server_message* mod_mysql_authenticator_impl::authenticate(
        mod_mysql_pool* local_pool,
        const mod_mysql_client_info* cinfo,
        const mod_mysql_ranged_ref<const apr_byte_t>& pw_seed,
        const mod_mysql_ranged_ref<const apr_byte_t>& pw_hash)
{
    mod_mysql_server_message msg(
            MOD_MYSQL_MSG_PASSWORD_NO_MATCH,
            "Authentication failure");
    apr_status_t s = APR_SUCCESS;

    if (mod_mysql_run_authenticate(*local_pool, &s, &msg,
            static_cast<const mod_mysql_client_info_t*>(cinfo),
            pw_seed.get_referent(), pw_seed.get_size(),
            pw_hash.get_referent(), pw_hash.get_size()) == DECLINED) {
        if (s) {
            throw mod_mysql_error(s, APLOG_MARK);
        }
        return new(local_pool) mod_mysql_server_message(msg);
    }

    return 0;
}
// }}}
// }}}

// {{{ mod_mysql::get_module_rec()
inline module* mod_mysql::get_module_rec()
{
    return &mysql_module;
}
// }}}

// {{{ mod_mysql::create_server_config()
inline void *mod_mysql::create_server_config(apr_pool_t *pool, server_rec *s)
{
    mod_mysql_pool* _pool = mod_mysql_pool::wrap(pool);

    return new(_pool) mod_mysql_config(_pool, provider_namespace,
            provider_version);
}
// }}}

// {{{ mod_mysql::hook_process_connection()
int mod_mysql::hook_process_connection(conn_rec* c)
{
    config_t* config = static_cast<config_t *>(ap_get_module_config(
                c->base_server->module_config, get_module_rec()));
    if (!config->is_function_enabled()) {
        return DECLINED;
    }

    mod_mysql_conn_logger log(c);

    try {
        return mod_mysql_connection(c, config, &log, auth).process();
    } catch (mod_mysql_error e) {
        log.err(APLOG_MARK, e.get_status(), e.get_message());
        return DECLINED;
    }
}
// }}}

// {{{ mod_mysql::register_hooks()
void mod_mysql::register_hooks(apr_pool_t *pool)
{
    ap_hook_process_connection(&mod_mysql::hook_process_connection,
            NULL, NULL, APR_HOOK_MIDDLE);

    try {
        mod_mysql::init();
    } catch (mod_mysql_error e) {
        fprintf(stderr, "ERROR\n");
    }
}
// }}}

// {{{ mod_mysql::get_locale_registry()
mod_mysql_locale_registry* mod_mysql::get_locale_registry()
{
    return locale_reg;
}
// }}}

// {{{ mod_mysql::init
void mod_mysql::init()
{
    {
        apr_pool_t* _pool;
        apr_status_t s;
        if ((s = apr_pool_create(&_pool, NULL))) {
            throw mod_mysql_error(s, APLOG_MARK);
        }

        mod_mysql::pool = mod_mysql_pool::wrap(_pool);
        mod_mysql::log = new(pool) mod_mysql_std_logger(_pool);
    }

    init_locale_registry();
    init_authenticator();
}
// }}}

// {{{ mod_mysql::init_authenticator()
void mod_mysql::init_authenticator()
{
    mod_mysql_pool* pool = mod_mysql::pool->create_pool();

    mod_mysql::auth = new(pool) mod_mysql_authenticator_impl(pool);
}
// }}}

// {{{ mod_mysql::init_locale_registry
void mod_mysql::init_locale_registry()
{
    static mod_mysql_opaque_collation binary_collation("BINARY");

    mod_mysql_pool* pool = mod_mysql::pool->create_pool();
    mod_mysql_locale_registry* reg =
            new(pool) mod_mysql_locale_registry_impl(pool);

    mod_mysql_codeset* _codeset = 0;

    {
        static mod_mysql_opaque_collation _collation("BIG5_CHINESE_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_BIG5_CHINESE_CI,
                    _codeset, &_collation, "BIG5_CHINESE_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("LATIN2_CZECH_CS");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_LATIN2_CZECH_CS,
                    _codeset, &_collation, "LATIN2_CZECH_CS"));
    }
    {
        static mod_mysql_opaque_collation _collation("DEC8_SWEDISH_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_DEC8_SWEDISH_CI,
                    _codeset, &_collation, "DEC8_SWEDISH_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("CP850_GENERAL_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_CP850_GENERAL_CI,
                    _codeset, &_collation, "CP850_GENERAL_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("LATIN1_GERMAN1_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_LATIN1_GERMAN1_CI,
                    _codeset, &_collation, "LATIN1_GERMAN1_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("HP8_ENGLISH_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_HP8_ENGLISH_CI,
                    _codeset, &_collation, "HP8_ENGLISH_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("KOI8R_GENERAL_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_KOI8R_GENERAL_CI,
                    _codeset, &_collation, "KOI8R_GENERAL_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("LATIN1_SWEDISH_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_LATIN1_SWEDISH_CI,
                    _codeset, &_collation, "LATIN1_SWEDISH_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("LATIN2_GENERAL_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_LATIN2_GENERAL_CI,
                    _codeset, &_collation, "LATIN2_GENERAL_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("SWE7_SWEDISH_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_SWE7_SWEDISH_CI,
                    _codeset, &_collation, "SWE7_SWEDISH_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("SWE7_ENGLISH_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_ASCII_GENERAL_CI,
                    _codeset, &_collation, "SWE7_ENGLISH_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("UJIS_JAPANESE_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_UJIS_JAPANESE_CI,
                    _codeset, &_collation, "UJIS_JAPANESE_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("SJIS_JAPANESE_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_SJIS_JAPANESE_CI,
                    _codeset, &_collation, "SJIS_JAPANESE_CI"));
    } 
    {
        static mod_mysql_opaque_collation _collation("CP1251_BULGARIAN_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_CP1251_BULGARIAN_CI,
                    _codeset, &_collation, "CP1251_BULGARIAN_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("LATIN1_DANISH_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_LATIN1_DANISH_CI,
                    _codeset, &_collation, "LATIN1_DANISH_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("HEBREW_GENERAL_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_HEBREW_GENERAL_CI,
                    _codeset, &_collation, "HEBREW_GENERAL_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("TIS620_THAI_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_TIS620_THAI_CI,
                    _codeset, &_collation, "TIS620_THAI_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("EUCKR_KOREAN_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_EUCKR_KOREAN_CI,
                    _codeset, &_collation, "EUCKR_KOREAN_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("LATIN7_ESTONIAN_CS");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_LATIN7_ESTONIAN_CS,
                    _codeset, &_collation, "LATIN7_ESTONIAN_CS"));
    }
    {
        static mod_mysql_opaque_collation _collation("LATIN2_HUNGARIAN_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_LATIN2_HUNGARIAN_CI,
                    _codeset, &_collation, "LATIN2_HUNGARIAN_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("KOI8U_GENERAL_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_KOI8U_GENERAL_CI,
                    _codeset, &_collation, "KOI8U_GENERAL_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("CP1251_UKRAINIAN_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_CP1251_UKRAINIAN_CI,
                    _codeset, &_collation, "CP1251_UKRAINIAN_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("GB2312_CHINESE_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_GB2312_CHINESE_CI,
                    _codeset, &_collation, "GB2312_CHINESE_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("GREEK_GENERAL_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_GREEK_GENERAL_CI,
                    _codeset, &_collation, "GREEK_GENERAL_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("CP1250_GENERAL_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_CP1250_GENERAL_CI,
                    _codeset, &_collation, "CP1250_GENERAL_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("LATIN2_CROATIAN_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_LATIN2_CROATIAN_CI,
                    _codeset, &_collation, "LATIN2_CROATIAN_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("GBK_CHINESE_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_GBK_CHINESE_CI,
                    _codeset, &_collation, "GBK_CHINESE_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("CP1257_LITHUANIAN_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_CP1257_LITHUANIAN_CI,
                    _codeset, &_collation, "CP1257_LITHUANIAN_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("LATIN5_TURKISH_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_LATIN5_TURKISH_CI,
                    _codeset, &_collation, "LATIN5_TURKISH_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("LATIN1_GERMAN2_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_LATIN1_GERMAN2_CI,
                    _codeset, &_collation, "LATIN1_GERMAN2_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("ARMSCII8_GENERAL_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_ARMSCII8_GENERAL_CI,
                    _codeset, &_collation, "ARMSCII8_GENERAL_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("UTF8_GENERAL_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_UTF8_GENERAL_CI,
                    _codeset, &_collation, "UTF8_GENERAL_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("CP1250_CZECH_CS");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_CP1250_CZECH_CS,
                    _codeset, &_collation, "CP1250_CZECH_CS"));
    }
    {
        static mod_mysql_opaque_collation _collation("UCS2_GENERAL_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_UCS2_GENERAL_CI,
                    _codeset, &_collation, "UCS2_GENERAL_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("CP866_GENERAL_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_CP866_GENERAL_CI,
                    _codeset, &_collation, "CP866_GENERAL_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("KEYBCS2_GENERAL_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_KEYBCS2_GENERAL_CI,
                    _codeset, &_collation, "KEYBCS2_GENERAL_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("MACCE_GENERAL_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_MACCE_GENERAL_CI,
                    _codeset, &_collation, "MACCE_GENERAL_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("MACROMAN_GENERAL_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_MACROMAN_GENERAL_CI,
                    _codeset, &_collation, "MACROMAN_GENERAL_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("CP852_GENERAL_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_CP852_GENERAL_CI,
                    _codeset, &_collation, "CP852_GENERAL_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("LATIN7_GENERAL_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_LATIN7_GENERAL_CI,
                    _codeset, &_collation, "LATIN7_GENERAL_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("LATIN7_GENERAL_CS");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_LATIN7_GENERAL_CS,
                    _codeset, &_collation, "LATIN7_GENERAL_CS"));
    }
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_MACCE_BIN,
                _codeset, &binary_collation, "MACCE_BIN"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_LATIN1_BIN,
                _codeset, &binary_collation, "LATIN1_BIN"));
    {
        static mod_mysql_opaque_collation _collation("LATIN1_GENERAL_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_LATIN1_GENERAL_CI,
                    _codeset, &_collation, "LATIN1_GENERAL_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("LATIN1_GENERAL_CS");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_LATIN1_GENERAL_CS,
                    _codeset, &_collation, "LATIN1_GENERAL_CS"));
    }
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_CP1251_BIN,
                _codeset, &binary_collation, "CP1251_BIN"));
    {
        static mod_mysql_opaque_collation _collation("CP1251_GENERAL_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_CP1251_GENERAL_CI,
                    _codeset, &_collation, "CP1251_GENERAL_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("CP1251_GENERAL_CS");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_CP1251_GENERAL_CS,
                    _codeset, &_collation, "CP1251_GENERAL_CS"));
    }
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_MACROMAN_BIN,
                _codeset, &binary_collation, "MACROMAN_BIN"));
    {
        static mod_mysql_opaque_collation _collation("MACROMAN_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_MACROMAN_CI,
                    _codeset, &_collation, "MACROMAN_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("MACROMAN_CI_AI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_MACROMAN_CI_AI,
                    _codeset, &_collation, "MACROMAN_CI_AI"));
    }
    {
        static mod_mysql_opaque_collation _collation("MACROMAN_CS");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_MACROMAN_CS,
                    _codeset, &_collation, "MACROMAN_CS"));
    }
    {
        static mod_mysql_opaque_collation _collation("CP1256_GENERAL_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_CP1256_GENERAL_CI,
                    _codeset, &_collation, "CP1256_GENERAL_CI"));
    }
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_CP1257_BIN,
                _codeset, &binary_collation, "CP1257_BIN"));
    {
        static mod_mysql_opaque_collation _collation("CP1257_GENERAL_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_CP1257_GENERAL_CI,
                    _codeset, &_collation, "CP1257_GENERAL_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("CP1257_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_CP1257_CI,
                    _codeset, &_collation, "CP1257_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("CP1257_CS");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_CP1257_CS,
                    _codeset, &_collation, "CP1257_CS"));
    }
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_BINARY,
                _codeset, &binary_collation, "BINARY"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_ARMSCII8_BIN,
                _codeset, &binary_collation, "ARMSCII8_BIN"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_ASCII_BIN,
                _codeset, &binary_collation, "ASCII_BIN"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_CP1250_BIN,
                _codeset, &binary_collation, "CP1250_BIN"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_CP1256_BIN,
                _codeset, &binary_collation, "CP1256_BIN"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_CP866_BIN,
                _codeset, &binary_collation, "CP866_BIN"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_DEC8_BIN,
                _codeset, &binary_collation, "DEC8_BIN"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_GREEK_BIN,
                _codeset, &binary_collation, "GREEK_BIN"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_HEBREW_BIN,
                _codeset, &binary_collation, "HEBREW_BIN"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_HP8_BIN,
                _codeset, &binary_collation, "HP8_BIN"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_KEYBCS2_BIN,
                _codeset, &binary_collation, "KEYBCS2_BIN"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_KOI8R_BIN,
                _codeset, &binary_collation, "KOI8R_BIN"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_KOI8U_BIN,
                _codeset, &binary_collation, "KOI8U_BIN"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_LATIN2_BIN,
                _codeset, &binary_collation, "LATIN2_BIN"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_LATIN5_BIN,
                _codeset, &binary_collation, "LATIN5_BIN"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_LATIN7_BIN,
                _codeset, &binary_collation, "LATIN7_BIN"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_CP850_BIN,
                _codeset, &binary_collation, "CP850_BIN"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_CP852_BIN,
                _codeset, &binary_collation, "CP852_BIN"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_SWE7_BIN,
                _codeset, &binary_collation, "SWE7_BIN"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_UTF8_BIN,
                _codeset, &binary_collation, "UTF8_BIN"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_BIG5_BIN,
                _codeset, &binary_collation, "BIG5_BIN"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_EUCKR_BIN,
                _codeset, &binary_collation, "EUCKR_BIN"));
    {
        static mod_mysql_opaque_collation _collation("GB2312");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_GB2312,
                    _codeset, &_collation, "GB2312"));
    }
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_GBK_BIN,
                _codeset, &binary_collation, "GBK_BIN"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_SJIS_BIN,
                _codeset, &binary_collation, "SJIS_BIN"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_TIS620_BIN,
                _codeset, &binary_collation, "TIS620_BIN"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_UCS2_BIN,
                _codeset, &binary_collation, "UCS2_BIN"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_UJIS_BIN,
                _codeset, &binary_collation, "UJIS_BIN"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_GEOSTD8_GENERAL_CI,
                _codeset, &binary_collation, "GEOSTD8_GENERAL_CI"));
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_GEOSTD8_BIN,
                _codeset, &binary_collation, "GEOSTD8_BIN"));
    {
        static mod_mysql_opaque_collation _collation("LATIN1_SPANISH_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_LATIN1_SPANISH_CI,
                    _codeset, &_collation, "LATIN1_SPANISH_CI"));
    }
    {
        static mod_mysql_opaque_collation _collation("EUCJPMS_JAPANESE_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_EUCJPMS_JAPANESE_CI,
                    _codeset, &_collation, "EUCJPMS_JAPANESE_CI"));
    }
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_EUCJPMS_BIN,
                _codeset, &binary_collation, "EUCJPMS_BIN"));
    {
        static mod_mysql_opaque_collation _collation("CP932_JAPANESE_CI");
        reg->add(new(pool) mod_mysql_locale(
                    MOD_MYSQL_LOC_CP932_JAPANESE_CI,
                    _codeset, &_collation, "CP932_JAPANESE_CI"));
    } 
    reg->add(new(pool) mod_mysql_locale(
                MOD_MYSQL_LOC_CP932_BIN,
                _codeset, &binary_collation, "CP932_BIN"));

    reg->count();

    mod_mysql::locale_reg = reg;
}
// }}}

extern "C" {
static const command_rec commands[] = {
    mod_mysql_config_handler<mod_mysql, FLAG>::rec(
            "MySQLProtocol", RSRC_CONF,
            "Usage: MySQLProtocol (on|off)",
            &mod_mysql_config::set_function_enabled),
    mod_mysql_config_handler<mod_mysql, TAKE1>::rec(
            "MySQLMaxPacketLen", RSRC_CONF,
            "Usage: MySQLMaxPacketLen (maximum length of the packet)",
            &mod_mysql_config::set_function_enabled),
    mod_mysql_config_handler<mod_mysql, TAKE1>::rec(
            "MySQLServiceProvider", RSRC_CONF,
            "Usage: MySQLServiceProvider (provider)",
            &mod_mysql_config::set_provider_name),
    { NULL }
};

module AP_MODULE_DECLARE_DATA mysql_module = {
    STANDARD20_MODULE_STUFF,
    // create per-directory config structure
    NULL,
    // merge per-directory(?) config structure
    NULL,
    // create per-server config structure
    &mod_mysql::create_server_config,
    // merge per-server config ...
    NULL,
    /* command handlers */
    commands,
    /* hook registerar */
    &mod_mysql::register_hooks
};
} // extern "C"

// vim: sts=4 sw=4 ts=4 et fdm=marker
