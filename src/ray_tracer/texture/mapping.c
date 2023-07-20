/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mapping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 15:14:00 by tdameros          #+#    #+#             */
/*   Updated: 2023/06/19 15:14:00 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include "object.h"
#include "math/vector.h"
#include "math/matrix.h"
#include "math/modulo.h"
#include "ray_tracer/texture.h"

t_vector2f	calculate_object_map(const t_vector3f hit_position,
								const t_object *object)
{
	if (object->type == SPHERE)
		return (calculate_spherical_map(hit_position, object));
	else if (object->type == CYLINDER || object->type == CYLINDER_INF)
		return (calculate_cylindrical_map(hit_position, object));
	else if (object->type == PLANE)
		return (calculate_planar_map(hit_position, object));
	return ((t_vector2f){0.f, 0.f});
}

t_vector2f	calculate_spherical_map(const t_vector3f hit_position,
									const t_object *sphere)
{
	const t_vector3f	position = vector3f_subtract(hit_position,
			sphere->position);
	const float			theta = atan2f(position.x, position.z);
	const float			phi = acosf(position.y / sphere->radius);
	const float			raw_u = (float)(theta / (2.f * M_PI));

	return (vector2f_create(1.f - (raw_u + 0.5f), (float)(1.f - phi / M_PI)));
}

#include "math/conversion.h"
#include "math/quaternion.h"

t_vector2f	calculate_cylindrical_map(const t_vector3f hit_position,
										const t_object *cylinder)
{
	t_vector3f	position;
	float		theta;
	float		raw_u;
	float		u;
	float		v;

	position = vector3f_subtract(hit_position,
			cylinder->position);

	float	rotation_theta = convert_radians_to_degrees(atan2f(cylinder->axe.x, cylinder->axe.z));
	float 	rotation_phi = convert_radians_to_degrees(acosf(cylinder->axe.y));

	position = quaternionf_rotate_vector3f(-rotation_theta, vector3f_create(0, 1, 0), position);
	position = quaternionf_rotate_vector3f(-rotation_phi, vector3f_create(1, 0, 0), position);

	theta = atan2f(position.x / cylinder->radius,
			position.z / cylinder->radius);
	raw_u = theta / (float)(2.f * M_PI);
	u = 1.f - (raw_u + 0.5f);
	v = (0.5f + position.y / cylinder->height);
	return (vector2f_create(u, v));
}

t_vector2f	calculate_planar_map(const t_vector3f hit_position,
									const t_object *plane)
{
	t_vector3f	position;
	float		u;
	float		v;

	position = vector3f_subtract(hit_position,
			plane->position);

	float	rotation_theta = convert_radians_to_degrees(atan2f(plane->axe.x, plane->axe.z));
	float 	rotation_phi = convert_radians_to_degrees(acosf(plane->axe.y));

	position = quaternionf_rotate_vector3f(-rotation_theta, vector3f_create(0, 1, 0), position);
	position = quaternionf_rotate_vector3f(-rotation_phi, vector3f_create(1, 0, 0), position);

	u = modulof_positive(position.x, 1);
	v = modulof_positive(position.z, 1);
	return (vector2f_create(u, v));
}

t_vector2f	calculate_cap_map(const t_vector3f hit_position,
								const t_object *object)
{
	t_vector3f	position;
	float		u;
	float		v;

	position = vector3f_subtract(hit_position, object->position);

	float	rotation_theta = convert_radians_to_degrees(atan2f(object->axe.x, object->axe.z));
	float 	rotation_phi = convert_radians_to_degrees(acosf(object->axe.y));

	position = quaternionf_rotate_vector3f(-rotation_theta, vector3f_create(0, 1, 0), position);
	position = quaternionf_rotate_vector3f(-rotation_phi, vector3f_create(1, 0, 0), position);

	u = modulof_positive(position.x / (object->radius * 2), 1);
	v = modulof_positive(position.z / (object->radius * 2), 1);
	return (vector2f_create(u, v));
}
