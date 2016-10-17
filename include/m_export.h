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
#ifndef _TOOLS_INCLUDE_M_EXPORT_H_
# define _TOOLS_INCLUDE_M_EXPORT_H_

/**
 * @brief Only set to visible all function from the API.
 */
# ifdef __GNUC__
#  define export __attribute__((visibility("default")))
# else
#  define export
# endif

#endif /* !_TOOLS_INCLUDE_M_EXPORT_H_ */
