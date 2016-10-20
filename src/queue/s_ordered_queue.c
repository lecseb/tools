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
#include "queue/s_ordered_queue.h"
#include "tree/s_rb_tree.h"
#include "m_alloc.h"
#include "m_utils.h"

struct s_ordered_queue {
	enum e_ordered ordering;
	struct s_rb_tree *root;
	uint32_t size;
};

struct s_ordered_queue *s_ordered_queue_new(enum e_ordered ordering)
{
	struct s_ordered_queue *queue = _malloc(sizeof(struct s_ordered_queue));
	queue->ordering = ordering;
	return queue;
}

void s_ordered_queue_delete(struct s_ordered_queue *queue)
{
	m_return_if_fail(queue);

	if (queue->root)
		s_rb_tree_delete(queue->root);
	_free(queue);
}

void s_ordered_queue_delete_full(struct s_ordered_queue *queue,
	t_destroy_func func)
{
	m_return_if_fail(queue);

	if (queue->root)
		s_rb_tree_delete_full(queue->root, func);
	_free(queue);
}

uint8_t s_ordered_queue_empty(const struct s_ordered_queue *queue)
{
	m_return_val_if_fail(queue, 1);

	return (queue->size <= 0);
}

void *s_ordered_queue_pop(struct s_ordered_queue *queue, t_compare_func cmp)
{
	m_return_val_if_fail(queue, NULL);
	m_return_val_if_fail(cmp, NULL);
	m_return_val_if_fail(!s_ordered_queue_empty(queue), NULL);

	void *data = NULL;
	switch (queue->ordering) {
	case e_ordered_increase:
		data = s_rb_tree_nth_smallest(queue->root, 1);
		break;
	case e_ordered_decrease:
		data = s_rb_tree_nth_smallest(queue->root, queue->size - 1);
		break;
	}

	if (s_rb_tree_exist(queue->root, cmp, data) == 0) {
		queue->root = s_rb_tree_remove(queue->root, cmp, NULL, data);
		queue->size--;
	}

	return data;
}

int s_ordered_queue_push(struct s_ordered_queue *queue, t_compare_func cmp,
	void *data)
{
	m_return_val_if_fail(queue, -EINVAL);
	m_return_val_if_fail(cmp, -EINVAL);

	queue->root = s_rb_tree_add(queue->root, cmp, data);
	queue->size++;
	return 0;
}
