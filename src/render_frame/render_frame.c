/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_frame.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 18:12:50 by vfries            #+#    #+#             */
/*   Updated: 2023/05/07 18:38:22 by vfries           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

#include "mlx.h"
#include "libft.h"

#include "engine.h"
#include "gui/box.h"
#include "render_frame.h"
#include "ray_tracer/render.h"
#include "gui/utils.h"

#include "font/render.h"

static void	render_minirt(t_engine *minirt);
static void	update_placed_object_position(t_engine *engine);
static void	update_mouse_position(t_engine *engine, t_vector2i *mouse_position);

int	render_frame(t_engine *minirt)
{
	const struct timeval	start_time = get_current_time();

	render_minirt(minirt);
	print_fps_counter(minirt, start_time);
	return (0);
}

#if defined __linux__

static void	render_minirt(t_engine *minirt)
{
	render_raytracing(minirt);
	if (minirt->gui.is_hidden && minirt->gui.hidden_ratio == 1.0)
		mlx_put_image_to_window(minirt->window.mlx, minirt->window.window,
								minirt->ray_traced_image.data, 0, 0);
	else
	{
		put_background(&minirt->main_image, &minirt->ray_traced_image);
		render_user_interface(minirt);
		mlx_put_image_to_window(minirt->window.mlx, minirt->window.window,
			minirt->main_image.data, 0, 0);
	}
}
#elif defined __APPLE__

#include <stdio.h>
#define SCALE_FACTOR 0.5f
static void	render_minirt(t_engine *engine)
{
	update_placed_object_position(engine);
//	render_raytracing(engine);
	change_image_color(&engine->ray_traced_image, COLOR_TRANSPARENT);

	static int test = false;
	if (test == false)
		srand(time(NULL));
	if (test)
		return;
	test = true;

	t_glyph_outline	*glyph = engine->gui.font.glyphs + get_glyph_index('1', &engine->gui.font.ttf);
	size_t		*contours_limits;
	t_vector	points;
	if (get_glyph_points(&points, glyph, &contours_limits) < 0)
		return printf("Failed to get glyph points\n"), (void)exit(1);
	for (size_t i = 0; i < points.length; i++)
	{
		((t_vector2f *)points.data)[i].x *= 0.5f;
		((t_vector2f *)points.data)[i].y *= 0.5f;
	}
	t_glyph_generated_points gen_points = {points.data, points.length, contours_limits, glyph->numberOfContours};
	t_dlist	*polygon = get_polygon_from_contours(points, glyph->numberOfContours, contours_limits);
	if (polygon == NULL)
		return printf("Failed to get polygon\n"), (void)exit(1);
	float gwgew = ft_dlstsize(polygon);
	float fewgew = 0.f;
	for (t_dlist *cursor = polygon; cursor != NULL; cursor = cursor->next)
	{
		t_color	color = {255.f, 255.f * (fewgew / gwgew), 255.f * (fewgew / gwgew)};
		fewgew++;
		t_vector2f *point_f = cursor->content;
		t_vector2i point = {point_f->x + 100, (1.f - (point_f->y + 100) / engine->ray_traced_image.height) * engine->ray_traced_image.height};
		if (point.x >= 0 && point.x < engine->ray_traced_image.width && point.y >= 0 && point.y < engine->ray_traced_image.height)
			put_pixel_on_image(&engine->ray_traced_image, point.y, point.x, vec_rgb_to_uint(color));
	}

//	size_t polygon_size = ft_dlstsize(polygon);
//	t_vector2i	*points_i = malloc(sizeof(*points_i) * polygon_size);
//
//	t_dlist	*cursor = polygon;
//	for (size_t j = 0; j < polygon_size; j++)
//	{
//		points_i[j].x = ((t_vector2f *)cursor->content)->x + 400.f;
//		float tmp = (((t_vector2f *)cursor->content)->y + 100) / engine->ray_traced_image.height;
//		points_i[j].y = (1.f - tmp) * engine->ray_traced_image.height;
//		cursor = cursor->next;
//	}
//	for (size_t i = 0; i < polygon_size - 1; i++)
//		draw_line(points_i[i], points_i[i + 1], &engine->ray_traced_image, COLOR_WHITE);
//	draw_line(points_i[0], points_i[polygon_size - 1], &engine->ray_traced_image, COLOR_WHITE);

	t_triangles	triangles = triangulate_polygon_and_free_polygon_list(polygon, glyph->bounds.xMax * SCALE_FACTOR, gen_points);
	if (triangles.size == 0)
		return printf("triangulate_polygon failed\n"), (void)exit(1);


	for (size_t i = 0; i < triangles.size; i++)
	{
//		t_vector2i	points_i[3] = {
//			{triangles.data[i].a.x + 700.f, triangles.data[i].a.y + 100},
//			{triangles.data[i].b.x + 700.f, triangles.data[i].b.y + 100},
//			{triangles.data[i].c.x + 700.f, triangles.data[i].c.y + 100}
//		};
		t_vector2i	points_i[3] = {
			{triangles.data[i].a.x + 700.f, (1.f - (triangles.data[i].a.y + 100) / engine->ray_traced_image.height) * engine->ray_traced_image.height},
			{triangles.data[i].b.x + 700.f, (1.f - (triangles.data[i].b.y + 100) / engine->ray_traced_image.height) * engine->ray_traced_image.height},
			{triangles.data[i].c.x + 700.f, (1.f - (triangles.data[i].c.y + 100) / engine->ray_traced_image.height) * engine->ray_traced_image.height}
		};
		draw_line(points_i[0], points_i[1], &engine->ray_traced_image, COLOR_WHITE);
		draw_line(points_i[1], points_i[2], &engine->ray_traced_image, COLOR_WHITE);
		draw_line(points_i[2], points_i[0], &engine->ray_traced_image, COLOR_WHITE);
	}
	printf("Done\n");



//	draw_glyph(engine, points, contours_limits, glyph->numberOfContours);

	ft_vector_destroy(&points);
	free(contours_limits);
	free(triangles.data);

	mlx_put_image_to_window(engine->window.mlx, engine->window.window,
		engine->ray_traced_image.data, 0, 0);
//	render_user_interface(engine);
}
#else
# error "Unsuported OS"
#endif

static void	update_placed_object_position(t_engine *engine)
{
	t_vector2i	mouse_position;
	size_t		ray_index;
	t_vector3f	direction;

	if (engine->object_being_placed == NULL)
		return ;
	mouse_position = get_mouse_position(engine);
	update_mouse_position(engine, &mouse_position);
	ray_index = mouse_position.x + mouse_position.y \
			* (int)engine->camera.viewport.size.x;
	direction = engine->camera.rays[ray_index].direction;
	engine->object_being_placed->position = (t_vector3f){
		.x = engine->camera.position.x + engine->object_being_placed_distance \
			* direction.x,
		.y = engine->camera.position.y + engine->object_being_placed_distance \
			* direction.y,
		.z = engine->camera.position.z + engine->object_being_placed_distance \
			* direction.z
	};
}

static void	update_mouse_position(t_engine *engine, t_vector2i *mouse_position)
{
	bool	should_update_mouse_position;

	should_update_mouse_position = (
			mouse_position->x >= engine->camera.viewport.size.x
			|| mouse_position->y >= engine->camera.viewport.size.y
			|| mouse_position->x < 0 || mouse_position->y < 0);
	if (should_update_mouse_position == false)
		return ;
	while (mouse_position->x >= engine->camera.viewport.size.x)
		mouse_position->x -= engine->camera.viewport.size.x;
	while (mouse_position->x < 0)
		mouse_position->x += engine->camera.viewport.size.x;
	while (mouse_position->y >= engine->camera.viewport.size.y)
		mouse_position->y -= engine->camera.viewport.size.y;
	while (mouse_position->y < 0)
		mouse_position->y += engine->camera.viewport.size.y;
	mlx_mouse_move(engine->window.window, mouse_position->x, mouse_position->y);
}
