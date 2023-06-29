/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_glyph.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 14:36:00 by vfries            #+#    #+#             */
/*   Updated: 2023/06/25 14:36:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font/render.h"

void	draw_glyph(const t_triangles glyph_triangles, const float scale,
			t_image *image, const unsigned int color, const float x_offset,
			const float y_offset)
{
	size_t		i;
	t_triangle	triangle_to_draw;

	i = glyph_triangles.size;
	while (i--)
	{
		triangle_to_draw.a = vector2f_multiply(glyph_triangles.data[i].a,
				scale);
		triangle_to_draw.a.x += x_offset;
		triangle_to_draw.a.y += y_offset;
		triangle_to_draw.b = vector2f_multiply(glyph_triangles.data[i].b,
				scale);
		triangle_to_draw.b.x += x_offset;
		triangle_to_draw.b.y += y_offset;
		triangle_to_draw.c = vector2f_multiply(glyph_triangles.data[i].c,
				scale);
		triangle_to_draw.c.x += x_offset;
		triangle_to_draw.c.y += y_offset;
		draw_triangle(triangle_to_draw, image, color);
	}
}
