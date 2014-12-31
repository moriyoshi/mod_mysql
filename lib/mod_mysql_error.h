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
#ifndef _MOD_MYSQL_ERROR_H
#define _MOD_MYSQL_ERROR_H

#include <apr_errno.h>

class mod_mysql_error
{
private:
	const char* msg;
    const char *file;
    int line;
    apr_status_t status;

public:
	mod_mysql_error(apr_status_t status, const char *file,
            int line, const char* msg = 0);

	const char* get_message() const;
    int get_line_number() const;
    const char* get_file_name() const;
    apr_status_t get_status() const;
};

// {{{ mod_mysql_error::mod_mysql_error
inline mod_mysql_error::mod_mysql_error(apr_status_t _status,
        const char* _file, int _line, const char* _msg)
		: status(_status), file(_file), line(_line), msg(_msg)
{
}
// }}}

// {{{ mod_mysql_error::get_message
inline const char* mod_mysql_error::get_message() const
{
	return msg;
}
// }}}

// {{{ mod_mysql_error::get_line_number
inline int mod_mysql_error::get_line_number() const
{
	return line;
}
// }}}

// {{{ mod_mysql_error::get_file_name
inline const char* mod_mysql_error::get_file_name() const
{
	return file;
}
// }}}

// {{{ mod_mysql_error::get_status
inline apr_status_t mod_mysql_error::get_status() const
{
	return status;
}
// }}}

#endif /* _MOD_MYSQL_ERROR_H */
/*
 * vim: sts=4 sw=4 ts=4 et fdm=marker
 */
