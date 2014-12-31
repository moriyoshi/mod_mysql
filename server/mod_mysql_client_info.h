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
#ifndef _MOD_MYSQL_CLIENT_INFO_H
#define _MOD_MYSQL_CLIENT_INFO_H

#include "mod_mysql_capabilities.h"
#include "mod_mysql_instantiable.h"
#include "mod_mysql_locale.h"

#ifndef MOD_MYSQL_CLIENT_INFO_T_DEFINED
typedef struct mod_mysql_client_info_t mod_mysql_client_info_t;
#define MOD_MYSQL_CLIENT_INFO_T_DEFINED
#endif /* MOD_MYSQL_CLIENT_INFO_T_DEFINED */

struct mod_mysql_client_info_t
{
    const unsigned char* ipaddr;
    const unsigned char* hostname;
    const unsigned char* logname;
    mod_mysql_capabilities_t capabilities;
    mod_mysql_locale_t* locale;
    unsigned long max_allowed_packet;
    const unsigned char* user;
};

#ifdef __cplusplus
class mod_mysql_client_info
        : public mod_mysql_base, public mod_mysql_client_info_t,
          public mod_mysql_instantiable
{
public:
    mod_mysql_client_info(mod_mysql_capabilities caps,
            mod_mysql_locale_t* loc, unsigned long max_allowed_packet,
            const unsigned char* user, const unsigned char* ipaddr,
            const unsigned char* hostname, const unsigned char* logname);
};

// {{{ mod_mysql_client_info::mod_mysql_client_info()
inline mod_mysql_client_info::mod_mysql_client_info(
        mod_mysql_capabilities _caps,
        mod_mysql_locale_t* _loc, unsigned long _max_allowed_packet,
        const unsigned char* _user, const unsigned char* _ipaddr = 0,
        const unsigned char* _hostname = 0, const unsigned char* _logname = 0)
{
    capabilities       = _caps;
    locale             = _loc;
    max_allowed_packet = _max_allowed_packet;
    user               = _user;
    ipaddr             = _ipaddr;
    hostname           = _hostname;
    logname            = _logname;
}
// }}}
#endif

#endif /* _MOD_MYSQL_CLIENT_INFO_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
