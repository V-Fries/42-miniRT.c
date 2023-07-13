/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   z.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 00:15:00 by vfries            #+#    #+#             */
/*   Updated: 2023/06/03 00:15:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gui/box.h"
#include "hooks.h"

void	position_input_box_z_on_click_plus(t_gui_box *self, t_engine *engine,
											int y, int x)
{
	t_object	*object;
	t_light		*light;

	(void)self;
	(void)y;
	(void)x;
	object = engine->gui.selected_object.object;
	light = engine->gui.selected_object.light;
	if (light != NULL)
	{
		engine->scene_changed = true;
		light->position.z += engine->gui.object_modification_amount;
		light_calculate_cache(light);
		update_xyz_float_input_boxes(engine, light->position,
			&engine->gui.float_input_boxes.position);
	}
	else if (object != NULL)
	{
		engine->scene_changed = true;
		object_move(object, (t_vector3f){0.f, 0.f, 1.f},
			engine->gui.object_modification_amount);
		update_xyz_float_input_boxes(engine, object->position,
			&engine->gui.float_input_boxes.position);
	}
}

void	position_input_box_z_on_click_minus(t_gui_box *self, t_engine *engine,
											int y, int x)
{
	t_object	*object;
	t_light		*light;

	(void)self;
	(void)y;
	(void)x;
	object = engine->gui.selected_object.object;
	light = engine->gui.selected_object.light;
	if (light != NULL)
	{
		engine->scene_changed = true;
		light->position.z -= engine->gui.object_modification_amount;
		light_calculate_cache(light);
		update_xyz_float_input_boxes(engine, light->position,
			&engine->gui.float_input_boxes.position);
	}
	else if (object != NULL)
	{
		engine->scene_changed = true;
		object_move(object, (t_vector3f){0.f, 0.f, 1.f},
			-engine->gui.object_modification_amount);
		update_xyz_float_input_boxes(engine, object->position,
			&engine->gui.float_input_boxes.position);
	}
}
