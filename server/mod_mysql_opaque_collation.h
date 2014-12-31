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
#ifndef _MOD_MYSQL_OPAQUE_COLLATION_H
#define _MOD_MYSQL_OPAQUE_COLLATION_H

#include "mod_mysql_base.h"
#include "mod_mysql_collation.h"
#include "mod_mysql_instantiable.h"

class mod_mysql_opaque_collation
        : public mod_mysql_collation, mod_mysql_base,
          public mod_mysql_instantiable
{
private:
    const char* name;
public:
    mod_mysql_opaque_collation(const char* name);

    virtual const char* get_name();
    virtual mod_mysql_charset* get_target_charset();
    virtual int compare(mod_mysql_opaque_cp_t first,
            mod_mysql_opaque_cp_t second);
};

#endif /* _MOD_MYSQL_OPAQUE_COLLATION_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
