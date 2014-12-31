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
#ifndef _MOD_MYSQL_DUALITY_H
#define _MOD_MYSQL_DUALITY_H

template <typename _Tfirst, typename _Tlast> class mod_mysql_duality
{
public:
    virtual _Tfirst& get_first() = 0;
    virtual _Tlast& get_last() = 0;
};

#endif /* _MOD_MYSQL_DUALITY_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
