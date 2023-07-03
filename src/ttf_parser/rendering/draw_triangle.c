/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_triangle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 14:42:00 by vfries            #+#    #+#             */
/*   Updated: 2023/06/25 14:42:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include "font/render.h"

#define PIXEL_DIVISION 8.f

static bool		is_point_in_triangle(t_vector2f point,
					const t_triangle *triangle);
static float	cross_product(t_vector2f v1, t_vector2f v2);

void	draw_triangle(const t_triangle triangle, t_image *image,
			const unsigned int color)
{
	const int		x_limit = roundf(fmaxf(triangle.a.x, fmaxf(triangle.b.x, triangle.c.x)));
	const int 		y_limit = roundf(fmaxf(triangle.a.y, fmaxf(triangle.b.y, triangle.c.y)));
	const int		x_start = roundf(fminf(triangle.a.x, fminf(triangle.b.x,
			triangle.c.x)));
	const unsigned int	*image_limit = image->address + image->width * image->height;
	int				x;
	int				y;
	const t_color	color_vec = get_t_color_from_uint(color);

	y = roundf(fminf(triangle.a.y, fminf(triangle.b.y, triangle.c.y))) - 1;
	while (++y <= y_limit)
	{
		x = x_start - 1;
		while (++x <= x_limit)
		{
			unsigned int *dst = image->address + y * image->width + x;
			if (dst < image->address)
				continue ;
			if (dst >= image_limit)
				return ;
			int	nb_of_points_in_triangle = 0;
			for (int sub_y = 0; sub_y < PIXEL_DIVISION; sub_y++)
				for (int sub_x = 0; sub_x < PIXEL_DIVISION; sub_x++)
					nb_of_points_in_triangle += is_point_in_triangle(
						(t_vector2f){x + sub_x / PIXEL_DIVISION + 1.f / PIXEL_DIVISION / 2,
									 y + sub_y / PIXEL_DIVISION + 1.f / PIXEL_DIVISION / 2}, &triangle);
			t_color new_color = vector3f_multiply(color_vec, nb_of_points_in_triangle);
			new_color = vector3f_add(new_color, vector3f_multiply(get_t_color_from_uint(*dst), PIXEL_DIVISION * PIXEL_DIVISION - nb_of_points_in_triangle));
			new_color = vector3f_divide(new_color, PIXEL_DIVISION * PIXEL_DIVISION);
			*dst = rgb_to_uint(new_color);
		}
	}
}

static bool	is_point_in_triangle(t_vector2f point, const t_triangle *triangle)
{
	const float	ap = cross_product(vector2f_subtract(triangle->b, triangle->a),
			vector2f_subtract(point, triangle->a));
	const float	bp = cross_product(vector2f_subtract(triangle->c, triangle->b),
			vector2f_subtract(point, triangle->b));
	const float	cp = cross_product(vector2f_subtract(triangle->a, triangle->c),
			vector2f_subtract(point, triangle->c));

	return ((ap >= -0.1f && bp >= -0.1f && cp >= -0.1f) || (ap <= 0.1f && bp <= 0.1f && cp <= 0.1f));
//	return ((ap >= 0.f && bp >= 0.f && cp >= 0.f) || (ap <= 0.f && bp <= 0.f && cp <= 0.f));
}

static float	cross_product(t_vector2f v1, t_vector2f v2)
{
	return (v1.x * v2.y - v1.y * v2.x);
}
