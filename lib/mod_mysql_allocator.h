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

#include "mod_mysql_memblock.h"
#ifndef _MOD_MYSQL_ALLOCATOR_H

#include "safety.h"
#include "mod_mysql_mapper.h"

class mod_mysql_allocator: public mod_mysql_mapper
{
public:
    virtual void* alloc(size_t sz) = 0;
    template<typename _T> _T* alloc_obj(int m = 1);
    template<typename _T> mod_mysql_memblock<_T>* alloc_block(int m = 1);
    virtual void release(void* ptr) = 0;
    virtual void* realloc(void* ptr, size_t sz) = 0;
};

// {{{ mod_mysql_allocator::alloc_block()
template<typename _T>
inline mod_mysql_memblock<_T>* mod_mysql_allocator::alloc_block(int m)
{
    return mod_mysql_memblock<_T>::create(this,
            safe_size_t_multiply(sizeof(_T), m));
}
// }}}

// {{{ mod_mysql_allocator::alloc_obj<>()
template<typename _T>
inline _T* mod_mysql_allocator::alloc_obj(int m)
{
    return reinterpret_cast<_T*>(alloc(
            safe_size_t_multiply(sizeof(_T), m)));
}
// }}}

#define _MOD_MYSQL_ALLOCATOR_H
#endif /* _MOD_MYSQL_ALLOCATOR_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
