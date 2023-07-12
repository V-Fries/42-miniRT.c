/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_object_attributes_modification_box.c        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 11:08:00 by vfries            #+#    #+#             */
/*   Updated: 2023/06/01 11:08:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gui/box.h"
#include "hooks.h"

static int	init_object_attributes_modification_box_content(t_engine *engine,
				t_gui_box *gui_box, t_selected_object selected_object);

int	update_object_attributes_modification_box(t_engine *engine)
{
	destroy_t_gui_box(&engine->window,
		engine->gui.object_attributes_modification_box);
	return (init_object_attributes_modification_box_content(engine,
			engine->gui.object_attributes_modification_box,
			engine->gui.selected_object));
}

static int	init_object_attributes_modification_box_content(t_engine *engine,
				t_gui_box *gui_box, const t_selected_object selected_object)
{
	if (selected_object.object != NULL)
	{
		if (selected_object.object->type == SPHERE)
			return (init_sphere_attributes_modification_box(engine, gui_box));
		else if (selected_object.object->type == PLANE)
			return (init_plane_attributes_modification_box(engine, gui_box));
		else if (selected_object.object->type == CYLINDER)
			return (init_cylinder_attributes_modification_box(engine, gui_box));
		else if (selected_object.object->type == CONE)
			return (init_cone_attributes_modification_box(engine, gui_box));
	}
//	else if (selected_object.light != NULL)
//		// TODO light box
	ft_print_error("Object type not supported\n\n");
	return (0);
}
