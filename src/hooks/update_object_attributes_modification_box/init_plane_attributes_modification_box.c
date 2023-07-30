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

static void	add_transformation_boxes(t_engine *engine, t_gui_box *gui_box);
static void	add_reflection_boxes(t_engine *engine, t_gui_box *gui_box);
static void	add_texture_boxes(t_engine *engine, t_gui_box *gui_box);

void	init_plane_attributes_modification_box(t_engine *engine,
			t_gui_box *gui_box)
{
	gui_box->children.size = 6;
	gui_box->children.data = malloc(sizeof(*gui_box->children.data)
			* gui_box->children.size);
	if (gui_box->children.data == NULL)
		ft_fatal_error("init_plane_attributes_modification_box: "
			"malloc failed");
	add_transformation_boxes(engine, gui_box);
	add_reflection_boxes(engine, gui_box);
	add_texture_boxes(engine, gui_box);
}

static void	add_transformation_boxes(t_engine *engine, t_gui_box *gui_box)
{
	int	y;

	y = get_transformations_boxes_index(gui_box);
	add_position_box(engine, gui_box->children.data, &y, gui_box);
	add_normal_box(engine, gui_box->children.data + 1, &y, gui_box);
}

static void	add_reflection_boxes(t_engine *engine, t_gui_box *gui_box)
{
	int	y;

	y = get_reflections_boxes_index(gui_box);
	add_reflection_box(engine, gui_box->children.data + 2, &y, gui_box);
	add_specular_reflection_box(engine, gui_box->children.data + 3, &y,
		gui_box);
}

static void	add_texture_boxes(t_engine *engine, t_gui_box *gui_box)
{
	int	y;

	y = get_texture_boxes_index(gui_box);
	add_outline_checkerboard_size_box(engine, gui_box->children.data + 4,
		&y, gui_box);
	add_checkered_pattern_color_toggle_box(engine, gui_box->children.data + 5,
		&y, gui_box);
}
