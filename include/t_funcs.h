/**
 * This file is part of libtools
 *
 * libtools is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * libtools is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with libtools.  If not, see <http:www.gnu.org/licenses/>.
 */
#ifndef _TOOLS_INCLUDE_T_FUNCS_H_
# define _TOOLS_INCLUDE_T_FUNCS_H_

/**
 * @brief Specifies the type of function which is called when a data element is
 * destroyed. It is passed the pointer to the data element and should free any
 * memory and resources allocated for it.
 * @param data[in] : the data element
 */
typedef int (*t_destroy_func)(void *data);

/**
 * @brief A function of this signature is used to copy the node data when doing
 * a deep-copy of a data.
 * @param data[in] : a pointer to the data which should be copied
 * @param user_data[in] : additional data
 * @return A pointer to the copy
 */
typedef void *(*t_copy_func)(void *data);

/**
 * @brief Specifies the type of functions passed threw *_foreach() function
 * @param data[in] : the element's data
 * @param user_data[in] : additional data
 */
typedef int (*t_foreach_func)(void *data, void *user_data);

/**
 * @brief Compare two user value
 * @param src[in] : source
 * @param dst[in] : destination
 * @return < 0 if src is less than dst, = 0 if equal, > 0 if greater
 */
typedef int (*t_compare_func)(void *src, void *dst);

#endif /* !_TOOLS_INCLUDE_T_FUNCS_H_ */
