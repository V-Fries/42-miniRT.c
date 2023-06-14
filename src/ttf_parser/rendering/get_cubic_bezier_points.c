/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_bezier_points.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 01:15:00 by vfries            #+#    #+#             */
/*   Updated: 2023/06/13 01:15:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "math/vector.h"

static void			get_lines_direction(const t_vector2f *points,
						t_vector2f *lines_direction);
static t_vector2f	get_point(const t_vector2f *points, float t,
						const t_vector2f *lines_direction);

///
/// \param points assumed to be of size 4
/// \param number_of_points
/// \return
t_vector2f	*get_cubic_bezier_points(const t_vector2f *points,
				const size_t number_of_points)
{
	t_vector2f	*result;
	const float	incrementer_t = 1.f / (float)number_of_points;
	float		t;
	size_t		i;
	t_vector2f	lines_direction[3];

	result = malloc(sizeof(*result) * number_of_points);
	if (result == NULL)
		return (NULL);
	get_lines_direction(points, lines_direction);
	t = 0.f;
	i = -1;
	while (++i < number_of_points)
	{
		result[i] = get_point(points, t, lines_direction);
		t += incrementer_t;
		if (t > 1.f)
			t = 1.f;
	}
	return (result);
}

static void	get_lines_direction(const t_vector2f *points,
				t_vector2f *lines_direction)
{
	lines_direction[0] = vector2f_subtract(points[1], points[0]);
	lines_direction[1] = vector2f_subtract(points[2], points[1]);
	lines_direction[2] = vector2f_subtract(points[3], points[2]);
}

static t_vector2f	get_point(const t_vector2f *points, const float t,
						const t_vector2f *lines_direction)
{
	const t_vector2f	sub_points[3] = {
		vector2f_add(vector2f_multiply(lines_direction[0], t), points[0]),
		vector2f_add(vector2f_multiply(lines_direction[1], t), points[1]),
		vector2f_add(vector2f_multiply(lines_direction[2], t), points[2])
	};
	const t_vector2f	sub_lines_direction[2] = {
		vector2f_subtract(sub_points[1], sub_points[0]),
		vector2f_subtract(sub_points[2], sub_points[1]),
	};
	const t_vector2f	final_line[2] = {
		vector2f_add(vector2f_multiply(sub_lines_direction[0], t),
			sub_points[0]),
		vector2f_add(vector2f_multiply(sub_lines_direction[1], t),
			sub_points[1]),
	};
	const t_vector2f	final_line_direction
		= vector2f_subtract(final_line[1], final_line[0]);

	return (vector2f_add(vector2f_multiply(final_line_direction, t),
			final_line[0]));
}
