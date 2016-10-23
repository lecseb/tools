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
#include "m_alloc.h"
#include "m_utils.h"

/**
 * @brief Allocates space for element. It is called by
 * s_list_append(), s_list_prepend(), s_list_insert()
 * @param data[in] : element data
 * @param next[in] : next list element
 * @return a pointer to the newly-allocated list element
 */
static struct s_list *_s_list_new(void *data,
	struct s_list *next)
{
	struct s_list *list = _malloc(sizeof(struct s_list));
	list->data = data;
	list->next = next;

	return list;
}

void s_list_delete(struct s_list *list)
{
	m_return_if_fail(list);

	if (m_s_list_next(list))
		s_list_delete(m_s_list_next(list));
	_free(list);
}

void s_list_delete_full(struct s_list *list, t_destroy_func func)
{
	m_return_if_fail(list);
	m_return_if_fail(func);

	s_list_foreach(list, (t_foreach_func)func, NULL);
	s_list_delete(list);
}

struct s_list *s_list_append(struct s_list *list, void *data)
{
	struct s_list *last = NULL, *new_list = _s_list_new(data, NULL);

	if (list) {
		last = s_list_last(list);
		last->next = new_list;
		return list;
	} else {
		return new_list;
	}
}

struct s_list *s_list_prepend(struct s_list *list, void *data)
{
	return _s_list_new(data, list);
}

struct s_list *s_list_insert(struct s_list *list, void *data,
	uint32_t position)
{
	struct s_list *new_list = NULL, *tmp_list = NULL;

	if (position == 0)
		return s_list_append(list, data);

	tmp_list = s_list_get_nth(list, position - 1);
	if (!tmp_list)
		return s_list_append(list, data);

	new_list = _s_list_new(data, tmp_list->next);
	tmp_list->next = new_list;

	return list;
}


struct s_list *s_list_remove_element(struct s_list *list,
	struct s_list *element)
{
	m_return_val_if_fail(element, list);

	struct s_list *tmp = list;
	while (tmp->next != element)
		tmp = m_s_list_next(tmp);

	if (tmp) {
		tmp->next = element->next;
		element->next = NULL;
	}

	return list;
}


struct s_list *s_list_remove(struct s_list *list, void *data)
{
	struct s_list *tmp = list;

	while (tmp) {
		if (tmp->data != data) {
			tmp = tmp->next;
		} else {
			list = s_list_remove_element(list, tmp);
			_free(tmp);
			break;
		}
	}
	return list;
}

struct s_list *s_list_remove_all(struct s_list *list, void *data)
{
	struct s_list *tmp = list;

	while (tmp) {
		if (tmp->data != data) {
			tmp = tmp->next;
		} else {
			struct s_list *next = tmp->next;
			list = next;
			_free(tmp);
			tmp = next;
		}
	}
	return list;
}

uint32_t s_list_size(struct s_list *list)
{
	uint32_t length = 0;

	while (list) {
		length++;
		list = list->next;
	}
	return length;
}

struct s_list *s_list_copy(struct s_list *list)
{
	return s_list_deep_copy(list, NULL);
}

struct s_list *s_list_deep_copy(struct s_list *list, t_copy_func func)
{
	struct s_list *new_list = NULL, *last = NULL;

	if (list) {
		new_list = _malloc(sizeof(struct s_list));
		if (func)
			new_list->data = func(list->data);
		else
			new_list->data = list->data;
		last = new_list;
		list = list->next;
		while (list) {
			last->next = _malloc(sizeof(struct s_list));
			last = last->next;
			if (func)
				last->data = func(list->data);
			else
				last->data = list->data;
			list = list->next;
		}
		last->next = NULL;
	}
	return new_list;
}

struct s_list *s_list_concat(struct s_list *list1, struct s_list *list2)
{
	struct s_list *tmp_list;
	if (list2) {
		tmp_list = s_list_last(list1);
		if (tmp_list)
			tmp_list->next = list2;
		else
			list1 = list2;
	}
	return list1;
}

struct s_list *s_list_find(struct s_list *list, void *data)
{
	while (list) {
		if (list->data == data)
			break;
		list = list->next;
	}
	return list;
}

void s_list_foreach(struct s_list *list, t_foreach_func func,
	void *user_data)
{
	while (list) {
		struct s_list *next = list->next;
		(*func)(list->data, user_data);
		list = next;
	}
}

struct s_list *s_list_last(struct s_list *list)
{
	if (list) {
		while (list->next)
			list = list->next;
	}
	return list;
}

struct s_list *s_list_get_nth(struct s_list *list, uint32_t nth)
{
	while ((nth-- > 0) && list)
		list = list->next;
	return list;
}
