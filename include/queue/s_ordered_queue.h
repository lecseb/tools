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
#ifndef _TOOLS_INCLUDE_QUEUE_S_ORDERED_QUEUE_H_
# define _TOOLS_INCLUDE_QUEUE_S_ORDERED_QUEUE_H_

# include <stdint.h>
# include "m_export.h"
# include "t_funcs.h"

/**
 * @brief Define the type of ordering inside the queue
 */
export enum e_ordered {
	e_ordered_increase,
	e_ordered_decrease
};

/**
 * @brief The ordered queue structure (opaque)
 */
export struct s_ordered_queue;

/**
 * @brief Allocate a new ordered queue instance
 * @return a valid pointer on success, NULL on error
 */
export struct s_ordered_queue *s_ordered_queue_new(enum e_ordered ordering);

/**
 * @brief Deallocate an ordered queue instance.
 * @param queue[in] : queue to delete
 * @note that function only delete the container, not the user pointer.
 * To do that, use s_ordered_queue_delete_full() instead
 */
export void s_ordered_queue_delete(struct s_ordered_queue *queue);

/**
 * @brief Deallocate an ordered queue instance and user pointer too
 * @param queue[in] : queue to delete
 * @param func[in] : delete function associate to the user data
 */
export void s_ordered_queue_delete_full(struct s_ordered_queue *queue,
	t_destroy_func func);

/**
 * @brief Get the current queue size
 * @param queue[in] : queue to investigate
 * @return a 0 if the queue contained element, 1 all other case
 */
export uint8_t s_ordered_queue_empty(const struct s_ordered_queue *queue);

/**
 * @brief Remove an element from the queue and return it
 * @param queue[in] : queue to modify
 * @param cmp[in] : comparison operator
 * @return a data pointer on success, NULL on error
 */
export void *s_ordered_queue_pop(struct s_ordered_queue *queue,
	t_compare_func cmp);

/**
 * @brief Add an element data into the queue
 * @param queue[in] : queue to modify
 * @param cmp[in] : comparison operator
 * @param data[in] : data to push
 * @return 0 on success, -errno on error
 */
export int s_ordered_queue_push(struct s_ordered_queue *queue,
	t_compare_func cmp, void *data);

#endif /* !_TOOLS_INCLUDE_QUEUE_S_ORDERED_QUEUE_H_ */
