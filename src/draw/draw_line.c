/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 10:05:00 by tdameros          #+#    #+#             */
/*   Updated: 2023/09/17 10:05:00 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include "engine.h"

static void	put_pixel(t_raytraced_pixels *pixels, t_vector2i point,
				t_vector3f color);
static void	put_line_dx_greater(t_raytraced_pixels *pixels,
				t_vector2i point1, t_vector2i point2, t_vector3f color);
static void	put_line_dy_greater(t_raytraced_pixels *pixels,
				t_vector2i point1, t_vector2i point2, t_vector3f color);

void	draw_line(t_raytraced_pixels *pixels,
				t_vector2i point1, t_vector2i point2,
				t_vector3f color)
{
	double	dx;
	double	dy;

	if (point1.x < 0 || point2.x < 0)
		return ;
	dx = fabs((double) point2.x - point1.x);
	dy = fabs((double) point2.y - point1.y);
	if (dx >= dy)
		put_line_dx_greater(pixels, point1, point2, color);
	else
		put_line_dy_greater(pixels, point1, point2, color);
}

static void	put_line_dx_greater(t_raytraced_pixels *pixels,
								t_vector2i point1, t_vector2i point2,
								t_vector3f color)
{
	const double	dx = fabs((double) point2.x - point1.x);
	const double	dy = fabs((double) point2.y - point1.y);
	const double	m = dy / dx;
	double			e;
	int				i;

	i = 0;
	e = 0;
	while (i++ <= dx)
	{
		put_pixel(pixels, point1, color);
		if (point1.x > point2.x)
			point1.x--;
		else
			point1.x++;
		e -= m;
		if (e <= -0.5)
		{
			if (point1.y > point2.y)
				point1.y--;
			else
				point1.y++;
			e += 1;
		}
	}
}

static void	put_line_dy_greater(t_raytraced_pixels *pixels,
								t_vector2i point1, t_vector2i point2,
								t_vector3f color)
{
	const double	dx = fabs((double) point2.x - point1.x);
	const double	dy = fabs((double) point2.y - point1.y);
	const double	m = dx / dy;
	double			e;
	int				i;

	i = 0;
	e = 0;
	while (i++ <= dy)
	{
		put_pixel(pixels, point1, color);
		if (point1.y > point2.y)
			point1.y--;
		else
			point1.y++;
		e -= m;
		if (e <= -0.5)
		{
			if (point1.x > point2.x)
				point1.x--;
			else
				point1.x++;
			e += 1;
		}
	}
}

static void	put_pixel(t_raytraced_pixels *pixels, t_vector2i point,
					t_vector3f color)
{
	if (point.x <= 0 || point.x >= pixels->width)
		return ;
	if (point.y <= 0 || point.y >= pixels->height)
		return ;
	pixels->data[point.x + point.y * pixels->width] = color;
}


