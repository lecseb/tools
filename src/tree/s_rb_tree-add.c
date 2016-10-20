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
#include "m_alloc.h"
#include "m_utils.h"

/**
 * @brief Allocate a new binary search tree node instance
 * @param parent[in] : parent node of the newly allocate one
 * @param data[in] : data to store
 * @return a valid pointer
 */
static struct s_rb_tree *_s_rb_tree_new(struct s_rb_tree *parent, void *data)
{
	struct s_rb_tree *new = _malloc(sizeof(struct s_rb_tree));
	new->data = data;
	new->parent = parent;
	return new;
}

/**
 * @brief Convenience macro to check if we are in left left case
 * @param x[in] : inserted node
 */
# define _s_rb_tree_is_ll_case(x) \
	(_s_rb_tree_is_left(_s_rb_tree_get_grand_parent(x), \
		_s_rb_tree_get_parent(x)) && \
		_s_rb_tree_is_left(_s_rb_tree_get_parent(x), x))

/**
 * @brief Convenience macro to check if we are in left right case
 * @param x[in] : inserted node
 */
# define _s_rb_tree_is_lr_case(x) \
	(_s_rb_tree_is_left(_s_rb_tree_get_grand_parent(x), \
		_s_rb_tree_get_parent(x)) && \
		_s_rb_tree_is_right(_s_rb_tree_get_parent(x), x))

/**
 * @brief Convenience macro to check if we are in right right case
 * @param x[in] : inserted node
 */
# define _s_rb_tree_is_rr_case(x) \
	(_s_rb_tree_is_right(_s_rb_tree_get_grand_parent(x), \
		_s_rb_tree_get_parent(x)) && \
		_s_rb_tree_is_right(_s_rb_tree_get_parent(x), x))

/**
 * @brief Convenience macro to check if we are in right left case
 * @param x[in] : inserted node
 */
# define _s_rb_tree_is_rl_case(x) \
	(_s_rb_tree_is_right(_s_rb_tree_get_grand_parent(x), \
		_s_rb_tree_get_parent(x)) && \
		_s_rb_tree_is_left(_s_rb_tree_get_parent(x), x))

/**
 * @brief Rearrange the tree to be a valid red/black tree
 * @param x[in] : newly added node
 */
static void _s_rb_tree_rearrange(struct s_rb_tree *x)
{
	m_return_if_fail(x);

	if (_s_rb_tree_get_color(_s_rb_tree_get_parent(x)) == _e_red ||
			!_s_rb_tree_get_parent(x)) {
		if (_s_rb_tree_is_ll_case(x)) {
ll_case:
			_s_rb_tree_set_color(x, _e_black);
			_s_rb_tree_right_rotate(_s_rb_tree_get_grand_parent(x));
		} else if (_s_rb_tree_is_lr_case(x)) {
			_s_rb_tree_left_rotate(_s_rb_tree_get_grand_parent(x));
			_s_rb_tree_left_rotate(_s_rb_tree_get_parent(x));
			goto ll_case;
		} else if (_s_rb_tree_is_rr_case(x)) {
rr_case:
			_s_rb_tree_set_color(x, _e_black);
			_s_rb_tree_left_rotate(_s_rb_tree_get_grand_parent(x));
		} else if (_s_rb_tree_is_rl_case(x)) {
			_s_rb_tree_set_color(_s_rb_tree_get_parent(x), _e_black);
			_s_rb_tree_right_rotate(_s_rb_tree_get_parent(x));
			goto rr_case;
		}
		if (_s_rb_tree_get_parent(x))
			_s_rb_tree_rearrange(_s_rb_tree_get_parent(x));
	}
}

/**
 * @brief Add an element in the tree according to the binary search tree
 * algorithm
 * @param tree[in] : root to modify
 * @param compare[in] : compare operator
 * @param destroy[in] : destroy operator
 * @param data[in] : data to add in the tree
 */
static struct s_rb_tree *_s_bs_tree_add(struct s_rb_tree *tree,
	t_compare_func compare, void *data)
{
	m_return_val_if_fail(tree, tree);
	m_return_val_if_fail(compare, tree);

	int ret = compare(_s_rb_tree_get_data(tree), data);
	if (ret > 0) {
		if (!_s_rb_tree_get_left(tree)) {
			_s_rb_tree_set_left(tree, _s_rb_tree_new(tree, data));
			return _s_rb_tree_get_left(tree);
		}
		return _s_bs_tree_add(_s_rb_tree_get_left(tree), compare, data);
	} else {
		if (!_s_rb_tree_get_right(tree)) {
			_s_rb_tree_set_right(tree, _s_rb_tree_new(tree, data));
			return _s_rb_tree_get_right(tree);
		}
		return _s_bs_tree_add(_s_rb_tree_get_right(tree), compare,
			data);
	}
}

struct s_rb_tree *s_rb_tree_add(struct s_rb_tree *tree, t_compare_func compare,
	void *data)
{
	m_return_val_if_fail(compare, tree);

	/* 1) perform a bst insertion or a creation if no root */
	if (tree) {
		struct s_rb_tree *x = _s_bs_tree_add(tree, compare, data);
		_s_rb_tree_rearrange(x);
	} else {
		tree = _s_rb_tree_new(NULL, data);
	}
	/* 2) change color if x is root */
	_s_rb_tree_set_color(tree, _e_black);
	return tree;
}
