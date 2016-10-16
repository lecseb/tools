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
#ifndef _TOOLS_INCLUDE_S_QUEUE_H_
# define _TOOLS_INCLUDE_S_QUEUE_H_

# include <stdint.h>
# include "m_export.h"
# include "t_callback.h"

/**
 * @brief The queue structure (opaque)
 */
export struct s_queue;

/**
 * @brief Allocate a new queue instance
 * @return a valid pointer on success, NULL on error
 */
export struct s_queue *s_queue_new(void);

/**
 * @brief Deallocate a queue instance.
 * @param queue[in] : queue to delete
 * @note that function only delete the container, not the user pointer.
 * To do that, use s_queue_delete_full() instead
 */
export void s_queue_delete(struct s_queue *queue);

/**
 * @brief Deallocate a queue instance and user pointer too
 * @param queue[in] : queue to delete
 * @param func[in] : delete function associate to the user data
 */
export void s_queue_delete_full(struct s_queue *queue, t_destroy_func func);

/**
 * @brief Get the current queue size
 * @param queue[in] : queue to investigate
 * @return a size on success, 0 on error
 */
export uint8_t s_queue_empty(const struct s_queue *queue);

/**
 * @brief Remove an element from the queue and return it
 * @param queue[in] : queue to modify
 * @return a data pointer on success, NULL on error
 */
export void *s_queue_pop(struct s_queue *queue);

/**
 * @brief Add an element data into the queue
 * @param queue[in] : queue to modify
 * @param data[in] : data to push
 * @return 0 on success, -errno on error
 */
export int s_queue_push(struct s_queue *queue, void *data);

#endif /* !_TOOLS_INCLUDE_S_QUEUE_H_ */
