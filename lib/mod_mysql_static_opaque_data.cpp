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
#include "mod_mysql_static_opaque_data.h"
#ifdef DEBUG
#include <stdio.h>.
#endif

extern "C" {

// {{{ mod_mysql_static_opaque_data_finalize()
void
mod_mysql_static_opaque_data_finalize(mod_mysql_static_opaque_data_t *self)
{
#ifdef DEBUG
    if (self->refcount > 0) {
        fprintf(stderr, "Something went wrong: 0 < refcount for mod_mysql_static_opaque_data_t*) %p", self);
    }
#endif
}
// }}}

// {{{ mod_mysql_static_opaque_data_get_size()
apr_status_t
mod_mysql_static_opaque_data_get_size(mod_mysql_static_opaque_data_t *self,
        size_t *retval)
{
    *retval = self->ref.iov_len;
    return APR_SUCCESS;
}
// }}}

// {{{ mod_mysql_static_opaque_data_map()
apr_status_t
mod_mysql_static_opaque_data_map(mod_mysql_static_opaque_data_t *self,
        void **retval, apr_pool_t* pool)
{
    *retval = self->ref.iov_base;
    self->refcount++;
    return APR_SUCCESS;
}
// }}}

// {{{ mod_mysql_static_opaque_data_unmap()
apr_status_t
mod_mysql_static_opaque_data_unmap(mod_mysql_static_opaque_data_t *self,
        void *blk)
{
    self->refcount--;
    return APR_SUCCESS;
}
// }}}

} // extern "C"

// vim: sts=4 sw=4 ts=4 et fdm=marker
