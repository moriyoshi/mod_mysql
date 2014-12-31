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
#ifndef _MOD_MYSQL_RESULT_H
#define _MOD_MYSQL_RESULT_H

#define MOD_MYSQL_STAT_IN_TRANSACTION           0x0001
#define MOD_MYSQL_STAT_AUTOCOMMIT               0x0002
#define MOD_MYSQL_STAT_MORE_RESULTS             0x0004
#define MOD_MYSQL_STAT_MORE_RESULTS_EXISTS      0x0008
#define MOD_MYSQL_STAT_QUERY_NO_GOOD_INDEX_USED 0x0010
#define MOD_MYSQL_STAT_QUERY_NO_INDEX_USED      0x0020

#ifndef MOD_MYSQL_FIELDS_T_DEFINED
typedef struct mod_mysql_fields_t mod_mysql_fields_t;
#define MOD_MYSQL_FIELDS_T_DEFINED
#endif /* MOD_MYSQL_FIELDS_T_DEFINED */

#ifndef MOD_MYSQL_RESULT_T_DEFINED
typedef struct mod_mysql_result_t mod_mysql_result_t;
#define MOD_MYSQL_RESULT_T_DEFINED
#endif /* MOD_MYSQL_RESULT_T_DEFINED */

struct mod_mysql_result_t
{
    unsigned int code;
    mod_mysql_fields_t *fields;
    int num_affected_rows;
    int num_warnings;
    int last_insert_id;
};

#ifdef __cplusplus
#include "mod_mysql_base.h"
#include "mod_mysql_bitmask.h"
#include "mod_mysql_instantiable.h"

class mod_mysql_pool;
class mod_mysql_fields;

class mod_mysql_result
        : public mod_mysql_base, public mod_mysql_instantiable
{
public:
    struct code_t: public mod_mysql_bitmask_t<unsigned int>
    {
        inline code_t(): mod_mysql_bitmask_t<unsigned int>(0) {}
        inline code_t(unsigned int v)
                : mod_mysql_bitmask_t<unsigned int>(v) {}
        static const unsigned int IN_TRANSACTION           =
                MOD_MYSQL_STAT_IN_TRANSACTION;
        static const unsigned int AUTOCOMMIT               =
                MOD_MYSQL_STAT_AUTOCOMMIT;
        static const unsigned int MORE_RESULTS             =
                MOD_MYSQL_STAT_MORE_RESULTS;
        static const unsigned int MORE_RESULTS_EXISTS      =
                MOD_MYSQL_STAT_MORE_RESULTS_EXISTS;
        static const unsigned int QUERY_NO_GOOD_INDEX_USED =
                MOD_MYSQL_STAT_QUERY_NO_GOOD_INDEX_USED;
        static const unsigned int QUERY_NO_INDEX_USED      =
                MOD_MYSQL_STAT_QUERY_NO_INDEX_USED;
    };
private:
    mod_mysql_result_t* dgt;
    mod_mysql_pool* pool;
    mod_mysql_fields* fields;

public:
    mod_mysql_result(mod_mysql_pool* pool,
            code_t code, mod_mysql_fields* fields = 0, 
            int num_affected_rows = 0, int num_warnings = 0,
            int last_insert_id = 0);
    mod_mysql_result(mod_mysql_pool* pool, mod_mysql_result_t* dgt);
    mod_mysql_fields* get_fields() const;
    int get_num_affected_rows() const;
    int get_last_insert_id() const;
    int get_num_warnings() const;
    code_t get_code() const;
};

// {{{ mod_mysql_result::get_num_affected_rows()
inline int mod_mysql_result::get_num_affected_rows() const
{
    return dgt->num_affected_rows;
}
// }}}

// {{{ mod_mysql_result::get_last_insert_id()
inline int mod_mysql_result::get_last_insert_id() const
{
    return dgt->last_insert_id;
}
// }}}

// {{{ mod_mysql_result::get_num_warnings()
inline int mod_mysql_result::get_num_warnings() const
{
    return dgt->num_warnings;
}
// }}}

// {{{ mod_mysql_result::get_code()
inline mod_mysql_result::code_t mod_mysql_result::get_code() const
{
    return dgt->code;
}
// }}}
#endif

#endif /* _MOD_MYSQL_RESULT_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
