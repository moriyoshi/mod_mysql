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
#ifndef _MOD_MYSQL_DELEGATES_H
#define _MOD_MYSQL_DELEGATES_H

#include <stdlib.h>

#define MOD_MYSQL_DG_SLOT_SIZE 16384

// {{{ template<> union mod_mysql_arg_0_dg
template<class _T, typename _Trt>
union mod_mysql_arg_0_dg
{
public:
	typedef _Trt (_T::*func_t)();
	typedef _Trt (_T::*const_func_t)() const;

private:
	func_t nc;
	const_func_t c;

private:
    void* operator new(size_t sz, void* slot);

public:
    mod_mysql_arg_0_dg();

	mod_mysql_arg_0_dg(func_t _func);

	mod_mysql_arg_0_dg(const_func_t _func);

	operator mod_mysql_arg_0_dg*();

    _Trt operator()(const _T& instance);

	_Trt operator()(_T& instance);
};

// {{{ mod_mysql_arg_0_dg<>::mod_mysql_arg_0_dg
template<class _T, typename _Trt>
inline mod_mysql_arg_0_dg<_T, _Trt>::mod_mysql_arg_0_dg()
{
}

template<class _T, typename _Trt>
inline mod_mysql_arg_0_dg<_T, _Trt>::mod_mysql_arg_0_dg(func_t _func): nc(_func)
{
}

template<class _T, typename _Trt>
inline mod_mysql_arg_0_dg<_T, _Trt>::mod_mysql_arg_0_dg(const_func_t _func)
        : c(_func)
{
}
// }}}

// {{{ mod_mysql_arg_0_dg<>::operator new()
template<class _T, typename _Trt>
inline void* mod_mysql_arg_0_dg<_T, _Trt>::operator new(size_t sz, void* slot)
{
    return slot;
}
// }}}

// {{{ mod_mysql_arg_0_dg<>::operator mod_mysql_arg_0_dg*()
template<class _T, typename _Trt>
inline mod_mysql_arg_0_dg<_T, _Trt>::operator mod_mysql_arg_0_dg<_T, _Trt>*()
{
	static mod_mysql_arg_0_dg slots[MOD_MYSQL_DG_SLOT_SIZE];
    static int slot_idx = 0;

    if (slot_idx >= sizeof(slots) / sizeof(slots[0])) {
        abort();
    }

	return new(&slots[slot_idx++]) mod_mysql_arg_0_dg(*this);
}
// }}}

// {{{ mod_mysql_arg_0_dg<>::operator()
template<class _T, typename _Trt>
inline _Trt mod_mysql_arg_0_dg<_T, _Trt>::operator()(const _T& instance)
{
	return (instance.*c)();
}

template<class _T, typename _Trt>
inline _Trt mod_mysql_arg_0_dg<_T, _Trt>::operator()(_T& instance)
{
	return (instance.*nc)();
}
// }}}
// }}}

// {{{ template<> union mod_mysql_arg_1_dg
template<class _T, typename _Trt, typename _Targ>
union mod_mysql_arg_1_dg
{
public:
	typedef _Trt (_T::*func_t)(_Targ arg);
	typedef _Trt (_T::*const_func_t)(_Targ arg) const;

private:
	func_t nc;
	const_func_t c;

private:
    void* operator new(size_t sz, void* slot);

public:
	mod_mysql_arg_1_dg();

	mod_mysql_arg_1_dg(func_t _func);

	mod_mysql_arg_1_dg(const_func_t _func);

	operator mod_mysql_arg_1_dg*();

    _Trt operator()(const _T& instance, _Targ arg);

	_Trt operator()(_T& instance, _Targ arg);
};

// {{{ mod_mysql_arg_1_dg<>::mod_mysql_arg_1_dg
template<class _T, typename _Trt, typename _Targ>
inline mod_mysql_arg_1_dg<_T, _Trt, _Targ>::mod_mysql_arg_1_dg()
{
}

template<class _T, typename _Trt, typename _Targ>
inline mod_mysql_arg_1_dg<_T, _Trt, _Targ>::mod_mysql_arg_1_dg(
		func_t _func)
		: nc(_func)
{
}

template<class _T, typename _Trt, typename _Targ>
inline mod_mysql_arg_1_dg<_T, _Trt, _Targ>::mod_mysql_arg_1_dg(
		const_func_t _func)
		: c(_func)
{
}
// }}}

// {{{ mod_mysql_arg_1_dg<>::operator new()
template<class _T, typename _Trt, typename _Targ>
inline void* mod_mysql_arg_1_dg<_T, _Trt, _Targ>::operator new(size_t sz,
        void* slot)
{
    return slot;
}
// }}}

// {{{ mod_mysql_arg_1_dg<>::operator mod_mysql_arg_1_dg*()
template<class _T, typename _Trt, typename _Targ>
inline mod_mysql_arg_1_dg<_T, _Trt, _Targ>::operator mod_mysql_arg_1_dg<_T, _Trt, _Targ>*()
{
	static mod_mysql_arg_1_dg slots[MOD_MYSQL_DG_SLOT_SIZE];
    static int slot_idx = 0;

    if (slot_idx >= sizeof(slots) / sizeof(slots[0])) {
        abort();
    }

	return new(&slots[slot_idx++]) mod_mysql_arg_1_dg(*this);
}
// }}}

// {{{ mod_mysql_arg_1_dg<>::operator()
template<class _T, typename _Trt, typename _Targ>
inline _Trt mod_mysql_arg_1_dg<_T, _Trt, _Targ>::operator()(
		const _T& instance, _Targ arg)
{
	return (instance.*c)(arg);
}

template<class _T, typename _Trt, typename _Targ>
inline _Trt mod_mysql_arg_1_dg<_T, _Trt, _Targ>::operator()(
		_T& instance, _Targ arg)
{
	return (instance.*nc)(arg);
}
// }}}
// }}}

// {{{ template<> union mod_mysql_arg_2_dg
template<class _T, typename _Trt, typename _Targ1, typename _Targ2>
union mod_mysql_arg_2_dg
{
public:
	typedef _Trt (_T::*func_t)(_Targ1, _Targ2);
	typedef _Trt (_T::*const_func_t)(_Targ1, _Targ2) const;

private:
	func_t nc;
	const_func_t c;

private:
    void* operator new(size_t sz, void* slot);

public:
	mod_mysql_arg_2_dg();

	mod_mysql_arg_2_dg(func_t _func);

	mod_mysql_arg_2_dg(const_func_t _func);

	operator mod_mysql_arg_2_dg*();

    _Trt operator()(const _T& instance, _Targ1 arg1, _Targ2 arg2);

	_Trt operator()(_T& instance, _Targ1 arg1, _Targ2 arg2);
};

// {{{ mod_mysql_arg_2_dg<>::mod_mysql_arg_2_dg
template<class _T, typename _Trt, typename _Targ1, typename _Targ2>
inline mod_mysql_arg_2_dg<_T, _Trt, _Targ1, _Targ2>::mod_mysql_arg_2_dg()
{
}

template<class _T, typename _Trt, typename _Targ1, typename _Targ2>
inline mod_mysql_arg_2_dg<_T, _Trt, _Targ1, _Targ2>::mod_mysql_arg_2_dg(
		func_t _func)
		: nc(_func)
{
}

template<class _T, typename _Trt, typename _Targ1, typename _Targ2>
inline mod_mysql_arg_2_dg<_T, _Trt, _Targ1, _Targ2>::mod_mysql_arg_2_dg(
		const_func_t _func)
		: c(_func)
{
}
// }}}

// {{{ mod_mysql_arg_2_dg<>::operator new()
template<class _T, typename _Trt, typename _Targ1, typename _Targ2>
inline void* mod_mysql_arg_2_dg<_T, _Trt, _Targ1, _Targ2>::operator new(
        size_t sz, void* slot)
{
    return slot;
}
// }}}

// {{{ mod_mysql_arg_2_dg<>::operator mod_mysql_arg_2_dg*()
template<class _T, typename _Trt, typename _Targ1, typename _Targ2>
inline mod_mysql_arg_2_dg<_T, _Trt, _Targ1, _Targ2>::operator mod_mysql_arg_2_dg<_T, _Trt, _Targ1, _Targ2>*()
{
	static mod_mysql_arg_2_dg slots[MOD_MYSQL_DG_SLOT_SIZE];
    static int slot_idx = 0;

    if (slot_idx >= sizeof(slots) / sizeof(slots[0])) {
        abort();
    }

	return new(&slots[slot_idx++]) mod_mysql_arg_2_dg(*this);
}
// }}}

// {{{ mod_mysql_arg_2_dg<>::operator()
template<class _T, typename _Trt, typename _Targ1, typename _Targ2>
inline _Trt mod_mysql_arg_2_dg<_T, _Trt, _Targ1, _Targ2>::operator()(
		const _T& instance, _Targ1 arg1, _Targ2 arg2)
{
	return (instance.*c)(arg1, arg2);
}

template<class _T, typename _Trt, typename _Targ1, typename _Targ2>
inline _Trt mod_mysql_arg_2_dg<_T, _Trt, _Targ1, _Targ2>::operator()(
		_T& instance, _Targ1 arg1, _Targ2 arg2)
{
	return (instance.*nc)(arg1, arg2);
}
// }}}
// }}}

// {{{ template<> union mod_mysql_arg_3_dg
template<class _T, typename _Trt, typename _Targ1, typename _Targ2,
        typename _Targ3>
union mod_mysql_arg_3_dg
{
public:
	typedef _Trt (_T::*func_t)(_Targ1, _Targ2, _Targ3);
	typedef _Trt (_T::*const_func_t)(_Targ1, _Targ2, _Targ3) const;

private:
	func_t nc;
	const_func_t c;

private:
    void* operator new(size_t sz, void* slot);

public:
	mod_mysql_arg_3_dg();

	mod_mysql_arg_3_dg(func_t _func);

	mod_mysql_arg_3_dg(const_func_t _func);

	operator mod_mysql_arg_3_dg*();

    _Trt operator()(const _T& instance, _Targ1 arg1, _Targ2 arg2, _Targ3 arg3);

	_Trt operator()(_T& instance, _Targ1 arg1, _Targ2 arg2, _Targ3 arg3);
};

// {{{ mod_mysql_arg_3_dg<>::mod_mysql_arg_3_dg
template<class _T, typename _Trt, typename _Targ1, typename _Targ2,
        typename _Targ3>
inline mod_mysql_arg_3_dg<_T, _Trt, _Targ1, _Targ2, _Targ3>::mod_mysql_arg_3_dg()
{
}

template<class _T, typename _Trt, typename _Targ1, typename _Targ2,
        typename _Targ3>
inline mod_mysql_arg_3_dg<_T, _Trt, _Targ1, _Targ2, _Targ3>::mod_mysql_arg_3_dg(
		func_t _func)
		: nc(_func)
{
}

template<class _T, typename _Trt, typename _Targ1, typename _Targ2,
        typename _Targ3>
inline mod_mysql_arg_3_dg<_T, _Trt, _Targ1, _Targ2, _Targ3>::mod_mysql_arg_3_dg(
		const_func_t _func)
		: c(_func)
{
}
// }}}

// {{{ mod_mysql_arg_3_dg<>::operator new()
template<class _T, typename _Trt, typename _Targ1, typename _Targ2, typename _Targ3>
inline void* mod_mysql_arg_3_dg<_T, _Trt, _Targ1, _Targ2, _Targ3>::operator new(
        size_t sz, void* slot)
{
    return slot;
}
// }}}

// {{{ mod_mysql_arg_3_dg<>::operator mod_mysql_arg_3_dg*()
template<class _T, typename _Trt, typename _Targ1, typename _Targ2,
        typename _Targ3>
inline mod_mysql_arg_3_dg<_T, _Trt, _Targ1, _Targ2, _Targ3>::operator mod_mysql_arg_3_dg<_T, _Trt, _Targ1, _Targ2, _Targ3>*()
{
	static mod_mysql_arg_3_dg slots[MOD_MYSQL_DG_SLOT_SIZE];
    static int slot_idx = 0;

    if (slot_idx >= sizeof(slots) / sizeof(slots[0])) {
        abort();
    }

	return new(&slots[slot_idx++]) mod_mysql_arg_3_dg(*this);
}
// }}}

// {{{ mod_mysql_arg_3_dg<>::operator()
template<class _T, typename _Trt, typename _Targ1, typename _Targ2,
        typename _Targ3>
inline _Trt mod_mysql_arg_3_dg<_T, _Trt, _Targ1, _Targ2, _Targ3>::operator()(
		const _T& instance, _Targ1 arg1, _Targ2 arg2, _Targ3 arg3)
{
	return (instance.*c)(arg1, arg2, arg3);
}

template<class _T, typename _Trt, typename _Targ1, typename _Targ2,
        typename _Targ3>
inline _Trt mod_mysql_arg_3_dg<_T, _Trt, _Targ1, _Targ2, _Targ3>::operator()(
		_T& instance, _Targ1 arg1, _Targ2 arg2, _Targ3 arg3)
{
	return (instance.*nc)(arg1, arg2, arg3);
}
// }}}
// }}}

#endif /* _MOD_MYSQL_DELEGATES_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
