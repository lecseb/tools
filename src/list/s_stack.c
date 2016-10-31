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
#include "list/s_list.h"
#include "list/s_stack.h"
#include "m_alloc.h"
#include "m_utils.h"

struct s_stack {
	struct s_list *list;
};

struct s_stack *s_stack_new(void)
{
	struct s_stack *new = _malloc(sizeof(struct s_stack));
	return new;
}

void s_stack_delete(struct s_stack *stack)
{
	m_return_if_fail(stack);

	s_list_delete(stack->list);
	_free(stack);
}

void s_stack_delete_full(struct s_stack *stack, t_destroy_func func)
{
	m_return_if_fail(stack);
	m_return_if_fail(func);

	s_list_delete_full(stack->list, func);
	_free(stack);
}

uint8_t s_stack_empty(const struct s_stack *stack)
{
	m_return_val_if_fail(stack, 1);

	return stack->list == NULL;
}

void *s_stack_pop(struct s_stack *stack)
{
	m_return_val_if_fail(stack, NULL);

	if (!s_stack_empty(stack)) {
		struct s_list *elt = stack->list;
		void *data = m_list_data(elt);
		stack->list = s_list_remove_element(stack->list, elt);
		s_list_delete(elt);
		return data;
	}

	return NULL;
}

int s_stack_push(struct s_stack *stack, void *data)
{
	m_return_val_if_fail(stack, -EINVAL);

	stack->list = s_list_append(stack->list, data);
	return 0;
}
