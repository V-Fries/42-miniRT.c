/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_convert_to_array.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 22:11:00 by vfries            #+#    #+#             */
/*   Updated: 2023/06/14 22:11:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "ft_vector.h"
#include "ft_mem.h"

static void	*make_copy_of_vector(const t_vector *vector, size_t *size);

void	*ft_vector_convert_to_array(const t_vector *vector, size_t *size,
			const bool make_copy)
{
	if (make_copy)
		return (make_copy_of_vector(vector, size));
	if (size != NULL)
		*size = vector->length;
	return (vector->data);
}

static void	*make_copy_of_vector(const t_vector *vector, size_t *size)
{
	void	*copy;

	if (size != NULL)
		*size = vector->length;
	copy = malloc(vector->length * vector->elem_size);
	if (copy == NULL)
		return (NULL);
	ft_memcpy(copy, vector->data, vector->length);
	return (copy);
}
