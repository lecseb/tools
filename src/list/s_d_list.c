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
#include "list/s_d_list.h"
#include "m_alloc.h"
#include "m_utils.h"

/**
 * @brief Allocates space for element. It is called by
 * s_d_list_append(), s_d_list_prepend(), s_d_list_insert()
 * @param prev[in] : previous list element
 * @param data[in] : element data
 * @param next[in] : next list element
 * @return a pointer to the newly-allocated list element
 */
static struct s_d_list *_s_d_list_new(struct s_d_list *prev, void *data,
	struct s_d_list *next)
{
	struct s_d_list *list = _malloc(sizeof(struct s_d_list));
	list->data = data;
	list->prev = prev;
	list->next = next;

	return list;
}

void s_d_list_delete(struct s_d_list *list)
{
	m_return_if_fail(list);

	if (s_d_list_next(list))
		s_d_list_delete(s_d_list_next(list));
	_free(list);
}

void s_d_list_delete_full(struct s_d_list *list, t_destroy_func func)
{
	m_return_if_fail(list);
	m_return_if_fail(func);

	s_d_list_foreach(list, (t_foreach_func)func, NULL);
	s_d_list_delete(list);
}

struct s_d_list *s_d_list_append(struct s_d_list *list, void *data)
{
	struct s_d_list *last = NULL, *new_list = _s_d_list_new(NULL, data,
		NULL);

	if (list) {
		last = s_d_list_last(list);
		last->next = new_list;
		new_list->prev = last;
		return list;
	} else {
		new_list->prev = NULL;
		return new_list;
	}
}

struct s_d_list *s_d_list_prepend(struct s_d_list *list, void *data)
{
	struct s_d_list *new_list = _s_d_list_new(NULL, data, list);

	if (list) {
		new_list->prev = list->prev;
		if (list->prev)
			list->prev->next = new_list;
		list->prev = new_list;
	} else {
		new_list->prev = NULL;
	}
	return new_list;
}

struct s_d_list *s_d_list_insert(struct s_d_list *list, void *data,
	uint32_t position)
{
	struct s_d_list *new_list = NULL, *tmp_list = NULL;

	if (position == 0)
		return s_d_list_append(list, data);

	tmp_list = s_d_list_get_nth(list, position);
	if (!tmp_list)
		return s_d_list_append(list, data);

	new_list = _s_d_list_new(tmp_list->prev, data, tmp_list);
	tmp_list->prev->next = new_list;
	tmp_list->prev = new_list;

	return list;
}


struct s_d_list *s_d_list_remove_element(struct s_d_list *list,
	struct s_d_list *element)
{
	m_return_val_if_fail(element, list);

	if (element->prev)
		if (element->prev->next == element)
			element->prev->next = element->next;
	if (element->next)
		if (element->next->prev == element)
			element->next->prev = element->prev;

	if (element == list)
		list = list->next;

	element->next = NULL;
	element->prev = NULL;

	return list;
}


struct s_d_list *s_d_list_remove(struct s_d_list *list, void *data)
{
	struct s_d_list *tmp = list;

	while (tmp) {
		if (tmp->data != data) {
			tmp = tmp->next;
		} else {
			list = s_d_list_remove_element(list, tmp);
			_free(tmp);
			break;
		}
	}
	return list;
}

struct s_d_list *s_d_list_remove_all(struct s_d_list *list, void *data)
{
	struct s_d_list *tmp = list;

	while (tmp) {
		if (tmp->data != data) {
			tmp = tmp->next;
		} else {
			struct s_d_list *next = tmp->next;

			if (tmp->prev)
				tmp->prev->next = next;
			else
				list = next;
			if (next)
				next->prev = tmp->prev;

			_free(tmp);
			tmp = next;
		}
	}
	return list;
}

uint32_t s_d_list_size(struct s_d_list *list)
{
	uint32_t length = 0;

	while (list) {
		length++;
		list = list->next;
	}
	return length;
}

struct s_d_list *s_d_list_copy(struct s_d_list *list)
{
	return s_d_list_deep_copy(list, NULL);
}

struct s_d_list *s_d_list_deep_copy(struct s_d_list *list, t_copy_func func)
{
	struct s_d_list *new_list = NULL, *last = NULL;

	if (list) {
		new_list = _malloc(sizeof(struct s_d_list));
		if (func)
			new_list->data = func(list->data);
		else
			new_list->data = list->data;
		new_list->prev = NULL;
		last = new_list;
		list = list->next;
		while (list) {
			last->next = _malloc(sizeof(struct s_d_list));
			last->next->prev = last;
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

struct s_d_list *s_d_list_concat(struct s_d_list *list1, struct s_d_list *list2)
{
	struct s_d_list *tmp_list;
	if (list2) {
		tmp_list = s_d_list_last(list1);
		if (tmp_list)
			tmp_list->next = list2;
		else
			list1 = list2;
		list2->prev = tmp_list;
	}
	return list1;
}

struct s_d_list *s_d_list_find(struct s_d_list *list, void *data)
{
	while (list) {
		if (list->data == data)
			break;
		list = list->next;
	}
	return list;
}

void s_d_list_foreach(struct s_d_list *list, t_foreach_func func,
	void *user_data)
{
	while (list) {
		struct s_d_list *next = list->next;
		(*func)(list->data, user_data);
		list = next;
	}
}

struct s_d_list *s_d_list_first(struct s_d_list *list)
{
	if (list) {
		while (list->prev)
			list = list->prev;
	}
	return list;
}

struct s_d_list *s_d_list_last(struct s_d_list *list)
{
	if (list) {
		while (list->next)
			list = list->next;
	}
	return list;
}

struct s_d_list *s_d_list_get_nth(struct s_d_list *list, uint32_t nth)
{
	while ((nth-- > 0) && list)
		list = list->next;
	return list;
}
