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
#include "queue/s_queue.h"
#include "tree/s_bs_tree.h"
#include "m_alloc.h"
#include "m_utils.h"

/**
 * @brief The binary search tree structure
 * @param data: user data stored
 * @param left: left child
 * @param right: right child
 */
struct s_bs_tree {
	void *data;
	struct s_bs_tree *left;
	struct s_bs_tree *right;
};

/**
 * @brief A convenience macro to get the data in an element.
 */
# define m_s_bs_tree_get_data(tree) ((tree) ? (tree)->data : NULL)

/**
 * @brief A convenience macro to get the left element of a node.
 */
# define m_s_bs_tree_get_left(tree) ((tree) ? (tree)->left : NULL)

/**
 * @brief A convenience macro to get the right element of a node.
 */
# define m_s_bs_tree_get_right(tree) ((tree) ? (tree)->right : NULL)

/**
 * @brief Allocate a new node
 * @param data[in]: data tp store in the node
 * @return a valid pointer on success, NULL on error
 */
static struct s_bs_tree *_s_bs_tree_new(void *data)
{
	struct s_bs_tree *tree = _malloc(sizeof(struct s_bs_tree));
	tree->data = data;
	return tree;
}

void s_sb_tree_delete(struct s_bs_tree *tree)
{
	m_return_if_fail(tree);

	if (m_s_bs_tree_get_left(tree))
		s_bs_tree_delete(m_s_bs_tree_get_left(tree));
	if (m_s_bs_tree_get_right(tree))
		s_bs_tree_delete(m_s_bs_tree_get_right(tree));
	_free(tree);
}

void s_bs_tree_delete_full(struct s_bs_tree *tree, t_destroy_func destroy)
{
	m_return_if_fail(tree);

	if (m_s_bs_tree_get_left(tree))
		s_bs_tree_delete_full(m_s_bs_tree_get_left(tree), destroy);
	if (m_s_bs_tree_get_right(tree))
		s_bs_tree_delete_full(m_s_bs_tree_get_right(tree), destroy);
	if (destroy)
		destroy(m_s_bs_tree_get_data(tree));
	_free(tree);
}

/**
 * -----------------------------------------------------------------------------
 * find implementation
 * -----------------------------------------------------------------------------
 */
/**
 * @brief Core function
 */
static struct s_bs_tree *_s_bs_tree_nth_smallest(struct s_bs_tree *tree,
	uint32_t nth, uint32_t k)
{
	m_return_val_if_fail(tree, NULL);
	m_return_val_if_fail(nth > 0, NULL);

	struct s_bs_tree *data = (m_s_bs_tree_get_left(tree)) ?
		_s_bs_tree_nth_smallest(m_s_bs_tree_get_left(tree), nth, k) :
		NULL;

	if (data)
		return data;
	else if (++k == nth)
		return tree;

	return (m_s_bs_tree_get_right(tree)) ?
		_s_bs_tree_nth_smallest(m_s_bs_tree_get_right(tree), nth, k) :
		data;
}

void *s_bs_tree_nth_smallest(struct s_bs_tree *tree, uint32_t nth)
{
	return m_s_bs_tree_get_data(_s_bs_tree_nth_smallest(tree, nth, 0));
}

/**
 * @brief Core function
 */
static struct s_bs_tree *_s_bs_tree_nth_biggest(struct s_bs_tree *tree,
	uint32_t nth, uint32_t k)
{
	m_return_val_if_fail(tree, NULL);
	m_return_val_if_fail(nth > 0, NULL);

	struct s_bs_tree *data = (m_s_bs_tree_get_right(tree)) ?
		s_bs_tree_nth_biggest(m_s_bs_tree_get_right(tree), nth) : NULL;

	if (data)
		return data;
	else if (++k == nth)
		return tree;

	return (m_s_bs_tree_get_left(tree)) ?
		s_bs_tree_nth_biggest(m_s_bs_tree_get_left(tree), nth) : data;
}

void *s_bs_tree_nth_biggest(struct s_bs_tree *tree, uint32_t nth)
{
	return m_s_bs_tree_get_data(_s_bs_tree_nth_biggest(tree, nth, 0));
}

int s_bs_tree_exist(struct s_bs_tree *tree, t_compare_func cmp, void *data)
{
	m_return_val_if_fail(tree, -EINVAL);
	m_return_val_if_fail(cmp, -EINVAL);

	int ret = cmp(m_s_bs_tree_get_data(tree), data);
	if (ret == 0)
		return 0;
	else if (ret > 0)
		return (m_s_bs_tree_get_left(tree)) ?
			s_bs_tree_exist(m_s_bs_tree_get_left(tree), cmp, data) :
			-EAGAIN;
	else
		return (m_s_bs_tree_get_right(tree)) ?
			s_bs_tree_exist(m_s_bs_tree_get_right(tree), cmp,
				data) : -EAGAIN;
}

/**
 * -----------------------------------------------------------------------------
 * add implementation
 * -----------------------------------------------------------------------------
 */
/**
 * @brief Core algorithm
 */
static struct s_bs_tree *_s_bs_tree_add(struct s_bs_tree *tree,
	t_compare_func compare, void *data)
{
	m_return_val_if_fail(tree, tree);
	m_return_val_if_fail(compare, tree);

	int ret = compare(m_s_bs_tree_get_data(tree), data);
	if (ret > 0) {
		if (!m_s_bs_tree_get_left(tree)) {
			tree->left = _s_bs_tree_new(data);
			return m_s_bs_tree_get_left(tree);
		}
		return s_bs_tree_add(m_s_bs_tree_get_left(tree), compare,
			data);
	} else {
		if (!m_s_bs_tree_get_right(tree)) {
			tree->right = _s_bs_tree_new(data);
			return m_s_bs_tree_get_right(tree);
		}
		return s_bs_tree_add(m_s_bs_tree_get_right(tree), compare,
			data);
	}
}

struct s_bs_tree *s_bs_tree_add(struct s_bs_tree *tree,
	t_compare_func compare, void *data)
{
	return (tree) ? _s_bs_tree_add(tree, compare, data) : _s_bs_tree_new(data);
}

/**
 * -----------------------------------------------------------------------------
 * remove implementation
 * -----------------------------------------------------------------------------
 */
/**
 * @brief Get the minimum data value from the tree according to the given
 * tree
 * @param btree[in] : root tree to search
 * @return a valid tree pointer on succes, NULL on error
 */
static struct s_bs_tree *_s_bs_tree_find_min(struct s_bs_tree *tree)
{
	m_return_val_if_fail(tree, tree);

	struct s_bs_tree *curr = tree;

	while (m_s_bs_tree_get_right(curr))
		curr = m_s_bs_tree_get_right(curr);

	return curr;
}

/**
 * @brief Convenient macro to delete only one node and avoid the recursion
 */
# define _s_bs_tree_delete_single(root, operator) \
	(root)->left = (tree)->right = NULL; \
	if (operator) \
		s_bs_tree_delete_full((tree), (operator))

struct s_bs_tree *s_bs_tree_remove(struct s_bs_tree *tree,
	t_compare_func compare, t_destroy_func destroy, void *data)
{
	m_return_val_if_fail(tree, tree);
	m_return_val_if_fail(compare, tree);

	struct s_bs_tree *left = m_s_bs_tree_get_left(tree);
	struct s_bs_tree *right = m_s_bs_tree_get_right(tree);
	int ret = compare(m_s_bs_tree_get_data(tree), data);

	if (ret == 0) {
		if (!left) {
			_s_bs_tree_delete_single(tree, destroy);
			return right;
		} else if (!right) {
			_s_bs_tree_delete_single(tree, destroy);
			return left;
		}
		struct s_bs_tree *tmp = _s_bs_tree_find_min(right);
		tree->data = tmp->data;
		tree->right = s_bs_tree_remove(right, compare, destroy,
			tmp->data);
	} else if (ret > 0) {
		tree->left = s_bs_tree_remove(left, compare, destroy, data);
	} else {
		tree->right = s_bs_tree_remove(right, compare, destroy, data);
	}
	return tree;
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
static int _s_bs_tree_depth_pre(struct s_bs_tree *tree,
	t_foreach_func foreach, void *user_data)
{
	m_return_val_if_fail(tree, -EINVAL);
	m_return_val_if_fail(foreach, -EINVAL);

	struct s_bs_tree *left = m_s_bs_tree_get_left(tree);
	struct s_bs_tree *right = m_s_bs_tree_get_right(tree);

	int ret = foreach(m_s_bs_tree_get_data(tree), user_data);
	ret |= (left) ? _s_bs_tree_depth_pre(left, foreach, user_data) : 0;
	ret |= (right) ? _s_bs_tree_depth_pre(right, foreach, user_data) : 0;

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
static int _s_bs_tree_depth_post(struct s_bs_tree *tree,
	t_foreach_func foreach, void *user_data)
{
	m_return_val_if_fail(tree, -EINVAL);
	m_return_val_if_fail(foreach, -EINVAL);

	struct s_bs_tree *left = m_s_bs_tree_get_left(tree);
	struct s_bs_tree *right = m_s_bs_tree_get_right(tree);

	int ret = (left) ? _s_bs_tree_depth_post(left, foreach, user_data) : 0;
	ret |= (right) ? _s_bs_tree_depth_post(right, foreach, user_data) : 0;
	ret |= foreach(m_s_bs_tree_get_data(tree), user_data);

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
static int _s_bs_tree_depth_in(struct s_bs_tree *tree, t_foreach_func foreach,
	void *user_data)
{
	m_return_val_if_fail(tree, -EINVAL);
	m_return_val_if_fail(foreach, -EINVAL);

	struct s_bs_tree *left = m_s_bs_tree_get_left(tree);
	struct s_bs_tree *right = m_s_bs_tree_get_right(tree);

	int ret = (left) ? _s_bs_tree_depth_post(left, foreach, user_data) : 0;
	ret |= foreach(m_s_bs_tree_get_data(tree), user_data);
	ret |= (right) ? _s_bs_tree_depth_post(right, foreach, user_data) : 0;

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
static int _s_bs_tree_breath(struct s_bs_tree *tree, t_foreach_func foreach,
	void *data)
{
	m_return_val_if_fail(tree, -EINVAL);
	m_return_val_if_fail(foreach, -EINVAL);

	int ret = 0;
	struct s_queue *queue = s_queue_new();
	ret = s_queue_push(queue, tree);

	while (!s_queue_empty(queue)) {
		struct s_bs_tree *tmp = s_queue_pop(queue);
		struct s_bs_tree *left = m_s_bs_tree_get_left(tmp);
		struct s_bs_tree *right = m_s_bs_tree_get_right(tmp);

		ret |= foreach(m_s_bs_tree_get_data(tmp), data);
		ret |= (left) ? s_queue_push(queue, left) : 0;
		ret |= (right) ? s_queue_push(queue, right) : 0;
	}

	s_queue_delete(queue);
	return ret;
}

int s_bs_tree_foreach(struct s_bs_tree *tree, enum e_tree_browse type,
	t_foreach_func foreach, void *user_data)
{
	m_return_val_if_fail(tree, -EINVAL);
	m_return_val_if_fail(foreach, -EINVAL);

	switch (type) {
	case e_tree_depth_pre:
		return _s_bs_tree_depth_pre(tree, foreach, user_data);
	case e_tree_depth_post:
		return _s_bs_tree_depth_post(tree, foreach, user_data);
	case e_tree_depth_in:
		return _s_bs_tree_depth_in(tree, foreach, user_data);
	case e_tree_breath:
		return _s_bs_tree_breath(tree, foreach, user_data);
	}
	return 0;
}
