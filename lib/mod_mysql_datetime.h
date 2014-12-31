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
#ifndef _MOD_MYSQL_DATETIME_H
#define _MOD_MYSQL_DATETIME_H

#ifdef __cplusplus
extern "C" {
#endif

apr_status_t
mod_mysql_datetime_parse_sql(apr_time_exp_t *self, const unsigned char *repr);

apr_status_t
mod_mysql_datetime_as_sql(apr_time_exp_t *self, unsigned char **retval,
        apr_pool_t *pool);

#ifdef __cplusplus
}
#endif

#endif /* _MOD_MYSQL_DATETIME_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
