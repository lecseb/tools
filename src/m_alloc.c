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
#include <stdlib.h>
#include "m_alloc.h"
#include "m_utils.h"

void *_calloc(uint32_t size, uint32_t nbr)
{
	void *alloc = malloc(size * nbr);

	if (!alloc)
		assert(0);

	memset(alloc, 0, size * nbr);
	return alloc;
}

void *_malloc(uint32_t size)
{
	void *alloc = malloc(size);

	if (!alloc)
		assert(0);

	memset(alloc, 0, size);
	return alloc;
}

void _free(void *ptr)
{
	m_return_if_fail(ptr);

	if (ptr)
		free(ptr);
}

void *_realloc(void *ptr, uint32_t size)
{
	ptr = realloc(ptr, size);
	if (!ptr)
		assert(0);
	return ptr;
}
