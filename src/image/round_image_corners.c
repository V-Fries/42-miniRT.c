/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   round_image_corners.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 18:39:06 by vfries            #+#    #+#             */
/*   Updated: 2023/07/30 18:39:24 by vfries           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdbool.h>

#include "image.h"
#include "colors.h"

static bool	is_in_top_left_corner(int x, int y, int radius);
static bool	is_in_top_right_corner(int x, int y, int radius, t_image *image);
static bool	is_in_bottom_left_corner(int x, int y, int radius, t_image *image);
static bool	is_in_bottom_right_corner(int x, int y, int radius, t_image *image);

void	round_image_corners(t_image *image, int radius)
{
	const int	max_radius = fmin(image->height, image->width) / 2;
	int			y;
	int			x;

	radius = fmax(0, fmin(radius, max_radius));
	y = image->height;
	while (y--)
	{
		x = image->width;
		while (x--)
		{
			if (is_in_top_left_corner(x, y, radius)
				|| is_in_top_right_corner(x, y, radius, image)
				|| is_in_bottom_left_corner(x, y, radius, image)
				|| is_in_bottom_right_corner(x, y, radius, image))
				put_pixel_on_image(image, y, x, COLOR_TRANSPARENT);
		}
	}
}

static bool	is_in_top_left_corner(int x, int y, int radius)
{
	return (y < radius && x < radius
		&& powf(y - radius, 2) + powf(x - radius, 2) > powf(radius, 2));
}

static bool	is_in_top_right_corner(int x, int y, int radius, t_image *image)
{
	return (y < radius && x >= image->width - radius
		&& powf(y - radius, 2) + powf(x - image->width + radius, 2)
		> powf(radius, 2));
}

static bool	is_in_bottom_left_corner(int x, int y, int radius, t_image *image)
{
	return (y >= image->height - radius && x < radius
		&& powf(y - image->height + radius, 2)
		+ powf(x - radius, 2) > powf(radius, 2));
}

static bool	is_in_bottom_right_corner(int x, int y, int radius, t_image *image)
{
	return (y >= image->height - radius && x >= image->width - radius
		&& powf(y - image->height + radius, 2)
		+ powf(x - image->width + radius, 2) > powf(radius, 2));
}
