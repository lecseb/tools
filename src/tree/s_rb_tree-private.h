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
#ifndef _TOOLS_S_RB_TREE_PRIVATE_H_
# define _TOOLS_S_RB_TREE_PRIVATE_H_

# include "tree/s_rb_tree.h"

/**
 * @brief Color for a specific node
 */
enum _e_color {
	_e_red,
	_e_black,
	_e_d_black
};

/**
 * @brief The binary search tree structure
 * @param data: user data stored
 * @param color: the color associate to the node
 * @param parent: parent node
 * @param left: left child
 * @param right: right child
 */
struct s_rb_tree {
	void *data;
	enum _e_color color;
	struct s_rb_tree *parent;
	struct s_rb_tree *left;
	struct s_rb_tree *right;
};

/**
 * @brief A convenience macro to get the data in an element.
 */
# define m_s_rb_tree_get_data(tree) ((tree) ? (tree)->data : NULL)

/**
 * @brief A convenience macro to get the data in an element.
 */
# define m_s_rb_tree_get_color(tree) ((tree) ? (tree)->color : _e_black)

/**
 * @brief A convenience macro to get the left element of a node.
 */
# define m_s_rb_tree_get_left(tree) ((tree) ? (tree)->left : NULL)

/**
 * @brief A convenience macro to get the right element of a node.
 */
# define m_s_rb_tree_get_right(tree) ((tree) ? (tree)->right : NULL)

/**
 * @brief A convenience macro to get the parent node of a node
 */
# define m_s_rb_tree_get_parent(tree) ((tree) ? (tree)->parent : NULL)

/**
 * @brief A convenience macro to get the grand parent node of a node
 */
# define m_s_rb_tree_get_grand_parent(tree) \
	m_s_rb_tree_get_parent(m_s_rb_tree_get_parent(tree))

/**
 * @brief A convenience macro to get sibling node of a node
 */
# define m_s_rb_tree_get_sibling(tree) \
	(m_s_rb_tree_get_left(m_s_rb_tree_get_parent(tree)) == tree ? \
		m_s_rb_tree_get_right(m_s_rb_tree_get_parent(tree)) : \
		m_s_rb_tree_get_left(m_s_rb_tree_get_parent(tree)))

/**
 * @brief A convenience macro to get the position of the node according to its
 * parent (ie get 1 is node is the left one)
 */
# define m_s_rb_tree_is_left(parent, node) \
	((parent) ? (parent)->left == node : 0)

/**
 * @brief A convenience macro to get the position of the node according to its
 * parent (ie get 1 is node is the right one)
 */
# define m_s_rb_tree_is_right(parent, node) \
	((parent) ? (parent)->right == node : 0)

# define m_s_rb_tree_is_black(node) \
	((node)->color == _e_black || (node)->color == _e_d_black)

/**
 * @brief A convenience macro to change the data of a specific node
 */
# define m_s_rb_tree_set_data(tree, value) { \
	do { \
		if (tree) { \
			(tree)->data = value; \
		} \
	} while (0); \
}

/**
 * @brief A convenience macro to change the color of a specific node
 */
# define m_s_rb_tree_set_color(tree, col) { \
	do { \
		if (tree) { \
			(tree)->color = col; \
		} \
	} while (0); \
}

/**
 * @brief A convenience macro to get the parent node of a node
 */
# define m_s_rb_tree_set_parent(tree, node) { \
	do { \
		if (tree) { \
			(tree)->parent = (node); \
		} \
	} while (0); \
}

/**
 * @brief A convenience macro to change the left of a specific node
 */
# define m_s_rb_tree_set_left(tree, node) { \
	do { \
		if (tree) { \
			(tree)->left = node; \
		} \
	} while (0); \
}

/**
 * @brief A convenience macro to change the right of a specific node
 */
# define m_s_rb_tree_set_right(tree, node) { \
	do { \
		if (tree) { \
			(tree)->right = node; \
		} \
	} while (0); \
}

/**
 * @brief A convenience macro to switch data from dst to src
 */
# define m_s_rb_tree_switch_data(dst, src, destroy) { \
	do { \
		if (destroy) { \
			destroy(m_s_rb_tree_get_data(dst)); \
		} \
		m_s_rb_tree_set_data(dst, m_s_rb_tree_get_data(src)); \
	} while (0); \
}

/**
 * @brief Get the uncle of a specific node
 * @param tree[in] : node to browse
 * @return a valid pointer on success, NULL on error or not found
 */
struct s_rb_tree *_s_rb_tree_get_uncle(struct s_rb_tree *tree);

/**
 * @brief Perform a right rotate operation on node
 * @param node[in] : node to rotate
 * @return return the new root
 */
void _s_rb_tree_right_rotate(struct s_rb_tree *a);

/**
 * @brief Perform a left rotate operation on node
 * @param node[in] : node to rotate
 * @return the new root of that rotation
 */
void _s_rb_tree_left_rotate(struct s_rb_tree *a);

#endif /* !_TOOLS_S_RB_TREE_PRIVATE_H_ */
