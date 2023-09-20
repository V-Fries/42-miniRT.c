/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_object.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 18:42:20 by vfries            #+#    #+#             */
/*   Updated: 2023/05/07 18:42:20 by vfries           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECT_H
# define OBJECT_H

# include <stddef.h>
# include <stdbool.h>

# include "math/vector.h"
# include "colors.h"
# include "material.h"
# include "mesh.h"
#include "math/matrix.h"

# define LIGHT (-1)

enum e_object_type
{
	SPHERE,
	PLANE,
	CYLINDER,
	CYLINDER_INF,
	CONE,
	MESH,
};

typedef struct s_cone_cache
{
	t_vector3f	endpoint1;
	t_vector3f	endpoint2;
	float		radius_divide_height;
	t_vector3f	cap_normal;
	float		cap_d;
}	t_cone_cache;

typedef struct s_cylinder_cache
{
	t_vector3f	endpoint1;
	t_vector3f	endpoint2;

	t_vector3f	cap1_normal;
	float		cap1_d;
	t_vector3f	cap2_normal;
	float		cap2_d;
}	t_cylinder_cache;

typedef struct s_plane_cache
{
	float	d;
}	t_plane_cache;

typedef struct s_sphere_cache
{
	float	square_radius;
}	t_sphere_cache;

typedef struct s_mesh_object_cache
{
	t_matrix4	rotation;
	t_matrix4	translation;
	t_matrix4	scale;
	t_vector3f	scale_vector;
	t_vectors3f	vertex;
	t_vectors3f	normals;
	char		*obj_file_path;
}	t_mesh_object_cache;

union u_object_cache
{
	t_cone_cache		cone;
	t_cylinder_cache	cylinder;
	t_plane_cache		plane;
	t_sphere_cache		sphere;
	t_mesh_object_cache	mesh;
};

typedef struct s_object_size
{
	float	radius;
	float	height;
}	t_object_size;

typedef struct s_bounding_box
{
	t_vector3f	top_face[4];
	t_vector3f	bottom_face[4];
	t_vector3f	a;
	t_vector3f	b;
	t_vector3f	c;
	t_vector3f	d;
	t_vector3f	e;
	t_vector3f	f;
	t_vector3f	g;
	t_vector3f	h;
	t_vector3f	aabb_min;
	t_vector3f	aabb_max;
}	t_bounding_box;

typedef struct s_object
{
	enum e_object_type		type;
	t_vector3f				position;
	float					radius;
	float					height;
	t_vector3f				axis;
	t_vector3f				axis_degrees;
	t_material				material;
	union u_object_cache	cache;
	char					*name;
	t_mesh					mesh;
	t_bounding_box			bounding_box;
}	t_object;

typedef struct s_objects
{
	t_object			*data;
	size_t				length;
	size_t				size;
}	t_objects;


//	cone/create.c
t_object	cone_create(const t_vector3f position, const t_vector3f axis,
				const t_object_size size, const t_material material);
void		cone_calculate_cache(t_object *cone);
void		cone_calculate_bounding_box(t_object *cone);

//	cone/transformations.c
void		cone_move(t_object *cone, const t_vector3f movement_axis,
				const float distance);
void		cone_rotate(t_object *cone, const t_vector3f rotation_axis,
				const float degrees);
void		cone_set_position(t_object *cone, const t_vector3f position);
void		cone_set_height(t_object *cone, const float height);
void		cone_set_radius(t_object *cone, const float radius);

//	cylinder/create.c
t_object	cylinder_create(const t_vector3f position, const t_vector3f axis,
				const t_object_size size, const t_material material);
void		cylinder_calculate_cache(t_object *cylinder);
t_object	cylinder_infinite_create(const t_vector3f position,
				const t_vector3f axis,
				const float radius,
				const t_material material);
void		cylinder_calculate_bounding_box(t_object *cylinder);

//	cylinder/transformations.c
void		cylinder_move(t_object *cylinder, const t_vector3f movement_axis,
				const float distance);
void		cylinder_rotate(t_object *cylinder, const t_vector3f rotation_axis,
				const float degrees);
void		cylinder_set_position(t_object *cylinder,
				const t_vector3f position);
void		cylinder_set_height(t_object *cylinder, const float height);
void		cylinder_set_radius(t_object *cylinder, const float radius);

//	mesh/cache.c
void		mesh_object_update_vertex(t_object *mesh_object);
void		mesh_object_update_normals(t_object *mesh_object);
void		mesh_cache_free(t_mesh_object_cache *cache);

//	mesh/create.c
int			mesh_object_initialize(t_object *mesh_object, const char *obj_file,
				t_material material);
void		mesh_free(t_mesh *mesh);
void		mesh_calculate_bounding_box(t_object *mesh_object);

// mesh/mesh_deep_copy.c
int			mesh_deep_copy(t_mesh *dst, const t_mesh *src);

//	mesh/transformations.c
void		mesh_object_move(t_object *mesh_object,
				const t_vector3f movement_axis, const float distance);
void		mesh_object_set_position(t_object *mesh_object,
				const t_vector3f position);
void		mesh_object_set_rotation(t_object *mesh_object,
				const t_vector3f rotation_axis);
void		mesh_object_set_scale(t_object *mesh_object,
				const t_vector3f scale);
//	mesh/utils.c
t_vector3f	mesh_get_vertexes_from_face(const t_object *mesh_object,
										  size_t face_index, int vertex_index);


//	plane/create.c
t_object	plane_create(const t_vector3f position, const t_vector3f normal,
				const t_material material);
void		plane_calculate_cache(t_object *plane);

//	plane/transformations.c
void		plane_move(t_object *plane, const t_vector3f movement_axis,
				const float distance);
void		plane_rotate(t_object *plane, const t_vector3f rotation_axis,
				const float degrees);
void		plane_set_position(t_object *plane, const t_vector3f position);

//	sphere/create.c
t_object	sphere_create(const t_vector3f position, const float radius,
				const t_material material);
void		sphere_calculate_cache(t_object *sphere);
void		sphere_calculate_bounding_box(t_object *sphere);

//	sphere/transformations.c
void		sphere_move(t_object *sphere, const t_vector3f movement_axis,
				const float distance);
void		sphere_set_position(t_object *sphere, const t_vector3f position);
void		sphere_set_radius(t_object *sphere, const float radius);

// object_deep_copy.c
int			object_deep_copy(t_object *dst, const t_object *src);

//	objects.c
int			initialize_objects_array(t_objects *objects, size_t size);
int			add_object_in_objects(t_objects *objects, t_object object);
int			remove_object_in_objects(t_objects *objects, size_t index);
void		free_object(t_object *object);
void		free_objects(t_objects *objects);

//	bounding_box.c
void		object_calculate_aabb_min_max(t_object *object);
void		object_calculate_bounding_box(t_object *object);

//	calculate_cache.c
void		object_calculate_cache(t_object *object);

//	transformations.c
void		object_move(t_object *object, const t_vector3f movement_axis,
				const float distance);
void		object_rotate(t_object *object, const t_vector3f rotation_axis,
				const float distance);
void		object_set_position(t_object *object, const t_vector3f position);
void		object_set_height(t_object *object, const float height);
void		object_set_radius(t_object *object, const float radius);

#endif