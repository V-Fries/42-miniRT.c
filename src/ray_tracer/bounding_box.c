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
#include "ray_tracer/bvh.h"


static void	safe_put_color(t_engine *engine, t_vector2i point, t_vector3f color);
static void	libx_put_line_dy_greater(t_engine *engine,
										t_vector2i point1, t_vector2i point2, t_vector3f color);
static void	libx_put_line_dx_greater(t_engine *engine,
										t_vector2i point1, t_vector2i point2, t_vector3f color);
void	libx_put_line(t_engine *engine, t_vector2i point1, t_vector2i point2, t_vector3f color);



static	t_vector2i	convert_world_point_in_screen_space(t_engine *engine, t_vector3f world_point);
static	void	draw_bounding_box(t_engine *engine, t_bounding_box bounding_box, t_vector3f color);
//static	t_bounding_box	calculate_objets_bounding_box(t_objects *objects);
static void	draw_bvh_node(t_engine *engine, t_bvh_node *node, t_vector3f color);
static void	draw_bvh_tree(t_engine *engine, t_bvh_node *root_node, int color_level);

void	render_bounding_box(t_engine *engine)
{
	for (size_t i = 0; i < engine->scene.objects.length; i++)
	{
		t_object	*object = &engine->scene.objects.data[i];
		if (object->type == SPHERE)
			sphere_calculate_bounding_box(object);
		else if (object->type == CYLINDER || object->type == CONE)
			cylinder_calculate_bounding_box(object);
	}
	t_bvh_node *root_node = bvh_node_create_root(&engine->scene.objects);
	bvh_node_update_bounding_box(root_node);
	bvh_subdivide(root_node, LEFT_AND_RIGHT);
	draw_bvh_tree(engine, root_node, 0);
	bvh_tree_free(root_node);
}

static void	draw_bvh_tree(t_engine *engine, t_bvh_node *root_node, int color_level)
{
	if (color_level >= 4)
		return;
	t_vector3f	colors[4] = {
			(t_vector3f){255.f, 0.f, 0.f},
			(t_vector3f){0.f, 255.f, 0.f},
			(t_vector3f){0.f, 0.f, 255.f},
			(t_vector3f){0.f, 100.f, 100.f}
	};
	if (root_node->is_leaf)
	{
		for (size_t i = 0; i < root_node->index_objects.length; i++)
		{
			t_object *object = &root_node->objects->data[root_node->index_objects.data[i]];
			if (object->type != MESH && object->type != PLANE)
				draw_bounding_box(engine, object->bounding_box, (t_vector3f){255.f, 255.f, 255.f});
		}
		draw_bvh_node(engine, root_node, (t_vector3f){255.f, 0.f, 255.f});
		return;
	}
	draw_bvh_node(engine, root_node, colors[color_level]);
	if (root_node->left_node != NULL)
		draw_bvh_tree(engine, root_node->left_node, color_level + 1);
	if (root_node->right_node != NULL)
		draw_bvh_tree(engine, root_node->right_node, color_level + 1);
}

static	void	draw_bounding_box(t_engine *engine, t_bounding_box bounding_box, t_vector3f color)
{
	t_vector2i	a = convert_world_point_in_screen_space(engine, bounding_box.top_face[0]);
	t_vector2i	b = convert_world_point_in_screen_space(engine, bounding_box.top_face[1]);
	t_vector2i	c = convert_world_point_in_screen_space(engine, bounding_box.top_face[2]);
	t_vector2i	d = convert_world_point_in_screen_space(engine, bounding_box.top_face[3]);
	t_vector2i	e = convert_world_point_in_screen_space(engine, bounding_box.bottom_face[0]);
	t_vector2i	f = convert_world_point_in_screen_space(engine, bounding_box.bottom_face[1]);
	t_vector2i	g = convert_world_point_in_screen_space(engine, bounding_box.bottom_face[2]);
	t_vector2i	h = convert_world_point_in_screen_space(engine, bounding_box.bottom_face[3]);

	libx_put_line(engine, a, b, color);
	libx_put_line(engine, b, c, color);
	libx_put_line(engine, c, d, color);
	libx_put_line(engine, d, a, color);

	libx_put_line(engine, e, f, color);
	libx_put_line(engine, f, g, color);
	libx_put_line(engine, g, h, color);
	libx_put_line(engine, h, e, color);

	libx_put_line(engine, a, e, color);
	libx_put_line(engine, b, f, color);
	libx_put_line(engine, c, g, color);
	libx_put_line(engine, d, h, color);
}

static void	draw_bvh_node(t_engine *engine, t_bvh_node *node, t_vector3f color)
{
	t_bounding_box	bounding_box;

	t_vector3f max = node->aabb_max;
	t_vector3f min = node->aabb_min;
	bounding_box.top_face[0] = (t_vector3f){max.x, max.y, min.z};
	bounding_box.top_face[1] = max;
	bounding_box.top_face[2] = (t_vector3f){min.x, max.y, max.z};
	bounding_box.top_face[3] = (t_vector3f){min.x, max.y, min.z};
	bounding_box.bottom_face[0] = (t_vector3f){max.x, min.y, min.z};
	bounding_box.bottom_face[1] = (t_vector3f){max.x, min.y, max.z};
	bounding_box.bottom_face[2] = (t_vector3f){min.x, min.y, max.z};
	bounding_box.bottom_face[3] = min;
	draw_bounding_box(engine, bounding_box, color);
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

static void	safe_put_color(t_engine *engine, t_vector2i point, t_vector3f color)
{
	if (point.x <= 0 || point.x >= engine->raytraced_pixels.width)
		return;
	if (point.y <= 0 || point.y >= engine->raytraced_pixels.height)
		return;
	engine->raytraced_pixels.data[point.x + point.y * engine->raytraced_pixels.width] = color;
}


/**
 * Put a line between 2 points in mlx image.
 * (protected if there are pixels is outside the image)
 */
void	libx_put_line(t_engine *engine, t_vector2i point1, t_vector2i point2, t_vector3f color)
{
	double	dx;
	double	dy;

	if (point1.x < 0 || point2.x < 0)
		return ;
	dx = fabs((double) point2.x - point1.x);
	dy = fabs((double) point2.y - point1.y);
	if (dx >= dy)
		libx_put_line_dx_greater(engine, point1, point2, color);
	else
		libx_put_line_dy_greater(engine, point1, point2, color);
}

/**
 * Put a line between 2 points in mlx image if dx is greater.
 * (protected if there are pixels is outside the image)
 */
static void	libx_put_line_dx_greater(t_engine *engine,
										t_vector2i point1, t_vector2i point2, t_vector3f color)
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
		safe_put_color(engine, point1, color);
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
										t_vector2i point1, t_vector2i point2, t_vector3f color)
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
		safe_put_color(engine, point1, color);
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