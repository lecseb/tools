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
#include <string.h>
#include "s_rb_tree.h"
#include "s_rb_tree-private.h"
#include "m_alloc.h"
#include "m_utils.h"

void s_rb_tree_delete(struct s_rb_tree *tree)
{
	m_return_if_fail(tree);

	struct s_rb_tree *left = _s_rb_tree_get_left(tree);
	struct s_rb_tree *right = _s_rb_tree_get_right(tree);

	if (left)
		s_rb_tree_delete(left);
	if (right)
		s_rb_tree_delete(right);
	_free(tree);
}

void s_rb_tree_delete_full(struct s_rb_tree *tree, t_destroy_func destroy)
{
	m_return_if_fail(tree);

	struct s_rb_tree *left = _s_rb_tree_get_left(tree);
	struct s_rb_tree *right = _s_rb_tree_get_right(tree);

	if (left)
		s_rb_tree_delete_full(left, destroy);
	if (right)
		s_rb_tree_delete_full(right, destroy);
	if (destroy)
		destroy(_s_rb_tree_get_data(tree));
	_free(tree);
}

/**
 * -----------------------------------------------------------------------------
 * find implementation
 * -----------------------------------------------------------------------------
 */

void *s_rb_tree_nth_smallest(struct s_rb_tree *tree, uint32_t nth)
{
	m_return_val_if_fail(tree, NULL);
	m_return_val_if_fail(nth > 0, NULL);

	struct s_rb_tree *left = _s_rb_tree_get_left(tree);
	struct s_rb_tree *right = _s_rb_tree_get_right(tree);
	static uint32_t k = 0;

	struct s_rb_tree *elt = (left) ? s_rb_tree_nth_smallest(left, nth) : NULL;

	if (elt)
		return _s_rb_tree_get_data(elt);
	else if (++k == nth)
		return _s_rb_tree_get_data(tree);

	elt = (right) ? s_rb_tree_nth_smallest(right, nth) : elt;
	return _s_rb_tree_get_data(elt);
}

void *s_rb_tree_nth_biggest(struct s_rb_tree *tree, uint32_t nth)
{
	m_return_val_if_fail(tree, NULL);
	m_return_val_if_fail(nth > 0, NULL);

	struct s_rb_tree *left = _s_rb_tree_get_left(tree);
	struct s_rb_tree *right = _s_rb_tree_get_right(tree);
	static uint32_t k = 0;

	struct s_rb_tree *elt = (left) ? s_rb_tree_nth_biggest(right, nth) : NULL;

	if (elt)
		return _s_rb_tree_get_data(elt);
	else if (++k == nth)
		return _s_rb_tree_get_data(tree);

	elt = (right) ? s_rb_tree_nth_biggest(left, nth) : elt;
	return _s_rb_tree_get_left(elt);
}

/**
 * -----------------------------------------------------------------------------
 * foreach implementation
 * -----------------------------------------------------------------------------
 */

/**
 * @brief Pre-order traversal visit each node in a tree by recursively visiting
 * each node in the left and right subtrees of the root.
 * @param tree[in] : tree to visit
 * @param foreach[in] : foreach user callback
 * @param data[in] : user data pass through the callback
 * @return 0 on success, errno on error
 */
static int _s_rb_tree_depth_pre(struct s_rb_tree *tree,
	t_foreach_func foreach, void *user_data)
{
	m_return_val_if_fail(tree, -EINVAL);
	m_return_val_if_fail(foreach, -EINVAL);

	struct s_rb_tree *left = _s_rb_tree_get_left(tree);
	struct s_rb_tree *right = _s_rb_tree_get_right(tree);

	int ret = foreach(_s_rb_tree_get_data(tree), user_data);
	ret |= (left) ? _s_rb_tree_depth_pre(left, foreach, user_data) : 0;
	ret |= (right) ? _s_rb_tree_depth_pre(right, foreach, user_data) : 0;

	return ret;
}

/**
 * @brief Post-order traversal visit each node in a tree by recursively visiting
 * each node in the left and right subtrees of the root.
 * @param tree[in] : tree to visit
 * @param foreach[in] : foreach user callback
 * @param data[in] : user data pass through the callback
 * @return 0 on success, errno on error
 */
static int _s_rb_tree_depth_post(struct s_rb_tree *tree,
	t_foreach_func foreach, void *user_data)
{
	m_return_val_if_fail(tree, -EINVAL);
	m_return_val_if_fail(foreach, -EINVAL);

	struct s_rb_tree *left = _s_rb_tree_get_left(tree);
	struct s_rb_tree *right = _s_rb_tree_get_right(tree);

	int ret = (left) ? _s_rb_tree_depth_post(left, foreach, user_data) : 0;
	ret |= (right) ? _s_rb_tree_depth_post(right, foreach, user_data) : 0;
	ret |= foreach(_s_rb_tree_get_data(tree), user_data);

	return ret;
}

/**
 * @brief In-order traversal visit each node in a tree by recursively visiting
 * each node in the left and right subtrees of the root.
 * @param tree[in] : tree to visit
 * @param foreach[in] : foreach user callback
 * @param data[in] : user data pass through the callback
 * @return 0 on success, errno on error
 */
static int _s_rb_tree_depth_in(struct s_rb_tree *tree, t_foreach_func foreach,
	void *user_data)
{
	m_return_val_if_fail(tree, -EINVAL);
	m_return_val_if_fail(foreach, -EINVAL);

	struct s_rb_tree *left = _s_rb_tree_get_left(tree);
	struct s_rb_tree *right = _s_rb_tree_get_right(tree);

	int ret = (left) ? _s_rb_tree_depth_post(left, foreach, user_data) : 0;
	ret |= foreach(_s_rb_tree_get_data(tree), user_data);
	ret |= (right) ? _s_rb_tree_depth_post(right, foreach, user_data) : 0;

	return ret;
}

/**
 * @brief Contrasting with depth-first order is breadth-first order, which
 * always attempts to visit the node closest to the root that it has not already
 * visited. See breadth-first search for more information. Also called a
 * level-order traversal.
 * @param tree[in] : tree to visit
 * @param foreach[in] : foreach user callback
 * @param data[in] : user data pass through the callback
 * @return 0 on success, errno on error
 */
static int _s_rb_tree_breath(struct s_rb_tree *tree, t_foreach_func foreach,
	void *data)
{
	m_return_val_if_fail(tree, -EINVAL);
	m_return_val_if_fail(foreach, -EINVAL);

	int ret = 0;
	struct s_queue *queue = s_queue_new();
	ret = s_queue_push(queue, tree);

	while (!s_queue_empty(queue)) {
		struct s_rb_tree *tmp = s_queue_pop(queue);
		struct s_rb_tree *left = _s_rb_tree_get_left(tmp);
		struct s_rb_tree *right = _s_rb_tree_get_right(tmp);

		ret |= foreach(_s_rb_tree_get_data(tmp), data);
		ret |= (left) ? s_queue_push(queue, left) : 0;
		ret |= (right) ? s_queue_push(queue, right) : 0;
	}

	s_queue_delete(queue);
	return ret;
}

int s_rb_tree_foreach(struct s_rb_tree *tree, enum e_tree_browse type,
	t_foreach_func foreach, void *user_data)
{
	m_return_val_if_fail(tree, -EINVAL);
	m_return_val_if_fail(foreach, -EINVAL);

	switch (type) {
	case e_tree_depth_pre:
		return _s_rb_tree_depth_pre(tree, foreach, user_data);
	case e_tree_depth_post:
		return _s_rb_tree_depth_post(tree, foreach, user_data);
	case e_tree_depth_in:
		return _s_rb_tree_depth_in(tree, foreach, user_data);
	case e_tree_breath:
		return _s_rb_tree_breath(tree, foreach, user_data);
	}
	return 0;
}

/**
 * -----------------------------------------------------------------------------
 * debug implementation
 * -----------------------------------------------------------------------------
 */

#ifdef DEBUG

/**
 * @brief Dump a specific node
 * @param tree[in] : node to dump
 * @param file[in] : file to export the dump
 * @return 0 on success, errno on error
 */
static int _s_rb_tree_dump_node(struct s_rb_tree *tree, FILE *file)
{
	m_return_val_if_fail(tree, -EINVAL);
	m_return_val_if_fail(file, -EINVAL);

	char buff[512];
	const char *color;
	if (_s_rb_tree_get_color(tree) == _e_red) {
		color = "red";
	} else if (_s_rb_tree_get_color(tree) == _e_black) {
		color = "grey";
	} else {
		color = "green";
	}

	snprintf(buff, 512, "\t%d[color=%s,style=filled]\n",
		m_ptr_to_int(tree->data), color);
	fwrite(buff, strlen(buff), sizeof(char), file);

	return 0;
}

/**
 * @brief Dump an entire tree from the entry point tree
 * @param tree[in] : tree to dump
 * @param file[in] : file to export the dump
 * @return 0 on success, errno on error
 */
static int _s_rb_tree_dump_dot(struct s_rb_tree *tree, FILE *file)
{
	m_return_val_if_fail(tree, -EINVAL);
	m_return_val_if_fail(file, -EINVAL);

	char buff[512];
	struct s_rb_tree *left = _s_rb_tree_get_left(tree);
	struct s_rb_tree *right = _s_rb_tree_get_right(tree);

	int ret = _s_rb_tree_dump_node(tree, file);
	if (left) {
		snprintf(buff, 512, "\t%d -- %d;\n", m_ptr_to_int(tree->data),
			 m_ptr_to_int(left->data));
		fwrite(buff, strlen(buff), sizeof(char), file);
		ret |= _s_rb_tree_dump_dot(left, file);
	}
	if (right) {
		snprintf(buff, 512, "\t%d -- %d;\n", m_ptr_to_int(tree->data),
			 m_ptr_to_int(right->data));
		fwrite(buff, strlen(buff), sizeof(char), file);
		ret |= _s_rb_tree_dump_dot(right, file);
	}
	return ret;
}

#endif /* !DEBUG */

int s_rb_tree_dump_dot(struct s_rb_tree *tree, const char *file)
{
	m_return_val_if_fail(tree, -EINVAL);
	m_return_val_if_fail(file, -EINVAL);

	int ret = 0;
#ifdef DEBUG
	FILE *fd = fopen(file, "w+");

	if (fd) {
		char *header = "graph rb {\n";
		fwrite(header, strlen(header), sizeof(char), fd);

		ret = _s_rb_tree_dump_dot(tree, fd);

		fwrite("}\n", 2, sizeof(char), fd);

		fflush(fd);
		fclose(fd);
	}
#endif /* !DEBUG */
	return ret;
}
