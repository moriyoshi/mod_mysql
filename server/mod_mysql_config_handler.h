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
#ifndef _MOD_MYSQL_CONFIG_HANDLER_H
#define _MOD_MYSQL_CONFIG_HANDLER_H

#include <httpd.h>
#include <http_config.h>

#include "mod_mysql_config.h"
#include "mod_mysql_config_exception.h"
#include "mod_mysql_unsupported_exception.h"
#include "mod_mysql_delegates.h"

template<class _Tcfg>
class mod_mysql_config_traits
{
public:
    typedef _Tcfg config_t;
    static const module* get_module_rec();
};

template<class _Ttraits, enum cmd_how _Vhow>
class mod_mysql_config_handler
{
public:
    class arg_iterator: public mod_mysql_iterator<const char*> {
    public:
        typedef const char* item_t;

    public:
        apr_pool_t* pool;
        const char* args;
        const char* cur;

    public:
        arg_iterator(apr_pool_t* pool, const char* args);
        virtual item_t& current();
        virtual bool valid();
        virtual void remove();
        virtual void next();
    };

    typedef mod_mysql_arg_0_dg<typename _Ttraits::config_t, void> no_args_dg_t;
    typedef mod_mysql_arg_1_dg<typename _Ttraits::config_t, void, bool> bool_arg_dg_t;
    typedef mod_mysql_arg_1_dg<typename _Ttraits::config_t, void, const char*> arg_1_dg_t;
    typedef mod_mysql_arg_1_dg<typename _Ttraits::config_t, void, arg_iterator&> raw_args_dg_t;
    typedef mod_mysql_arg_2_dg<typename _Ttraits::config_t, void, const char*, const char*> arg_2_dg_t;
    typedef mod_mysql_arg_3_dg<typename _Ttraits::config_t, void, const char*, const char*, const char*> arg_3_dg_t;

    typedef enum cmd_how cmd_how_t;

    class rec: public command_rec {
        static const enum cmd_how how = _Vhow;

    public:
        rec(const char* name, int override, const char* msg,
                typename no_args_dg_t::func_t setter);

        rec(const char* name, int override, const char* msg,
                typename bool_arg_dg_t::func_t setter);

        rec(const char* name, int override, const char* msg,
                typename arg_1_dg_t::func_t setter);

        rec(const char* name, int override, const char* msg,
                typename arg_2_dg_t::func_t setter);

        rec(const char* name, int override, const char* msg,
                typename arg_3_dg_t::func_t setter);

        rec(const char* name, int override, const char* msg,
                typename raw_args_dg_t::func_t setter);
    };

public:
    static const char* raw_args(cmd_parms *parms, void *mconfig,
            const char *args);

    static const char* no_args(cmd_parms *parms, void *mconfig);

    static const char* arg_1(cmd_parms *parms, void *mconfig,
            const char *w);

    static const char* arg_2(cmd_parms *parms, void *mconfig,
            const char *w, const char *w2);

    static const char* arg_3(cmd_parms *parms, void *mconfig,
            const char *w, const char *w2, const char *w3);

    static const char* flag(cmd_parms *parms, void *mconfig, int on);
};

template<class _Ttraits, enum cmd_how _Vhow> inline
mod_mysql_config_handler<_Ttraits, FLAG>::rec::rec(const char* name,
        int override, const char* msg,
        typename bool_arg_dg_t::func_t setter)
{
    this->name = name;
    this->req_override = override;
    this->errmsg = msg;
    this->args_how = how;
    this->AP_FLAG = reinterpret_cast<const char* (*)()>(
            mod_mysql_config_handler::flag);
    this->cmd_data = bool_arg_dg_t(setter);
}

template<class _Ttraits, enum cmd_how _Vhow> inline
mod_mysql_config_handler<_Ttraits, RAW_ARGS>::rec::rec(const char* name,
        int override, const char* msg, typename raw_args_dg_t::func_t setter)
{
    this->name = name;
    this->req_override = override;
    this->errmsg = msg;
    this->args_how = how;
    this->AP_RAW_ARGS = reinterpret_cast<const char* (*)()>(
            mod_mysql_config_handler::raw_args);
    this->cmd_data = raw_args_dg_t(setter);
}

template<class _Ttraits, enum cmd_how _Vhow> inline
mod_mysql_config_handler<_Ttraits, NO_ARGS>::rec::rec(const char* name,
        int override, const char* msg, typename no_args_dg_t::func_t setter)
{
    this->name = name;
    this->req_override = override;
    this->errmsg = msg;
    this->args_how = how;
    this->AP_NO_ARGS = reinterpret_cast<const char * (*)()>(
            mod_mysql_config_handler::no_args);
    this->cmd_data = no_args_dg_t(setter); 
}

template<class _Ttraits, enum cmd_how _Vhow> inline
mod_mysql_config_handler<_Ttraits, _Vhow>::rec::rec(const char* name,
        int override, const char* msg, typename arg_1_dg_t::func_t setter)
{
    this->name = name;
    this->req_override = override;
    this->errmsg = msg;
    this->args_how = how;
    this->AP_TAKE1 = reinterpret_cast<const char * (*)()>(
            mod_mysql_config_handler::arg_1);
    this->cmd_data = arg_1_dg_t(setter);
}

template<class _Ttraits, enum cmd_how _Vhow> inline
mod_mysql_config_handler<_Ttraits, _Vhow>::rec::rec(const char* name,
        int override, const char* msg, typename arg_2_dg_t::func_t setter)
{
    this->name = name;
    this->req_override = override;
    this->errmsg = msg;
    this->args_how = how;
    this->AP_TAKE2 = reinterpret_cast<const char * (*)()>(
            mod_mysql_config_handler::arg_2);
    this->cmd_data = arg_2_dg_t(setter);
}

template<class _Ttraits, enum cmd_how _Vhow> inline
mod_mysql_config_handler<_Ttraits, _Vhow>::rec::rec(const char* name,
        int override, const char* msg, typename arg_3_dg_t::func_t setter)
{
    this->name = name;
    this->req_override = override;
    this->errmsg = msg;
    this->args_how = how;
    this->AP_TAKE3 = reinterpret_cast<const char * (*)()>(
            mod_mysql_config_handler::arg_3);
    this->cmd_data = arg_3_dg_t(setter);
}

template<class _Ttraits, enum cmd_how _Vhow> inline
mod_mysql_config_handler<_Ttraits, _Vhow>::arg_iterator::arg_iterator(
        apr_pool_t* _pool, const char* _args)
        : pool(_pool), args(_args), cur(0)
{
    if (args && args[0]) {
        cur = ap_getword_conf(pool, &args);
    }
}

template<class _Ttraits, enum cmd_how _Vhow>
typename mod_mysql_config_handler<_Ttraits, _Vhow>::arg_iterator::item_t&
mod_mysql_config_handler<_Ttraits, _Vhow>::arg_iterator::current()
{
    if (!cur) {
        return false;
    }

    return cur;
}

template<class _Ttraits, enum cmd_how _Vhow>
bool mod_mysql_config_handler<_Ttraits, _Vhow>::arg_iterator::valid()
{
    return !!cur;
}

template<class _Ttraits, enum cmd_how _Vhow>
void mod_mysql_config_handler<_Ttraits, _Vhow>::arg_iterator::next()
{
    if (args && args[0]) {
        cur = ap_getword_conf(pool, &args);
    }
}

template<class _Ttraits, enum cmd_how _Vhow>
void mod_mysql_config_handler<_Ttraits, _Vhow>::arg_iterator::remove()
{
    throw mod_mysql_unsupported_exception("Not supported");
}

template<class _Ttraits, enum cmd_how _Vhow>
const char* mod_mysql_config_handler<_Ttraits, _Vhow>::raw_args(
        cmd_parms *parms, void *mconfig, const char *args)
{
    if (!mconfig) {
        if (parms->override_opts & RSRC_CONF) {
            mconfig = ap_get_module_config(parms->server->module_config,
                    _Ttraits::get_module_rec());
        }
    }

    try {
            arg_iterator iter(parms->temp_pool, args);
            (*static_cast<raw_args_dg_t*>(parms->info))(
                    *static_cast<typename _Ttraits::config_t*>(mconfig),
                    iter);
    } catch (mod_mysql_config_exception* ex) {
        return ex->get_message();
    }

    return NULL;
}

template<class _Ttraits, enum cmd_how _Vhow>
const char* mod_mysql_config_handler<_Ttraits, _Vhow>::no_args(
        cmd_parms *parms, void *mconfig)
{
    if (!mconfig) {
        if (parms->override_opts & RSRC_CONF) {
            mconfig = ap_get_module_config(parms->server->module_config,
                    _Ttraits::get_module_rec());
        }
    }

    try {
        (*static_cast<no_args_dg_t*>(parms->info))(
                *static_cast/**/<typename _Ttraits::config_t*>(mconfig));
    } catch (mod_mysql_config_exception* ex) {
        return ex->get_message();
    }

    return NULL;
}

template<class _Ttraits, enum cmd_how _Vhow> const char*
mod_mysql_config_handler<_Ttraits, _Vhow>::arg_1(cmd_parms *parms, void *mconfig,
        const char *w)
{
    if (!mconfig) {
        if (parms->override_opts & RSRC_CONF) {
            mconfig = ap_get_module_config(parms->server->module_config,
                    _Ttraits::get_module_rec());
        }
    }

    try {
        (*static_cast<arg_1_dg_t*>(parms->info))(
                *static_cast/**/<typename _Ttraits::config_t*>(mconfig), w);
    } catch (mod_mysql_config_exception* ex) {
        return ex->get_message();
    }

    return NULL;
}

template<class _Ttraits, enum cmd_how _Vhow>
const char* mod_mysql_config_handler<_Ttraits, _Vhow>::arg_2(
        cmd_parms *parms, void *mconfig, const char *w1, const char *w2)
{
    if (!mconfig) {
        if (parms->override_opts & RSRC_CONF) {
            mconfig = ap_get_module_config(parms->server->module_config,
                    _Ttraits::get_module_rec());
        }
    }

    try {
        (*static_cast<arg_2_dg_t*>(parms->info))(
                *static_cast/**/<typename _Ttraits::config_t*>(mconfig), w1, w2);
    } catch (mod_mysql_config_exception* ex) {
        return ex->get_message();
    }

    return NULL;
}

template<class _Ttraits, enum cmd_how _Vhow>
const char* mod_mysql_config_handler<_Ttraits, _Vhow>::arg_3(
        cmd_parms *parms, void *mconfig, const char *w1, const char *w2,
        const char *w3)
{
    if (!mconfig) {
        if (parms->override_opts & RSRC_CONF) {
            mconfig = ap_get_module_config(parms->server->module_config,
                    _Ttraits::get_module_rec());
        }
    }

    try {
        (*static_cast<arg_3_dg_t*>(parms->info))(
                *static_cast/**/<typename _Ttraits::config_t*>(mconfig),
                w1, w2, w3);
    } catch (mod_mysql_config_exception* ex) {
        return ex->get_message();
    }

    return NULL;
}

template<class _Ttraits, enum cmd_how _Vhow>
const char* mod_mysql_config_handler<_Ttraits, _Vhow>::flag(
        cmd_parms *parms, void *mconfig, int on)
{
    if (!mconfig) {
        if (parms->override_opts & RSRC_CONF) {
            mconfig = ap_get_module_config(parms->server->module_config,
                    _Ttraits::get_module_rec());
        }
    }

    try {
        (*static_cast<bool_arg_dg_t*>(parms->info))(
                *static_cast/**/<typename _Ttraits::config_t*>(mconfig), on);
    } catch (mod_mysql_config_exception* ex) {
        return ex->get_message();
    }

    return NULL;
} 

#endif /* _MOD_MYSQL_CONFIG_HANDLER_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
