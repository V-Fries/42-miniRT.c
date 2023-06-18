/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 16:01:00 by tdameros          #+#    #+#             */
/*   Updated: 2023/05/07 16:01:00 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"

#include "scene.h"
#include "object.h"

int	init_scene(t_scene *scene)
{
	t_objects		objects;
	t_object		object;
	t_material		material;

	initialize_objects_array(&objects, 10);

	material = material_create(vector3f_create(1, 0, 1), 0, 0);
	material.reflect = 0.90f;
	object = sphere_create(vector3f_create(-4, 1, 0), 1.5f, material);
	add_object_in_objects(&objects, object);

	// Plane
	material = material_create(vector3f_create(0.8f, 0.5f, 0.2f), 0.1f, 0);
	object = plane_create(vector3f_create(0.0f, -0.5f, 0),
						  vector3f_create(0, 1, 0), material);
	add_object_in_objects(&objects, object);
	material = material_create(vector3f_create(1.0f, 1.0f, 0.2f), 0.1f, 0);

	object = plane_create(vector3f_create(0.0f, 0.0f, 10.0f),
						  vector3f_create(0, 0, -1), material);
	add_object_in_objects(&objects, object);

	material = material_create(vector3f_create(1.0f, 0.0f, 0.2f), 0.1f, 0);
	object = plane_create(vector3f_create(0.0f, 0.0f, -10.0f),
						  vector3f_create(0, 0, 1), material);
	add_object_in_objects(&objects, object);

	material = material_create(vector3f_create(0.7f, 0.3f, 0.2f), 0.1f, 0);
	object = plane_create(vector3f_create(10.0f, 0.0f, 0.0f),
						  vector3f_create(-1, 0, 0), material);
	add_object_in_objects(&objects, object);

	material = material_create(vector3f_create(0.7f, 0.3f, 0.9f), 0.1f, 0);
	object = plane_create(vector3f_create(-10.0f, 0.0f, 0.0f),
						  vector3f_create(1, 0, 0), material);
	add_object_in_objects(&objects, object);

	material = material_create(vector3f_create(0.3f, 0.3f, 0.3f), 0.1f, 0);
	object = plane_create(vector3f_create(0.0f, 10.0f, 0.0f),
						  vector3f_create(0, -1, 0), material);
	add_object_in_objects(&objects, object);

	material = material_create(vector3f_create(0.2f, 0.3f, 1.0f), 0.1f, 0);
	object = sphere_create(vector3f_create(2, -101, 0), 100, material);
//	add_object_in_objects(&objects, object);

	scene->sky_color = vector3f_create(0, 0, 0);
	scene->light.position = vector3f_create(5, 5, 5);
	scene->light.color = vector3f_create(1, 1, 1);
	scene->light.brightness = 0.5f;
	scene->ambient_light.color = vector3f_create(1, 1, 1);
	scene->ambient_light.brightness = 0;
	scene->objects = objects;
	return (0);
}
