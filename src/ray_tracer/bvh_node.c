/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_node.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 20:58:00 by tdameros          #+#    #+#             */
/*   Updated: 2023/09/14 20:58:00 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <float.h>

#include "libft.h"

#include "object.h"
#include "vectors.h"
#include "ray_tracer/bvh.h"


t_bvh_node	*bvh_node_create(const t_objects *objects)
{
	t_bvh_node	*node;

	node = malloc(sizeof(*node));
	if (node == NULL)
		return (NULL);
	ft_bzero(node, sizeof(*node));
	node->objects = objects;
	// TODO: secure
	vectors_int_initialize(&node->index_objects, 10);
	return (node);
}

void	bvh_node_free(t_bvh_node *node)
{
	vectors_int_free(&node->index_objects);
	free(node);
}

void	bvh_tree_free(t_bvh_node *root_node)
{
	if (root_node == NULL)
		return;
	bvh_tree_free(root_node->left_node);
	bvh_tree_free(root_node->right_node);
	bvh_node_free(root_node);
}

t_bvh_node	*bvh_node_create_root(const t_objects	*objects)
{
	t_bvh_node	*root_node;

	root_node = bvh_node_create(objects);
	if (root_node == NULL)
		return (NULL);
	for (size_t i = 0; i < objects->length; i++)
	{
		t_object object = objects->data[i];
		if (object.type == SPHERE || object.type == CYLINDER || object.type == CONE)
			vectors_int_add(&root_node->index_objects, (int)i);
	}
	root_node->nb_split_objects = objects->length;
	return (root_node);
}

void	bvh_node_update_bounding_box(t_bvh_node *node)
{
	t_vector3f	min;
	t_vector3f	max;
	t_object 	object;


	min = (t_vector3f){FLT_MAX, FLT_MAX, FLT_MAX};
	max = (t_vector3f){-FLT_MAX, -FLT_MAX, -FLT_MAX};
	for (size_t i = 0; i < node->index_objects.length; i++)
	{
		object = node->objects->data[node->index_objects.data[i]];
		if (object.type == SPHERE || object.type == CYLINDER || object.type == CONE)
		{
			min = vector3f_min(min, object.bounding_box.aabb_min);
			max = vector3f_max(max, object.bounding_box.aabb_max);
		}
	}
	node->aabb_min = min;
	node->aabb_max = max;
}

float	get(t_vector3f vector3f, int i)
{
	if (i == 0)
		return (vector3f.x);
	else if (i == 1)
		return (vector3f.y);
	return (vector3f.z);
}


enum e_bvh_side	get_bounding_box_side(t_bounding_box *bh, int axis, int split_pos)
{
	bool	left = false;
	bool	right = false;

	for (int i = 0; i < 4; i++)
	{
		if (get(bh->top_face[i], axis) < split_pos)
			left = true;
		else
			right = true;
	}
	for (int i = 0; i < 4; i++)
	{
		if (get(bh->bottom_face[i], axis) < split_pos)
			left = true;
		else
			right = true;
	}
	if (left && right)
		return (LEFT_AND_RIGHT);
	else if (left)
		return (LEFT);
	return (RIGHT);
}

void	bvh_subdivide(t_bvh_node *node, enum e_bvh_side side)
{
	if (node->index_objects.length <= 2)
	{
		node->is_leaf = true;
		return;
	}
	t_vector3f	extent = vector3f_subtract(node->aabb_max, node->aabb_min);

	int axis = 0;
	if (extent.y > extent.x)
		axis = 1;
	if (extent.z > get(extent, axis))
		axis = 2;
	float splitPos = get(node->aabb_min, axis) + get(extent, axis) * 0.5f;
	node->left_node = bvh_node_create(node->objects);
	node->right_node = bvh_node_create(node->objects);

	// Secure
	node->left_node->previous_node = node;
	node->right_node->previous_node = node;
	node->nb_split_objects = node->index_objects.length;

	for (size_t i = 0; i < node->index_objects.length; i++)
	{
		size_t index_obj = node->index_objects.data[i];
		t_bounding_box	*box = &node->objects->data[index_obj].bounding_box;
		enum e_bvh_side side = get_bounding_box_side(box, axis, splitPos);
		if (side == LEFT_AND_RIGHT || side == LEFT)
			vectors_int_add(&node->left_node->index_objects, index_obj);
		if (side == LEFT_AND_RIGHT || side == RIGHT)
			vectors_int_add(&node->right_node->index_objects, index_obj);
	}
	if (node->previous_node != NULL && (node->left_node->index_objects.length == node->previous_node->nb_split_objects
		|| node->right_node->index_objects.length == node->previous_node->nb_split_objects))
	{
		bvh_node_free(node->left_node);
		bvh_node_free(node->right_node);
		(void) side;
		node->previous_node->left_node = NULL;
		node->previous_node->right_node = NULL;
		node->previous_node->is_leaf = true;
		node->previous_node->index_objects = node->index_objects;
		node->index_objects.data = NULL;
		bvh_node_free(node);
		return;
	}
	vectors_int_free(&node->index_objects);
	bvh_node_update_bounding_box(node->left_node);
	bvh_node_update_bounding_box(node->right_node);
	bvh_subdivide(node->left_node, LEFT);
	bvh_subdivide(node->right_node, RIGHT);
}