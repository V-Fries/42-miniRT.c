/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 18:34:00 by tdameros          #+#    #+#             */
/*   Updated: 2023/08/08 18:34:00 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include "object.h"
#include "vectors.h"

static char	*get_obj_name(const char *obj_file);
static int	init_cache(t_object *mesh_object);

/*
 * Return code:
 * 1 mesh object has been successful initialize
 * 0 obj_file is empty
 * -1 an error has been encountered
 */
int	mesh_object_initialize(t_object *mesh_object, const char *obj_file,
				t_material material)
{
	int	return_code;

	ft_bzero(mesh_object, sizeof(*mesh_object));
	mesh_object->type = MESH;
	mesh_object->material = material;
	return_code = initialize_mesh_with_obj(&mesh_object->mesh, obj_file);
	if (return_code < 0)
		return (return_code);
	mesh_object->name = get_obj_name(obj_file);
	if (mesh_object->name == NULL)
	{
		mesh_free(&mesh_object->mesh);
		return (-1);
	}
	if (init_cache(mesh_object) < 0)
	{
		mesh_free(&mesh_object->mesh);
		return (-1);
	}
	return (0);
}

static char	*get_obj_name(const char *obj_file)
{
	char	*obj_file_without_full_path;
	char	*obj_name;

	obj_file_without_full_path = ft_strrchr(obj_file, '/');
	if (obj_file_without_full_path != NULL)
	{
		obj_file_without_full_path++;
		obj_name = ft_substr(obj_file_without_full_path, 0,
				ft_strlen(obj_file_without_full_path) - 4);
	}
	else
		obj_name = ft_substr(obj_file, 0, ft_strlen(obj_file) - 4);
	if (obj_name != NULL)
		obj_name[0] = ft_toupper(obj_name[0]);
	return (obj_name);
}

void	mesh_free(t_mesh *mesh)
{
	vectors3f_free(&mesh->base_vertex);
	vectors3f_free(&mesh->normals);
	mesh_faces_free(&mesh->faces);
	ft_bzero(mesh, sizeof(*mesh));
}

static int	init_cache(t_object *mesh_object)
{
	t_mesh_object_cache	*cache;

	cache = &mesh_object->cache.mesh;
	if (vectors3f_initialize(&cache->vertex,
			mesh_object->mesh.base_vertex.length) < 0)
		return (-1);
	cache->vertex.length = mesh_object->mesh.base_vertex.length;
	cache->translation = create_translation_matrix(mesh_object->position);
	cache->scale_vector = (t_vector3f){1, 1, 1};
	cache->scale = create_scale_matrix(cache->scale_vector);
	mesh_object->axis = (t_vector3f){0, 0, 0};
	mesh_object->axis_degrees = mesh_object->axis;
	cache->rotation = create_rotation_matrix(mesh_object->axis);
	mesh_object_calculate_cache(mesh_object);
	return (0);
}
