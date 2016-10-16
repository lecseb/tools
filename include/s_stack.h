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
#ifndef _UTILS_S_STACK_H_
# define _UTILS_S_STACK_H_

/**
 * @brief The stack structure (opaque)
 */
struct s_stack;

/**
 * @brief Allocate a new stack instance
 * @return a valid pointer on success, NULL on error
 */
struct s_stack *s_stack_new(void);

/**
 * @brief Deallocate a stack instance.
 * @param stack[in] : stack to delete
 * @note that function only delete the container, not the user pointer.
 * To do that, use s_stack_delete_full() instead
 */
void s_stack_delete(struct s_stack *stack);

/**
 * @brief Deallocate a stack instance and user pointer too
 * @param stack[in] : stack to delete
 * @param func[in] : delete function associate to the user data
 */
void s_stack_delete_full(struct s_stack *stack, t_destroy_func func);

/**
 * @brief Get the current stack size
 * @param stack[in] : stack to investigate
 * @return a size on success, 0 on error
 */
uint8_t s_stack_empty(const struct s_stack *stack);

/**
 * @brief Remove an element from the stack and return it
 * @param stack[in] : stack to modify
 * @return a data pointer on success, NULL on error
 */
void *s_stack_pop(struct s_stack *stack);

/**
 * @brief Add an element data into the stack
 * @param stack[in] : stack to modify
 * @param data[in] : data to push
 * @return 0 on success, -errno on error
 */
int s_stack_push(struct s_stack *stack, void *data);

#endif /* !_UTILS_S_STACK_H_ */
