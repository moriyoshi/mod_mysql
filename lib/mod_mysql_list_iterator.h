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
#ifndef _MOD_MYSQL_LIST_ITERATOR_H
#define _MOD_MYSQL_LIST_ITERATOR_H

#include "mod_mysql_base.h"
#include "mod_mysql_ordinal_iterator.h"
#include "mod_mysql_instantiable.h"

template<typename _Tcntnr>
class mod_mysql_list_iterator
        : public mod_mysql_ordinal_iterator<typename _Tcntnr::item_t>,
          public mod_mysql_instantiable,
          public mod_mysql_base
{
public:
    typedef typename mod_mysql_ordinal_iterator<
            typename _Tcntnr::item_t>::item_t item_t;

private:
    _Tcntnr& cntnr;
    int idx;
    int cnt;
    bool removed;
    bool retrieved;

public:
    mod_mysql_list_iterator(_Tcntnr& cntnr);
    virtual item_t& current();
    virtual bool valid();
    virtual int get_index();
    virtual void next();
    virtual void prev();
    virtual void remove();
    virtual void replace(const item_t& item);
    virtual int count() const;
};


// {{{ mod_mysql_list_iterator<>::mod_mysql_list_iterator()
template<typename _Tcntnr>
mod_mysql_list_iterator<_Tcntnr>::mod_mysql_list_iterator(_Tcntnr& _cntnr)
        : cntnr(_cntnr), idx(0), cnt(_cntnr.count()), removed(false)
{
}
// }}}

// {{{ mod_mysql_list_iterator<>::current()
template<typename _Tcntnr>
typename mod_mysql_list_iterator<_Tcntnr>::item_t&
mod_mysql_list_iterator<_Tcntnr>::current()
{
    if (removed)
        throw mod_mysql_illegal_state_exception("Element is already removed.");

    if (idx >= cnt)
        throw mod_mysql_illegal_state_exception("All the elements have already been enumerated");

    return cntnr.refer(idx);
}
// }}}

// {{{ mod_mysql_list_iterator<>::valid()
template<typename _Tcntnr>
bool mod_mysql_list_iterator<_Tcntnr>::valid()
{
    return idx < cnt;
}
// }}}

// {{{ mod_mysql_list_iterator<>::get_index()
template<typename _Tcntnr>
int mod_mysql_list_iterator<_Tcntnr>::get_index()
{
    return removed ? idx + 1: idx;
}
// }}}

// {{{ mod_mysql_list_iterator<>::next()
template<typename _Tcntnr>
void mod_mysql_list_iterator<_Tcntnr>::next()
{
    if (idx < cnt) {
        removed = false;
        idx++;
    }
}
// }}}

// {{{ mod_mysql_list_iterator<>::prev()
template<typename _Tcntnr>
void mod_mysql_list_iterator<_Tcntnr>::prev()
{
    if (idx >= 0) {
        removed = false;
        idx--;
    }
}
// }}}

// {{{ mod_mysql_list_iterator<>::remove()
template<typename _Tcntnr>
void mod_mysql_list_iterator<_Tcntnr>::remove()
{
    if (removed)
        throw mod_mysql_illegal_state_exception("Element is already removed.");

    removed = true;
    cntnr.remove(idx);
    idx--;
    cnt--;
}
// }}}

// {{{ mod_mysql_list_iterator<>::replace()
template<typename _Tcntnr>
void mod_mysql_list_iterator<_Tcntnr>::replace(const item_t& v)
{
    if (removed)
        throw mod_mysql_illegal_state_exception("Element is already removed.");

    cntnr.set(idx, v);
}
// }}}

// {{{ mod_mysql_list_iterator<>::count()
template<typename _Tcntnr>
int mod_mysql_list_iterator<_Tcntnr>::count() const
{
    return cntnr.count();
}
// }}}

#endif /* _MOD_MYSQL_LIST_ITERATOR_H */
