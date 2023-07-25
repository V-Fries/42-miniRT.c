/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_checkered_pattern_toggle_box.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 18:30:00 by vfries            #+#    #+#             */
/*   Updated: 2023/07/19 18:30:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "gui/box.h"
#include "gui/UI.h"
#include "hooks.h"
#include "font/render.h"
#include "events.h"

static void	checkered_pattern_toggle_box_on_click(t_gui_box *self,
				t_engine *engine, t_click_data click_data);
static void	update_checkered_pattern_color_toggle_box(t_engine *engine);
static void	update_images(t_engine *engine, t_gui_box *gui_box);

int	add_checkered_pattern_toggle_box(t_engine *engine, t_gui_box *gui_box,
		int *i, t_gui_box *parent)
{
	if (add_toggle_box(engine, gui_box, i, parent) < 0)
		return (-1);
	update_images(engine, gui_box->children.data);
	change_image_color(&gui_box->children.data[1].image, COLOR_TRANSPARENT);
	write_centered_string_to_image(&engine->gui.font,
		&gui_box->children.data[1].image, "Checked pattern");
	gui_box->children.data->on_click = &checkered_pattern_toggle_box_on_click;
	return (0);
}

static void	checkered_pattern_toggle_box_on_click(t_gui_box *self,
				t_engine *engine, t_click_data click_data)
{
	t_object	*object;

	object = engine->gui.selected_object.object;
	if (click_data.button != BUTTON_LEFT || object == NULL)
		return ;
	if (object->material.texture.outline_type == CHECKERBOARD)
	{
		object->material.texture.cap_type = NONE;
		object->material.texture.outline_type = NONE;
	}
	else
	{
		object->material.texture.cap_type = CHECKERBOARD;
		object->material.texture.outline_type = CHECKERBOARD;
	}
	update_checkered_pattern_color_toggle_box(engine);
	object->material.texture.outline_checkerboard.size.y = 5;
	object->material.texture.outline_checkerboard.size.x = 5;
	object->material.texture.outline_checkerboard.albedo = (t_vector3f){
		1.f - object->material.albedo.x,
		1.f - object->material.albedo.y,
		1.f - object->material.albedo.z};
	object->material.texture.cap_checkerboard.size.y = 5;
	object->material.texture.cap_checkerboard.size.x = 5;
	object->material.texture.cap_checkerboard.albedo = (t_vector3f){
			1.f - object->material.albedo.x,
			1.f - object->material.albedo.y,
			1.f - object->material.albedo.z};
	object_calculate_cache(object);
	update_images(engine, self);
	redraw_icons(engine, engine->gui.selected_object.object->material);
	update_xy_float_input_boxes(engine,
		object->material.texture.outline_checkerboard.size,
		&engine->gui.float_input_boxes.checkered_pattern_size);
	engine->scene_changed = true;
}

static void	update_checkered_pattern_color_toggle_box(t_engine *engine)
{
	if (engine->gui.selected_object.object->material.texture.outline_type
		!= CHECKERBOARD)
		checkered_pattern_color_toggle_box_on_click(
			engine->gui.checkered_pattern_color_toggle_box, engine,
			(t_click_data){(t_vector2i){0}, BUTTON_LEFT});
}

static void	update_images(t_engine *engine, t_gui_box *gui_box)
{
	change_image_color(&gui_box->image, COLOR_TRANSPARENT);
	change_image_color(&gui_box->on_hover_image, HOVER_GUI_COLOR);
	if (engine->gui.selected_object.object->material.texture.outline_type
		== CHECKERBOARD)
	{
		image_draw_check_mark(&gui_box->image, COLOR_WHITE,
			TOGGLE_BOX_BUTTON_OUTLINE_WIDTH);
		image_draw_check_mark(&gui_box->on_hover_image, COLOR_WHITE,
			TOGGLE_BOX_BUTTON_OUTLINE_WIDTH);
	}
	image_draw_outline(&gui_box->image, TOGGLE_BOX_BUTTON_OUTLINE_WIDTH,
		COLOR_BLACK);
	image_draw_outline(&gui_box->on_hover_image,
		TOGGLE_BOX_BUTTON_OUTLINE_WIDTH, COLOR_BLACK);
}
