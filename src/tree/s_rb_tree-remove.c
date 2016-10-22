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
 * @brief Convenience macro to delete only one node without reccursion
 * @param node[in] : node to delete
 * @param operator[in] : user data deletion operator
 */
#define m_s_rb_tree_delete_single(node, operator) { \
	(node)->left = (node)->right = NULL; \
	if (operator) \
		s_rb_tree_delete_full((node), (operator)); \
	else \
		s_rb_tree_delete(node); \
}

/**
 * @brief Binary search tree remove function definition
 */
static struct s_rb_tree *_s_bs_tree_remove(struct s_rb_tree *v,
	t_compare_func cmp, t_destroy_func destroy, void *data);

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
	for (cur = v; m_s_rb_tree_get_left(cur); cur = m_s_rb_tree_get_left(cur))
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
		m_s_rb_tree_set_data(v, m_s_rb_tree_get_data(u));
		m_s_rb_tree_set_color(v, _e_black);
		m_s_rb_tree_set_left(v, m_s_rb_tree_get_left(u));
		m_s_rb_tree_set_parent(m_s_rb_tree_get_left(u), v);
		m_s_rb_tree_set_right(v, m_s_rb_tree_get_right(u));
		m_s_rb_tree_set_parent(m_s_rb_tree_get_right(u), v);
		m_s_rb_tree_delete_single(u, NULL);
		return v;
	} else {
		if (m_s_rb_tree_is_left(m_s_rb_tree_get_parent(v), v)) {
			m_s_rb_tree_set_left(m_s_rb_tree_get_parent(v), NULL);
		} else if (m_s_rb_tree_is_right(m_s_rb_tree_get_parent(v), v)) {
			m_s_rb_tree_set_right(m_s_rb_tree_get_parent(v), NULL);
		}
		m_s_rb_tree_delete_single(v, destroy);
		return NULL;
	}
}

/**
 * @brief Convenient macro to check if we reach the case a)
 * @param s[in] : sibling node of v (node to remove)
 */
#define m_s_rb_tree_is_black_sibling_red_child(s) \
	(m_s_rb_tree_get_color(s) == _e_black && \
		(m_s_rb_tree_get_color(m_s_rb_tree_get_left(s)) == _e_red || \
		m_s_rb_tree_get_color(m_s_rb_tree_get_right(s)) == _e_red))

/**
 * @brief Perform the case a)
 * @param s[in] : sibling node of v (node to remove)
 */
static void _s_rb_tree_black_sibling_red_child(struct s_rb_tree *s)
{
	/* (i) left left case */
	if (m_s_rb_tree_is_left(m_s_rb_tree_get_parent(s), s) &&
		m_s_rb_tree_get_color(m_s_rb_tree_get_left(s)) == _e_red) {
ll_case:
		m_s_rb_tree_set_color(s, _e_red);
		m_s_rb_tree_set_color(m_s_rb_tree_get_parent(s), _e_black);
		m_s_rb_tree_set_color(m_s_rb_tree_get_left(s), _e_black);
		_s_rb_tree_right_rotate(m_s_rb_tree_get_parent(s));
	/* (ii) left right case */
	} else if (m_s_rb_tree_is_left(m_s_rb_tree_get_parent(s), s) &&
		m_s_rb_tree_get_color(m_s_rb_tree_get_right(s)) == _e_red) {
		_s_rb_tree_left_rotate(s);
		goto ll_case;
	/* (iii) right right case */
	} else if (m_s_rb_tree_is_right(m_s_rb_tree_get_parent(s), s) &&
		m_s_rb_tree_get_color(m_s_rb_tree_get_right(s)) == _e_red) {
rr_case:
		m_s_rb_tree_set_color(s, _e_red);
		m_s_rb_tree_set_color(m_s_rb_tree_get_parent(s), _e_black);
		m_s_rb_tree_set_color(m_s_rb_tree_get_right(s), _e_black);
		_s_rb_tree_left_rotate(m_s_rb_tree_get_parent(s));
	} else if (m_s_rb_tree_is_right(m_s_rb_tree_get_parent(s), s) &&
		m_s_rb_tree_get_color(m_s_rb_tree_get_left(s)) == _e_red) {
		_s_rb_tree_right_rotate(s);
		goto rr_case;
	}
}

/**
 * @brief Convenient macro to check if we reach the case b)
 * @param s[in] : sibling node of v (node to remove)
 */
#define m_s_rb_tree_is_black_sibling_black_childs(s) \
	(m_s_rb_tree_get_color(s) == _e_black && \
		m_s_rb_tree_get_color(m_s_rb_tree_get_left(s)) == _e_black && \
		m_s_rb_tree_get_color(m_s_rb_tree_get_right(s)) == _e_black)

/**
 * @brief Perform the case c)
 * @param u[in] : child node of v (node to remove)
 * @param s[in] : sibling of u
 */
static void _s_rb_tree_red_sibling(struct s_rb_tree *u, struct s_rb_tree *s)
{
	m_return_if_fail(u);
	m_return_if_fail(s);

	m_s_rb_tree_set_color(u, _e_red);
	m_s_rb_tree_set_color(s, _e_black);

	/* (i) */
	if (m_s_rb_tree_is_left(u, s)) {
		_s_rb_tree_right_rotate(m_s_rb_tree_get_parent(u));
	/* (ii) */
	} else {
		_s_rb_tree_left_rotate(m_s_rb_tree_get_parent(u));
	}
	m_s_rb_tree_set_color(s, _e_red);
	m_s_rb_tree_set_color(u, _e_black);
}

/**
 * @brief Reduce double black conflict into the tree (apply case 3)
 * @param u[in] : child of node (possibly older v)
 */
static void _s_rb_tree_reduce_d_black(struct s_rb_tree *v)
{
	m_return_if_fail(v);

	/* 3) */
	while (m_s_rb_tree_get_color(v) == _e_d_black) {
		struct s_rb_tree *s = m_s_rb_tree_get_sibling(v);

		/* a) */
		if (m_s_rb_tree_is_black_sibling_red_child(s)) {
			_s_rb_tree_black_sibling_red_child(s);
			m_s_rb_tree_set_color(v, _e_black);
		/* b) */
		} else if (m_s_rb_tree_is_black_sibling_black_childs(s)) {
			struct s_rb_tree *p = m_s_rb_tree_get_parent(v);
			m_s_rb_tree_set_color(s, _e_red);
			m_s_rb_tree_set_color(v, _e_black);
			v = m_s_rb_tree_get_parent(v);
			if (m_s_rb_tree_get_color(p) == _e_black) {
				m_s_rb_tree_set_color(v, _e_d_black);
			} else {
				m_s_rb_tree_set_color(v, _e_black);
			}
		/* c) */
		} else if (m_s_rb_tree_get_color(s) == _e_red) {
			_s_rb_tree_red_sibling(v, s);
		}
	}
}

/**
 * @brief Remove leaf from the tree
 * @param v[in] : node to remove
 * @param destroy[in] : user data destroy function
 * @return NULL
 */
static struct s_rb_tree *_s_rb_tree_remove_no_child(struct s_rb_tree *v,
	t_destroy_func destroy)
{
	m_return_val_if_fail(v, v);

	if (m_s_rb_tree_is_black(v)) {
		m_s_rb_tree_set_color(v, _e_d_black);
		_s_rb_tree_reduce_d_black(v);
	}
	if (m_s_rb_tree_is_left(m_s_rb_tree_get_parent(v), v)) {
		m_s_rb_tree_set_left(m_s_rb_tree_get_parent(v), NULL);
	} else {
		m_s_rb_tree_set_right(m_s_rb_tree_get_parent(v), NULL);
	}
	m_s_rb_tree_delete_single(v, destroy);
	return NULL;
}

/**
 * @brief Remove one child node from the tree
 * @param v[in] : node to remove
 * @param destroy[in] : user data destroy function
 * @return v
 */
static struct s_rb_tree *_s_rb_tree_remove_one_child(struct s_rb_tree *v,
	t_destroy_func destroy)
{
	m_return_val_if_fail(v, v);

	struct s_rb_tree *tmp = m_s_rb_tree_get_left(v) ?
		m_s_rb_tree_get_left(v) : m_s_rb_tree_get_right(v);

	m_s_rb_tree_switch_data(v, tmp, destroy);
	if (m_s_rb_tree_is_black(v) && m_s_rb_tree_is_black(tmp)) {
		m_s_rb_tree_set_color(tmp, _e_d_black);
		_s_rb_tree_reduce_d_black(v);
	}
	m_s_rb_tree_set_color(tmp, _e_black);
	if (m_s_rb_tree_is_left(m_s_rb_tree_get_parent(v), v)) {
		m_s_rb_tree_set_left(m_s_rb_tree_get_parent(v), tmp);
		m_s_rb_tree_set_parent(tmp, m_s_rb_tree_get_parent(v));
	} else {
		m_s_rb_tree_set_right(m_s_rb_tree_get_parent(v), tmp);
		m_s_rb_tree_set_parent(tmp, m_s_rb_tree_get_parent(v));
	}
	m_s_rb_tree_delete_single(v, NULL);
	return tmp;
}

/**
 * @brief Remove one child node from the tree
 * @param v[in] : node to remove
 * @param destroy[in] : user data destroy function
 * @return v
 */
static struct s_rb_tree *_s_rb_tree_remove_two_child(struct s_rb_tree *v,
	t_compare_func cmp, t_destroy_func destroy)
{
	m_return_val_if_fail(v, v);

	struct s_rb_tree *tmp = _s_bs_tree_find_min(m_s_rb_tree_get_left(v));

	m_s_rb_tree_switch_data(v, tmp, destroy);
	m_s_rb_tree_set_color(tmp, _e_d_black);
	_s_bs_tree_remove(m_s_rb_tree_get_left(v), cmp, NULL,
		m_s_rb_tree_get_data(tmp));

	return v;
}

/**
 * @brief Remove core algorithm. Apply a binary search tree remove algorith +
 * rearrangement process
 * @param v[in] : node to delete
 * @param cmp[in] : user compare function
 * @param del[in] : user destroy function
 * @param data[in] : data to find and remove
 * @return the possibly changed tree root
 */
static struct s_rb_tree *_s_bs_tree_remove(struct s_rb_tree *v,
	t_compare_func cmp, t_destroy_func destroy, void *data)
{
	m_return_val_if_fail(v, v);
	m_return_val_if_fail(cmp, v);

	int ret = cmp(m_s_rb_tree_get_data(v), data);
	if (ret == 0) {
		if (!m_s_rb_tree_get_left(v) && !m_s_rb_tree_get_right(v)) {
			return _s_rb_tree_remove_no_child(v, destroy);
		} else if (!m_s_rb_tree_get_left(v) || !m_s_rb_tree_get_right(v)) {
			return _s_rb_tree_remove_one_child(v, destroy);
		} else {
			return _s_rb_tree_remove_two_child(v, cmp, destroy);
		}
	} else if (ret > 0) {
		if (m_s_rb_tree_get_left(v))
			_s_bs_tree_remove(m_s_rb_tree_get_left(v), cmp, destroy,
				data);
	} else {
		if (m_s_rb_tree_get_right(v))
			_s_bs_tree_remove(m_s_rb_tree_get_right(v), cmp, destroy,
				data);
	}
	return v;
}

struct s_rb_tree *s_rb_tree_remove(struct s_rb_tree *v,
	t_compare_func compare, t_destroy_func destroy, void *data)
{
	m_return_val_if_fail(v, v);
	m_return_val_if_fail(compare, v);

	v = _s_bs_tree_remove(v, compare, destroy, data);
	if (v)
		m_s_rb_tree_set_color(v, _e_black);
	return v;
}
