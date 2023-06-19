/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_glyph_points.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 17:59:00 by vfries            #+#    #+#             */
/*   Updated: 2023/06/14 17:59:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"

#include "font/ttf_parser.h"
#include "font/render.h"
#include "math/vector.h"

#define NB_OF_POINT_ON_CURVE_TO_GENERATE 10000

t_vector2f	*get_glyph_points(size_t *result_size, const t_glyph_outline *glyph,
				size_t **end_of_generated_contours)
{
	int16_t			contour_index;
	uint16_t		i;
	t_vector		result;
	t_vector		current_points;
	t_vector2f		tmp[3];

	*end_of_generated_contours = malloc(sizeof(**end_of_generated_contours) * glyph->numberOfContours); // TODO secure
	ft_vector_create(&result, sizeof(t_vector2f), 0);
	ft_vector_create(&current_points, sizeof(t_vector2f), 0);
	i = -1;
	contour_index = -1;
	while (++contour_index < glyph->numberOfContours)
	{
		uint16_t	contour_start_index = i + 1;
		bool		first_elem_of_contour = true;
		bool		contour_has_only_on_curve_points = true;

		while (++i < glyph->endPtsOfContours[contour_index])
		{
			uint16_t	next_index = (i + 1 - contour_start_index)
				% (glyph->endPtsOfContours[contour_index] - contour_start_index)
				+ contour_start_index;
			if (glyph->flags[i].on_curve)
			{
				tmp[0] = (t_vector2f){glyph->xCoordinates[i], glyph->yCoordinates[i]};
				ft_vector_add_elem(&current_points, tmp); // TODO secure
				continue ;
			}
			if (first_elem_of_contour)
			{
				contour_has_only_on_curve_points = false;
				if (glyph->flags[next_index].on_curve)
				{
					tmp[0] = (t_vector2f){glyph->xCoordinates[i], glyph->yCoordinates[i]};
					ft_vector_add_elem(&current_points, tmp); // TODO secure
					continue ;
				}
				tmp[0] = (t_vector2f){
					.x = glyph->xCoordinates[i] + (glyph->xCoordinates[next_index] - glyph->xCoordinates[i]) / 2.f,
					.y = glyph->yCoordinates[i] + (glyph->yCoordinates[next_index] - glyph->yCoordinates[i]) / 2.f};
				ft_vector_add_elem(&current_points, tmp); // TODO secure
				first_elem_of_contour = false;
			}
			tmp[0] = ((t_vector2f *)current_points.data)[current_points.length - 1];
			tmp[1] = (t_vector2f){glyph->xCoordinates[i], glyph->yCoordinates[i]};
			tmp[2] = (t_vector2f){glyph->xCoordinates[next_index], glyph->yCoordinates[next_index]};

			if (glyph->flags[next_index].on_curve == false)
				vector2f_add(tmp[1], vector2f_divide(vector2f_subtract(tmp[2], tmp[1]), 2.f));
			else
				i++;
			get_quadratic_bezier_points(&current_points, tmp, NB_OF_POINT_ON_CURVE_TO_GENERATE); // TODO secure
		}

		if (glyph->flags[i-1].on_curve)
			ft_vector_add_elem(&current_points, current_points.data); // TODO secure

		if (contour_has_only_on_curve_points == false)
		{
			//handle first point if its off
			tmp[0] = ((t_vector2f *)current_points.data)[current_points.length - 1];
			tmp[1] = (t_vector2f){.x = glyph->xCoordinates[contour_start_index], .y = glyph->yCoordinates[contour_start_index]};
			tmp[2] = ((t_vector2f *)current_points.data)[0];
			get_quadratic_bezier_points(&current_points, tmp, NB_OF_POINT_ON_CURVE_TO_GENERATE); // TODO secure
		}
		ft_vector_append(&result, &current_points); // TODO secure
		current_points.length = 0;
		(*end_of_generated_contours)[contour_index] = result.length;
	}
	ft_vector_destroy(&current_points);
	return (ft_vector_convert_to_array(&result, result_size, false, false));
}
