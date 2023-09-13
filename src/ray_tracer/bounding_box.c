/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounding_box.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 12:29:00 by tdameros          #+#    #+#             */
/*   Updated: 2023/09/13 12:29:00 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <float.h>

#include "render_frame.h"

static	t_vector2i	convert_world_point_in_screen_space(t_engine *engine, t_vector3f world_point);
static void	safe_put_color(t_engine *engine, t_vector2i point);

static	void	draw_bounding_box(t_engine *engine, t_bounding_box bounding_box);

static void	libx_put_line_dy_greater(t_engine *engine,
										t_vector2i point1, t_vector2i point2);
static void	libx_put_line_dx_greater(t_engine *engine,
										t_vector2i point1, t_vector2i point2);
void	libx_put_line(t_engine *engine, t_vector2i point1, t_vector2i point2);
static	t_bounding_box	calculate_objets_bounding_box(t_objects *objects);

void	render_bounding_box(t_engine *engine)
{
	for (size_t i = 0; i < engine->scene.objects.length; i++)
	{
		t_object	object = engine->scene.objects.data[i];
		if (object.type == SPHERE)
		{
			sphere_calculate_bounding_box(&object);
			draw_bounding_box(engine, object.bounding_box);
		}
		else if (object.type == CYLINDER || object.type == CONE)
		{
			cylinder_calculate_bounding_box(&object);
			draw_bounding_box(engine, object.bounding_box);
		}
	}
	t_bounding_box box = calculate_objets_bounding_box(&engine->scene.objects);
	draw_bounding_box(engine, box);
}


static	t_bounding_box	calculate_objets_bounding_box(t_objects *objects)
{
	t_bounding_box	bounding_box;
	t_vector3f 		min;
	t_vector3f 		max;


	min = (t_vector3f){FLT_MAX, FLT_MAX, FLT_MAX};
	max = (t_vector3f){-FLT_MAX, -FLT_MAX, -FLT_MAX};
	for (size_t  i = 0; i < objects->length; i++)
	{
		t_object	object = objects->data[i];
		if (object.type == SPHERE || object.type == CYLINDER || object.type == CONE)
		{
			if (object.type == SPHERE)
				sphere_calculate_bounding_box(&object);
			else
				cylinder_calculate_bounding_box(&object);
			min = vector3f_min(min, object.bounding_box.a);
			min = vector3f_min(min, object.bounding_box.b);
			min = vector3f_min(min, object.bounding_box.c);
			min = vector3f_min(min, object.bounding_box.d);
			min = vector3f_min(min, object.bounding_box.e);
			min = vector3f_min(min, object.bounding_box.f);
			min = vector3f_min(min, object.bounding_box.g);
			min = vector3f_min(min, object.bounding_box.h);

			max = vector3f_max(max, object.bounding_box.a);
			max = vector3f_max(max, object.bounding_box.b);
			max = vector3f_max(max, object.bounding_box.c);
			max = vector3f_max(max, object.bounding_box.d);
			max = vector3f_max(max, object.bounding_box.e);
			max = vector3f_max(max, object.bounding_box.f);
			max = vector3f_max(max, object.bounding_box.g);
			max = vector3f_max(max, object.bounding_box.h);
		}
	}
	bounding_box.a = (t_vector3f){max.x, max.y, min.z};
	bounding_box.b = max;
	bounding_box.c = (t_vector3f){min.x, max.y, max.z};
	bounding_box.d = (t_vector3f){min.x, max.y, min.z};
	bounding_box.e = (t_vector3f){max.x, min.y, min.z};
	bounding_box.f = (t_vector3f){max.x, min.y, max.z};
	bounding_box.g = (t_vector3f){min.x, min.y, max.z};
	bounding_box.h = min;
	return (bounding_box);
}

static	void	draw_bounding_box(t_engine *engine, t_bounding_box bounding_box)
{
	t_vector2i	a = convert_world_point_in_screen_space(engine, bounding_box.a);
	t_vector2i	b = convert_world_point_in_screen_space(engine, bounding_box.b);
	t_vector2i	c = convert_world_point_in_screen_space(engine, bounding_box.c);
	t_vector2i	d = convert_world_point_in_screen_space(engine, bounding_box.d);
	t_vector2i	e = convert_world_point_in_screen_space(engine, bounding_box.e);
	t_vector2i	f = convert_world_point_in_screen_space(engine, bounding_box.f);
	t_vector2i	g = convert_world_point_in_screen_space(engine, bounding_box.g);
	t_vector2i	h = convert_world_point_in_screen_space(engine, bounding_box.h);

	libx_put_line(engine, a, b);
	libx_put_line(engine, b, c);
	libx_put_line(engine, c, d);
	libx_put_line(engine, d, a);

	libx_put_line(engine, e, f);
	libx_put_line(engine, f, g);
	libx_put_line(engine, g, h);
	libx_put_line(engine, h, e);

	libx_put_line(engine, a, e);
	libx_put_line(engine, b, f);
	libx_put_line(engine, c, g);
	libx_put_line(engine, d, h);
}

static	t_vector2i	convert_world_point_in_screen_space(t_engine *engine,
													t_vector3f world_point)
{
	t_vector4f world = (t_vector4f){world_point.x, world_point.y, world_point.z, 1};

	t_vector4f view = matrix4_multiply_vector4(&engine->camera.view, world);
	t_vector4f projection = matrix4_multiply_vector4(&engine->camera.projection, view);

	if (projection.w < engine->camera.near_clip)
		return ((t_vector2i){-1, -1});
	projection = vector4f_divide(projection, projection.w);

	t_vector2f screen_pos = (t_vector2f){projection.x, projection.y};
	screen_pos = vector2f_divide(vector2f_add(screen_pos, (t_vector2f){1.f, 1.f}), 2);

	screen_pos.y = 1.f - screen_pos.y;
	screen_pos.y = screen_pos.y * engine->camera.viewport.size.y;
	screen_pos.x = screen_pos.x * engine->camera.viewport.size.x;

	return ((t_vector2i){(int) screen_pos.x, (int) screen_pos.y});
}

static void	safe_put_color(t_engine *engine, t_vector2i point)
{
	if (point.x <= 0 || point.x >= engine->raytraced_pixels.width)
		return;
	if (point.y <= 0 || point.y >= engine->raytraced_pixels.height)
		return;
	engine->raytraced_pixels.data[point.x + point.y * engine->raytraced_pixels.width] =\
	(t_vector3f) {255.f, 255.f, 255.f};
}


/**
 * Put a line between 2 points in mlx image.
 * (protected if there are pixels is outside the image)
 */
void	libx_put_line(t_engine *engine, t_vector2i point1, t_vector2i point2)
{
	double	dx;
	double	dy;

	if (point1.x < 0 || point2.x < 0)
		return ;
	dx = fabs((double) point2.x - point1.x);
	dy = fabs((double) point2.y - point1.y);
	if (dx >= dy)
		libx_put_line_dx_greater(engine, point1, point2);
	else
		libx_put_line_dy_greater(engine, point1, point2);
}

/**
 * Put a line between 2 points in mlx image if dx is greater.
 * (protected if there are pixels is outside the image)
 */
static void	libx_put_line_dx_greater(t_engine *engine,
										t_vector2i point1, t_vector2i point2)
{
	const double	dx = fabs((double) point2.x - point1.x);
	const double	dy = fabs((double) point2.y - point1.y);
	const double	m = dy / dx;
	double			e;
	int				i;

	i = 0;
	e = 0;
	while (i++ <= dx)
	{
		safe_put_color(engine, point1);
		if (point1.x > point2.x)
			point1.x--;
		else
			point1.x++;
		e -= m;
		if (e <= -0.5)
		{
			if (point1.y > point2.y)
				point1.y--;
			else
				point1.y++;
			e += 1;
		}
	}
}

/**
 * Put a line between 2 points in mlx image if dy is greater.
 * (protected if there are pixels is outside the image)
 */
static void	libx_put_line_dy_greater(t_engine *engine,
										t_vector2i point1, t_vector2i point2)
{
	const double	dx = fabs((double) point2.x - point1.x);
	const double	dy = fabs((double) point2.y - point1.y);
	const double	m = dx / dy;
	double			e;
	int				i;

	i = 0;
	e = 0;
	while (i++ <= dy)
	{
		safe_put_color(engine, point1);
		if (point1.y > point2.y)
			point1.y--;
		else
			point1.y++;
		e -= m;
		if (e <= -0.5)
		{
			if (point1.x > point2.x)
				point1.x--;
			else
				point1.x++;
			e += 1;
		}
	}
}