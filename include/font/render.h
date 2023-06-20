/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 01:51:00 by vfries            #+#    #+#             */
/*   Updated: 2023/06/13 01:51:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include <stddef.h>

# include "math/vector.h"
# include "font/ttf_parser.h"
# include "image.h"

typedef struct s_triangle
{
	t_vector2f	a;
	t_vector2f	b;
	t_vector2f	c;
}	t_triangle;

typedef struct s_triangles
{
	t_triangle	*data;
	size_t		size;
}	t_triangles;

bool		line_clipping(t_vector2i *start, t_vector2i *end, t_image *img);
void		draw_line(t_vector2i start, t_vector2i end, t_image *img,
				unsigned int color);

int			get_glyph_points(t_vector *dest, const t_glyph_outline *glyph,
				size_t **end_of_generated_contours);
t_list		*get_polygon_from_contours(t_vector points,
				int16_t number_of_contours, const size_t *contours_limits);

int			get_quadratic_bezier_points(t_vector *dest,
				const t_vector2f *points, size_t number_of_points);
int			get_cubic_bezier_points(t_vector *dest, const t_vector2f *points,
				size_t number_of_points);

#endif
