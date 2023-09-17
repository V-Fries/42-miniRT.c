/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadow.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/02 23:27:00 by tdameros          #+#    #+#             */
/*   Updated: 2023/07/02 23:27:00 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"
#include "ray_tracer/rays.h"

static t_hit	calculate_shadow_ray_intersection(const t_ray *ray,
					const float light_distance,
					const t_scene *scene);

bool	is_shadow_pixel(const t_scene *scene, const t_light light,
						const t_hit object_hit,
						const t_vector3f reverse_light_direction)
{
	const t_vector3f	new_position = vector3f_add(object_hit.position, \
			vector3f_multiply(vector3f_multiply(object_hit.ray.direction, -1), \
			HIT_DISPLACEMENT));
	const t_ray			shadow_ray = ray_create(new_position, \
								reverse_light_direction);
	const float			light_distance = vector3f_length(
			vector3f_subtract(light.position, object_hit.position));
	const t_hit			shadow_hit = calculate_shadow_ray_intersection(\
								&shadow_ray, light_distance, scene);

	return (shadow_hit.hit);
}

static t_hit	calculate_shadow_ray_intersection(const t_ray *ray,
												const float light_distance,
												const t_scene *scene)
{
	size_t		index;
	t_hit		hit;

	index = 0;
	while (index < scene->objects.length)
	{
		if (scene->objects.data[index].type == PLANE)
		{
			hit = calculate_object_distance(ray, scene->objects.data + index);
			if (hit.distance > 0.f && hit.distance < light_distance)
				return (hit_object(ray, scene->objects.data + index, hit));
		}
		index++;
	}
	t_hit bvh_hit = objects_bvh_calculate_ray_intersection(ray, scene->bvh_tree);
	if (bvh_hit.distance > 0.f && bvh_hit.distance < light_distance)
		return (hit_object(ray, scene->objects.data + bvh_hit.index_obj, bvh_hit));
	return (miss_hit());
}
