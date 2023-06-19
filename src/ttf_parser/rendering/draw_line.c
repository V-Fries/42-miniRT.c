/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 13:57:00 by vfries            #+#    #+#             */
/*   Updated: 2023/06/19 13:57:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>

#include "libft.h"

#include "math/vector.h"
#include "font/render.h"
#include "image.h"

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

static void	draw_low_slope(t_vector2i start, t_bresenham params, t_image *img);
static void	draw_high_slope(t_vector2i start, t_bresenham params, t_image *img);
static void	draw_straight_line(t_vector2i start, t_vector2i end, t_image *img,
				unsigned int color);

void	draw_line(t_vector2i start, t_vector2i end, t_image *img,
			unsigned int color)
{
	t_bresenham	params;

	if (line_clipping(&start, &end, img) == false)
		return ;
	if (start.x == end.x || start.y == end.y)
		return (draw_straight_line(start, end, img, color));
	params.error_x = ft_abs(end.x - start.x);
	params.error_y = ft_abs(end.y - start.y);
	params.diff_x = 2 * params.error_x;
	params.diff_y = 2 * params.error_y;
	params.starting_error_x = params.error_x;
	params.starting_error_y = params.error_y;
	params.x_incr = 1;
	if (start.x > end.x)
		params.x_incr = -1;
	params.y_incr = 1;
	if (start.y > end.y)
		params.y_incr = -1;
	params.color = color;
	if (params.starting_error_x > params.starting_error_y)
		draw_low_slope(start, params, img);
	else
		draw_high_slope(start, params, img);
}


static void	draw_low_slope(t_vector2i start, t_bresenham params, t_image *img)
{
	int	i;

	i = -1;
	while (++i <= params.starting_error_x)
	{
		img->address[start.y * img->width + start.x] = params.color;
		start.x += params.x_incr;
		params.error_x -= params.diff_y;
		if (params.error_x < 0)
		{
			start.y += params.y_incr;
			params.error_x += params.diff_x;
		}
	}
}

static void	draw_high_slope(t_vector2i start, t_bresenham params, t_image *img)
{
	int	i;

	i = -1;
	while (++i <= params.starting_error_y)
	{
		img->address[start.y * img->width + start.x] = params.color;
		start.y += params.y_incr;
		params.error_y -= params.diff_x;
		if (params.error_y < 0)
		{
			start.x += params.x_incr;
			params.error_y += params.diff_y;
		}
	}
}

static void	draw_straight_line(t_vector2i start, t_vector2i end, t_image *img,
				unsigned int color)
{
	if (start.x == end.x)
	{
		if (start.y > end.y)
			return (draw_straight_line(end, start, img, color));
		while (start.y <= end.y)
			img->address[start.y++ * img->width + start.x] = color;
	}
	else
	{
		if (start.x > end.x)
			return (draw_straight_line(end, start, img, color));
		while (start.x <= end.x)
			img->address[start.y * img->width + start.x++] = color;
	}
}
