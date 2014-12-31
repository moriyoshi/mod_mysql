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
#ifndef _MOD_MYSQL_CONFIG_H
#define _MOD_MYSQL_CONFIG_H

#include "mod_mysql_base.h"
#include "mod_mysql_array_list.h"
#include "mod_mysql_hash_table.h"
#include "mod_mysql_service_factory.h"
#include "mod_mysql_instantiable.h"

class mod_mysql_pool;

/**
 * module-local information storage structure
 */
class mod_mysql_config
        : public mod_mysql_base, public mod_mysql_service_factory,
          public mod_mysql_instantiable
{
public:
    typedef enum {
        UNSET = -1,
        OFF = 0,
        ON = 1
    } tristate_t;

private:
    /** pool associated to instance */
    mod_mysql_pool* pool;

    /** flag to enable this module */
    tristate_t function_enabled;

    /** maximum allowable length of the packet */
    unsigned long max_allowed_packet;

    /** service provider's name */
    const char* provider_name;

    /** singleton service instance */
    mod_mysql_service* srv;

    /** provider namespace */
    const char* provider_namespace;

    /** provider version */
    const char* provider_version;

public:
    mod_mysql_config(mod_mysql_pool* pool, const char* provider_namespace,
            const char* provider_version);
    mod_mysql_config(mod_mysql_pool* pool, const mod_mysql_config& override);

    mod_mysql_config* merge(mod_mysql_pool* pool,
            const mod_mysql_config& override);

    bool is_function_enabled() const;
    void set_function_enabled(bool flag);
    unsigned long get_max_allowed_packet() const;
    void set_max_allowed_packet(unsigned long len);
    const char* get_provider_name() const;
    void set_provider_name(const char* name);
    virtual mod_mysql_service* get_service();
};
#endif /* _MOD_MYSQL_CONFIG_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
