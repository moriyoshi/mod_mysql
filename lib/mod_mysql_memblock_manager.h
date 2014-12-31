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
#ifndef _MOD_MYSQL_MEMBLOCK_MANAGER_H
#define _MOD_MYSQL_MEMBLOCK_MANAGER_H

#include "mod_mysql_memblock.h"
#include "mod_mysql_ptr_map.h"
#include "mod_mysql_array.h"

class mod_mysql_memblock_manager
{
    friend class mod_mysql_memblock_base;

protected:
    mod_mysql_ptr_map<mod_mysql_memblock_base*>* blocks;

protected:
    mod_mysql_memblock_manager(mod_mysql_pool* pool);
    ~mod_mysql_memblock_manager();

    void register_block(mod_mysql_memblock_base* cntnr);
    mod_mysql_memblock_base* get_container(void* ptr);
};

// {{{ mod_mysql_memblock_manager::mod_mysql_memblock_manager()
inline mod_mysql_memblock_manager::mod_mysql_memblock_manager(
        mod_mysql_pool* pool)
        : blocks(new(pool) mod_mysql_ptr_map(pool))
{
}
// }}}

// {{{ mod_mysql_memblock_manager::register_block()
inline void
mod_mysql_memblock_manager::register_block(mod_mysql_memblock_base* cntnr)
{
    blocks.add(cntnr);
}
// }}}

// {{{ mod_mysql_memblock_manager::get_container()
inline mod_mysql_memblock_base*
mod_mysql_memblock_manager::get_container(void* ptr)
{
    mod_mysql_memblock_base** elem = blocks.get(ptr);
    return elem ? *elem: 0;
}
// }}}

#endif /* _MOD_MYSQL_MEMBLOCK_MANAGER_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
