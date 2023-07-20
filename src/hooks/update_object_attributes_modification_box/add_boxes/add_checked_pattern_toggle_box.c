/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_checked_pattern_toggle_box.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 18:30:00 by vfries            #+#    #+#             */
/*   Updated: 2023/07/19 18:30:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include "engine.h"
#include "gui/box.h"
#include "gui/object_modification_box.h"
#include "gui/UI.h"
#include "hooks.h"

static int	init_checked_pattern_toggle_box_children(t_engine *engine,
				t_gui_box *gui_box);

int	add_checked_pattern_toggle_box(t_engine *engine, t_gui_box *gui_box, int *i,
		t_gui_box *parent)
{
	*gui_box = create_t_gui_box(engine, (t_gui_box_create){parent, \
		(t_vector2i){\
				.x = 0, \
				.y = *i}, \
		(t_vector2i){\
				.x = parent->size.x, \
				.y = get_normal_box_size(parent)}, \
		true});
	if (errno == EINVAL || errno == ENOMEM)
		return (-1);
	*i += gui_box->size.y + OBJECT_ATTRIBUTE_BOX_OFFSET;
	if (init_checked_pattern_toggle_box_children(engine, gui_box) < 0)
	{
		destroy_t_image(&engine->window, &gui_box->image);
		ft_bzero(gui_box, sizeof(*gui_box));
		return (-1);
	}
	gui_box->draw = &default_gui_box_draw;
	gui_box->on_click = &default_gui_box_on_click;
	change_image_color(&gui_box->image, SUB_GUI_COLOR);
	round_image_corners(&gui_box->image, BOX_ROUNDING_RADIUS);
	return (0);
}

static int	init_checked_pattern_toggle_box_children(t_engine *engine,
				t_gui_box *gui_box)
{

}
