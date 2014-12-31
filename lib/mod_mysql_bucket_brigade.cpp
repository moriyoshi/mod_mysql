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
#include "mod_mysql_pool.h"
#include "mod_mysql_exception.h"
#include "mod_mysql_bucket_brigade.h"

// {{{ mod_mysql_bucket_brigade::create_iterator()
mod_mysql_iterator<mod_mysql_bucket*>*
mod_mysql_bucket_brigade::create_iterator(mod_mysql_pool* pool) const
{
    return new(pool) iterator(this);
}
// }}}

// {{{ mod_mysql_bucket_brigade::iterator::iterator()
mod_mysql_bucket_brigade::iterator::iterator(const mod_mysql_bucket_brigade* bb)
        : cur(static_cast<mod_mysql_bucket*>(APR_BRIGADE_FIRST(bb->bb))),
          sentinel(static_cast<mod_mysql_bucket*>(
              APR_BRIGADE_SENTINEL(bb->bb))),
          nxt(0), removed(false)
{
}
// }}}

// {{{ mod_mysql_bucket_brigade::iterator::current()
mod_mysql_bucket*& mod_mysql_bucket_brigade::iterator::current()
{
    if (!cur || cur == sentinel)
        throw mod_mysql_exception("All the items have already been iterated");
    if (removed)
        throw mod_mysql_exception("Element is already removed");
    if (!nxt)
        nxt = cur->get_next();
    return cur;
}
// }}}

// {{{ mod_mysql_bucket_brigade::iterator::valid()
bool mod_mysql_bucket_brigade::iterator::valid()
{
    return cur && cur != sentinel;
}
// }}}

// {{{ mod_mysql_bucket_brigade::iterator::next()
void mod_mysql_bucket_brigade::iterator::next()
{
    if (nxt) {
        cur = nxt;
        nxt = 0;
        removed = false;
    } else if (cur && cur != sentinel) {
        cur = cur->get_next();
    }
}
// }}}

// {{{ mod_mysql_bucket_brigade::iterator::remove()
void mod_mysql_bucket_brigade::iterator::remove()
{
    if (!nxt)
        nxt = cur->get_next();
    cur->unlink();
    removed = true;
}
// }}}

// vim: sts=4 sw=4 ts=4 et fdm=marker
