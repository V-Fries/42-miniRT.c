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

# include "engine.h"
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

typedef struct s_segment
{
	t_vector2f	a;
	t_vector2f	b;
}	t_segment;

typedef struct s_glyph_generated_points
{
	t_vector2f	*points;
	size_t		size;
	size_t		*contours_limits;
	int16_t		nb_of_contours;
}	t_glyph_generated_points;

typedef struct s_bresenham
{
	int				error_x;
	int				error_y;
	int				diff_x;
	int				diff_y;
	int				starting_error_x;
	int				starting_error_y;
	int				x_incr;
	int				y_incr;
	unsigned int	color;
}	t_bresenham;

bool		line_clipping(t_vector2i *start, t_vector2i *end, t_image *img);
void		draw_glyph(t_triangles glyph_triangles, float scale,
				t_image *image, unsigned int color);
void		draw_line(t_vector2i start, t_vector2i end, t_image *img,
				unsigned int color);
void		draw_triangle(t_triangle triangle, t_image *image,
				unsigned int color);

int			get_glyph_points(t_vector *dest, const t_glyph_outline *glyph,
				size_t **end_of_generated_contours);
t_dlist		*get_polygon_from_contours(t_vector points,
				int16_t number_of_contours, const size_t *contours_limits);
t_triangles	triangulate_polygon_and_free_polygon_list(t_dlist *polygon,
				float x_max, t_glyph_generated_points points);

int			get_quadratic_bezier_points(t_vector *dest,
				const t_vector2f *points, size_t number_of_points);

bool		do_segments_intersect(t_vector2f a, t_vector2f b, t_vector2f c,
				t_vector2f d);

#endif
