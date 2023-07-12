/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redraw_icons.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 13:56:00 by vfries            #+#    #+#             */
/*   Updated: 2023/07/12 13:56:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "gui/box.h"
#include "gui/UI.h"
#include "gui/utils.h"

static void	redraw_object_creation_box(t_gui_box *gui_box,
				enum e_object_type type, t_material material);

void	redraw_icons(t_engine *engine, t_material material)
{
	engine->gui.material_to_assign_to_new_objects = material;
	redraw_object_creation_box(engine->gui.object_creation_boxes->data + 0,
		SPHERE, material);
	redraw_object_creation_box(engine->gui.object_creation_boxes->data + 1,
		PLANE, material);
	redraw_object_creation_box(engine->gui.object_creation_boxes->data + 2,
		CYLINDER, material);
	redraw_object_creation_box(engine->gui.object_creation_boxes->data + 3,
		CONE, material);
}

static void	redraw_object_creation_box(t_gui_box *gui_box,
				const enum e_object_type type, const t_material material)
{
	change_image_color(&gui_box->image, COLOR_TRANSPARENT);
	change_image_color(&gui_box->on_hover_image, HOVER_GUI_COLOR);
	draw_icon(&gui_box->image, type, COLOR_TRANSPARENT, material);
	draw_icon(&gui_box->on_hover_image, type, HOVER_GUI_COLOR, material);
	round_image_corners(&gui_box->on_hover_image, BOX_ROUNDING_RADIUS);
	round_image_corners(&gui_box->image, BOX_ROUNDING_RADIUS);
}
