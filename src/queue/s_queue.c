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
#include "queue/s_queue.h"
#include "list/s_d_list.h"
#include "m_alloc.h"
#include "m_utils.h"

/**
 * @brief The queue structure
 */
struct s_queue {
	struct s_d_list *list;
	struct s_d_list *tail;
};

struct s_queue *s_queue_new(void)
{
	struct s_queue *new = _malloc(sizeof(struct s_queue));
	return new;
}

void s_queue_delete(struct s_queue *queue)
{
	m_return_if_fail(queue);

	if (queue->list)
		s_d_list_delete(queue->list);
	_free(queue);
}

void s_queue_delete_full(struct s_queue *queue, t_destroy_func func)
{
	m_return_if_fail(queue);
	m_return_if_fail(func);

	s_d_list_delete_full(queue->list, func);
	_free(queue);
}

uint8_t s_queue_empty(const struct s_queue *queue)
{
	m_return_val_if_fail(queue, 0);

	return queue->list == NULL;
}

void *s_queue_pop(struct s_queue *queue)
{
	m_return_val_if_fail(queue, NULL);

	struct s_d_list *tail = queue->tail;
	if (tail) {
		void *data = m_d_list_data(tail);
		queue->tail = m_d_list_previous(queue->tail);
		queue->list = s_d_list_remove_element(queue->list, tail);
		s_d_list_delete(tail);
		return data;
	}
	return NULL;
}

int s_queue_push(struct s_queue *queue, void *data)
{
	m_return_val_if_fail(queue, -EINVAL);
	m_return_val_if_fail(data, -EINVAL);

	struct s_d_list *new = s_d_list_prepend(queue->list, data);
	if (new != queue->list) {
		if (!queue->list)
			queue->tail = new;
		queue->list = new;
		return 0;
	}
	return -EAGAIN;
}
