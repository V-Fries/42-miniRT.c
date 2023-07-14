/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_normal_box.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 05:16:00 by vfries            #+#    #+#             */
/*   Updated: 2023/06/03 05:16:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gui/box.h"
#include "hooks.h"
#include "font/render.h"

int	add_normal_box(t_engine *engine, t_gui_box *gui_box, int *i,
		t_gui_box *parent)
{
	if (add_x_y_z_box(engine, gui_box, i, parent) < 0)
		return (-1);
	write_centered_string_to_image(&engine->gui.font,
		&gui_box->children.data[0].image, "Rotation");
	get_x_y_z_input_box(gui_box, 'x', '-')->on_click \
		= &normal_input_box_x_on_click_minus;
	get_x_y_z_input_box(gui_box, 'x', 't')->on_click \
		= &normal_input_box_x_on_click_text;
	get_x_y_z_input_box(gui_box, 'x', '+')->on_click \
		= &normal_input_box_x_on_click_plus;
	engine->gui.float_input_boxes.normal.x
		= get_x_y_z_input_box(gui_box, 'x', 'b');
	get_x_y_z_input_box(gui_box, 'y', '-')->on_click \
		= &normal_input_box_y_on_click_minus;
	get_x_y_z_input_box(gui_box, 'y', 't')->on_click \
		= &normal_input_box_y_on_click_text;
	get_x_y_z_input_box(gui_box, 'y', '+')->on_click \
		= &normal_input_box_y_on_click_plus;
	engine->gui.float_input_boxes.normal.y
		= get_x_y_z_input_box(gui_box, 'y', 'b');
	get_x_y_z_input_box(gui_box, 'z', '-')->on_click \
		= &normal_input_box_z_on_click_minus;
	get_x_y_z_input_box(gui_box, 'z', 't')->on_click \
		= &normal_input_box_z_on_click_text;
	get_x_y_z_input_box(gui_box, 'z', '+')->on_click \
		= &normal_input_box_z_on_click_plus;
	engine->gui.float_input_boxes.normal.z
		= get_x_y_z_input_box(gui_box, 'z', 'b');
	return (0);
}
