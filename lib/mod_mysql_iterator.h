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
#ifndef _MOD_MYSQL_ITERATOR_H
#define _MOD_MYSQL_ITERATOR_H

template <typename _T> class mod_mysql_iterator
{
public:
    typedef _T item_t;

public:
    virtual item_t& current() = 0;
    virtual bool valid() = 0;
    virtual void next() = 0;
    virtual void remove() = 0;
    mod_mysql_iterator operator++(int);
    mod_mysql_iterator& operator++();
    item_t& operator*();
};

// {{{ mod_mysql_iterator<>::operator()
template <typename _T>
inline mod_mysql_iterator<_T> mod_mysql_iterator<_T>::operator++(int)
{
    mod_mysql_iterator<_T> save = *this;
    next();
    return save;
}

template <typename _T>
inline mod_mysql_iterator<_T>& mod_mysql_iterator<_T>::operator++()
{
    next();
    return *this;
}
// }}}

// {{{ mod_mysql_iterator<>::operator*()
template <typename _T>
inline typename mod_mysql_iterator<_T>::item_t&
mod_mysql_iterator<_T>::operator*()
{
    return current();
}
// }}}

#endif /* _MOD_MYSQL_ITERATOR_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
