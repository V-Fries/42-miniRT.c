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
#include <math.h>

#include "font/render.h"

static int		get_drawn_outlines(t_image *drawn_outlines, t_engine *engine,
					t_glyph_outline_bounds bounds,
					t_glyph_generated_points points);
static bool		is_triangle_inside_glyph(const t_vector2f *current_point,
					const t_vector2f *next_point,
					const t_vector2f *next_next_point,
					t_glyph_generated_points points,
					t_glyph_outline_bounds bounds);
static size_t	get_nb_of_intersected_segments(t_vector2f p1, t_vector2f p2,
					t_glyph_generated_points points);
static bool		is_point_inside_glyph(const t_image *draw_outlines,
					t_vector2f point, t_glyph_outline_bounds bounds);
static int		add_triangle(t_vector *triangles, t_dlist *cursor,
					t_dlist *next, t_dlist *next_next);
#include <stdlib.h>
#include <stdio.h>
#include "mlx.h"
t_triangles	triangulate_polygon(t_engine *engine, t_dlist *polygon,
				const t_glyph_outline_bounds bounds, t_glyph_generated_points points)
{
	t_image		drawn_outlines;
	t_dlist		*next;
	t_dlist		*next_next;
	t_dlist		*first_elem;
	t_vector	triangles;
	size_t		previous_size;
	size_t		size;

	if (get_drawn_outlines(&drawn_outlines, engine, bounds, points) < 0)
		return ((t_triangles){NULL, 0});
//	mlx_put_image_to_window(engine->window.mlx, engine->window.window, drawn_outlines.data, 0, 0);
	ft_vector_create(&triangles, sizeof(t_triangle), 0);
	first_elem = polygon;
	size = ft_dlstsize(polygon);
	previous_size = size;
	while (size > 3) // TODO check that size if at least 3 at the start
	{
		if (polygon->next == NULL)
			next = first_elem;
		else
			next = polygon->next;
		if (next->next == NULL)
			next_next = first_elem;
		else
			next_next = next->next;
		if (is_triangle_inside_glyph(polygon->content, next->content, next_next->content, points, bounds))
		{
			if (next == first_elem)
				first_elem = next_next;
			if (add_triangle(&triangles, polygon, next, next_next) < 0)
				return ((t_triangles){NULL, 0}); // TODO free stuff
			polygon = first_elem;
			size--;
		}
		else if (polygon->next != NULL)
			polygon = polygon->next;
		else
		{
			polygon = first_elem;
			if (size == previous_size) {
				printf("triangulate_polygon failed\n");
				return ((t_triangles){triangles.data, triangles.length});
//				return ((t_triangles){NULL, 0}); // TODO free stuff
			}
			previous_size = size;
		}
	}
	polygon = ft_dlstfirst(polygon);
	add_triangle(&triangles, polygon, polygon->next, polygon->next->next);
	ft_dlstclear(&polygon, &free);
	destroy_t_image(&engine->window, &drawn_outlines);
	ft_vector_minimize_size(&triangles);
	return ((t_triangles){triangles.data, triangles.length});
}

static int	get_drawn_outlines(t_image *drawn_outlines, t_engine *engine,
				const t_glyph_outline_bounds bounds, t_glyph_generated_points points)
{
	size_t		i;
	size_t		contour_start;
	int16_t		contour;
	t_vector2i	points_int[2];

	if (init_image(drawn_outlines, &engine->window,
			bounds.xMax - bounds.xMin + 10, bounds.yMax - bounds.yMin) < 0)
		return (-1);
	change_image_color(drawn_outlines, COLOR_BLACK); // TODO is this necessary?
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
			points_int[0] = (t_vector2i){points.points[i].x - bounds.xMin, points.points[i].y - bounds.yMin};
			points_int[1] = (t_vector2i){points.points[i + 1].x - bounds.xMin, points.points[i + 1].y - bounds.yMin};
			draw_line(points_int[0], points_int[1], drawn_outlines, COLOR_WHITE);
		}
		points_int[0] = (t_vector2i){points.points[i].x - bounds.xMin, points.points[i].y - bounds.yMin};
		points_int[1] = (t_vector2i){points.points[contour_start].x - bounds.xMin, points.points[contour_start].y - bounds.yMin};
		draw_line(points_int[0], points_int[1], drawn_outlines, COLOR_WHITE);
	}
	return (0);
}

bool	is_angle_concave(t_vector2f p1, t_vector2f p2, t_vector2f p3)
{
	const float crossProduct = (p2.x - p1.x) * (p3.y - p2.y)
		- (p3.x - p2.x) * (p2.y - p1.y);

	if (crossProduct < 0)
		return (true);
	return (false);
}

static bool	is_triangle_inside_glyph(const t_vector2f *current_point,
				const t_vector2f *next_point, const t_vector2f *next_next_point,
				const t_glyph_generated_points points,
				const t_glyph_outline_bounds bounds)
{
	t_vector2f			mid_point;
//	const t_vector2f	triangle[3] = {*current_point, *next_point,
//			*next_next_point};

	if (get_nb_of_intersected_segments(*current_point, *next_point, points)
		|| get_nb_of_intersected_segments(*next_point, *next_next_point, points)
		|| get_nb_of_intersected_segments(*next_next_point, *current_point, points))
		return (false);
	mid_point = vector2f_divide(vector2f_add(vector2f_add(
			*current_point, *next_point), *next_next_point), 3.f);
	return (get_nb_of_intersected_segments(mid_point, (t_vector2f){bounds.xMax + 100, mid_point.y}, points) % 2);
	(void)is_point_inside_glyph;
}

static size_t	get_nb_of_intersected_segments(const t_vector2f p1,
					const t_vector2f p2, const t_glyph_generated_points points)
{
	int16_t		contour;
	size_t		contour_start;
	size_t		i;
	t_vector2f	current_points[2];
	size_t		nb_of_intersected_segments;

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
			if ((vector2f_are_equal(p1, points.points[i]) || vector2f_are_equal(p1, points.points[i + 1]))
				&& (vector2f_are_equal(p2, points.points[i]) || vector2f_are_equal(p2, points.points[i + 1])))
				continue ;
			if (vector2f_are_equal(p1, points.points[i]) || vector2f_are_equal(p1, points.points[i + 1]))
				current_points[0] = vector2f_add(vector2f_multiply(vector2f_subtract(p2, p1), 0.05f), p1);
			else
				current_points[0] = p1;
			if (vector2f_are_equal(p2, points.points[i]) || vector2f_are_equal(p2, points.points[i + 1]))
				current_points[1] = vector2f_add(vector2f_multiply(vector2f_subtract(p1, p2), 0.05f), p2);
			else
				current_points[1] = p2;
			if (do_segments_intersect(current_points[0], current_points[1], points.points[i], points.points[i + 1]))
				nb_of_intersected_segments++;
		}
		if ((vector2f_are_equal(p1, points.points[i]) || vector2f_are_equal(p1, points.points[contour_start]))
			&& (vector2f_are_equal(p2, points.points[i]) || vector2f_are_equal(p2, points.points[contour_start])))
			continue ;
		if (vector2f_are_equal(p1, points.points[i]) || vector2f_are_equal(p1, points.points[contour_start]))
			current_points[0] = vector2f_add(vector2f_multiply(vector2f_subtract(p2, p1), 0.05f), p1);
		else
			current_points[0] = p1;
		if (vector2f_are_equal(p2, points.points[i]) || vector2f_are_equal(p2, points.points[contour_start]))
			current_points[1] = vector2f_add(vector2f_multiply(vector2f_subtract(p1, p2), 0.05f), p2);
		else
			current_points[1] = p2;
		if (do_segments_intersect(current_points[0], current_points[1], points.points[i], points.points[contour_start]))
			nb_of_intersected_segments++;
	}
	return (nb_of_intersected_segments);
}

static bool	is_point_inside_glyph(const t_image *draw_outlines,
				t_vector2f point, const t_glyph_outline_bounds bounds)
{
	size_t	collision;
	ssize_t	row;
	int 	i;
	bool	is_at_edge;

	point.y -= bounds.yMin;
	point.x -= bounds.xMin;
	row = (int)point.y * draw_outlines->width;
	collision = 0;
	is_at_edge = false;
	i = point.x;
	while (i < draw_outlines->width
		   && draw_outlines->address[row + i] == COLOR_WHITE)
	{
		is_at_edge = true;
		i++;
	}
	while (i < draw_outlines->width)
	{
		if (draw_outlines->address[row + i] == COLOR_WHITE)
			collision++;
		i++;
		while (i < draw_outlines->width
			   && draw_outlines->address[row + i] == COLOR_WHITE)
			i++;
	}
	return ((collision % 2) || (is_at_edge && collision == 0));
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
	if (cursor->next != NULL && cursor->next->next != NULL)
	{
		cursor->next = next_next;
		next_next->previous = cursor;
	}
	else
	{
		cursor->next = NULL;
		next_next->previous = NULL;
	}
	free(next->content);
	free(next);
	return (0);
}
