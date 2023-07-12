/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_icon.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 01:03:00 by vfries            #+#    #+#             */
/*   Updated: 2023/07/09 01:03:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "ray_tracer/render.h"
#include "gui/utils.h"

static int	tmp_camera_create(t_camera *camera, t_vector2f viewport);
static int	init_tmp_scene(t_engine *tmp_engine, enum e_object_type type,
				t_material material, t_vector3f sky_color);

int	draw_icon(t_image *image, const enum e_object_type type,
		const unsigned int background_color, const t_material material)
{
	t_engine		tmp_engine;
	const t_color	sky_color = vector3f_divide(
			get_t_color_from_uint(background_color), 255.f);

	ft_bzero(&tmp_engine, sizeof(tmp_engine));
	tmp_engine.ray_traced_image = *image;
	if (tmp_camera_create(&tmp_engine.camera,
			(t_vector2f){image->width, image->height}) < 0)
		return (-1);
	if (init_tmp_scene(&tmp_engine, type, material, sky_color) < 0)
		return (-1);
	render_icon(&tmp_engine, background_color);
//	free(tmp_engine.camera.rays);
	free_objects(&tmp_engine.scene.objects);
	free_lights(&tmp_engine.scene.lights);
	return (0);
}

static int	tmp_camera_create(t_camera *camera, t_vector2f viewport)
{
	camera->viewport.number_of_pixels = viewport.x * viewport.y;
//	camera->rays = malloc(sizeof(*camera->rays)
//			* camera->viewport.number_of_pixels);
//	if (camera->rays == NULL)
//		return (-1);
	camera->viewport.size = viewport;
	camera->position = vector3f_create(0, 0, 0);
	camera->direction = vector3f_create(0, 0, -1);
	camera->up_vector = vector3f_create(0, 1, 0);
	camera->view = matrix4_create_identity();
	camera->inverse_view = matrix4_create_identity();
	camera->projection = matrix4_create_identity();
	camera->inverse_projection = matrix4_create_identity();
	camera->vertical_fov = 45;
	camera->near_clip = 0.1f;
	camera->far_clip = 100;
	camera->pitch = 0;
	camera->rotation_speed = 0.3f;
	camera->lock = true;
	camera_recalculate_view(camera);
	camera_recalculate_projection(camera);
//	camera_recalculate_rays(camera);
	return (0);
}

static int	init_tmp_scene(t_engine *tmp_engine, const enum e_object_type type,
				const t_material material, const t_vector3f sky_color)
{
	t_object		object;
	t_light			light;

	// TODO secure all the things

	initialize_objects_array(&tmp_engine->scene.objects, 1);
	initialize_lights_array(&tmp_engine->scene.lights, 1);

	if (type == SPHERE)
	{
//		t_vector3f	top = tmp_engine->camera.rays[((int)tmp_engine->camera.viewport.size.y / 15) * (int)tmp_engine->camera.viewport.size.x + (int)tmp_engine->camera.viewport.size.x / 2].direction;
//		t_vector3f	bottom = tmp_engine->camera.rays[((int)tmp_engine->camera.viewport.size.y - (int)tmp_engine->camera.viewport.size.y / 15) * (int)tmp_engine->camera.viewport.size.x + (int)tmp_engine->camera.viewport.size.x / 2].direction;
		t_vector3f	top = get_ray_direction(&tmp_engine->camera, tmp_engine->camera.viewport.size.y / 15.f, tmp_engine->camera.viewport.size.x / 2.f);
		t_vector3f	bottom = get_ray_direction(&tmp_engine->camera, tmp_engine->camera.viewport.size.y - tmp_engine->camera.viewport.size.y / 15.f, tmp_engine->camera.viewport.size.x / 2.f);
		top = vector3f_multiply(top, 2.f / top.z);
		bottom = vector3f_multiply(bottom, 2.f / bottom.z);
		float	radius = vector3f_length(vector3f_subtract(top, bottom)) / 2.f;
		object = sphere_create(vector3f_create(0, 0, -2.f),
				radius, material);
	}
	else if (type == PLANE)
		object = plane_create(vector3f_create(0, 0, -2),
				vector3f_create(0, 0, -1), material);
	else if (type == CYLINDER)
		object = cylinder_create(vector3f_create(0, 0, -2),
				vector3f_create(0, 1, 0), 1, 2.0f, material);
	else// if (type == CONE)
		object = cone_create(vector3f_create(0, 0, -2),
				vector3f_create(0, 1, 0), 2, 5, material);
	add_object_in_objects(&tmp_engine->scene.objects, object);

	light.color = vector3f_create(1, 1, 1);
	light.brightness = 0.5f;
	light.position = vector3f_create(5, 5, 5);
	add_light_in_lights(&tmp_engine->scene.lights, light);

	tmp_engine->scene.sky_color = sky_color;
	tmp_engine->scene.ambient_light.color = vector3f_create(1, 1, 1);
	tmp_engine->scene.ambient_light.brightness = 0.2f;
	return (0);
}
