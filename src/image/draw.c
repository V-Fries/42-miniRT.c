/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 13:50:00 by vfries            #+#    #+#             */
/*   Updated: 2023/06/05 13:50:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include "image.h"
#include "math/vector.h"
#include "colors.h"
#include "libft.h"

void	draw_circle(t_image *image, const t_vector2i circle_center,
			const float radius, const unsigned int color)
{
	const float	radius_squared = radius * radius;
	int			y_offset;
	int			y;
	int			x;

	y = circle_center.y - radius - 1;
	if (y < 0)
		y = 0;
	y_offset = image->width * (y + 1);
	while (y < image->height && y < circle_center.y + (int)radius + 1)
	{
		x = circle_center.x - radius - 1;
		if (x < 0)
			x = 0;
		while (x < image->width && x < circle_center.x + (int)radius + 1)
		{
			if (powf(x - circle_center.x, 2.f) + powf(y - circle_center.y, 2.f)
				<= radius_squared)
				image->address[y_offset + x] = color;
			x++;
		}
		y_offset += image->line_length;
		y++;
	}
}
