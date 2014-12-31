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
#ifndef _MOD_MYSQL_SERVICE_H
#define _MOD_MYSQL_SERVICE_H

#ifndef MOD_MYSQL_SERVICE_T_DEFINED
#define MOD_MYSQL_SERVICE_T_DEFINED
typedef struct mod_mysql_service_t mod_mysql_service_t;
#endif /* MOD_MYSQL_SERVICE_T_DEFINED */

#ifndef MOD_MYSQL_COMMAND_HANDLER_T_DEFINED
#define MOD_MYSQL_COMMAND_HANDLER_T_DEFINED
typedef struct mod_mysql_command_handler_t mod_mysql_command_handler_t;
#endif /* MOD_MYSQL_COMMAND_HANDLER_T_DEFINED */

#ifndef MOD_MYSQL_CLIENT_INFO_T_DEFINED
typedef struct mod_mysql_client_info_t mod_mysql_client_info_t;
#define MOD_MYSQL_CLIENT_INFO_T_DEFINED
#endif /* MOD_MYSQL_CLIENT_INFO_T_DEFINED */

typedef struct mod_mysql_service_vtbl_t
{
    mod_mysql_command_handler_t *(*create_command_handler_fn)(
            mod_mysql_service_t *self,
            apr_pool_t *pool, mod_mysql_client_info_t *cinfo);
} mod_mysql_service_vtbl_t;

struct mod_mysql_service_t
{
    mod_mysql_service_vtbl_t *_vtbl;
};

#define mod_mysql_service_create_command_handler(self, pool, cinfo) \
        ((mod_mysql_service_t *)(self))->_vtbl->create_command_handler_fn( \
            (mod_mysql_service_t *)(self), pool, cinfo)

#ifdef __cplusplus
class mod_mysql_command_handler;
class mod_mysql_client_info;

class mod_mysql_service
{
public:
    virtual mod_mysql_command_handler* create_command_handler(
            mod_mysql_pool* pool, mod_mysql_client_info* cinfo) = 0;
};

#endif

#endif /* _MOD_MYSQL_SERVICE_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
