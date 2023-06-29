/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangulate_polygon.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 23:44:00 by vfries            #+#    #+#             */
/*   Updated: 2023/06/20 23:44:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

#include "font/render.h"

static bool		is_triangle_inside_glyph(const t_vector2f *current_point,
					const t_vector2f *next_point,
					const t_vector2f *next_next_point,
					t_glyph_generated_points points,
					float x_max);
static size_t	get_nb_of_intersected_segments(t_vector2f p1, t_vector2f p2,
					t_glyph_generated_points points);
static int		add_intersection(t_segment segment_1, t_segment segment_2);
static int		add_triangle(t_vector *triangles, t_dlist *cursor,
					t_dlist *next, t_dlist *next_next);

t_triangles	triangulate_polygon_and_free_polygon_list(t_dlist *polygon,
				const float x_max, t_glyph_generated_points points)
{
	t_dlist		*next;
	t_dlist		*next_next;
	t_dlist		*first_elem;
	t_vector	triangles;
	size_t		previous_size;
	size_t		size;

	ft_vector_create(&triangles, sizeof(t_triangle), 0);
	first_elem = polygon;
	size = ft_dlstsize(polygon);
	previous_size = size;
	while (size > 3)
	{
		if (polygon->next == NULL)
			next = first_elem;
		else
			next = polygon->next;
		if (next->next == NULL)
			next_next = first_elem;
		else
			next_next = next->next;
		if (is_triangle_inside_glyph(polygon->content, next->content, next_next->content, points, x_max))
		{
			if (next == first_elem)
				first_elem = next_next;
			if (add_triangle(&triangles, polygon, next, next_next) < 0)
			{
				ft_dlstclear(&polygon, &free);
				ft_vector_destroy(&triangles);
				return ((t_triangles){NULL, 0}); // TODO free stuff
			}
			polygon = first_elem;
			size--;
		}
		else if (polygon->next != NULL)
			polygon = polygon->next;
		else
		{
			polygon = first_elem;
			if (size == previous_size)
			{
//				printf("triangulate_polygon failed\n");
				ft_dlstclear(&polygon, &free);
//				ft_vector_destroy(&triangles);
//				return ((t_triangles){NULL, 0});
				ft_vector_minimize_size(&triangles);
				return ((t_triangles){triangles.data, triangles.length});
			}
			previous_size = size;
		}
	}
	polygon = ft_dlstfirst(polygon);
	add_triangle(&triangles, polygon, polygon->next, polygon->next->next); // TODO check that size if at least 3 at the start
	ft_dlstclear(&polygon, &free);
	ft_vector_minimize_size(&triangles);
	return ((t_triangles){triangles.data, triangles.length});
}

static bool	is_triangle_inside_glyph(const t_vector2f *current_point,
				const t_vector2f *next_point, const t_vector2f *next_next_point,
				const t_glyph_generated_points points,
				const float x_max)
{
	t_vector2f			mid_point;

	if (get_nb_of_intersected_segments(*current_point, *next_point, points)
		|| get_nb_of_intersected_segments(*next_point, *next_next_point, points)
		|| get_nb_of_intersected_segments(*next_next_point, *current_point,
				points))
		return (false);
	mid_point = vector2f_divide(vector2f_add(vector2f_add(
			*current_point, *next_point), *next_next_point), 3.f);
	return (get_nb_of_intersected_segments(mid_point,
			(t_vector2f){x_max + 100, mid_point.y}, points) % 2);
}

static size_t	get_nb_of_intersected_segments(const t_vector2f p1,
					const t_vector2f p2, const t_glyph_generated_points points)
{
	int16_t			contour;
	size_t			contour_start;
	size_t			i;
	const t_segment	segment_1 = {p1, p2};
	size_t			nb_of_intersected_segments;

	nb_of_intersected_segments = 0;
	contour = -1;
	while (++contour < points.nb_of_contours)
	{
		if (contour != 0)
			contour_start = points.contours_limits[contour - 1];
		else
			contour_start = 0;
		i = contour_start - 1;
		while (++i < points.contours_limits[contour] - 1)
		{
			nb_of_intersected_segments += add_intersection(segment_1,
					(t_segment){points.points[i], points.points[i + 1]});
		}
		nb_of_intersected_segments += add_intersection(segment_1,
				(t_segment){points.points[i], points.points[contour_start]});
	}
	return (nb_of_intersected_segments);
}

static int	add_intersection(t_segment segment_1, t_segment segment_2)
{
	t_segment	corrected_segment_1;

	if ((vector2f_are_equal(segment_1.a, segment_2.a)
			|| vector2f_are_equal(segment_1.a, segment_2.b))
		&& (vector2f_are_equal(segment_1.b, segment_2.a)
			|| vector2f_are_equal(segment_1.b, segment_2.b)))
		return (0);
	if (vector2f_are_equal(segment_1.a, segment_2.a)
		|| vector2f_are_equal(segment_1.a, segment_2.b))
		corrected_segment_1.a = vector2f_add(vector2f_multiply(
				vector2f_subtract(segment_1.b, segment_1.a), 0.05f),
					segment_1.a);
	else
		corrected_segment_1.a = segment_1.a;
	if (vector2f_are_equal(segment_1.b, segment_2.a)
		|| vector2f_are_equal(segment_1.b, segment_2.b))
		corrected_segment_1.b = vector2f_add(vector2f_multiply(
				vector2f_subtract(segment_1.a, segment_1.b), 0.05f),
					segment_1.b);
	else
		corrected_segment_1.b = segment_1.b;
	return (do_segments_intersect(corrected_segment_1.a, corrected_segment_1.b,
			segment_2.a, segment_2.b));
}

static int	add_triangle(t_vector *triangles, t_dlist *cursor, t_dlist *next,
						   t_dlist *next_next)
{
	t_triangle	triangle;

	triangle.a = *(t_vector2f *)cursor->content;
	triangle.b = *(t_vector2f *)next->content;
	triangle.c = *(t_vector2f *)next_next->content;
	if (ft_vector_add_elem(triangles, &triangle) != VECTOR_SUCCESS)
		return (-1);
	if (next->previous != NULL)
		next->previous->next = next->next;
	if (next->next != NULL)
		next->next->previous = next->previous;
	free(next->content);
	free(next);
	return (0);
}
