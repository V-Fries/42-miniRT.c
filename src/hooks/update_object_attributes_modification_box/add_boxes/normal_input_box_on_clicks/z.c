/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   z.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 00:16:00 by vfries            #+#    #+#             */
/*   Updated: 2023/06/03 00:16:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */


#include "gui/box.h"
#include "hooks.h"

void	normal_input_box_z_on_click_plus(t_gui_box *self, t_engine *engine,
										int y, int x)
{
	t_object			*object;

	(void)self;
	(void)y;
	(void)x;
	object = engine->gui.selected_object.object;
	if (object == NULL)
		return ;
	object_rotate(object, (t_vector3f){0.f, 0.f, 1.f},
		engine->gui.object_rotation_degrees);
	engine->scene_changed = true;
	update_xyz_float_input_boxes(engine, object->axe,
		&engine->gui.float_input_boxes.normal);
}

void	normal_input_box_z_on_click_minus(t_gui_box *self, t_engine *engine,
										int y, int x)
{
	t_object			*object;

	(void)self;
	(void)y;
	(void)x;
	object = engine->gui.selected_object.object;
	if (object == NULL)
		return ;
	object_rotate(object, (t_vector3f){0.f, 0.f, 1.f},
		-engine->gui.object_rotation_degrees);
	engine->scene_changed = true;
	update_xyz_float_input_boxes(engine, object->axe,
		&engine->gui.float_input_boxes.normal);
}
