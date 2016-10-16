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
#include "s_rb_tree-private.h"
#include "m_utils.h"

struct s_rb_tree *_s_rb_tree_get_uncle(struct s_rb_tree *tree)
{
	m_return_val_if_fail(tree, tree);

	struct s_rb_tree *p = _s_rb_tree_get_parent(tree);
	struct s_rb_tree *gp = _s_rb_tree_get_grand_parent(tree);

	if (_s_rb_tree_is_left(gp, p))
		return _s_rb_tree_get_right(gp);
	else
		return _s_rb_tree_get_left(gp);
}

/**
 * Exemple :
 *          a              b
 *         / \            / \
 *        b   c   =>     d   a
 *       / \                / \
 *      d   e              e   c
 * the content of C/D/E stay unchanged after the rotation
 */
void _s_rb_tree_right_rotate(struct s_rb_tree *a)
{
	m_return_if_fail(a);

	void *data = _s_rb_tree_get_data(a);
	enum _e_color color = _s_rb_tree_get_color(a);
	struct s_rb_tree *b = _s_rb_tree_get_left(a);
	struct s_rb_tree *a_right = _s_rb_tree_get_right(a);
	struct s_rb_tree *d = _s_rb_tree_get_left(b);

	/* in that case (a) -> (b) */
	_s_rb_tree_set_data(a, _s_rb_tree_get_data(b));
	_s_rb_tree_set_color(a, _s_rb_tree_get_color(b));
	_s_rb_tree_set_left(a, _s_rb_tree_get_left(b));
	_s_rb_tree_set_right(a, b);
	_s_rb_tree_set_parent(d, a);

	/* in that case (b) -> (a) */
	_s_rb_tree_set_data(b, data);
	_s_rb_tree_set_color(b, color);
	_s_rb_tree_set_left(b, _s_rb_tree_get_right(b));
	_s_rb_tree_set_right(b, a_right);
}


/**
 * Exemple :
 *          a              c
 *         / \            / \
 *        b   c   =>     a   e
 *           / \        / \
 *          d   e      b   d
 * the content of C/D/E stay unchanged after the rotation
 */
void _s_rb_tree_left_rotate(struct s_rb_tree *a)
{
	m_return_if_fail(a);

	void *data = _s_rb_tree_get_data(a);
	enum _e_color color = _s_rb_tree_get_color(a);
	struct s_rb_tree *c = _s_rb_tree_get_right(a);
	struct s_rb_tree *a_left = _s_rb_tree_get_left(a);
	struct s_rb_tree *e = _s_rb_tree_get_right(c);

	/* in that case (a) -> (c) */
	_s_rb_tree_set_data(a, _s_rb_tree_get_data(c));
	_s_rb_tree_set_color(a, _s_rb_tree_get_color(c));
	_s_rb_tree_set_left(a, c);
	_s_rb_tree_set_right(a, _s_rb_tree_get_right(c));
	_s_rb_tree_set_parent(e, a);

	 /* in that case (c) -> (a) */
	_s_rb_tree_set_data(c, data);
	_s_rb_tree_set_color(c, color);
	_s_rb_tree_set_right(c, _s_rb_tree_get_left(c));
	_s_rb_tree_set_left(c, a_left);
}
