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
#ifndef _MOD_MYSQL_RANGED_REF_H
#define _MOD_MYSQL_RANGED_REF_H

#ifdef HAVE_SYS_UIO_H
#include <sys/uio.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAVE_SYS_UIO_H
typedef struct iovec mod_mysql_ranged_ref_t;
#else
typedef struct mod_mysql_ranged_ref_t
{
    void *iov_base;
    size_t iov_len;
} mod_mysql_ranged_ref_t;
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
#include "mod_mysql_assert.h"
#include "mod_mysql_invalid_argument_exception.h"

#include "safety.h"

template<typename _T> class mod_mysql_ranged_ref
        : public mod_mysql_ranged_ref_t
{
public:
    mod_mysql_ranged_ref();
    mod_mysql_ranged_ref(_T* ptr, size_t count);
    mod_mysql_ranged_ref(_T* ptr, int count);
    mod_mysql_ranged_ref(const mod_mysql_ranged_ref& that);
    int count() const;
    size_t get_size() const;
    _T* get_referent() const;
    operator _T*&();
    operator _T*() const;
    _T& operator [](int idx);
};

// {{{ mod_mysql_ranged_ref<>::mod_mysql_ranged_ref()
template<typename _T>
inline mod_mysql_ranged_ref<_T>::mod_mysql_ranged_ref()
{
    iov_base = 0;
    iov_len = 0;
}

template<typename _T>
inline mod_mysql_ranged_ref<_T>::mod_mysql_ranged_ref(_T* _ptr, size_t _sz)
{
    mod_mysql_assert(_sz % sizeof(_ptr) == 0);
    iov_base = const_cast<void*>(static_cast<void const*>(_ptr));
    iov_len  = _sz;
}

template<typename _T>
inline mod_mysql_ranged_ref<_T>::mod_mysql_ranged_ref(_T* _ptr, int _count)
{
    iov_base = const_cast<void*>(static_cast<void const*>(_ptr));
    iov_len  = safe_size_t_multiply(sizeof(_T), _count);
}
    
template<typename _T>
inline mod_mysql_ranged_ref<_T>::mod_mysql_ranged_ref(
        const mod_mysql_ranged_ref& that)
{
    iov_base = that.iov_base;
    iov_len = that.iov_len;
}
// }}}

// {{{ mod_mysql_ranged_ref<>::count()
template<typename _T>
inline int mod_mysql_ranged_ref<_T>::count() const
{
    return safe_size_t_cast(int, iov_len / sizeof(_T));
}
// }}}

// {{{ mod_mysql_ranged_ref<>::get_size()
template<typename _T>
inline size_t mod_mysql_ranged_ref<_T>::get_size() const
{
    return iov_len;
}
// }}}

// {{{ mod_mysql_ranged_ref<>::get_referent()
template<typename _T>
inline _T* mod_mysql_ranged_ref<_T>::get_referent() const
{
    return static_cast<_T*>(iov_base);
}
// }}}

// {{{ mod_mysql_ranged_ref<>::operator _T*()
template<typename _T>
inline mod_mysql_ranged_ref<_T>::operator _T*&()
{
    return *reinterpret_cast<_T**>(&iov_base);
}

template<typename _T>
inline mod_mysql_ranged_ref<_T>::operator _T*() const
{
    return static_cast<_T*>(iov_base);
}
// }}}

// {{{ mod_mysql_ranged_ref<>::operator[]()
template<typename _T>
inline _T& mod_mysql_ranged_ref<_T>::operator [](int idx)
{
    if (idx < 0 || safe_size_t_multiply(idx, sizeof(_T)) >= iov_len) {
        throw mod_mysql_invalid_argument_exception();        
    }

    return static_cast<_T*>(iov_base)[idx];
}
// }}}

#endif

#endif /* _MOD_MYSQL_RANGED_REF_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
