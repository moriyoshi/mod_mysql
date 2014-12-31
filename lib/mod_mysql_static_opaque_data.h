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
#ifndef _MOD_MYSQL_STATIC_OPAQUE_DATA_H
#define _MOD_MYSQL_STATIC_OPAQUE_DATA_H

#include "mod_mysql_referent.h"
#include "mod_mysql_ranged_ref.h"
#include "mod_mysql_opaque_data.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MOD_MYSQL_STATIC_OPAQUE_DATA_DEFINED
typedef struct mod_mysql_static_opaque_data_t mod_mysql_static_opaque_data_t;
#define MOD_MYSQL_STATIC_OPAQUE_DATA_DEFINED
#endif /* MOD_MYSQL_STATIC_OPAQUE_DATA_DEFINED */

struct mod_mysql_static_opaque_data_t
{
    mod_mysql_opaque_data_t _super;
    mod_mysql_ranged_ref_t ref;
    mod_mysql_refcount_t refcount;
};

#define mod_mysql_static_opaque_data_init(self, ptr, sz) do { \
    MOD_MYSQL_OPAQUE_DATA_VTBL(self)->get_size_fn = \
            (apr_status_t(*)(mod_mysql_opaque_data_t*, size_t*)) \
            mod_mysql_static_opaque_data_get_size; \
    MOD_MYSQL_OPAQUE_DATA_VTBL(self)->map_fn = \
            (apr_status_t(*)(mod_mysql_opaque_data_t*, void**, apr_pool_t*)) \
            mod_mysql_static_opaque_data_map; \
    MOD_MYSQL_OPAQUE_DATA_VTBL(self)->unmap_fn = \
            (apr_status_t(*)(mod_mysql_opaque_data_t*, void*)) \
            mod_mysql_static_opaque_data_unmap; \
    ((mod_mysql_static_opaque_data_t *)(self))->ref.iov_base = ptr; \
    ((mod_mysql_static_opaque_data_t *)(self))->ref.iov_len = sz; \
} while (0)

apr_status_t
mod_mysql_static_opaque_data_get_size(mod_mysql_static_opaque_data_t *self,
        size_t *retval);
apr_status_t
mod_mysql_static_opaque_data_map(mod_mysql_static_opaque_data_t *self,
        void **retval, apr_pool_t* pool);
apr_status_t
mod_mysql_static_opaque_data_unmap(mod_mysql_static_opaque_data_t *self,
        void *blk);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _MOD_MYSQL_STATIC_OPAQUE_DATA_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
