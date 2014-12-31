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
#ifndef _MOD_MYSQL_CONNECTION_H
#define _MOD_MYSQL_CONNECTION_H

#include <httpd.h>
#include <util_filter.h>

#include "mod_mysql_base.h"
#include "mod_mysql_pool.h"
#include "mod_mysql_logger.h"
#include "mod_mysql_config.h"
#include "mod_mysql_locale.h"
#include "mod_mysql_bucket_brigade.h"
#include "mod_mysql_bucket_factory.h"
#include "mod_mysql_accessor.h"
#include "mod_mysql_service.h"

// {{{ class mod_mysql_ap_output_filter_pump
class mod_mysql_ap_output_filter_pump: public mod_mysql_bucket_brigade
{
private:
    apr_pool_t* pool;
    apr_bucket_alloc_t* list;
    ap_filter_t* filters;

public:
    mod_mysql_ap_output_filter_pump(apr_pool_t* pool, apr_bucket_alloc_t* list,
            struct ap_filter_t* filters);

    bool pass();

    void reload();

    void write_nts(const char *str);
};

// {{{ mod_mysql_ap_output_filter_pump::mod_mysql_ap_output_filter_pump()
inline mod_mysql_ap_output_filter_pump::mod_mysql_ap_output_filter_pump(
        apr_pool_t* _pool, apr_bucket_alloc_t* _list,
        struct ap_filter_t* _filters)
        : pool(_pool), list(_list),
          mod_mysql_bucket_brigade(0), filters(_filters)
{
    brigade_flusher = ap_filter_flush;
    reload();
}
// }}}

// {{{ mod_mysql_ap_output_filter_pump::pass()
inline bool mod_mysql_ap_output_filter_pump::pass()
{
    apr_status_t s = ap_pass_brigade(filters, bb);

    if (s == AP_NOBODY_WROTE) {
        return false;
    } else if (s) {
        throw mod_mysql_error(s, APLOG_MARK);
    }

    return true;
}
// }}}

// {{{ mod_mysql_ap_output_filter_pump::reload()
inline void mod_mysql_ap_output_filter_pump::reload()
{
    bb = apr_brigade_create(pool, list);
}
// }}}

// {{{ mod_mysql_ap_output_filter_pump::write_nts()
inline void mod_mysql_ap_output_filter_pump::write_nts(const char* str)
{
    write(str, strlen(str) + 1);
}
// }}}
// }}}

// {{{ class mod_mysql_ap_input_filter_pump
class mod_mysql_ap_input_filter_pump: public mod_mysql_bucket_brigade
{
private:
    ap_filter_t* filters;

public:
    mod_mysql_ap_input_filter_pump(apr_pool_t* pool, apr_bucket_alloc_t* list,
            struct ap_filter_t* filters);

    bool fill(ap_input_mode_t mode, apr_read_type_e to_block, apr_off_t nbytes);
};

// {{{ mod_mysql_ap_input_filter_pump::mod_mysql_ap_input_filter_pump()
inline mod_mysql_ap_input_filter_pump::mod_mysql_ap_input_filter_pump(
        apr_pool_t* pool, apr_bucket_alloc_t* list,
        struct ap_filter_t* _filters)
        : mod_mysql_bucket_brigade(0), filters(_filters)
{
    brigade_flusher = ap_filter_flush;
    bb = apr_brigade_create(pool, list);
}
// }}}

// {{{ mod_mysql_ap_input_filter_pump::fill()
inline bool mod_mysql_ap_input_filter_pump::fill(
        ap_input_mode_t mode, apr_read_type_e to_block, apr_off_t nbytes)
{
    apr_status_t s = ap_get_brigade(filters, bb, mode, to_block, nbytes);

    if (s == AP_NOBODY_READ) {
        return false;
    } else if (s) {
        throw mod_mysql_error(s, APLOG_MARK);
    }

    return true;
}
// }}}
// }}}

class mod_mysql_client_info;
class mod_mysql_server_message;
class mod_mysql_result;
class mod_mysql_authenticator;
class mod_mysql_command_handler;

class mod_mysql_connection: public mod_mysql_base
{
public:
    static const int MAX_USER_NAME_LENGTH = 255;
    static const int PW_HASH_LEN_41       = 20;
    static const int PW_HASH_LEN_323      = 8;
private:
    conn_rec* conn;
    mod_mysql_pool* pool;
    mod_mysql_config* cfg;
    mod_mysql_logger* log;
    mod_mysql_authenticator* auth;
    mod_mysql_ap_input_filter_pump in;
    mod_mysql_ap_output_filter_pump out;
    mod_mysql_bucket_factory bf;

    unsigned int packet_seq;
    mod_mysql_client_info* cinfo;
    apr_byte_t pw_seed[32];

    DECLARE_ACCESSOR(cmd_hdlr, mod_mysql_command_handler*)
        ACCESSOR_PARAM(mod_mysql_connection)

        inline void init()
        {
            value = 0;
        }

        inline mod_mysql_command_handler* const& get()
        {
            if (!param.cinfo) {
                throw mod_mysql_illegal_state_exception("Not connected");   
            }

            if (!value) {
                value = param.cfg->get_service()->create_command_handler(
                        param.pool, param.cinfo);
            }

            return value;
        }

        inline void set(mod_mysql_command_handler* const& val)
        {
            throw mod_mysql_unsupported_exception("Not supported");
        }
    END_DECLARE_ACCESSOR(cmd_hdlr)

private:
    bool do_greet();
    bool do_initial_handshake();
    bool do_expect_client_greet();
    void do_handle_packet(mod_mysql_pool* my_pool, const apr_byte_t* pkt,
            apr_size_t pkt_sz);
    mod_mysql_server_message* do_authenticate(mod_mysql_pool* pool,
            const mod_mysql_ranged_ref<const apr_byte_t>& pw_hash);
    void dump(const unsigned char* buf, apr_size_t sz);

public:
    mod_mysql_connection(conn_rec* conn,
            mod_mysql_config* cfg,
            mod_mysql_logger* logger,
            mod_mysql_authenticator* auth);

    int process();
    void pack_and_flush();
    void reply(const mod_mysql_server_message& msg);
    void reply(const mod_mysql_result& msg);
    apr_size_t read(unsigned char* buf, apr_size_t buf_sz);
    void waste(apr_size_t buf_sz);
    apr_size_t wait_for_packet();
};

#endif /* _MOD_MYSQL_CONNECTION_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
