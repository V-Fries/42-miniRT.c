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
static void	render_minirt(t_engine *engine)
{
	update_placed_object_position(engine);
//	render_raytracing(engine);
	change_image_color(&engine->ray_traced_image, COLOR_BLACK);

//	t_glyph_outline	letter_c = engine->gui.font.glyphs[get_glyph_index('t', &engine->gui.font.ttf)];
//
//	static bool print = true;
//
//	for (int16_t i = 0; i < letter_c.endPtsOfContours[letter_c.numberOfContours-1]; i++) {
//		int y = engine->ray_traced_image.height / 2 - letter_c.yCoordinates[i] + (letter_c.yMax) / 2;
//		int x = engine->ray_traced_image.width / 2 + letter_c.xCoordinates[i] - (letter_c.xMax) / 2;
//		put_pixel_on_image(&engine->ray_traced_image, y, x, COLOR_WHITE);
//		if (print) {
//			printf("y = %i, x = %i\n", y, x);
//		}
//	}
//	print = false;

	t_vector2f	points[3] = {{0, 300}, {800, 4}, {15, 600}};
	size_t		number_of_points = 100000;
	t_vector2f	*bezier_points = get_bezier_points(points, number_of_points);

	for (size_t i = 0; i < number_of_points; i++)
		put_pixel_on_image(&engine->ray_traced_image, bezier_points[i].y, bezier_points[i].x, COLOR_WHITE);
	free(bezier_points);

	put_pixel_on_image(&engine->ray_traced_image, points[0].y, points[0].x, COLOR_RED);
	put_pixel_on_image(&engine->ray_traced_image, points[1].y, points[1].x, COLOR_RED);
	put_pixel_on_image(&engine->ray_traced_image, points[2].y, points[2].x, COLOR_RED);

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
