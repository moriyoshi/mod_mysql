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
#ifndef _MOD_MYSQL_REFERENT_H
#define _MOD_MYSQL_REFERENT_H

#include <apr_atomic.h>

#ifdef HAVE_ASSERT_H
#include <assert.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MOD_MYSQL_REFERENT_T_DEFINED
typedef struct mod_mysql_referent_t mod_mysql_referent_t;
#define MOD_MYSQL_REFERENT_T_DEFINED
#endif /* MOD_MYSQL_REFERENT_T_DEFINED */

typedef struct
{
    void (*finalize_fn)(mod_mysql_referent_t *);
} mod_mysql_referent_vtbl_t;

typedef apr_uint32_t mod_mysql_refcount_t;

struct mod_mysql_referent_t
{
    mod_mysql_referent_vtbl_t *_vtbl;
    mod_mysql_refcount_t refcount;
};

#ifdef HAVE_ASSERT_H
#define _mod_mysql_referent_add_ref(self) do { \
    assert(apr_atomic_inc32( \
            &((mod_mysql_referent_t *)(self))->refcount) + 1); \
} while (0)
#else
#define _mod_mysql_referent_add_ref(self) do { \
    apr_atomic_inc32( \
            &((mod_mysql_referent_t *)(self))->refcount); \
} while (0)
#endif

#define _mod_mysql_referent_dispose(self) do { \
    if (!apr_atomic_dec32(&((mod_mysql_referent_t *)(self))->refcount)) \
        (((mod_mysql_referent_t *)(self))->_vtbl->finalize_fn)( \
                (mod_mysql_referent_t *)(self)); \
} while (0)

#define mod_mysql_referent_add_ref(self) _mod_mysql_referent_add_ref(self)
#define mod_mysql_referent_dispose(self) _mod_mysql_referent_dispose(self)

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _MOD_MYSQL_REFERENT_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
