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
#ifndef _MOD_MYSQL_STD_LOGGER_H
#define _MOD_MYSQL_STD_LOGGER_H

#include <httpd.h>

#include "mod_mysql_base.h"
#include "mod_mysql_logger.h"
#include "mod_mysql_instantiable.h"

class mod_mysql_std_logger
        : public mod_mysql_base, public mod_mysql_logger,
          public mod_mysql_instantiable
{
private:
    apr_pool_t* p;

public:
    mod_mysql_std_logger(apr_pool_t* p);

    mod_mysql_std_logger(const mod_mysql_std_logger& that);

    virtual void log(const char* file, int line, int level,
            const char *msg, apr_status_t status);
};

// {{{ mod_mysql_std_logger::mod_mysql_std_logger()
inline mod_mysql_std_logger::mod_mysql_std_logger(apr_pool_t* _p)
        : p(_p)
{
}

inline mod_mysql_std_logger::mod_mysql_std_logger(
        const mod_mysql_std_logger& that)
        : p(that.p)
{
}
// }}}

#endif /* _MOD_MYSQL_STD_LOGGER_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
