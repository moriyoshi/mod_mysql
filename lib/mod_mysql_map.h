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
#ifndef _MOD_MYSQL_MAP_H
#define _MOD_MYSQL_MAP_H

#include "mod_mysql_pair.h"
#include "mod_mysql_iterator.h"

class mod_mysql_pool;

template<typename _Tkey, typename _Telem> class mod_mysql_map
{
public:
    typedef mod_mysql_pair<const _Tkey, _Telem> entry;

public:
    virtual void set(const _Tkey& key, const _Telem& val) = 0;
    virtual _Telem* get(const _Tkey& key) = 0;
    virtual void remove(const _Tkey& key) = 0;
    virtual int count() = 0;
    virtual mod_mysql_iterator<entry>*
        create_iterator(mod_mysql_pool* p) const = 0;
};

#endif /* _MOD_MYSQL_MAP_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
