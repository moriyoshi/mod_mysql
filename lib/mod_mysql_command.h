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
#ifndef _MOD_MYSQL_COMMAND_H
#define _MOD_MYSQL_COMMAND_H

enum mod_mysql_command {
    MOD_MYSQL_COMMAND_SLEEP               =   0,
    MOD_MYSQL_COMMAND_QUIT                =   1,
    MOD_MYSQL_COMMAND_INIT_DB             =   2,
    MOD_MYSQL_COMMAND_QUERY               =   3,
    MOD_MYSQL_COMMAND_FIELD_LIST          =   4,
    MOD_MYSQL_COMMAND_CREATE_DB           =   5,
    MOD_MYSQL_COMMAND_DROP_DB             =   6,
    MOD_MYSQL_COMMAND_REFRESH             =   7,
    MOD_MYSQL_COMMAND_SHUTDOWN            =   8,
    MOD_MYSQL_COMMAND_STATISTICS          =   9,
    MOD_MYSQL_COMMAND_PROCESS_INFO        =  10,
    MOD_MYSQL_COMMAND_CONNECT             =  11,
    MOD_MYSQL_COMMAND_PROCESS_KILL        =  12,
    MOD_MYSQL_COMMAND_DEBUG               =  13,
    MOD_MYSQL_COMMAND_PING                =  14,
    MOD_MYSQL_COMMAND_TIME                =  15,
    MOD_MYSQL_COMMAND_DELAYED_INSERT      =  16,
    MOD_MYSQL_COMMAND_CHANGE_USER         =  17,
    MOD_MYSQL_COMMAND_BINLOG_DUMP         =  18,
    MOD_MYSQL_COMMAND_TABLE_DUMP          =  19,
    MOD_MYSQL_COMMAND_CONNECT_OUT         =  20,
    MOD_MYSQL_COMMAND_REGISTER_SLAVE      =  21,
    MOD_MYSQL_COMMAND_PREPARE             =  22,
    MOD_MYSQL_COMMAND_EXECUTE             =  23,
    MOD_MYSQL_COMMAND_LONG_DATA           =  24,
    MOD_MYSQL_COMMAND_CLOSE_STMT          =  25,
    MOD_MYSQL_COMMAND_RESET_STMT          =  26,
    MOD_MYSQL_COMMAND_SET_OPTION          =  27,
    MOD_MYSQL_COMMAND_FETCH               =  28,
    MOD_MYSQL_COMMAND_UPLOAD_FILE         = 251,
    MOD_MYSQL_COMMAND_MESSAGE             = 255
};

#endif /* _MOD_MYSQL_COMMAND_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
