/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_glyph_points.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 17:59:00 by vfries            #+#    #+#             */
/*   Updated: 2023/06/14 17:59:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font/ttf_parser.h"
#include "math/vector.h"

t_vector2f	get_glyph_points(const t_glyph_outline *glyph)
{
	const size_t	number_of_points
		= glyph->endPtsOfContours[glyph->numberOfContours - 1];
	size_t			i;

	i = 0;
	while (i < number_of_points && glyph->flags[i].on_curve == false)
		i++;
	i--;
	while (++i < number_of_points)
	{

	}
}
