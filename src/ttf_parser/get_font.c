/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_font.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 18:53:00 by vfries            #+#    #+#             */
/*   Updated: 2023/06/27 18:53:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "font/render.h"

static int						get_glyph_triangles(t_triangles *triangles,
									uint8_t i, const t_ttf *ttf);
static void						reverse_y(t_triangles *triangles, float y_max);
static t_glyph_outline_bounds	get_glyph_bounds(
									const t_triangles *glyph_triangles);
static void						get_min_max(const t_triangle *triangle,
									t_vector2f *min, t_vector2f *max);

int	get_font(t_font *font, char *font_file)
{
	t_ttf	ttf;
	uint8_t	i;

	font->glyphs = ft_calloc(127, sizeof(*font->glyphs));
	font->glyphs_size = ft_calloc(127, sizeof(*font->glyphs_size));
	if (font->glyphs == NULL || font->glyphs_size == NULL)
	{
		free(font->glyphs);
		free(font->glyphs_size);
		return (-1);
	}
	if (ttf_parser(&ttf, font_file) < 0)
	{
		free(font->glyphs);
		free(font->glyphs_size);
		return (-1);
	}
	i = -1;
	while (++i < 127)
	{
		if (get_glyph_triangles(font->glyphs + i, i, &ttf) < 0)
		{
			free(font->glyphs);
			free(font->glyphs_size);
			destroy_t_ttf(&ttf);
			return (-1);
		}
		font->glyphs_size[i] = get_glyph_bounds(font->glyphs + i);
	}
	font->bounds.yMax = ttf.head.yMax;
	font->bounds.xMax = ttf.head.xMax;
	font->bounds.yMin = ttf.head.yMin;
	font->bounds.xMin = ttf.head.xMin;
	return (0);
}

static int	get_glyph_triangles(t_triangles *triangles, const uint8_t i,
				const t_ttf *ttf)
{
	const t_glyph_outline	*glyph = ttf->glyphs + get_glyph_index(i, ttf);
	t_vector				generated_points;
	size_t					*contours_limits;
	t_dlist					*polygon;

	if (glyph->numberOfContours <= 0)
		return (0);
	if (get_glyph_points(&generated_points, glyph, &contours_limits) < 0)
		return (-1);
	polygon = get_polygon_from_contours(generated_points,
			glyph->numberOfContours, contours_limits);
	if (polygon == NULL)
		return (ft_vector_destroy(&generated_points), -1);
	*triangles = triangulate_polygon_and_free_polygon_list(polygon,
			glyph->bounds.xMax, (t_glyph_generated_points){
			generated_points.data, generated_points.length, contours_limits,
			glyph->numberOfContours});
	ft_vector_destroy(&generated_points);
	if (triangles->data == NULL)
	{
		triangles->size = 0;
		return (0);
	}
	reverse_y(triangles, ttf->head.yMax);
	return (0);
}


static void	reverse_y(t_triangles *triangles, float y_max)
{
	size_t	i;

	i = triangles->size;
	while (i--)
	{
		triangles->data[i].a.y = (1.f - triangles->data[i].a.y / y_max) * y_max;
		triangles->data[i].b.y = (1.f - triangles->data[i].b.y / y_max) * y_max;
		triangles->data[i].c.y = (1.f - triangles->data[i].c.y / y_max) * y_max;
	}
}

static t_glyph_outline_bounds	get_glyph_bounds(
									const t_triangles *glyph_triangles)
{
	t_vector2f	min;
	t_vector2f	max;
	size_t		i;

	min = (t_vector2f){FLT_MAX, FLT_MAX};
	max = (t_vector2f){FLT_MIN, FLT_MIN};
	i = glyph_triangles->size;
	while (i--)
		get_min_max(glyph_triangles->data + i, &min, &max);
	return ((t_glyph_outline_bounds){
		.xMin = min.x,
		.xMax = max.x,
		.yMin = min.y,
		.yMax = max.y});
}

static void	get_min_max(const t_triangle *triangle, t_vector2f *min,
				t_vector2f *max)
{
	max->x = fmaxf(triangle->a.x, max->x);
	max->x = fmaxf(triangle->b.x, max->x);
	max->x = fmaxf(triangle->c.x, max->x);
	max->y = fmaxf(triangle->a.y, max->y);
	max->y = fmaxf(triangle->b.y, max->y);
	max->y = fmaxf(triangle->c.y, max->y);
	min->x = fminf(triangle->a.x, min->x);
	min->x = fminf(triangle->b.x, min->x);
	min->x = fminf(triangle->c.x, min->x);
	min->y = fminf(triangle->a.y, min->y);
	min->y = fminf(triangle->b.y, min->y);
	min->y = fminf(triangle->c.y, min->y);
}
