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
#ifndef _MOD_MYSQL_LOCALE_H
#define _MOD_MYSQL_LOCALE_H

enum mod_mysql_locale_e {
    MOD_MYSQL_LOC_BIG5_CHINESE_CI         =  1,
    MOD_MYSQL_LOC_LATIN2_CZECH_CS         =  2,
    MOD_MYSQL_LOC_DEC8_SWEDISH_CI         =  3,
    MOD_MYSQL_LOC_CP850_GENERAL_CI        =  4,
    MOD_MYSQL_LOC_LATIN1_GERMAN1_CI       =  5,
    MOD_MYSQL_LOC_HP8_ENGLISH_CI          =  6,
    MOD_MYSQL_LOC_KOI8R_GENERAL_CI        =  7,
    MOD_MYSQL_LOC_LATIN1_SWEDISH_CI       =  8,
    MOD_MYSQL_LOC_LATIN2_GENERAL_CI       =  9,
    MOD_MYSQL_LOC_SWE7_SWEDISH_CI         = 10,
    MOD_MYSQL_LOC_ASCII_GENERAL_CI        = 11,
    MOD_MYSQL_LOC_UJIS_JAPANESE_CI        = 12,
    MOD_MYSQL_LOC_SJIS_JAPANESE_CI        = 13,
    MOD_MYSQL_LOC_CP1251_BULGARIAN_CI     = 14,
    MOD_MYSQL_LOC_LATIN1_DANISH_CI        = 15,
    MOD_MYSQL_LOC_HEBREW_GENERAL_CI       = 16,
    MOD_MYSQL_LOC_TIS620_THAI_CI          = 18,
    MOD_MYSQL_LOC_EUCKR_KOREAN_CI         = 19,
    MOD_MYSQL_LOC_LATIN7_ESTONIAN_CS      = 20,
    MOD_MYSQL_LOC_LATIN2_HUNGARIAN_CI     = 21,
    MOD_MYSQL_LOC_KOI8U_GENERAL_CI        = 22,
    MOD_MYSQL_LOC_CP1251_UKRAINIAN_CI     = 23,
    MOD_MYSQL_LOC_GB2312_CHINESE_CI       = 24,
    MOD_MYSQL_LOC_GREEK_GENERAL_CI        = 25,
    MOD_MYSQL_LOC_CP1250_GENERAL_CI       = 26,
    MOD_MYSQL_LOC_LATIN2_CROATIAN_CI      = 27,
    MOD_MYSQL_LOC_GBK_CHINESE_CI          = 28,
    MOD_MYSQL_LOC_CP1257_LITHUANIAN_CI    = 29,
    MOD_MYSQL_LOC_LATIN5_TURKISH_CI       = 30,
    MOD_MYSQL_LOC_LATIN1_GERMAN2_CI       = 31,
    MOD_MYSQL_LOC_ARMSCII8_GENERAL_CI     = 32,
    MOD_MYSQL_LOC_UTF8_GENERAL_CI         = 33,
    MOD_MYSQL_LOC_CP1250_CZECH_CS         = 34,
    MOD_MYSQL_LOC_UCS2_GENERAL_CI         = 35,
    MOD_MYSQL_LOC_CP866_GENERAL_CI        = 36,
    MOD_MYSQL_LOC_KEYBCS2_GENERAL_CI      = 37,
    MOD_MYSQL_LOC_MACCE_GENERAL_CI        = 38,
    MOD_MYSQL_LOC_MACROMAN_GENERAL_CI     = 39,
    MOD_MYSQL_LOC_CP852_GENERAL_CI        = 40,
    MOD_MYSQL_LOC_LATIN7_GENERAL_CI       = 41,
    MOD_MYSQL_LOC_LATIN7_GENERAL_CS       = 42,
    MOD_MYSQL_LOC_MACCE_BIN               = 43,
    MOD_MYSQL_LOC_LATIN1_BIN              = 47,
    MOD_MYSQL_LOC_LATIN1_GENERAL_CI       = 48,
    MOD_MYSQL_LOC_LATIN1_GENERAL_CS       = 49,
    MOD_MYSQL_LOC_CP1251_BIN              = 50,
    MOD_MYSQL_LOC_CP1251_GENERAL_CI       = 51,
    MOD_MYSQL_LOC_CP1251_GENERAL_CS       = 52,
    MOD_MYSQL_LOC_MACROMAN_BIN            = 53,
    MOD_MYSQL_LOC_MACROMAN_CI             = 54,
    MOD_MYSQL_LOC_MACROMAN_CI_AI          = 55,
    MOD_MYSQL_LOC_MACROMAN_CS             = 56,
    MOD_MYSQL_LOC_CP1256_GENERAL_CI       = 57,
    MOD_MYSQL_LOC_CP1257_BIN              = 58,
    MOD_MYSQL_LOC_CP1257_GENERAL_CI       = 59,
    MOD_MYSQL_LOC_CP1257_CI               = 60,
    MOD_MYSQL_LOC_CP1257_CS               = 61,
    MOD_MYSQL_LOC_BINARY                  = 63,
    MOD_MYSQL_LOC_ARMSCII8_BIN            = 64,
    MOD_MYSQL_LOC_ASCII_BIN               = 65,
    MOD_MYSQL_LOC_CP1250_BIN              = 66,
    MOD_MYSQL_LOC_CP1256_BIN              = 67,
    MOD_MYSQL_LOC_CP866_BIN               = 68,
    MOD_MYSQL_LOC_DEC8_BIN                = 69,
    MOD_MYSQL_LOC_GREEK_BIN               = 70,
    MOD_MYSQL_LOC_HEBREW_BIN              = 71,
    MOD_MYSQL_LOC_HP8_BIN                 = 72,
    MOD_MYSQL_LOC_KEYBCS2_BIN             = 73,
    MOD_MYSQL_LOC_KOI8R_BIN               = 74,
    MOD_MYSQL_LOC_KOI8U_BIN               = 75,
    MOD_MYSQL_LOC_LATIN2_BIN              = 77,
    MOD_MYSQL_LOC_LATIN5_BIN              = 78,
    MOD_MYSQL_LOC_LATIN7_BIN              = 79,
    MOD_MYSQL_LOC_CP850_BIN               = 80,
    MOD_MYSQL_LOC_CP852_BIN               = 81,
    MOD_MYSQL_LOC_SWE7_BIN                = 82,
    MOD_MYSQL_LOC_UTF8_BIN                = 83,
    MOD_MYSQL_LOC_BIG5_BIN                = 84,
    MOD_MYSQL_LOC_EUCKR_BIN               = 85,
    MOD_MYSQL_LOC_GB2312                  = 86,
    MOD_MYSQL_LOC_GBK_BIN                 = 87,
    MOD_MYSQL_LOC_SJIS_BIN                = 88,
    MOD_MYSQL_LOC_TIS620_BIN              = 89,
    MOD_MYSQL_LOC_UCS2_BIN                = 90,
    MOD_MYSQL_LOC_UJIS_BIN                = 91,
    MOD_MYSQL_LOC_GEOSTD8_GENERAL_CI      = 92,
    MOD_MYSQL_LOC_GEOSTD8_BIN             = 93,
    MOD_MYSQL_LOC_LATIN1_SPANISH_CI       = 94,
    MOD_MYSQL_LOC_CP932_JAPANESE_CI       = 95,
    MOD_MYSQL_LOC_CP932_BIN               = 96,
    MOD_MYSQL_LOC_EUCJPMS_JAPANESE_CI     = 97,
    MOD_MYSQL_LOC_EUCJPMS_BIN             = 98,
};

#ifndef MOD_MYSQL_COLLATION_T_DEFINED
typedef struct mod_mysql_collation mod_mysql_collation_t;
#define MOD_MYSQL_COLLATION_T_DEFINED
#endif /* MOD_MYSQL_COLLATION_T_DEFINED */

#ifndef MOD_MYSQL_CODESET_T_DEFINED
typedef struct mod_mysql_codeset_t mod_mysql_codeset_t;
#define MOD_MYSQL_CODESET_T_DEFINED
#endif /* MOD_MYSQL_CODESET_T_DEFINED */

typedef struct _mod_mysql_locale_t
{
    enum mod_mysql_locale_e code;
    const char* name;
    const char** aliases;
    mod_mysql_codeset_t* cs;
    mod_mysql_collation_t* col;
} mod_mysql_locale_t;

#ifdef __cplusplus
#include "mod_mysql_base.h"
#include "mod_mysql_instantiable.h"
#include "mod_mysql_collation.h"
#include "mod_mysql_codeset.h"


class mod_mysql_locale
        : public mod_mysql_base, public mod_mysql_locale_t,
          public mod_mysql_instantiable 
{
public:
    typedef enum mod_mysql_locale_e code_e;
public:
    mod_mysql_locale(const code_e code,
            mod_mysql_codeset* cs, mod_mysql_collation* col,
            const char* name);

    mod_mysql_codeset* get_codeset();
    mod_mysql_collation* get_collation();
    const char* get_name();
    const code_e& get_code();
    operator code_e();
    bool operator==(const mod_mysql_locale_t& that) const;
};

// {{{ mod_mysql_locale::mod_mysql_locale()
inline mod_mysql_locale::mod_mysql_locale(const code_e _code,
        mod_mysql_codeset* _cs, mod_mysql_collation* _col,
        const char* _name)
{
    ap_assert(!_col->get_target_charset()
            || _cs->get_charset() == _col->get_target_charset());
    code = _code;
    cs   = _cs;
    col  = _col;
    name = _name;
}
// }}}

// {{{ mod_mysql_locale::operator code_e()
inline mod_mysql_locale::operator code_e()
{
    return get_code();
}
// }}}

// {{{ mod_mysql_locale::get_codeset()
inline mod_mysql_codeset* mod_mysql_locale::get_codeset()
{
    return static_cast<mod_mysql_codeset*>(cs);
}
// }}}

// {{{ mod_mysql_locale::get_collation()
inline mod_mysql_collation* mod_mysql_locale::get_collation()
{
    return col; 
}
// }}}

// {{{ mod_mysql_locale::get_name()
inline const char* mod_mysql_locale::get_name()
{
    return name;
}
// }}}

// {{{ mod_mysql_locale::get_code()
inline const mod_mysql_locale::code_e& mod_mysql_locale::get_code()
{
    return code;
}
// }}}

// {{{ mod_mysql_locale::operator==()
inline bool mod_mysql_locale::operator==(const mod_mysql_locale_t& that) const
{
    return code == that.code; 
}
// }}}
#endif

#endif /* _MOD_MYSQL_LOCALE_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
