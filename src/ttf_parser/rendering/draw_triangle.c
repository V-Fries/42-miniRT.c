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

static bool		is_point_in_triangle(t_vector2f point,
					const t_triangle *triangle);
static float	cross_product(t_vector2f v1, t_vector2f v2);

void	draw_triangle(const t_triangle triangle, t_image *image,
			const unsigned int color)
{
	const int		x_limit = roundf(fmaxf(triangle.a.x, fmaxf(triangle.b.x, triangle.c.x)));
	const size_t	y_limit = roundf(fmaxf(triangle.a.y, fmaxf(triangle.b.y, triangle.c.y)));
	const int		x_start = roundf(fminf(triangle.a.x, fminf(triangle.b.x,
			triangle.c.x)));
	const unsigned int	*image_limit = image->address + image->width * image->height;
	int				x;
	size_t			y;

	y = roundf(fminf(triangle.a.y, fminf(triangle.b.y, triangle.c.y))) - 1;
	while (++y <= y_limit)
	{
		x = x_start - 1;
		while (++x <= x_limit)
		{
			unsigned int *dst = image->address + (y + 100) * image->width + (x + 400);
			if (dst < image->address)
				continue ;
			if (dst >= image_limit)
				return ;
			if (is_point_in_triangle((t_vector2f){x + 0.5f, y + 0.5f}, &triangle))
				*dst = color;
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

	return ((ap >= -0.1f && bp >= -0.1f && cp >= -0.1f) || (ap <= 0.1 && bp <= 0.1 && cp <= 0.1));
//	return ((ap >= 0 && bp >= 0 && cp >= 0) || (ap <= 0 && bp <= 0 && cp <= 0));
}

static float	cross_product(t_vector2f v1, t_vector2f v2)
{
	return (v1.x * v2.y - v1.y * v2.x);
}
