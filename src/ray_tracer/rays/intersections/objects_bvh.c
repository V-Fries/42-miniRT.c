/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects_bvh.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 23:44:00 by tdameros          #+#    #+#             */
/*   Updated: 2023/09/15 23:44:00 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <float.h>

#include "ray_tracer/rays.h"
#include "ray_tracer/bvh.h"

bool ray_intersect_aabb(const t_ray *ray, const t_vector3f bmin,
						const t_vector3f bmax, t_hit *near_hit);
void	intersect_bvh(const t_ray *ray, const t_objects_bvh_node *node, t_hit *near_hit);

t_hit	objects_bvh_calculate_ray_intersection(const t_ray *ray, const t_objects_bvh_node *tree)
{
	t_hit	near_hit;

	near_hit = miss_hit();
	near_hit.distance = FLT_MAX;
	near_hit.index_obj = -1;

	intersect_bvh(ray, tree, &near_hit);
	if (near_hit.index_obj >= 0 && near_hit.distance > 0)
		near_hit.hit = true;
	else
		near_hit.hit = false;
	return (near_hit);
}

void	intersect_bvh(const t_ray *ray, const t_objects_bvh_node *node, t_hit *near_hit)
{
	if (!ray_intersect_aabb(ray, node->aabb_min, node->aabb_max, near_hit))
		return;
	if (node->is_leaf)
	{
		for (size_t i = 0; i < node->index_objects.length; i++)
		{
//			ft_printf("here\n");
			const t_object	*object = &node->objects->data[node->index_objects.data[i]];
			t_hit		hit = calculate_object_distance(ray, object);
			if (hit.distance > 0 && hit.distance < near_hit->distance)
			{
				*near_hit = hit;
				near_hit->index_obj = node->index_objects.data[i];
			}
		}
	}
	else
	{
		intersect_bvh(ray, node->left_node, near_hit);
		intersect_bvh(ray, node->right_node, near_hit);
	}
}

bool ray_intersect_aabb(const t_ray *ray, const t_vector3f bmin,
						 const t_vector3f bmax, t_hit *near_hit)
{
	float tmin = -FLT_MAX;
	float tmax = FLT_MAX;

	for (int a = 0; a < 3; ++a) {
		float invD = 1.0f / vector3f_get(ray->direction, a);
		float t0 = (vector3f_get(bmin,a) - vector3f_get(ray->origin, a)) * invD;
		float t1 = (vector3f_get(bmax,a) - vector3f_get(ray->origin, a)) * invD;
		if (invD < 0.0f) {
			float temp = t1;
			t1 = t0;
			t0 = temp;
		}

		tmin = t0 > tmin ? t0 : tmin;
		tmax = t1 < tmax ? t1 : tmax;

		if (tmax <= tmin)
			return false;
	}
	return (tmin < near_hit->distance && tmax >= ft_maxf(tmin, 0));
}