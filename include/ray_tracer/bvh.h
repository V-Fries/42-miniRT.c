/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 21:49:00 by tdameros          #+#    #+#             */
/*   Updated: 2023/09/14 21:49:00 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef BVH_HPP
# define BVH_HPP

#include "math/vector.h"
#include "vectors.h"
#include "object.h"

typedef struct s_bvh_node
{
	t_vector3f				aabb_min;
	t_vector3f				aabb_max;
	struct s_bvh_node		*left_node;
	struct s_bvh_node		*right_node;
	bool					is_leaf;
	t_vectors_int			index_objects;
	const t_objects			*objects;
	struct s_bvh_node		*previous_node;
	size_t					nb_split_objects;
}	t_bvh_node;

enum e_bvh_side
{
	LEFT,
	RIGHT,
	LEFT_AND_RIGHT,
};

t_bvh_node	*bvh_node_create(const t_objects *objects);
void		bvh_node_free(t_bvh_node *node);
t_bvh_node	*bvh_node_create_root(const t_objects	*objects);
void		bvh_node_update_bounding_box(t_bvh_node *node);
void		bvh_subdivide(t_bvh_node *node, enum e_bvh_side side);

void	bvh_tree_free(t_bvh_node *root_node);
#endif