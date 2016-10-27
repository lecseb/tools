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
#ifndef _TOOLS_INCLUDE_TREE_S_BS_TREE_H_
# define _TOOLS_INCLUDE_TREE_S_BS_TREE_H_

# include <stdint.h>
# include "e_tree.h"
# include "m_export.h"
# include "t_funcs.h"

/**
 * @brief Binary search tree structure (opaque)
 */
export struct s_bs_tree;

/**
 * @brief Deallocate a tree instance
 * @param tree[in] : instance to delete
 */
export void s_bs_tree_delete(struct s_bs_tree *tree);

/**
 * @brief Deallocate a tree instance
 * @param tree[in] : instance to delete
 * @param destroy[in] : destroy callback
 */
export void s_bs_tree_delete_full(struct s_bs_tree *tree,
	t_destroy_func destroy);

/**
 * @brief Add an element into a tree by following the binary search tree rule
 * @param tree[in] : tree to modify
 * @param compare[in] : function to compare element
 * @param destroy[in] : function to destroy user data
 * @param data[in] : data to push into the tree
 * @return the (possibly changed) root of the tree
 */
export struct s_bs_tree *s_bs_tree_add(struct s_bs_tree *tree,
	t_compare_func compare, void *data);

/**
 * @brief Remove an element into a tree by following the binary search tree rule
 * @param tree[in] : tree to modify
 * @param compare[in] : compare element function
 * @param destroy[in] : destroy element function
 * @param data[in] : data to push into the tree
 * @return the (possibly changed) root of the tree
 */
export struct s_bs_tree *s_bs_tree_remove(struct s_bs_tree *tree,
	t_compare_func compare, t_destroy_func destroy, void *data);

/**
 * @brief Get the nth smaller element from the tree
 * @param tree[in] : tree to browse
 * @param nth[in] : the nth smaller element
 * @return a valid pointer on success, NULL on error
 */
export int s_bs_tree_exist(struct s_bs_tree *tree, t_compare_func compare,
	void *data);

/**
 * @brief Get the nth smaller element from the tree
 * @param tree[in] : tree to browse
 * @param nth[in] : the nth smaller element
 * @return a valid pointer on success, NULL on error
 */
export void *s_bs_tree_nth_smallest(struct s_bs_tree *tree, uint32_t nth);

/**
 * @brief Get the nth smaller element from the tree
 * @param tree[in] : tree to browse
 * @param nth[in] : the nth smaller element
 * @return a valid pointer on success, NULL on error
 */
export void *s_bs_tree_nth_biggest(struct s_bs_tree *tree, uint32_t nth);

/**
 * @brief Browse the entire tree according to the type of search asked
 * @param tree[in] : instance to browse
 * @param type[in] : type of the search
 * @param foreach[in] : user callback for each node
 * @param data[in] : user data pass through the callback
 * @return 0 on success, errno on error
 */
export int s_bs_tree_foreach(struct s_bs_tree *tree, enum e_tree_browse type,
	t_foreach_func foreach, void *data);

#endif /* !_TOOLS_INCLUDE_TREE_S_BS_TREE_H_ */
