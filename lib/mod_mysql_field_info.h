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
#ifndef _MOD_MYSQL_FIELD_INFO_H
#define _MOD_MYSQL_FIELD_INFO_H

#include "mod_mysql_field_type.h"
#include "mod_mysql_locale.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MOD_MYSQL_FIELD_ATTR_NOT_NULL        0x0001
#define MOD_MYSQL_FIELD_ATTR_PRI_KEY         0x0002
#define MOD_MYSQL_FIELD_ATTR_UNIQUE_KEY      0x0004
#define MOD_MYSQL_FIELD_ATTR_MULTIPLE_KEY    0x0008
#define MOD_MYSQL_FIELD_ATTR_BLOB            0x0010
#define MOD_MYSQL_FIELD_ATTR_UNSIGNED        0x0020
#define MOD_MYSQL_FIELD_ATTR_ZEROFILL        0x0040
#define MOD_MYSQL_FIELD_ATTR_BINARY          0x0080
#define MOD_MYSQL_FIELD_ATTR_ENUM            0x0100
#define MOD_MYSQL_FIELD_ATTR_AUTO_INCREMENT  0x0200
#define MOD_MYSQL_FIELD_ATTR_TIMESTAMP       0x0400
#define MOD_MYSQL_FIELD_ATTR_SET             0x0800
#define MOD_MYSQL_FIELD_ATTR_NUM             0x8000

#ifndef MOD_MYSQL_FIELD_INFO_T_DEFINED
typedef struct mod_mysql_field_info_t mod_mysql_field_info_t;
#define MOD_MYSQL_FIELD_INFO_T_DEFINED
#endif /* MOD_MYSQL_FIELD_INFO_T_DEFINED */

struct mod_mysql_field_info_t
{
    mod_mysql_field_type_t type;
    unsigned int attrs;
    unsigned long len;
    const char* catalogue;
    const char* db_name;
    const char* tbl_name;
    const char* orig_tbl_name;
    const char* name;
    const char* orig_name;
    mod_mysql_locale_t* loc;
    int num_decs;
    int ord;
};

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
#include "mod_mysql_base.h"
#include "mod_mysql_bitmask.h"

#ifdef HAVE_STRING_H
#include <string.h>
#endif /* HAVE_STRING_H */

class mod_mysql_field_info
        : public mod_mysql_base, public mod_mysql_field_info_t
{
public:
    typedef mod_mysql_field_type_t type_t;
    struct attr_t: public mod_mysql_bitmask_t<unsigned int>
    {
        inline attr_t(): mod_mysql_bitmask_t<unsigned int>(0) {}
        inline attr_t(unsigned int v)
                : mod_mysql_bitmask_t<unsigned int>(v) {}
        static const unsigned int NOT_NULL =
                MOD_MYSQL_FIELD_ATTR_NOT_NULL;
        static const unsigned int PRI_KEY =
                MOD_MYSQL_FIELD_ATTR_PRI_KEY;
        static const unsigned int UNIQUE_KEY =
                MOD_MYSQL_FIELD_ATTR_UNIQUE_KEY;
        static const unsigned int MULTIPLE_KEY =
                MOD_MYSQL_FIELD_ATTR_MULTIPLE_KEY;
        static const unsigned int BLOB =
                MOD_MYSQL_FIELD_ATTR_BLOB;
        static const unsigned int UNSIGNED =
                MOD_MYSQL_FIELD_ATTR_UNSIGNED;
        static const unsigned int ZEROFILL =
                MOD_MYSQL_FIELD_ATTR_ZEROFILL;
        static const unsigned int BINARY =
                MOD_MYSQL_FIELD_ATTR_BINARY;
        static const unsigned int ENUM =
                MOD_MYSQL_FIELD_ATTR_ENUM;
        static const unsigned int AUTO_INCREMENT =
                MOD_MYSQL_FIELD_ATTR_AUTO_INCREMENT;
        static const unsigned int TIMESTAMP =
                MOD_MYSQL_FIELD_ATTR_TIMESTAMP;
        static const unsigned int SET =
                MOD_MYSQL_FIELD_ATTR_SET;
        static const unsigned int NUM =
                MOD_MYSQL_FIELD_ATTR_NUM;
    };

public:
    mod_mysql_field_info(type_t type, attr_t attrs, unsigned long len,
            const char* name, int ord, int num_decs, const char* orig_name = 0,
            const char* tbl_name = 0, const char* orig_tbl_name = 0,
            const char* db_name = 0, mod_mysql_locale_t* loc = 0);
    const char* get_catalogue_name() const;
    const char* get_table_name() const;
    const char* get_original_table_name() const;
    const char* get_database_name() const;
    type_t get_type() const;
    attr_t get_attributes() const;
    const char* get_name() const;
    const char* get_original_name() const;
    mod_mysql_locale* get_locale() const;
    unsigned long get_length() const;
    int get_num_decimals() const;
    int get_ordinal() const;
    bool operator==(const mod_mysql_field_info& that) const;
};

// {{{ mod_mysql_field_info::mod_mysql_field_info()
inline mod_mysql_field_info::mod_mysql_field_info(type_t _type, attr_t _attrs,
        unsigned long _len, const char* _name, int _ord, int _num_decs,
        const char* _orig_name, const char* _tbl_name,
        const char* _orig_tbl_name, const char* _db_name,
        mod_mysql_locale_t* _loc)
{
    type          = _type;
    attrs         = _attrs;
    name          = _name;
    len           = _len;
    ord           = _ord;
    num_decs      = _num_decs;
    orig_name     = _orig_name;
    tbl_name      = _tbl_name;
    orig_tbl_name = _orig_tbl_name;
    db_name       = _db_name;
    loc           = _loc;
}
// }}}

// {{{ mod_mysql_field_info::get_type()
inline mod_mysql_field_info::type_t mod_mysql_field_info::get_type() const
{
    return type;
}
// }}}

// {{{ mod_mysql_field_info::get_attributes()
inline mod_mysql_field_info::attr_t mod_mysql_field_info::get_attributes() const
{
    return attrs;
}
// }}}

// {{{ mod_mysql_field_info::get_catalogue_name()
inline const char* mod_mysql_field_info::get_catalogue_name() const
{
    return catalogue;
}
// }}}

// {{{ mod_mysql_field_info::get_database_name()
inline const char* mod_mysql_field_info::get_database_name() const
{
    return db_name;
}
// }}}

// {{{ mod_mysql_field_info::get_table_name()
inline const char* mod_mysql_field_info::get_table_name() const
{
    return tbl_name;
}
// }}}

// {{{ mod_mysql_field_info::get_original_table_name()
inline const char* mod_mysql_field_info::get_original_table_name() const
{
    return orig_tbl_name;
}
// }}}

// {{{ mod_mysql_field_info::get_name()
inline const char* mod_mysql_field_info::get_name() const
{
    return name;
}
// }}}

// {{{ mod_mysql_field_info::get_original_name()
inline const char* mod_mysql_field_info::get_original_name() const
{
    return orig_name;
}
// }}}

// {{{ mod_mysql_field_info::get_locale()
inline mod_mysql_locale* mod_mysql_field_info::get_locale() const
{
    return static_cast<mod_mysql_locale*>(loc);
}
// }}}

// {{{ mod_mysql_field_info::get_length()
inline unsigned long mod_mysql_field_info::get_length() const
{
    return len;
}
// }}}

// {{{ mod_mysql_field_info::get_num_decimals()
inline int mod_mysql_field_info::get_num_decimals() const
{
    return num_decs;
}
// }}}

// {{{ mod_mysql_field_info::get_ordinal()
inline int mod_mysql_field_info::get_ordinal() const
{
    return ord;
}
// }}}

// {{{ mod_mysql_field_info::operator==()
inline bool
mod_mysql_field_info::operator==(const mod_mysql_field_info& that) const
{
    return type == that.type && len == that.len &&
            strcmp(name, that.name) == 0 &&
            (!catalogue || !that.catalogue
                    || strcmp(catalogue, that.catalogue)) &&
            (!db_name || !that.db_name
                    || strcmp(db_name, that.db_name)) &&
            (!orig_tbl_name || !that.orig_tbl_name
                    || strcmp(orig_tbl_name, that.orig_tbl_name)) &&
            (!loc || !that.loc || *get_locale() == *that.get_locale());
}
// }}}
#endif

#endif /* _MOD_MYSQL_FIELD_INFO_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
