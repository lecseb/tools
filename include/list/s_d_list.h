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
#ifndef _TOOLS_INCLUDE_LIST_S_D_LIST_H_
# define _TOOLS_INCLUDE_LIST_S_D_LIST_H_

# include <stdint.h>
# include "m_export.h"
# include "t_funcs.h"

/**
 * @brief The list struct is used for each element in a doubly-linked list
 * @param data : holds the element's data, which can be a pointer to any kind of
 * data, or any integer value using the Type Conversion Macros?
 * @param previous : contains the link to the previous element in the list
 * @param next : contains the link to the next element in the list
 */
export struct s_d_list {
	void *data;
	struct s_d_list *prev;
	struct s_d_list *next;
};

/**
 * @brief A convenience macro to get the previous element in a list.
 */
# define m_d_list_previous(list) ((list) ? (list)->prev : NULL)

/**
 * @brief A convenience macro to get the next element in a list.
 */
# define m_d_list_next(list) ((list) ? (list)->next : NULL)

/**
 * @brief A convenience macro to get the data in an element.
 */
# define m_d_list_data(list) ((list) ? (list)->data : NULL)

/**
 * @brief Frees all of the memory used by a s_d_list. If list elements contain
 * dynamically-allocated memory, you should use s_d_list_free_full() or
 * free them manually first.
 * @param list[out] : list instance
 */
export void s_d_list_delete(struct s_d_list *list);

/**
 * @brief Convenience method, which frees all the memory used by a list, and
 * calls func on every element's data.
 * @param list[out] : list instance
 * @param func[in] : function pointer
 */
export void s_d_list_delete_full(struct s_d_list *list, t_destroy_func func);

/**
 * @brief Adds a new element at the end of the list. Note that the return
 * value is the new start of the list, if list was empty; make sure you store
 * the new value. s_d_list_append() has to traverse the entire list to find the
 * end, which is inefficient when adding multiple elements.
 * @param list[in] : list instance
 * @param data[in] : the data for the new element
 * @return either list or the new start of the s_d_list if list was NULL.
 */
export struct s_d_list *s_d_list_append(struct s_d_list *list, void *data);

/**
 * @brief Prepends a new element at the start of the list. Note that the
 * return value is the new start of the list.
 * @param list[in] : list instance
 * @param data[in]  the data for the new element
 * @return a pointer to the newly prepended element, which is the new start of
 * the list
 */
export struct s_d_list *s_d_list_prepend(struct s_d_list *list, void *data);

/**
 * @brief Inserts a new element into the list at the given position.
 * @param list[in] : list instance
 * @param data[in] : the data for the new element
 * @param position[in] : the position to insert the element. If this is
 * negative, or is larger than the number of elements in the list, the new
 * element is added on to the end of the list.
 * @return the (possibly changed) start of the list
 */
export struct s_d_list *s_d_list_insert(struct s_d_list *list, void *data,
	uint32_t position);

/**
 * @brief Removes an element from a list. If two elements contain the same
 * data, only the first is removed. If none of the elements contain the data,
 * the list is unchanged.
 * @param list[in] : list instance
 * @param data[in] : the data for the element to remove
 * @return the (possibly changed) start of the list
 */
export struct s_d_list *s_d_list_remove(struct s_d_list *list, void *data);

/**
 * @brief Removes all list nodes with data equal to data . Returns the new head
 * of the list.
 * @param list[in] : list instance
 * @param data[in] : data to remove
 * @return the (possibly changed) start of the list
 */
export struct s_d_list *s_d_list_remove_all(struct s_d_list *list, void *data);

/**
 * @brief Remove the link between the element and the list
 * @param list[in] : list to modify
 * @param elt[in] : element to remove
 * @return the (possibly changed) start of the list
 */
export struct s_d_list *s_d_list_remove_element(struct s_d_list *list,
	struct s_d_list *elt);

/**
 * @brief Gets the number of elements in a list.
 * @param list[in] : list instance
 * @return the number of elements in the list
 */
export uint32_t s_d_list_size(struct s_d_list *list);

/**
 * @brief Copies a list. This function only copy the list pointer and create a
 * new image of the previous list. To copy data contained too, use
 * s_d_list_copy_deep()
 * @param list[in] : list instance
 * @return the start of the new list that holds the same data as list
 */
export struct s_d_list *s_d_list_copy(struct s_d_list *list);

/**
 * @brief Makes a full copy of a list.
 * @param list[in] : list instance
 * @param func[in] : the function which is called to copy the data inside each
 * node, or NULL to use the original data.
 * @return the start of the new list that holds the same data as list
*/
export struct s_d_list *s_d_list_deep_copy(struct s_d_list *list,
	t_copy_func func);

/**
 * @brief Adds the second list onto the end of the first list. Note that the
 * elements of the second list are not copied. They are used directly.
 * @param list1[in] : a list, this must point to the top of the list
 * @param list2[in] : the list to add to the end of the first list, this must
 * point to the top of the list
 * @return the start of the new list, which equals list1 if not NULL
 */
export struct s_d_list *s_d_list_concat(struct s_d_list *list1,
	struct s_d_list *list2);

/**
 * @brief Iterate over elements contained into the list
 * @param list[in] : a list
 * @param func[in] : the function to call with each element's data
 * @param user_data[in] : user data to pass to the function
 */
export void s_d_list_foreach(struct s_d_list *list, t_foreach_func func,
	void *user_data);

/**
 * @brief Get the first element in a list
 * @param list[in] : a list
 * @return the first element in the list, or NULL if the list has no elements
 */
export struct s_d_list *s_d_list_first(struct s_d_list *list);

/**
 * @brief Get the last element in a list
 * @param list[in] : a list
 * @return the last element in the list, or NULL if the list has no elements
 */
export struct s_d_list *s_d_list_last(struct s_d_list *list);

/**
 * @brief Gets the element at the given position in a list.
 * @param list[in] : a list, this must be the top of the list
 * @param nth[in] : the position of the element, counting from 0
 * @return the element, or NULL if the position is off the end of the list
 */
export struct s_d_list *s_d_list_get_nth(struct s_d_list *list, uint32_t nth);

/**
 * @brief Finds the element in a list which contains the given data.
 * @param list[in] : a list
 * @param data[in] : the element data to find
 * @return the found list element, or NULL if it is not found
 */
export struct s_d_list *s_d_list_find(struct s_d_list *list, void *data);

#endif /* !_TOOLS_INCLUDE_LIST_S_D_LIST_H_ */
