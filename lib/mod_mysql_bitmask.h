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
#ifndef _MOD_MYSQL_BITMASK_H
#define _MOD_MYSQL_BITMASK_H

template<typename _T> struct mod_mysql_bitmask_t
{
private:
    _T v;

public:
    mod_mysql_bitmask_t(const _T v);
    operator _T();
    mod_mysql_bitmask_t operator +(const mod_mysql_bitmask_t& that);
    const mod_mysql_bitmask_t& operator +=(const mod_mysql_bitmask_t& that);
    mod_mysql_bitmask_t operator -(const mod_mysql_bitmask_t& that);
    const mod_mysql_bitmask_t& operator -=(const mod_mysql_bitmask_t& that);
    const mod_mysql_bitmask_t& operator &=(const mod_mysql_bitmask_t& that);
    const mod_mysql_bitmask_t& operator |=(const mod_mysql_bitmask_t& that);
};

// {{{ mod_mysql_bitmask_t<>::mod_mysql_bitmask_t()
template<typename _T>
inline mod_mysql_bitmask_t<_T>::mod_mysql_bitmask_t(const _T _v)
        : v(_v)
{
}
// }}}

// {{{ mod_mysql_bitmask_t<>::operator _T()
template<typename _T>
inline mod_mysql_bitmask_t<_T>::operator _T()
{
    return v;
}
// }}}

// {{{ mod_mysql_bitmask_t<>::operator +()
template<typename _T>
inline mod_mysql_bitmask_t<_T> mod_mysql_bitmask_t<_T>::operator +(
        const mod_mysql_bitmask_t<_T>& that)
{
    return v | that.v;
}
// }}}

// {{{ mod_mysql_bitmask_t<>::operator +=()
template<typename _T>
inline const mod_mysql_bitmask_t<_T>& mod_mysql_bitmask_t<_T>::operator +=(
        const mod_mysql_bitmask_t<_T>& that)
{
    v |= that.v;
    return *this;
}
// }}}

// {{{ mod_mysql_bitmask_t<>::operator -()
template<typename _T>
inline mod_mysql_bitmask_t<_T> mod_mysql_bitmask_t<_T>::operator -(
        const mod_mysql_bitmask_t<_T>& that)
{
    return v & ~that.v;
}
// }}}

// {{{ mod_mysql_bitmask_t<>::operator -=()
template<typename _T>
inline const mod_mysql_bitmask_t<_T>& mod_mysql_bitmask_t<_T>::operator -=(
        const mod_mysql_bitmask_t<_T>& that)
{
    v &= ~that.v;
    return *this;
}
// }}}

// {{{ mod_mysql_bitmask_t<>::operator |()
template<typename _T>
inline const mod_mysql_bitmask_t<_T>& mod_mysql_bitmask_t<_T>::operator |=(
        const mod_mysql_bitmask_t<_T>& that)
{
    v |= that.v;
    return *this;
}
// }}}

// {{{ mod_mysql_bitmask_t<>::operator &()
template<typename _T>
inline const mod_mysql_bitmask_t<_T>& mod_mysql_bitmask_t<_T>::operator &=(
        const mod_mysql_bitmask_t<_T>& that)
{
    v &= that.v;
    return *this;
}
// }}}


#endif /* _MOD_MYSQL_BITMASK_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
