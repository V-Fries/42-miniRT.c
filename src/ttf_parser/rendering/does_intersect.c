/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   does_intersect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 06:23:00 by vfries            #+#    #+#             */
/*   Updated: 2023/06/24 06:23:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>

#include "libft.h"

#include "math/vector.h"
#include "font/render.h"
#include "image.h"

static void	get_start_and_end(t_vector2i *start, t_vector2i *end,
				t_vector2f start_f, t_vector2f end_f,
				t_glyph_outline_bounds bounds);
static bool	draw_low_slope(t_vector2i start, t_bresenham params, t_image *img);
static bool	draw_high_slope(t_vector2i start, t_bresenham params, t_image *img);
static bool	check_straight_line(t_vector2i start, t_vector2i end, t_image *img);

bool	does_intersect(t_vector2f start_f, t_vector2f end_f, t_image *img,
				t_glyph_outline_bounds bounds)
{
	//TODO this is unused
	t_bresenham	params;
	t_vector2i	start;
	t_vector2i	end;

	get_start_and_end(&start, &end, start_f, end_f, bounds);
	if (line_clipping(&start, &end, img) == false)
		return (false);
	if (start.x == end.x || start.y == end.y)
		return (check_straight_line(start, end, img));
	params.error_x = ft_absi(end.x - start.x);
	params.error_y = ft_absi(end.y - start.y);
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
	if (params.starting_error_x > params.starting_error_y)
		return (draw_low_slope(start, params, img));
	return (draw_high_slope(start, params, img));
}

static void	get_start_and_end(t_vector2i *start, t_vector2i *end,
				t_vector2f start_f, t_vector2f end_f,
				t_glyph_outline_bounds bounds)
{
	const t_vector2f	start_f_transformed = vector2f_add(vector2f_multiply(
			vector2f_subtract(end_f, start_f), 0.1f), start_f);
	t_vector2f	end_f_transformed = vector2f_add(vector2f_multiply(
			vector2f_subtract(start_f, end_f), 0.1f), end_f);

	start->x = start_f_transformed.x - bounds.xMin;
	start->y = start_f_transformed.y - bounds.yMin;
	end->x = end_f_transformed.x - bounds.xMin;
	end->y = end_f_transformed.y - bounds.yMin;
}

static bool	draw_low_slope(t_vector2i start, t_bresenham params, t_image *img)
{
	int	i;

	i = -1;
	while (++i <= params.starting_error_x)
	{
		if (img->address[start.y * img->width + start.x] == COLOR_WHITE)
			return (true);
		start.x += params.x_incr;
		params.error_x -= params.diff_y;
		if (params.error_x < 0)
		{
			start.y += params.y_incr;
			params.error_x += params.diff_x;
		}
	}
	return (false);
}

static bool	draw_high_slope(t_vector2i start, t_bresenham params, t_image *img)
{
	int	i;

	i = -1;
	while (++i <= params.starting_error_y)
	{
		if (img->address[start.y * img->width + start.x] == COLOR_WHITE)
			return (true);
		start.y += params.y_incr;
		params.error_y -= params.diff_x;
		if (params.error_y < 0)
		{
			start.x += params.x_incr;
			params.error_y += params.diff_y;
		}
	}
	return (false);
}

static bool	check_straight_line(t_vector2i start, t_vector2i end, t_image *img)
{
	if (start.x == end.x)
	{
		if (start.y > end.y)
			return (check_straight_line(end, start, img));
		while (start.y <= end.y)
			if (img->address[start.y++ * img->width + start.x] == COLOR_WHITE)
				return (true);
	}
	else
	{
		if (start.x > end.x)
			return (check_straight_line(end, start, img));
		while (start.x <= end.x)
			if (img->address[start.y * img->width + start.x++] == COLOR_WHITE)
				return (true);
	}
	return (false);
}
