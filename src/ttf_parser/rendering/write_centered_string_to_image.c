/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_centered_string_to_image.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 01:43:00 by vfries            #+#    #+#             */
/*   Updated: 2023/06/28 01:43:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include "engine.h"
#include "font/render.h"


#include "stdio.h"
void	write_centered_string_to_image(const t_font *font, t_image *image,
			const char *string, const float size)
{
	const size_t	string_len = ft_strlen(string);
	const float		scale = size / font->bounds.yMax;
	float			x_offset;
	const float		y_offset = image->height / 2.f - scale * font->bounds.yMax
		- (scale * font->bounds.yMax - scale * font->bounds.yMin) / 2.f;
	size_t			i;

	x_offset = image->width / 2.f - (float)string_len / 2.f * font->bounds.xMax * scale;
	i = -1;
	while (string[++i] && x_offset + font->bounds.xMax * scale < image->width)
	{
		draw_glyph(font->glyphs[(int8_t)string[i]], scale, image, COLOR_WHITE,
			x_offset, y_offset);
		x_offset += font->bounds.xMax * scale;
	}
}
