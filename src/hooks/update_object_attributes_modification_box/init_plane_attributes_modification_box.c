/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_plane_attributes_modification_box.c           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 11:32:00 by vfries            #+#    #+#             */
/*   Updated: 2023/06/01 11:32:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "gui/box.h"
#include "hooks.h"

static int	add_transformation_boxes(t_engine *engine, t_gui_box *gui_box);
static int	add_reflection_boxes(t_engine *engine, t_gui_box *gui_box);

int	init_plane_attributes_modification_box(t_engine *engine,
		t_gui_box *gui_box)
{
	gui_box->children.size = 4;
	gui_box->children.data = malloc(sizeof(*gui_box->children.data)
			* gui_box->children.size);
	if (gui_box->children.data == NULL)
	{
		gui_box->children.size = 0;
		return (-1);
	}
	if (add_transformation_boxes(engine, gui_box) < 0)
		return (-1);
	if (add_reflection_boxes(engine, gui_box) < 0)
		return (-1);
	return (0);
}

static int	add_transformation_boxes(t_engine *engine, t_gui_box *gui_box)
{
	int	y;

	y = get_transformations_boxes_index(gui_box);
	if (add_position_box(engine, gui_box->children.data, &y, gui_box) < 0)
	{
		gui_box->children.size = 0;
		free(gui_box->children.data);
		gui_box->children.data = NULL;
		return (-1);
	}
	if (add_normal_box(engine, gui_box->children.data + 1, &y, gui_box) < 0)
	{
		gui_box->children.size = 1;
		destroy_t_gui_box(&engine->window, gui_box);
		return (-1);
	}
	return (0);
}

static int	add_reflection_boxes(t_engine *engine, t_gui_box *gui_box)
{
	int	y;

	y = get_reflections_boxes_index(gui_box);
	if (add_reflection_box(engine, gui_box->children.data + 2, &y, gui_box) < 0)
	{
		gui_box->children.size = 2;
		destroy_t_gui_box(&engine->window, gui_box);
		return (-1);
	}
	if (add_specular_reflection_box(engine, gui_box->children.data + 3, &y,
			gui_box) < 0)
	{
		gui_box->children.size = 3;
		destroy_t_gui_box(&engine->window, gui_box);
		return (-1);
	}
	return (0);
}
