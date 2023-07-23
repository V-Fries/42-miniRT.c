/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 21:01:00 by tdameros          #+#    #+#             */
/*   Updated: 2023/06/16 21:01:00 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "math/vector.h"
#include "object.h"
#include "ray_tracer/rays.h"
#include "ray_tracer/mapping.h"

static t_vector3f	calculate_cylinder_normal(const t_ray *ray,
						const t_object *cylinder,
						const t_hit hit_distance);
static bool			is_in_outline_cylinder(const t_ray *ray,
						const t_object *cylinder,
						const float distance);

t_hit	hit_cylinder(const t_ray *ray, const t_object *cylinder,
					const t_hit hit_distance)
{
	t_hit	hit;

	hit.distance = hit_distance.distance;
	if (hit.distance < 0)
	{
		hit.hit = false;
		return (hit);
	}
	hit.hit = true;
	hit.position = ray_at(ray, hit.distance);
	hit.normal = calculate_cylinder_normal(ray, cylinder, hit_distance);
	if (vector3f_dot(hit.normal, ray->direction) > 0)
		hit.normal = vector3f_multiply(hit.normal, -1);
	hit.object = cylinder;
	hit.ray = *ray;
	if (hit.object->material.is_checkered_pattern)
		hit.albedo = get_checked_pattern(hit, cylinder);
	else
		hit.albedo = cylinder->material.albedo;
	return (hit);
}

static t_vector3f	calculate_cylinder_normal(const t_ray *ray,
											const t_object *cylinder,
											const t_hit hit_distance)
{
	if (hit_distance.context == CAP1)
		return (cylinder->cache.cylinder.cap1_normal);
	if (hit_distance.context == CAP2)
		return (cylinder->cache.cylinder.cap2_normal);
	return (calculate_infinite_cylinder_normal(ray, cylinder,
			hit_distance.distance));
}

t_hit	calculate_cylinder_distance(const t_ray *ray, const t_object *cylinder)
{
	const t_hit	outline_distance = calculate_inf_cylinder_distance(
			ray, cylinder);
	t_hit		caps_distance;
	t_hit		hit;

	hit.distance = -1;
	hit.context = OUTLINE;
	if (outline_distance.distance < 0)
		return (hit);
	if (is_in_outline_cylinder(ray, cylinder, outline_distance.distance))
	{
		caps_distance = calculate_cylinder_caps_distance(ray, cylinder);
		hit.distance = ft_minf_positive(outline_distance.distance,
				caps_distance.distance);
		if (hit.distance == caps_distance.distance)
			return (caps_distance);
		return (outline_distance);
	}
	hit = calculate_cylinder_caps_distance(ray, cylinder);
	return (hit);
}

static bool	is_in_outline_cylinder(const t_ray *ray, const t_object *cylinder,
		const float distance)
{
	const t_vector3f	position = ray_at(ray, distance);
	const t_vector3f	endpoint1_position = vector3f_subtract(position,
			cylinder->cache.cylinder.endpoint1);
	const t_vector3f	endpoint2_position = vector3f_subtract(position,
			cylinder->cache.cylinder.endpoint2);

	return (vector3f_dot(endpoint1_position, cylinder->axe) > 0
		&& vector3f_dot(endpoint2_position, cylinder->axe) < 0);
}
