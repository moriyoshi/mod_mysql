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
#ifndef _MOD_MYSQL_H
#define _MOD_MYSQL_H

#include "mod_mysql_server_message.h"
#include "mod_mysql_client_info.h"

#if !defined(WIN32)
#define MOD_MYSQL_DECLARE(type)            type
#define MOD_MYSQL_DECLARE_NONSTD(type)     type
#define MOD_MYSQL_DECLARE_DATA
#elif defined(MOD_MYSQL_DECLARE_STATIC)
#define MOD_MYSQL_DECLARE(type)            type __stdcall
#define MOD_MYSQL_DECLARE_NONSTD(type)     type
#define MOD_MYSQL_DECLARE_DATA
#elif defined(MOD_MYSQL_DECLARE_EXPORT)
#define MOD_MYSQL_DECLARE(type)            __declspec(dllexport) type __stdcall
#define MOD_MYSQL_DECLARE_NONSTD(type)     __declspec(dllexport) type
#define MOD_MYSQL_DECLARE_DATA             __declspec(dllexport)
#else
#define MOD_MYSQL_DECLARE(type)            __declspec(dllimport) type __stdcall
#define MOD_MYSQL_DECLARE_NONSTD(type)     __declspec(dllimport) type
#define MOD_MYSQL_DECLARE_DATA             __declspec(dllimport)
#endif

#define MOD_MYSQL_SERVICE_PROVIDER_NAMESPACE "mod_mysql"
#define MOD_MYSQL_SERVICE_PROVIDER_VERSION   "1.0"

#ifndef MOD_MYSQL_SERVICE_T_DEFINED
#define MOD_MYSQL_SERVICE_T_DEFINED
typedef struct mod_mysql_service_t mod_mysql_service_t;
#endif /* MOD_MYSQL_SERVICE_T_DEFINED */

typedef struct mod_mysql_service_provider_t
{
    mod_mysql_service_t* (*create_service_fn)(mod_mysql_service_provider_t *,
            apr_pool_t*);
} mod_mysql_service_provider_t;

#define mod_mysql_service_provider_create_service(self, pool) \
        ((mod_mysql_service_provider_t *)(self))->create_service_fn( \
            (mod_mysql_service_provider_t *)(self), pool)

#ifdef __cplusplus
extern "C" {
#endif
APR_DECLARE_EXTERNAL_HOOK(
        mod_mysql, MOD_MYSQL, int, authenticate, 
        (apr_pool_t* pool, apr_status_t* status,
            mod_mysql_server_message_t* msg,
            const mod_mysql_client_info_t* cinfo,
            const apr_byte_t* pw_seed, apr_size_t pw_seed_len,
            const apr_byte_t* pw_hash, apr_size_t pw_hash_len));

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include <httpd.h>
#include <apr_pools.h>
#include "mod_mysql_config_handler.h"

class mod_mysql_pool;
class mod_mysql_logger;
class mod_mysql_locale_registry;
class mod_mysql_authenticator;
class mod_mysql_service;

class mod_mysql: public mod_mysql_config_traits<mod_mysql_config>
{
private:
    static mod_mysql_pool* pool;
    static mod_mysql_logger* log;
    static mod_mysql_locale_registry* locale_reg;
    static mod_mysql_authenticator* auth;

public:
    static const char provider_namespace[];
    static const char provider_version[];
    static const apr_size_t pw_seed_len_41 = 20;
    static const apr_size_t pw_seed_len_323 = 8;

private:
    static void init_locale_registry();
    static void init_authenticator();

public:
    static module* get_module_rec();
    static void *create_server_config(apr_pool_t *pool, server_rec *s);
    static int hook_process_connection(conn_rec* c);
    static void register_hooks(apr_pool_t *p);
    static mod_mysql_locale_registry* get_locale_registry();
    static mod_mysql_service* get_service(mod_mysql_config* cfg);
    static void init();
};
#endif

#endif /* _MOD_MYSQL_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
