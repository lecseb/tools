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
#include "s_rb_tree.h"
#include "s_rb_tree-private.h"
#include "m_utils.h"

/**
 * 1) Perform standard BST delete. When we perform standard delete operation in
 *    BST, we always end up deleting a node which is either leaf or has only one
 *    child (For an internal node, we copy the successor and then recursively
 *    call delete for successor, successor is always a leaf node or a node with
 *    one child). So we only need to handle cases where a node is leaf or has
 *    one child. Let v be the node to be deleted and u be the child that
 *    replaces v (Note that u is NULL when v is a leaf and color of NULL is
 *    considered as Black).
 * 2) Simple Case: If either u or v is red, we mark the replaced child as black
 *    (No change in black height). Note that both u and v cannot be red as v is
 *    parent of u and two consecutive reds are not allowed in red-black tree.
 * 3) If Both u and v are Black.
 *   3.1) Color u as double black.  Now our task reduces to convert this double
 *       black to single black. Note that If v is leaf, then u is NULL and color
 *       of NULL is considered as black. So the deletion of a black leaf also
 *       causes a double black.
 *   3.2) Do following while the current node u is double black or it is not
 *        root. Let sibling of node be s.
 *        (a): If sibling s is black and at least one of sibling’s children is
 *             red, perform rotation(s). Let the red child of s be r. This case
 *             can be divided in four subcases depending upon positions of s and
 *             r.
 *             (i) Left Left Case (s is left child of its parent and r is left
 *                 child of s or both children of s are red). This is mirror of
 *                 right right case shown in below diagram.
 *             (ii) Left Right Case (s is left child of its parent and r is
 *                  right child). This is mirror of right left case shown in
 *                  below diagram.
 *             (iii) Right Right Case (s is right child of its parent and r is
 *                  right child of s or both children of s are red)
 *        (b): If sibling is black and its both children are black, perform
 *             recoloring, and recur for the parent if parent is black.
 *        (c): If sibling is red, perform a rotation to move old sibling up,
 *             recolor the old sibling and parent. The new sibling is always
 *             black (See the below diagram). This mainly converts the tree to
 *             black sibling case (by rotation) and  leads to case (a) or (b).
 *             This case can be divided in two subcases.
 *             (i) Left Case (s is left child of its parent). This is mirror of
 *                 right right case shown in below diagram. We right rotate the
 *                 parent p.
 *            (iii) Right Case (s is right child of its parent). We left rotate
 *                 the parent p.
 *   3.3) If u is root, make it single black and return (Black height of
 *        complete tree reduces by 1).
 */
/**
 * @brief Convenience macro to delete only one node without reccursion
 * @param node[in] : node to delete
 * @param operator[in] : user data deletion operator
 */
#define _s_bs_tree_delete_single(node, operator) { \
	(node)->left = (node)->right = NULL; \
	if (operator) \
			s_rb_tree_delete_full((node), (operator)); \
	else \
			s_rb_tree_delete(node); \
}

/**
 * @brief Get the minimum data value from the tree according to the given
 * tree
 * @param btree[in] : root tree to search
 * @return a valid tree pointer on succes, NULL on error
 */
static struct s_rb_tree *_s_bs_tree_find_min(struct s_rb_tree *tree)
{
	m_return_val_if_fail(tree, tree);

	struct s_rb_tree *curr = tree;

	while (_s_rb_tree_get_left(curr))
		curr = _s_rb_tree_get_left(curr);

	return curr;
}

/**
 * @brief Reduce the double black from the tree (case a)
 * @param s[in] : sibling
 * @param s_left[in] : left sibling child
 * @param s_right[in] : right sibling child
 * @param r[in] : red sibling from s
 */
#define _s_rb_tree_reduce_case_a(s, s_left, s_right, r) { \
	do { \
		if (_s_rb_tree_is_left(_s_rb_tree_get_parent(s), s) && \
			(_s_rb_tree_is_left(s, r) || \
			(_s_rb_tree_get_color(s_left) == _e_red && \
			_s_rb_tree_get_color(s_right) == _e_red))) { \
ll_case: \
			m_warning_print("> %s: left left case\n", __func__); \
			/* case i left left case */ \
			_s_rb_tree_right_rotate(s); \
		} else if (_s_rb_tree_is_left(_s_rb_tree_get_parent(s), s) && \
			_s_rb_tree_is_right(s, r)) { \
			m_warning_print("> %s: left right case\n", __func__); \
			/* case ii left right case */ \
			_s_rb_tree_left_rotate(s); \
			goto ll_case; \
		} else if (_s_rb_tree_is_right(_s_rb_tree_get_parent(s), s) && \
			(_s_rb_tree_is_right(s, r) || \
			(_s_rb_tree_get_color(s_left) == _e_red && \
			_s_rb_tree_get_color(s_right) == _e_red))) { \
rr_case: \
			m_warning_print("> %s: right right case\n", __func__); \
			/* case iii right right case */ \
			_s_rb_tree_left_rotate(_s_rb_tree_get_parent(s)); \
		} else if (_s_rb_tree_is_right(_s_rb_tree_get_parent(s), s) && \
			_s_rb_tree_is_left(s, r)) { \
			m_warning_print("> %s: right left case\n", __func__); \
			/* case iv right left case */ \
			_s_rb_tree_right_rotate(s); \
			goto rr_case; \
		} \
	} while (0); \
}

/**
 * @brief Reduce the double black from the tree (case b)
 * @param s[in] : sibling
 * @param s_left[in] : left sibling child
 * @param s_right[in] : right sibling child
 * @param r[in] : red sibling from s
 */
#define _s_rb_tree_reduce_case_b(s, s_left, s_right, r)

/**
 * @brief Reduce the double black from the tree (case c)
 * @param s[in] : sibling
 * @param s_left[in] : left sibling child
 * @param s_right[in] : right sibling child
 * @param r[in] : red sibling from s
 */
#define _s_rb_tree_reduce_case_c(s, s_left, s_right, r)

/**
 * @brief Reduce double black from the tree
 * @pram node[in] : node to remove and resolve double black entry point
 */
static void _s_rb_tree_reduce_d_black(struct s_rb_tree *node)
{
	m_return_if_fail(node);

	struct s_rb_tree *s = _s_rb_tree_get_sibling(node);
	struct s_rb_tree *s_left = _s_rb_tree_get_left(s);
	struct s_rb_tree *s_right = _s_rb_tree_get_right(s);
	struct s_rb_tree *r = (_s_rb_tree_get_color(s_left) == _e_red) ?
		s_left : (_s_rb_tree_get_color(s_right) == _e_red) ? s_right : NULL;

	m_warning_print("> %s: s(%d) = %d\n", __func__,
		m_ptr_to_int(_s_rb_tree_get_data(s)),
		_s_rb_tree_get_color(s));
	m_warning_print("> %s: r(%d) = %d\n", __func__,
		m_ptr_to_int(_s_rb_tree_get_data(r)),
		_s_rb_tree_get_color(r));

	if (_s_rb_tree_get_color(s) == _e_black && r) {
		_s_rb_tree_reduce_case_a(s, s_left, s_right, r);
	} else if (_s_rb_tree_get_color(s) == _e_black &&
		_s_rb_tree_get_color(_s_rb_tree_get_left(s)) == _e_black &&
		_s_rb_tree_get_color(_s_rb_tree_get_right(s))) {
		_s_rb_tree_reduce_case_b(s, s_left, s_right, r);
	} else if (_s_rb_tree_get_color(s) == _e_red) {
		_s_rb_tree_reduce_case_c(s, s_left, s_right, r);
	}
}

/**
 * @brief Perform a simple change color and call double black reduction
 * if needed
 * @param tree[in] : node to remove
 * @param left[in] : left node to the removed one
 * @param right[in] : right node to the removed one
 */
#define _s_rb_tree_change_color(tree, left, right) { \
	do { \
		if (_s_rb_tree_get_color(tree) == _e_red || \
			_s_rb_tree_get_color(right) == _e_red || \
			_s_rb_tree_get_color(left) == _e_red) { \
			_s_rb_tree_set_color((right) ? right : left, _e_black); \
		} else if (_s_rb_tree_get_color(tree) == _e_black && \
			_s_rb_tree_get_color(right) == _e_black && \
			_s_rb_tree_get_color(left) == _e_black) { \
			_s_rb_tree_set_color((right) ? right : left, _e_d_black); \
			_s_rb_tree_reduce_d_black((right) ? right : left); \
		} \
	} while (0); \
}

struct s_rb_tree *s_rb_tree_remove(struct s_rb_tree *tree,
	t_compare_func compare, t_destroy_func destroy, void *data)
{
	m_return_val_if_fail(tree, tree);
	m_return_val_if_fail(compare, tree);

	struct s_rb_tree *left = _s_rb_tree_get_left(tree);
	struct s_rb_tree *right = _s_rb_tree_get_right(tree);

	int ret = compare(_s_rb_tree_get_data(tree), data);
	if (ret == 0) {
		if ((!left && right) || (left && !right)) {
			_s_rb_tree_change_color(tree, left, right);
			_s_bs_tree_delete_single(tree, destroy);
			return (!right) ? left : right;
		} else if (!right && !left) {
			_s_rb_tree_set_color(tree, _e_d_black);
			_s_rb_tree_reduce_d_black(tree);
			return tree;
		} else {
			struct s_rb_tree *tmp = _s_bs_tree_find_min(right);
			tree->data = _s_rb_tree_get_data(tmp);
			tree->right = s_rb_tree_remove(right, compare, destroy,
				_s_rb_tree_get_data(tmp));
		}
	} else if (ret > 0) {
		tree->left = s_rb_tree_remove(left, compare, destroy, data);
	} else {
		tree->right = s_rb_tree_remove(right, compare, destroy, data);
	}
	return tree;
}

