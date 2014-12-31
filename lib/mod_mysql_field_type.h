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
#ifndef _MOD_MYSQL_FIELD_TYPE_H
#define _MOD_MYSQL_FIELD_TYPE_H

typedef enum mod_mysql_field_type_t
{
    MOD_MYSQL_FIELD_TYPE_DECIMAL     =   0,
    MOD_MYSQL_FIELD_TYPE_TINY        =   1,
    MOD_MYSQL_FIELD_TYPE_SHORT       =   2,
    MOD_MYSQL_FIELD_TYPE_LONG        =   3,
    MOD_MYSQL_FIELD_TYPE_FLOAT       =   4,
    MOD_MYSQL_FIELD_TYPE_DOUBLE      =   5,
    MOD_MYSQL_FIELD_TYPE_NULL        =   6,
    MOD_MYSQL_FIELD_TYPE_TIMESTAMP   =   7,
    MOD_MYSQL_FIELD_TYPE_LONGLONG    =   8,
    MOD_MYSQL_FIELD_TYPE_INT24       =   9,
    MOD_MYSQL_FIELD_TYPE_DATE        =  10,
    MOD_MYSQL_FIELD_TYPE_TIME        =  11,
    MOD_MYSQL_FIELD_TYPE_DATETIME    =  12,
    MOD_MYSQL_FIELD_TYPE_YEAR        =  13,
    MOD_MYSQL_FIELD_TYPE_NEWDATE     =  14,
    MOD_MYSQL_FIELD_TYPE_ENUM        = 247,
    MOD_MYSQL_FIELD_TYPE_SET         = 248,
    MOD_MYSQL_FIELD_TYPE_TINY_BLOB   = 249,
    MOD_MYSQL_FIELD_TYPE_MEDIUM_BLOB = 250,
    MOD_MYSQL_FIELD_TYPE_LONG_BLOB   = 251,
    MOD_MYSQL_FIELD_TYPE_BLOB        = 252,
    MOD_MYSQL_FIELD_TYPE_VAR_STRING  = 253,
    MOD_MYSQL_FIELD_TYPE_STRING      = 254,
    MOD_MYSQL_FIELD_TYPE_GEOMETRY    = 255
} mod_mysql_field_type_t;

#endif /* _MOD_MYSQL_FIELD_TYPE_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
