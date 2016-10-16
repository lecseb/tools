/**
 * This file is part of libtools
 *
 * Foobar is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with libtools.  If not, see <http:www.gnu.org/licenses/>.
 */
#ifndef _TOOLS_INCLUDE_M_PRINT_H_
# define _TOOLS_INCLUDE_M_PRINT_H_

# include <errno.h>
# include <stdio.h>
# include <string.h>

/**
 * @brief print debug on screen
 * @param format[in] : format of the string to print
 * @param ...[in] : va args
 * @return the number of character printed on success, < 0 on error
 */
# ifdef DEBUG
#  define m_debug_fprint(stream, format, ...) { \
	fprintf(stream, format, ## __VA_ARGS__); \
   }
# else
#  define m_debug_fprint(stream, format, ...) { \
   }
# endif /* !DEBUG */

/**
 * @brief print warning on screen
 * @param format[in] : format of the string to print
 * @param ...[in] : va args
 * @return the number of character printed on success, < 0 on error
 */
# define m_warning_fprint(stream, format, ...) \
	fprintf(stream, format, ## __VA_ARGS__)

/**
 * @brief print error on screen
 * @param format[in] : format of the string to print
 * @param ...[in] : va args
 * @return the number of character printed on success, < 0 on error
 */
# define m_error_fprint(stream, format, ...) \
	fprintf(stream, format, ## __VA_ARGS__)

/**
 * @brief print debug on screen
 * @param format[in] : format of the string to print
 * @param ...[in] : va args
 * @return the number of character printed on success, < 0 on error
 */
# define m_debug_print(format, ...) \
	m_debug_fprint(stdout, format, ## __VA_ARGS__)

/**
 * @brief print warning on screen
 * @param format[in] : format of the string to print
 * @param ...[in] : va args
 * @return the number of character printed on success, < 0 on error
 */
# define m_warning_print(format, ...) \
	m_warning_fprint(stdout, "\033[33m"format"\033[00m", ## __VA_ARGS__)

/**
 * @brief print error on screen
 * @param format[in] : format of the string to print
 * @param ...[in] : va args
 * @return the number of character printed on success, < 0 on error
 */
# define m_error_print(format, ...) \
	m_error_fprint(stderr, "\033[31m"format"\033[00m", ## __VA_ARGS__);

/**
 * @brief print errno string representation
 * @param format[in] : format of the string to print
 * @param val[in] : errno value
 * @return the number of character printed on success, < 0 on error
 */
# define m_errno_print(val) \
	m_error_print("%s:%d %s\n", __func__, __LINE__, strerror(val));

#endif /* !_TOOLS_INCLUDE_M_PRINT_H_ */
