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
#ifndef _TOOLS_INCLUDE_TREE_E_TREE_H_
# define _TOOLS_INCLUDE_TREE_E_TREE_H_

# include "m_export.h"

/**
 * @brief Browse type definition order
 * @param e_tree_traversal_pre: traversal pre-order
 * @param e_tree_traversal_post: traversal post-order
 * @param e_tree_traversal_in: traversal in-order
 * @param e_tree_depth_first: depth-first order
 * @param e_tree_breath_first: breath-first order
 */
export enum e_tree_browse {
	e_tree_depth_pre,
	e_tree_depth_post,
	e_tree_depth_in,
	e_tree_breath
};

#endif /* !_TOOLS_INCLUDE_TREE_E_TREE_H_ */
