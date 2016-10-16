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
#ifndef _UTILS_M_ALLOC_H_
# define _UTILS_M_ALLOC_H_

# include <assert.h>
# include <stdint.h>

/**
 * @brief use to protect user against allocator's error
 * @param size[in] : nbr of byte (sizeof() result)
 * @param nbr[in] : nbr of size byte
 * @return a valid pointer or assert
 */
void *_calloc(uint32_t size, uint32_t nbr);

/**
 * @brief use to protect user against allocator's error
 * @param size[in] : nbr of byte (sizeof() result)
 * @return a valid pointer or assert
 */
void *_malloc(uint32_t size);

/**
 * @brief use to protect user against allocator's error
 * @param ptr[in] : old pointer value
 * @param size[in] : nbr of byte (sizeof() result)
 * @return a valid pointer or assert
 */
void *_realloc(void *ptr, uint32_t size);

/**
 * @brief use to protect user against allocator's error
 * @param ptr[in] : old pointer value
 */
void _free(void *ptr);

#endif /* !_UTILS_M_ALLOC_H_ */
