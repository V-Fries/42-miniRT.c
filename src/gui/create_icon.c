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

#include <math.h>
#include "engine.h"
#include "math/conversion.h"
#include "ray_tracer/render.h"

static int	init_tmp_engine(t_engine *tmp_engine, t_image *image,
				enum e_object_type type, t_vector3f sky_color);
static int	tmp_camera_create(t_camera *camera, t_vector2f viewport);
static int	init_tmp_scene(t_scene *scene, const t_camera *camera,
				enum e_object_type type, const t_vector3f sky_color);

void	draw_icon(t_image *image, const enum e_object_type type,
			const unsigned int sky_color_int)
{
	t_engine	tmp_engine;
	t_color		sky_color;

	sky_color = vector3f_divide(get_t_color_from_uint(sky_color_int), 255.f);
	init_tmp_engine(&tmp_engine, image, type, sky_color); // TODO secure me
	render_anti_aliased_raytracing(&tmp_engine);
	free(tmp_engine.camera.rays);
	free_objects(&tmp_engine.scene.objects);
	free_lights(&tmp_engine.scene.lights);
}

static int	init_tmp_engine(t_engine *tmp_engine, t_image *image,
				const enum e_object_type type, const t_vector3f sky_color)
{
	ft_bzero(tmp_engine, sizeof(*tmp_engine));
	tmp_engine->ray_traced_image = *image;
	if (tmp_camera_create(&tmp_engine->camera,
			(t_vector2f){image->width, image->height}) < 0)
		return (-1);
	if (init_tmp_scene(&tmp_engine->scene, &tmp_engine->camera, type,
			sky_color) < 0)
		return (-1);
	return (0);
}

static int	tmp_camera_create(t_camera *camera, t_vector2f viewport)
{
	camera->viewport.number_of_pixels = viewport.x * viewport.y;
	camera->rays = malloc(sizeof(*camera->rays)
			* camera->viewport.number_of_pixels);
	if (camera->rays == NULL)
		return (-1);
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
	camera_recalculate_rays(camera);
	return (0);
}
#include "stdio.h"
static int	init_tmp_scene(t_scene *scene, const t_camera *camera,
				const enum e_object_type type, const t_vector3f sky_color)
{
	t_object		object;
	t_light			light;
	t_material		material;

	// TODO secure all the things

	initialize_objects_array(&scene->objects, 1);
	initialize_lights_array(&scene->lights, 1);

	material = material_create(vector3f_create(1, 0, 0), 0, 0);
	material.is_checked_pattern = false;
	if (type == SPHERE)
	{
		t_vector3f	top = camera->rays[(int)((camera->viewport.size.y / 15.f) * camera->viewport.size.x + camera->viewport.size.x / 2.f)].direction;
		t_vector3f	bottom = camera->rays[(int)((camera->viewport.size.y - camera->viewport.size.y / 15.f) * camera->viewport.size.x + camera->viewport.size.x / 2.f)].direction;

		float degrees = convert_radians_to_degrees(acos(vector3f_dot(vector3f_create(0, 0, -2.f), top) / 2.f));
		float distance = atan(degrees) * 2.f;
		top = vector3f_multiply(top, distance / vector3f_length(top));
		degrees = convert_radians_to_degrees(acos(vector3f_dot(vector3f_create(0, 0, -2.f), bottom) / 2.f));
		distance = atan(degrees) * 2.f;
		bottom = vector3f_multiply(bottom, distance / vector3f_length(bottom));
		ft_swap_float(&top.x, &top.y);
		ft_swap_float(&bottom.x, &bottom.y);
		float	radius = vector3f_length(vector3f_subtract(top, bottom)) / 2.f;
		object = sphere_create(vector3f_create(0, 0, -2),
				radius, material);
		printf("top\n");
		vector3f_print(top);
		printf("bottom\n");
		vector3f_print(bottom);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
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
	add_object_in_objects(&scene->objects, object);

	light.color = vector3f_create(1, 1, 1);
	light.brightness = 0.5f;
	light.position = vector3f_create(5, 5, 5);
	add_light_in_lights(&scene->lights, light);

	scene->sky_color = sky_color;
	scene->ambient_light.color = vector3f_create(1, 1, 1);
	scene->ambient_light.brightness = 0.2f;
	return (0);
}
