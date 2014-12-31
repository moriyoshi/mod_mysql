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
#ifndef _MOD_MYSQL_CAPABILITIES_H
#define _MOD_MYSQL_CAPABILITIES_H

#define MOD_MYSQL_CAP_LONG_PASSWORD        0x00001
#define MOD_MYSQL_CAP_FOUND_ROWS           0x00002
#define MOD_MYSQL_CAP_LONG_FLAG            0x00004
#define MOD_MYSQL_CAP_CONNECT_WITH_DB      0x00008
#define MOD_MYSQL_CAP_NO_SCHEMA            0x00010
#define MOD_MYSQL_CAP_COMPRESS             0x00020
#define MOD_MYSQL_CAP_ODBC                 0x00040
#define MOD_MYSQL_CAP_LOCAL_FILES          0x00080
#define MOD_MYSQL_CAP_IGNORE_SPACE         0x00100
#define MOD_MYSQL_CAP_PROTOCOL_41          0x00200
#define MOD_MYSQL_CAP_INTERACTIVE          0x00400
#define MOD_MYSQL_CAP_SSL                  0x00800
#define MOD_MYSQL_CAP_IGNORE_SIGPIPE       0x01000
#define MOD_MYSQL_CAP_TRANSACTIONS         0x02000
#define MOD_MYSQL_CAP_SECURE_CONNECTION    0x08000
#define MOD_MYSQL_CAP_MULTI_STATEMENTS     0x10000
#define MOD_MYSQL_CAP_MULTI_RESULTS        0x20000

typedef unsigned int mod_mysql_capabilities_t;

#ifdef __cplusplus
#include "mod_mysql_bitmask.h"

struct mod_mysql_capabilities
        : public mod_mysql_bitmask_t<mod_mysql_capabilities_t>
{
    inline mod_mysql_capabilities()
            : mod_mysql_bitmask_t<unsigned int>(0) {}
    inline mod_mysql_capabilities(unsigned int v)
            : mod_mysql_bitmask_t<unsigned int>(v) {}
    static const unsigned int LONG_PASSWORD        =
            MOD_MYSQL_CAP_LONG_PASSWORD;
    static const unsigned int FOUND_ROWS           =
            MOD_MYSQL_CAP_FOUND_ROWS;
    static const unsigned int LONG_FLAG            =
            MOD_MYSQL_CAP_LONG_FLAG;
    static const unsigned int CONNECT_WITH_DB      =
            MOD_MYSQL_CAP_CONNECT_WITH_DB;
    static const unsigned int NO_SCHEMA            =
            MOD_MYSQL_CAP_NO_SCHEMA;
    static const unsigned int COMPRESS             =
            MOD_MYSQL_CAP_COMPRESS;
    static const unsigned int ODBC                 =
            MOD_MYSQL_CAP_ODBC;
    static const unsigned int LOCAL_FILES          =
            MOD_MYSQL_CAP_LOCAL_FILES;
    static const unsigned int IGNORE_SPACE         =
            MOD_MYSQL_CAP_IGNORE_SPACE;
    static const unsigned int PROTOCOL_41          =
            MOD_MYSQL_CAP_PROTOCOL_41;
    static const unsigned int INTERACTIVE          =
            MOD_MYSQL_CAP_INTERACTIVE;
    static const unsigned int SSL                  =
            MOD_MYSQL_CAP_SSL;
    static const unsigned int IGNORE_SIGPIPE       =
            MOD_MYSQL_CAP_IGNORE_SIGPIPE;
    static const unsigned int TRANSACTIONS         =
            MOD_MYSQL_CAP_TRANSACTIONS;
    static const unsigned int SECURE_CONNECTION  =
            MOD_MYSQL_CAP_SECURE_CONNECTION;
    static const unsigned int MULTI_STATEMENTS     =
            MOD_MYSQL_CAP_MULTI_STATEMENTS;
    static const unsigned int MULTI_RESULTS        =
            MOD_MYSQL_CAP_MULTI_RESULTS;
};
#endif

#endif /* _MOD_MYSQL_CAPABILITIES_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
