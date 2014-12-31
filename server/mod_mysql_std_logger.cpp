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
#include <http_log.h>
#include "mod_mysql_std_logger.h"

// {{{ mod_mysql_std_logger::log()
void mod_mysql_std_logger::log(const char *file, int line, int level,
        const char *msg, apr_status_t status)
{
    ap_log_perror(file, line, level, status, p, "%s", msg); 
}
// }}}

/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
