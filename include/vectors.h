/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 11:40:00 by tdameros          #+#    #+#             */
/*   Updated: 2023/08/08 11:40:00 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTORS_H
# define VECTORS_H

# include <stdlib.h>

# include "math/vector.h"

typedef struct s_vectors3f
{
	t_vector3f			*data;
	size_t				length;
	size_t				size;
}	t_vectors3f;

typedef struct s_vectors3i
{
	t_vector3i			*data;
	size_t				length;
	size_t				size;
}	t_vectors3i;

typedef struct s_mesh_face
{
	t_vector2i	vertex_a;
	t_vector2i	vertex_b;
	t_vector2i	vertex_c;
}	t_mesh_face;

typedef struct s_mesh_faces
{
	t_mesh_face			*data;
	size_t				length;
	size_t				size;
}	t_mesh_faces;

//	vectors3f.c
int	vectors3f_initialize(t_vectors3f *vectors3f, size_t size);
int	vectors3f_add(t_vectors3f *vectors3f, t_vector3f vector3f);
int	vectors3f_remove(t_vectors3f *vectors3f, const size_t index);
int	vectors3f_free(t_vectors3f *vectors3f);

//	vectors3i.c
int	vectors3i_initialize(t_vectors3i *vectors3i, size_t size);
int	vectors3i_add(t_vectors3i *vectors3i, t_vector3i vector3i);
int	vectors3i_remove(t_vectors3i *vectors3i, const size_t index);
int	vectors3i_free(t_vectors3i *vectors3i);

//	mesh_faces.c
int	mesh_faces_initialize(t_mesh_faces *mesh_faces, size_t size);
int	mesh_faces_add(t_mesh_faces *mesh_faces, t_mesh_face mesh_face);
int	mesh_faces_remove(t_mesh_faces *mesh_faces, const size_t index);
int	mesh_faces_free(t_mesh_faces *mesh_faces);

#endif