/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_texture_box.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 18:25:17 by vfries            #+#    #+#             */
/*   Updated: 2023/09/01 18:25:18 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include "engine.h"
#include "gui/UI.h"
#include "gui/box.h"
#include "gui/utils.h"
#include "gui/optional_boxes.h"

static void	init_children_boxes(t_engine *engine, t_gui_box *gui_box);

void	init_texture_box(t_engine *engine, t_gui_box *gui_box,
			const t_gui_box *main_gui_box, const t_gui_box *object_list_box)
{
	*gui_box = create_optional_box(engine, main_gui_box, object_list_box);
	init_children_boxes(engine, gui_box);
}

static void	init_children_boxes(t_engine *engine, t_gui_box *gui_box)
{
	create_vertical_boxes(engine, gui_box, "1 6 1 6 1 6 1 77 1",
		roundf(gui_box->size.y / 100.f));
	init_outline_cap_picker(engine, gui_box->children.data + 1);
	init_texture_normal_map_picker(engine, gui_box->children.data + 3);
	init_delete_box(engine, gui_box->children.data + 5);
	init_selection_boxes(engine, gui_box->children.data + 7);
}
