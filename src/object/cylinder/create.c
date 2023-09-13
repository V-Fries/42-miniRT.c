/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/09 00:29:42 by tdameros          #+#    #+#             */
/*   Updated: 2023/07/09 00:29:44 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include "object.h"

t_object	cylinder_create(const t_vector3f position, const t_vector3f axis,
							const t_object_size size, const t_material material)
{
	t_object	cylinder;

	ft_bzero(&cylinder, sizeof(cylinder));
	cylinder.type = CYLINDER;
	cylinder.position = position;
	cylinder.axis = axis;
	cylinder.radius = size.radius;
	cylinder.height = size.height;
	cylinder.material = material;
	cylinder.name = ft_strdup("Cylinder");
	cylinder_calculate_cache(&cylinder);
	return (cylinder);
}

void	cylinder_calculate_cache(t_object *cylinder)
{
	cylinder->cache.cone.endpoint1 = vector3f_subtract(cylinder->position,
			vector3f_multiply(cylinder->axis, cylinder->height / 2));
	cylinder->cache.cone.endpoint2 = vector3f_add(cylinder->position,
			vector3f_multiply(cylinder->axis, cylinder->height / 2));
	cylinder->cache.cylinder.cap1_normal = vector3f_multiply(cylinder->axis,
			-1);
	cylinder->cache.cylinder.cap2_normal = cylinder->axis;
	cylinder->cache.cylinder.cap1_d = -vector3f_dot(\
	cylinder->cache.cylinder.cap1_normal, cylinder->cache.cylinder.endpoint1);
	cylinder->cache.cylinder.cap2_d = -vector3f_dot(\
	cylinder->cache.cylinder.cap2_normal, cylinder->cache.cylinder.endpoint2);
}

t_object	cylinder_infinite_create(const t_vector3f position,
									const t_vector3f axis,
									const float radius,
									const t_material material)
{
	t_object	cylinder;

	cylinder.type = CYLINDER_INF;
	cylinder.position = position;
	cylinder.axis = axis;
	cylinder.radius = radius;
	cylinder.material = material;
	return (cylinder);
}

void	cylinder_calculate_bounding_box(t_object *cylinder)
{
	t_vector3f	center_bottom = vector3f_add(cylinder->position, vector3f_multiply(cylinder->axis, cylinder->height / 2));
	t_vector3f	center_top = vector3f_subtract(cylinder->position, vector3f_multiply(cylinder->axis, cylinder->height / 2));
	t_vector3f	circle_vector1;
	t_vector3f	circle_vector2;

	circle_vector1 = vector3f_cross(cylinder->axis, (t_vector3f){0.f, 1.f, 0.f});
	if (vector3f_length(circle_vector1) == 0)
		circle_vector1 = vector3f_cross(cylinder->axis, (t_vector3f){1.f, 0.f, 0.f});
	circle_vector1 = vector3f_unit(circle_vector1);
	circle_vector2 = vector3f_unit(vector3f_cross(circle_vector1, cylinder->axis));
	circle_vector1 = vector3f_multiply(circle_vector1, cylinder->radius);
	circle_vector2 = vector3f_multiply(circle_vector2, cylinder->radius);

	cylinder->bounding_box.a = vector3f_add(center_bottom, vector3f_add(circle_vector1, circle_vector2));
	cylinder->bounding_box.b = vector3f_add(center_bottom, vector3f_subtract(circle_vector2, circle_vector1));
	cylinder->bounding_box.c = vector3f_subtract(center_bottom, vector3f_add(circle_vector1, circle_vector2));
	cylinder->bounding_box.d = vector3f_add(center_bottom, vector3f_subtract(circle_vector1, circle_vector2));

	cylinder->bounding_box.e = vector3f_add(center_top, vector3f_add(circle_vector1, circle_vector2));
	cylinder->bounding_box.f = vector3f_add(center_top, vector3f_subtract(circle_vector2, circle_vector1));
	cylinder->bounding_box.g = vector3f_subtract(center_top, vector3f_add(circle_vector1, circle_vector2));
	cylinder->bounding_box.h = vector3f_add(center_top, vector3f_subtract(circle_vector1, circle_vector2));

}
