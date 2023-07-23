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
#include "light.h"
#include "engine.h"
#include "ray_tracer_gui_api.h"

int	init_scene(t_engine *engine)
{
	t_object		object;
	t_light			light;
	t_material		material;

	initialize_objects_array(&engine->scene.objects, 10);
	initialize_lights_array(&engine->scene.lights, 10);

//	material = material_create(vector3f_create(1, 0, 1), 0, 0);
//	material.is_checked_pattern = true;
//	material.checked_pattern_albedo = vector3f_create(0, 0, 0);
//	material.checked_pattern_size = vector2f_create(20, 10);
//	material.checked_pattern_albedo = vector3f_create(1, 0, 0);
//	material.reflect = 0.90f;,
//	object = sphere_create(vector3f_create(0, 0, 0), 1.5f, material);
//	add_object(engine, object);

	material = material_create(vector3f_create(1, 0, 0), 0, 0);
	material.is_checkered_pattern = false;
	material.checkered_pattern_albedo = vector3f_create(0, 0, 0);
	material.checkered_pattern_size = vector2f_create(20, 10);
	material.checkered_pattern_albedo = vector3f_create(1, 1, 1);
	material.specular = 0.5f;
//	object = sphere_create(vector3f_create(0, 0, 0), 1, material);
//	object = plane_create(vector3f_create(0, 0, 0), vector3f_create(0, 1, 0), material);
	t_object_size	size = (t_object_size){1, 2.0f};
//	object = cone_create(vector3f_create(0, 1, 1), vector3f_create(0, 1, 0), size, material);
	object = cone_create(vector3f_create(-2, 1, 0), vector3f_create(0, 1, 0), size, material);
//	object = cylinder_create(vector3f_create(1, 1, 1), vector3f_create(0, 1, 0), 1, 2.0f, material);
	add_object(engine, object);
	// Plane
	material = material_create(vector3f_create(1.0f, 1.0f, 1.0f), 0.1f, 0);
//	material.is_damier = true;
	object = plane_create(vector3f_create(0.0f, -1.f, 0),
						  vector3f_create(0, 1, 0), material);
	add_object(engine, object);


	material = material_create(vector3f_create(0.f, 1.f, 0.62f), 0.f, 0.f);
	object  = sphere_create(vector3f_create(-6.822397f, 1.227619f, 6.196026f), 0.7f, material);
	add_object(engine, object);

	material = material_create(vector3f_create(0.f, 1.f, 0.62f), 0.f, 0.f);
	size = (t_object_size){1, 2.0f};
	object  = cylinder_create(vector3f_create(-3.966613f, 1.685901f, 3.352188f),
							  vector3f_create(-0.632640f, -0.560695f, 0.534217f),
							  size, material);
	add_object(engine, object);

//	light.color = vector3f_create(1, 0, 0);
//	light.brightness = 0.5f;
//	light.position = vector3f_create(5, 5, 5);
//	add_light(engine, light);
//
//	light.color = vector3f_create(0, 1, 0);
//	light.brightness = 0.5f;
//	light.position = vector3f_create(-5, 5, 5);
//	add_light(engine, light);
//
//	light.color = vector3f_create(0, 0, 1);
//	light.brightness = 0.5f;
//	light.position = vector3f_create(5, 5, -5);
//	add_light(engine, light);
	light = light_create(vector3f_create(5, 5, 5), vector3f_create(1, 0, 0), 0.5f);
	add_light(engine, light);

	light = light_create(vector3f_create(-5, 5, 5), vector3f_create(0, 1, 0), 0.5f);
	add_light(engine, light);

	light = light_create(vector3f_create(5, 5, -5), vector3f_create(0, 0, 1),0.5f);
	add_light(engine, light);

	engine->scene.ambient_light = light_create(vector3f_create(0, 0, 0),
			vector3f_create(1, 1, 1), 0.2f);
	engine->scene.sky_color = vector3f_create(0, 0, 0);
	return (0);
}
