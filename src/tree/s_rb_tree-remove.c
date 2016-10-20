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
#include "s_rb_tree-private.h"
#include "m_utils.h"

/**
 * v = deleted node ; u = replaced child
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
 * @brief Internal double black node
 */
static struct s_rb_tree _d_black = {
	.data = NULL,
	.color = _e_d_black,
	.left = NULL,
	.right = NULL,
	.parent = NULL,
};

/**
 * @brief Convenience macro to delete only one node without reccursion
 * @param node[in] : node to delete
 * @param operator[in] : user data deletion operator
 */
#define _s_rb_tree_delete_single(node, operator) { \
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
struct s_rb_tree *_s_bs_tree_find_min(struct s_rb_tree *v)
{
	m_return_val_if_fail(v, v);

	struct s_rb_tree *cur = NULL;
	for (cur = v; _s_rb_tree_get_left(cur); cur = _s_rb_tree_get_left(cur))
		;

	return cur;
}

/**
 * @brief Use for implementing the case 2)
 * @param v[in] : node to remove
 * @param u[in] : child node of v
 * @param destroy[in] : user destroy function
 * @return the modified v node or NULL
 */
struct s_rb_tree *_s_bs_tree_switch(struct s_rb_tree *v,
	struct s_rb_tree *u, t_destroy_func destroy)
{
	m_return_val_if_fail(v, v);

	if (u) {
		_s_rb_tree_set_data(v, _s_rb_tree_get_data(u));
		_s_rb_tree_set_color(v, _e_black);
		_s_rb_tree_set_left(v, _s_rb_tree_get_left(u));
		_s_rb_tree_set_parent(_s_rb_tree_get_left(u), v);
		_s_rb_tree_set_right(v, _s_rb_tree_get_right(u));
		_s_rb_tree_set_parent(_s_rb_tree_get_right(u), v);
		_s_rb_tree_delete_single(u, NULL);
		return v;
	} else {
		if (_s_rb_tree_is_left(_s_rb_tree_get_parent(v), v)) {
			_s_rb_tree_set_left(_s_rb_tree_get_parent(v), NULL);
		} else if (_s_rb_tree_is_right(_s_rb_tree_get_parent(v), v)) {
			_s_rb_tree_set_right(_s_rb_tree_get_parent(v), NULL);
		}
		_s_rb_tree_delete_single(v, destroy);
		return NULL;
	}
}

/**
 * @brief Use to import double black node in the tree
 * @param v[in] : node to remove
 * @param u[in] : child node of v
 * @param destroy[in] : user destroy function
 * @return the double black node on success, NULL on error
 */
struct s_rb_tree *_s_bs_tree_d_black(struct s_rb_tree *v,
	struct s_rb_tree *u, t_destroy_func destroy)
{
	m_return_val_if_fail(v, v);

	struct s_rb_tree *p = _s_rb_tree_get_parent(v);
	struct s_rb_tree *db = &_d_black;

	_s_rb_tree_set_color(db, _e_d_black);
	if (_s_rb_tree_is_left(p, v)) {
		_s_rb_tree_set_left(p, db);
	} else {
		_s_rb_tree_set_right(p, db);
	}
	_s_rb_tree_set_parent(db, p);

	if (u) {
		if (_s_rb_tree_is_left(v, u)) {
			_s_rb_tree_set_left(db, u);
			_s_rb_tree_set_right(db, NULL);
		} else {
			_s_rb_tree_set_left(db, NULL);
			_s_rb_tree_set_right(db, u);
		}
	}
	_s_rb_tree_set_color(u, _e_black);
	_s_rb_tree_delete_single(v, destroy);
	return &_d_black;
}

/**
 * @brief Convenient macro to check if we reach the case a)
 * @param s[in] : sibling node of v (node to remove)
 */
# define _s_rb_tree_is_black_sibling_red_child(s) \
	(_s_rb_tree_get_color(s) == _e_black && \
		(_s_rb_tree_get_color(_s_rb_tree_get_left(s)) == _e_red || \
		_s_rb_tree_get_color(_s_rb_tree_get_right(s)) == _e_red))

/**
 * @brief Perform the case a)
 * @param s[in] : sibling node of v (node to remove)
 */
static void _s_rb_tree_black_sibling_red_child(struct s_rb_tree *u,
	struct s_rb_tree *s)
{
	/* (i) left left case */
	if (_s_rb_tree_is_left(_s_rb_tree_get_parent(s), s) &&
		_s_rb_tree_get_color(_s_rb_tree_get_left(s)) == _e_red) {
ll_case:
		_s_rb_tree_set_color(_s_rb_tree_get_left(s), _e_black);
		_s_rb_tree_right_rotate(_s_rb_tree_get_parent(s));
		if (u == &_d_black) {
			_s_rb_tree_set_color(u, _e_black);
			_s_rb_tree_set_left(_s_rb_tree_get_parent(u), NULL);
			_s_rb_tree_set_parent(u, NULL);
		} else {
			_s_rb_tree_set_color(u, _e_black);
		}
	/* (ii) left right case */
	} else if (_s_rb_tree_is_left(_s_rb_tree_get_parent(s), s) &&
		_s_rb_tree_get_color(_s_rb_tree_get_right(s)) == _e_red) {
		_s_rb_tree_set_color(_s_rb_tree_get_right(s), _e_black);
		_s_rb_tree_left_rotate(s);
		goto ll_case;
	/* (iii) right right case */
	} else if (_s_rb_tree_is_right(_s_rb_tree_get_parent(s), s) &&
		_s_rb_tree_get_color(_s_rb_tree_get_right(s)) == _e_red) {
rr_case:
		_s_rb_tree_set_color(_s_rb_tree_get_right(s), _e_black);
		_s_rb_tree_left_rotate(_s_rb_tree_get_parent(s));
		if (u == &_d_black) {
			_s_rb_tree_set_color(u, _e_black);
			_s_rb_tree_set_left(_s_rb_tree_get_parent(u), NULL);
			_s_rb_tree_set_parent(u, NULL);
		} else {
			_s_rb_tree_set_color(u, _e_black);
		}
	} else if (_s_rb_tree_is_right(_s_rb_tree_get_parent(s), s) &&
		_s_rb_tree_get_color(_s_rb_tree_get_left(s)) == _e_red) {
		_s_rb_tree_set_color(_s_rb_tree_get_left(s), _e_black);
		_s_rb_tree_right_rotate(s);
		goto rr_case;
	}
}

/**
 * @brief Convenient macro to check if we reach the case b)
 * @param s[in] : sibling node of v (node to remove)
 */
# define _s_rb_tree_is_black_sibling_black_childs(s) \
	(_s_rb_tree_get_color(s) == _e_black && \
		_s_rb_tree_get_color(_s_rb_tree_get_left(s)) == _e_black && \
		_s_rb_tree_get_color(_s_rb_tree_get_right(s)) == _e_black)

/**
 * @brief Perform the case b)
 * @param u[in] : child node of v (node to remove)
 * @param s[in] : sibling of u
 */
static struct s_rb_tree *_s_rb_tree_black_sibling_black_childs(
	struct s_rb_tree *u, struct s_rb_tree *s)
{
	m_return_val_if_fail(u, u);

	_s_rb_tree_set_color(s, _e_red);
	if (u == &_d_black) {
		_s_rb_tree_set_color(u, _e_black);
		if (_s_rb_tree_is_left(_s_rb_tree_get_parent(u), u)) {
			_s_rb_tree_set_left(_s_rb_tree_get_parent(u),
				NULL);
		} else {
			_s_rb_tree_set_right(_s_rb_tree_get_parent(u),
				NULL);
		}
	} else {
		_s_rb_tree_set_color(u, _e_black);
	}
	if (_s_rb_tree_get_color(_s_rb_tree_get_parent(u)) == _e_black) {
		_s_rb_tree_set_color(_s_rb_tree_get_parent(u), _e_d_black);
	} else {
		_s_rb_tree_set_color(_s_rb_tree_get_parent(u), _e_black);
	}
	return _s_rb_tree_get_parent(u);
}

/**
 * @brief Perform the case c)
 * @param u[in] : child node of v (node to remove)
 * @param s[in] : sibling of u
 */
static void _s_rb_tree_red_sibling(struct s_rb_tree *u, struct s_rb_tree *s)
{
	m_return_if_fail(u);
	m_return_if_fail(s);

	_s_rb_tree_set_color(u, _e_red);
	_s_rb_tree_set_color(s, _e_black);

	/* (i) */
	if (_s_rb_tree_is_left(u, s)) {
		_s_rb_tree_right_rotate(_s_rb_tree_get_parent(u));
	/* (ii) */
	} else {
		_s_rb_tree_left_rotate(_s_rb_tree_get_parent(u));
	}
	_s_rb_tree_set_color(s, _e_red);
	if (u == &_d_black) {
		_s_rb_tree_set_color(u, _e_black);
		if (_s_rb_tree_is_left(_s_rb_tree_get_parent(u), u)) {
			_s_rb_tree_set_left(_s_rb_tree_get_parent(u), NULL);
		} else {
			_s_rb_tree_set_right(_s_rb_tree_get_parent(u), NULL);
		}
	} else {
		_s_rb_tree_set_color(u, _e_black);
	}
}

/**
 * @brief Reduce double black conflict into the tree (apply case 3)
 * @param u[in] : child of node (possibly older v)
 */
static void _s_rb_tree_reduce_d_black(struct s_rb_tree *u)
{
	m_return_if_fail(u);

	/* 3) */
	while (_s_rb_tree_get_color(u) == _e_d_black &&
	       _s_rb_tree_get_parent(u)) {
		struct s_rb_tree *s = _s_rb_tree_get_sibling(u);

		/* a) */
		if (_s_rb_tree_is_black_sibling_red_child(s)) {
			_s_rb_tree_black_sibling_red_child(u, s);
		/* b) */
		} else if (_s_rb_tree_is_black_sibling_black_childs(s)) {
			u = _s_rb_tree_black_sibling_black_childs(u, s);
		/* c) */
		} else if (_s_rb_tree_get_color(s) == _e_red) {
			_s_rb_tree_red_sibling(u, s);
		}

		/* 3.3) */
		if (!_s_rb_tree_get_parent(u) &&
			_s_rb_tree_get_color(u) == _e_d_black)
			_s_rb_tree_set_color(u, _e_black);
	}
}

/**
 * @brief Remove core algorithm. Apply a binary search tree remove algorith +
 * rearrangement process
 * @param v[in] : node to delete
 * @param cmp[in] : user compare function
 * @param del[in] : user destroy function
 * @param data[in] : data to find and remove
 * @return the new pointer associate to the previous node location
 */
static struct s_rb_tree *_s_bs_tree_remove(struct s_rb_tree *v,
	t_compare_func cmp, t_destroy_func del, void *data)
{
	m_return_val_if_fail(v, v);
	m_return_val_if_fail(cmp, v);

	struct s_rb_tree *left = _s_rb_tree_get_left(v);
	struct s_rb_tree *right = _s_rb_tree_get_right(v);
	int ret = cmp(_s_rb_tree_get_data(v), data);

	if (ret == 0) {
		if (left && right) {
			struct s_rb_tree *tmp = _s_bs_tree_find_min(right);
			_s_rb_tree_set_data(v, _s_rb_tree_get_data(tmp));
			_s_rb_tree_set_right(v, _s_bs_tree_remove(right, cmp,
				del, _s_rb_tree_get_data(tmp)));
		} else {
			struct s_rb_tree *u = left ? left : right;
			if (_s_rb_tree_get_color(v) == _e_red ||
				_s_rb_tree_get_color(u) == _e_red) {
				return _s_bs_tree_switch(v, u, del);
			}
			/* both are black: in case of no u, we will
			 * replace the current v by an inner double
			 * black node */
			_s_rb_tree_reduce_d_black(_s_bs_tree_d_black(v, u, del));
			return NULL;
		}
	} else if (ret > 0) {
		_s_bs_tree_remove(left, cmp, del, data);
	} else {
		_s_bs_tree_remove(right, cmp, del, data);
	}
	return v;
}

struct s_rb_tree *s_rb_tree_remove(struct s_rb_tree *v,
	t_compare_func compare, t_destroy_func destroy, void *data)
{
	m_return_val_if_fail(v, v);
	m_return_val_if_fail(compare, v);

	if (compare(_s_rb_tree_get_data(v), data) == 0 &&
		!_s_rb_tree_get_left(v) && !_s_rb_tree_get_right(v)) {
		_s_rb_tree_delete_single(v, destroy);
		return NULL;
	}

	return _s_bs_tree_remove(v, compare, destroy, data);
}
