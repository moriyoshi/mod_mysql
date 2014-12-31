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
#ifndef _MOD_MYSQL_ORDINAL_ITERATOR_H
#define _MOD_MYSQL_ORDINAL_ITERATOR_H

#include "mod_mysql_iterator.h"

template <typename _T> class mod_mysql_ordinal_iterator
        : public mod_mysql_iterator<_T>
{
public:
    virtual typename mod_mysql_iterator<_T>::item_t& current() = 0;
    virtual bool valid() = 0;
    virtual int get_index() = 0;
    virtual void next() = 0;
    virtual void prev() = 0;
    virtual void remove() = 0;
    virtual void replace(const typename mod_mysql_iterator<_T>::item_t& item);
    virtual int count() const = 0;
    mod_mysql_ordinal_iterator operator--(int);
    mod_mysql_ordinal_iterator& operator--();
};

// {{{ mod_mysql_ordinal_iterator<>::replace()
template <typename _T>
void mod_mysql_ordinal_iterator<_T>::replace(
        const typename mod_mysql_iterator<_T>::item_t& item)
{
    current() = item;
}
// }}}

// {{{ mod_mysql_ordinal_iterator<>::operator--()
template <typename _T>
mod_mysql_ordinal_iterator<_T> mod_mysql_ordinal_iterator<_T>::operator--(int)
{
    mod_mysql_ordinal_iterator<_T> save = *this;
    prev();
    return save;
}

template <typename _T>
mod_mysql_ordinal_iterator<_T>& mod_mysql_ordinal_iterator<_T>::operator--()
{
    prev();
    return *this;
}
// }}}

#endif /* _MOD_MYSQL_ORDINAL_ITERATOR_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
