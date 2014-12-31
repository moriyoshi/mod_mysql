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
#ifndef _MOD_MYSQL_MEMBLOCK_H

#include <apr.h>
#include "mod_mysql_ranged_ref.h"
#include "mod_mysql_mapper.h"

class mod_mysql_allocator;

// {{{ class mod_mysql_memblock_base
class mod_mysql_memblock_base
{
protected:
    mod_mysql_mapper* mapper;
    mod_mysql_ranged_ref<apr_byte_t> desc;

protected:
    mod_mysql_memblock_base(mod_mysql_mapper* mapper,
            const mod_mysql_ranged_ref<apr_byte_t>& desc);
};

// {{{ mod_mysql_memblock_base::mod_mysql_memblock_base()
inline mod_mysql_memblock_base::mod_mysql_memblock_base(
        mod_mysql_mapper* _mapper,
        const mod_mysql_ranged_ref<apr_byte_t>& _desc)
{
    mapper = _mapper;
    desc   = _desc;
}
// }}}
// }}}

template<typename _T> class mod_mysql_memblock: mod_mysql_memblock_base
{
public:
    static mod_mysql_memblock* create(mod_mysql_allocator* ator, size_t sz);
    static mod_mysql_memblock* get_container(_T* orig);

protected:
    void* operator new(size_t sz, mod_mysql_allocator* ator, size_t blk_sz);

protected:
    mod_mysql_memblock(mod_mysql_mapper* mapper, size_t sz);

public:
    mod_mysql_memblock(mod_mysql_mapper* mapper,
            const mod_mysql_ranged_ref<_T>& desc);
    void operator delete(void* p);
    operator _T*();
    operator const mod_mysql_ranged_ref<_T>&() const;
};

#define _MOD_MYSQL_MEMBLOCK_H
#include "mod_mysql_allocator.h"

// {{{ mod_mysql_memblock<>::mod_mysql_memblock()
template<typename _T>
inline mod_mysql_memblock<_T>::mod_mysql_memblock(
        mod_mysql_mapper* _mapper, size_t _sz)
        : mod_mysql_memblock_base(
                _mapper, mod_mysql_ranged_ref<apr_byte_t>(
                    reinterpret_cast<apr_byte_t*>(
                        const_cast<mod_mysql_memblock<_T>*>(this)) + 1, _sz))
{
}

template<typename _T>
inline mod_mysql_memblock<_T>::mod_mysql_memblock(
        mod_mysql_mapper* _mapper, const mod_mysql_ranged_ref<_T>& _desc)
        : mod_mysql_memblock_base(_mapper,
                reinterpret_cast<const mod_mysql_ranged_ref<apr_byte_t>&>(
                    _desc))
{
}
// }}}

// {{{ mod_mysql_memblock<>::get_container()
template<typename _T>
inline mod_mysql_memblock<_T>*
mod_mysql_memblock<_T>::get_container(_T* orig)
{
    mod_mysql_memblock<_T>* const retval =
            reinterpret_cast<mod_mysql_memblock*>(orig) - 1;
    // possibly unsafe
    mod_mysql_assert(retval->desc.iov_base == orig);
    return retval;
}
// }}}

// {{{ mod_mysql_memblock<>::operator new()
template<typename _T>
inline void* mod_mysql_memblock<_T>::operator new(size_t sz,
        mod_mysql_allocator* ator, size_t blk_sz)
{
    return ator->alloc(blk_sz + sz);
}
// }}}

// {{{ mod_mysql_memblock<>::operator delete()
template<typename _T>
inline void mod_mysql_memblock<_T>::operator delete(void* p)
{
    reinterpret_cast<mod_mysql_memblock*>(p)->mapper->release(p);
}
// }}}

// {{{ mod_mysql_memblock<>::operator _T*()
template<typename _T>
inline mod_mysql_memblock<_T>::operator _T*()
{
    return reinterpret_cast<_T*>(this + 1);
}
// }}}

// {{{ mod_mysql_memblock<>::operator const mod_mysql_ranged_ref&<>()
template<typename _T>
inline mod_mysql_memblock<_T>::operator const mod_mysql_ranged_ref<_T>&() const
{
    return desc;
}
// }}}

// {{{ mod_mysql_memblock<>::create()
template<typename _T>
inline mod_mysql_memblock<_T>* mod_mysql_memblock<_T>::create(
        mod_mysql_allocator* ator, size_t sz)
{
    return new(ator, sz) mod_mysql_memblock(ator, sz);
}
// }}}

#endif /* _MOD_MYSQL_MEMBLOCK_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
