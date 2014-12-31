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
#ifndef _MOD_MYSQL_ACCESSOR_H
#define _MOD_MYSQL_ACCESSOR_H

template<typename _Tv> class mod_mysql_accessor
{
public:
    virtual void init() = 0;
    virtual _Tv const& get() = 0;
    virtual void set(_Tv const&) = 0;
    operator _Tv();
    _Tv const& operator =(_Tv const&);
};

// {{{ mod_mysql_accessor<>::operator _Tv()
template<typename _Tv>
inline mod_mysql_accessor<_Tv>::operator _Tv()
{
    return get();
}
// }}}

// {{{ mod_mysql_accessor<>::operator =()
template<typename _Tv>
inline _Tv const& mod_mysql_accessor<_Tv>::operator =(_Tv const& val)
{
    set(val);
    return get();
}
// }}}

#define DECLARE_ACCESSOR(name, type) \
    class accessor_ ## name: public mod_mysql_accessor<type> { \
        private: type value; \
        public: accessor_ ## name

#define ACCESSOR_PARAM(param_type) \
    (param_type& _param): param(_param) { init(); } \
    private: param_type& param; \
    public:

#define NO_ACCESSOR_PARAM() () { init(); }

#define END_DECLARE_ACCESSOR(name) } name;

#endif /* _MOD_MYSQL_ACCESSOR_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
