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

int	add_checkered_pattern_toggle_box(t_engine *engine, t_gui_box *gui_box,
		int *i, t_gui_box *parent)
{
	if (add_toggle_box(engine, gui_box, i, parent) < 0)
		return (-1);
	change_image_color(&gui_box->children.data->image, COLOR_TRANSPARENT);
	if (engine->gui.selected_object.object->material.is_checkered_pattern)
		image_draw_check_mark(&gui_box->children.data->image, COLOR_WHITE,
			TOGGLE_BOX_BUTTON_OUTLINE_WIDTH);
	image_draw_outline(&gui_box->children.data->image,
		TOGGLE_BOX_BUTTON_OUTLINE_WIDTH, COLOR_BLACK);
	change_image_color(&gui_box->children.data->on_hover_image,
		HOVER_GUI_COLOR);
	if (engine->gui.selected_object.object->material.is_checkered_pattern)
		image_draw_check_mark(&gui_box->children.data->on_hover_image,
			COLOR_WHITE, TOGGLE_BOX_BUTTON_OUTLINE_WIDTH);
	image_draw_outline(&gui_box->children.data->on_hover_image,
		TOGGLE_BOX_BUTTON_OUTLINE_WIDTH, COLOR_BLACK);
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
	object->material.is_checkered_pattern
		= !object->material.is_checkered_pattern;
	update_checkered_pattern_color_toggle_box(engine);
	object->material.checkered_pattern_size.y = 5;
	object->material.checkered_pattern_size.x = 5;
	object->material.checkered_pattern_albedo = (t_vector3f){
		1.f - object->material.albedo.x,
		1.f - object->material.albedo.y,
		1.f - object->material.albedo.z};
	object_calculate_cache(object);
	change_image_color(&self->image, COLOR_TRANSPARENT);
	if (engine->gui.selected_object.object->material.is_checkered_pattern)
		image_draw_check_mark(&self->image, COLOR_WHITE,
			TOGGLE_BOX_BUTTON_OUTLINE_WIDTH);
	image_draw_outline(&self->image, TOGGLE_BOX_BUTTON_OUTLINE_WIDTH,
		COLOR_BLACK);
	change_image_color(&self->on_hover_image, HOVER_GUI_COLOR);
	if (engine->gui.selected_object.object->material.is_checkered_pattern)
		image_draw_check_mark(&self->on_hover_image, COLOR_WHITE,
			TOGGLE_BOX_BUTTON_OUTLINE_WIDTH);
	image_draw_outline(&self->on_hover_image, TOGGLE_BOX_BUTTON_OUTLINE_WIDTH,
		COLOR_BLACK);
	redraw_icons(engine, engine->gui.selected_object.object->material);
	engine->scene_changed = true;
}

static void	update_checkered_pattern_color_toggle_box(t_engine *engine)
{
	if (!engine->gui.selected_object.object->material.is_checkered_pattern)
		checkered_pattern_color_toggle_box_on_click(
			engine->gui.checkered_pattern_color_toggle_box, engine,
			(t_click_data){(t_vector2i){0}, BUTTON_LEFT});
}
