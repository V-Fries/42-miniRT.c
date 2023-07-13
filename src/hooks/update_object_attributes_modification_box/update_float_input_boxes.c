/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_xyz_float_input_box.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 18:38:00 by vfries            #+#    #+#             */
/*   Updated: 2023/07/13 18:38:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "gui/utils.h"

static void	update_float_input_boxes_object(t_engine *engine);
static void	update_float_input_boxes_light(t_engine *engine);

void	update_xyz_float_input_boxes(t_engine *engine,
			const t_vector3f xyz,
			t_xyz_input_boxes_images *xyz_input_boxes_images)
{
	update_float_input_box(engine, xyz.x, xyz_input_boxes_images->x);
	update_float_input_box(engine, xyz.y, xyz_input_boxes_images->y);
	update_float_input_box(engine, xyz.z, xyz_input_boxes_images->z);
}

void	update_float_input_boxes(t_engine *engine)
{
	if (engine->gui.selected_object.object != NULL)
		return (update_float_input_boxes_object(engine));
	if (engine->gui.selected_object.light != NULL)
		return (update_float_input_boxes_light(engine));
}

static void	update_float_input_boxes_object(t_engine *engine)
{
	update_float_input_box(engine,
		engine->gui.selected_object.object->height,
		engine->gui.float_input_boxes.height);
	update_xyz_float_input_boxes(engine,
		engine->gui.selected_object.object->axe,
		&engine->gui.float_input_boxes.normal);
	update_xyz_float_input_boxes(engine,
		engine->gui.selected_object.object->position,
		&engine->gui.float_input_boxes.position);
	update_float_input_box(engine,
		engine->gui.selected_object.object->radius,
		engine->gui.float_input_boxes.radius);
	update_float_input_box(engine,
		engine->gui.selected_object.object->material.reflect,
		engine->gui.float_input_boxes.reflection);
	update_float_input_box(engine,
		engine->gui.selected_object.object->material.specular,
		engine->gui.float_input_boxes.specular_reflection);
}

static void	update_float_input_boxes_light(t_engine *engine)
{
	update_float_input_box(engine,
		engine->gui.selected_object.light->brightness,
		engine->gui.float_input_boxes.brightness);
	update_xyz_float_input_boxes(engine,
		engine->gui.selected_object.light->position,
		&engine->gui.float_input_boxes.position);
}

