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
#ifndef _UTILS_M_UTILS_H_
# define _UTILS_M_UTILS_H_

# include "m_print.h"

# ifndef __packed
#  define __packed __attribute__((__packed__));
# endif /* !__packed */

/**
 * @brief Convenient macro to return the max between two value
 * @param a[in] : first value to compare
 * @param b[in] : second value to compare
 * @return the max value between a and b
 */
# define m_max(a, b) \
	({ __typeof__(a) _a = (a); \
		__typeof__(b) _b = (b); \
		_a > _b ? _a : _b; })

/**
 * @brief Convenient macro to return the min between two value
 * @param a[in] : first value to compare
 * @param b[in] : second value to compare
 * @return the min value between a and b
 */
# define m_min(a, b) \
	({ __typeof__(a) _a = (a); \
		__typeof__(b) _b = (b); \
		_a < _b ? _a : _b; })

/**
 * @brief use to stop the software in emergency
 * @param cond[in] : condition to respect
 * @note assert is only activated in debug mode.
 */
# define m_assert(cond) { \
	do { \
		if (!(cond)) { \
			m_error_print("%s:%d condition failed '%s'\n", \
				__func__, __LINE__, #cond); \
			assert(0); \
		} \
	} while (0); \
}

/**
 * @brief test and return if condition failed
 * @param cond[in] : condition to respect
 * @note only used for entering function
 */
# define m_return_if_fail(cond) { \
	do { \
		if (!(cond)) { \
			m_error_print("%s:%d condition failed '%s'\n", \
				__func__, __LINE__, #cond); \
			return; \
		} \
	} while (0); \
}

/**
 * @brief test and return val if condition failed
 * @param cond[in] : condition to respect
 * @param val[in] : value to return
 * @note only used for entering function
 */
# define m_return_val_if_fail(cond, val) { \
	do { \
		if (!(cond)) { \
			m_error_print("%s:%d: condition failed '%s'\n", \
				__func__, __LINE__, #cond); \
			return val; \
		} \
	} while (0); \
}

# define m_goto_if_fail(cond, label) { \
	do { \
		if (!(cond)) { \
			m_error_print("%s:%d: condition failed '%s'\n", \
					__func__, __LINE__, #cond); \
			goto label; \
		} \
	} while (0); \
}

/**
 * @brief Convenient macro to convert int to pointer
 */
# define m_int_to_ptr(val) (void *)((long)val)

/**
 * @brief Convenient macro to convert pointer to int
 */
# define m_ptr_to_int(val) (int)((long)val)

#endif /* !_UTILS_M_UTILS_H_ */
