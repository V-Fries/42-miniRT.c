#include "object.h"

void	mesh_object_calculate_cache(t_object *mesh_object)
{
	size_t		i;
//	t_matrix4	global_matrix;

	if (mesh_object->type != MESH)
		return ;
//	global_matrix = matrix4_multiply(&mesh_object->cache.mesh.translation,
//			&mesh_object->cache.mesh.rotation);
//	global_matrix = matrix4_multiply(&global_matrix,
//			&mesh_object->cache.mesh.scale);
	i = mesh_object->cache.mesh.vertex.length;
	while (i--)
		mesh_object->cache.mesh.vertex.data[i] = matrix4_multiply_vector3(
				&mesh_object->cache.mesh.translation, mesh_object->mesh.base_vertex.data[i]);
}
