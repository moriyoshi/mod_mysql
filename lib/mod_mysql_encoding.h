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
#ifndef _MOD_MYSQL_ENCODING_H
#define _MOD_MYSQL_ENCODING_H

#include <apr_pools.h>

#ifndef MOD_MYSQL_ENCODER_T_DEFINED
typedef struct _mod_mysql_encoder_t mod_mysql_encoder_t;
#define MOD_MYSQL_ENCODER_T_DEFINED
#endif /* MOD_MYSQL_ENCODER_T_DEFINED */

#ifndef MOD_MYSQL_DECODER_T_DEFINED
typedef struct _mod_mysql_decoder_t mod_mysql_decoder_t;
#define MOD_MYSQL_DECODER_T_DEFINED
#endif /* MOD_MYSQL_DECODER_T_DEFINED */

typedef struct _mod_mysql_encoding_t {
    const char* name;
    mod_mysql_encoder_t* (*create_encoder_fn)(apr_pool_t*);
    mod_mysql_decoder_t* (*create_decoder_fn)(apr_pool_t*);
} mod_mysql_encoding_t;

#ifdef __cplusplus
class mod_mysql_pool;
class mod_mysql_encoder;
class mod_mysql_decoder;

class mod_mysql_encoding
        : public mod_mysql_encoding_t,
          public mod_mysql_base
{
public:
    const char* get_name();

    mod_mysql_encoder* create_encoder(mod_mysql_pool* pool);
    mod_mysql_decoder* create_decoder(mod_mysql_pool* pool);
};
#endif

#endif /* _MOD_MYSQL_ENCODING_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
