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
#ifndef _MOD_MYSQL_LOGGER_H
#define _MOD_MYSQL_LOGGER_H

#ifdef HAVE_STDARG_H
#include <stdarg.h>
#endif

#include <httpd.h>
#include <http_log.h>
#include "mod_mysql_error.h"

class mod_mysql_logger
{
public:
    static const int EMERG  = APLOG_EMERG;
    static const int ALERT  = APLOG_ALERT;
    static const int CRIT   = APLOG_CRIT;
    static const int ERR    = APLOG_ERR;
    static const int WARN   = APLOG_WARNING;
    static const int NOTICE = APLOG_NOTICE;
    static const int INFO   = APLOG_INFO;
    static const int DEBUG  = APLOG_DEBUG;
public:
    virtual void log(const char* file, int line, int level,
            const char* msg, apr_status_t status = APR_SUCCESS) = 0;
    void log(const char* file, int line, int level,
            apr_status_t status, const char* fmt, va_list args);
    void log(const char* file, int line, int level,
            apr_status_t status, const char* fmt, ...);
    void log(int level, const mod_mysql_error& e);
    void emerg(const char *file, int line, apr_status_t status,
            const char* fmt, ...);
    void alert(const char *file, int line, apr_status_t status,
            const char* fmt, ...);
    void crit(const char *file, int line, apr_status_t status,
            const char* fmt, ...);
    void err(const char *file, int line, apr_status_t status,
            const char* fmt, ...);
    void warn(const char *file, int line, apr_status_t status,
            const char* fmt, ...);
    void notice(const char *file, int line, apr_status_t status,
            const char* fmt, ...);
    void info(const char *file, int line, apr_status_t status,
            const char* fmt, ...);
    void debug(const char *file, int line, apr_status_t status,
            const char* fmt, ...);
};

// {{{ mod_mysql_logger::log()
inline void mod_mysql_logger::log(const char* file, int line, int level,
            apr_status_t status, const char* fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    log(file, line, level, status, fmt, args);
    va_end(args);
}

inline void mod_mysql_logger::log(int level, const mod_mysql_error& e)
{
    log(e.get_file_name(), e.get_line_number(), level,
            e.get_message(), e.get_status());
}
// }}}

// {{{ mod_mysql_logger::emerg()
inline void mod_mysql_logger::emerg(const char *file, int line,
        apr_status_t status, const char* fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    log(file, line, EMERG, status, fmt, args);
    va_end(args);
}
// }}}

// {{{ mod_mysql_logger::alert()
inline void mod_mysql_logger::alert(const char *file, int line,
        apr_status_t status, const char* fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    log(file, line, ALERT, status, fmt, args);
    va_end(args);
}
// }}}

// {{{ mod_mysql_logger::crit()
inline void mod_mysql_logger::crit(const char *file, int line,
        apr_status_t status, const char* fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    log(file, line, CRIT, status, fmt, args);
    va_end(args);
}
// }}}

// {{{ mod_mysql_logger::err()
inline void mod_mysql_logger::err(const char *file, int line,
        apr_status_t status, const char* fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    log(file, line, ERR, status, fmt, args);
    va_end(args);
}
// }}}

// {{{ mod_mysql_logger::warn()
inline void mod_mysql_logger::warn(const char *file, int line,
        apr_status_t status, const char* fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    log(file, line, WARN, status, fmt, args);
    va_end(args);
}
// }}}

// {{{ mod_mysql_logger::notice()
inline void mod_mysql_logger::notice(const char *file, int line,
        apr_status_t status, const char* fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    log(file, line, NOTICE, status, fmt, args);
    va_end(args);
}
// }}}

// {{{ mod_mysql_logger::info()
inline void mod_mysql_logger::info(const char *file, int line,
        apr_status_t status, const char* fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    log(file, line, INFO, status, fmt, args);
    va_end(args);
}
// }}}

// {{{ mod_mysql_logger::debug()
inline void mod_mysql_logger::debug(const char *file, int line,
        apr_status_t status, const char* fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    log(file, line, DEBUG, status, fmt, args);
    va_end(args);
}
// }}}

#endif /* _MOD_MYSQL_LOGGER_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
