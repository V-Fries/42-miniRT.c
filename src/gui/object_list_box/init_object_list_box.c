/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_object_list_box.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 11:39:00 by vfries            #+#    #+#             */
/*   Updated: 2023/07/16 11:39:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>

#include "engine.h"
#include "gui/box.h"
#include "gui/UI.h"
#include "gui/utils.h"

static int	init_object_gui_box_children(t_engine *engine, t_gui_box *parent,
				const t_object *object);

int	init_object_list_box(t_engine *engine, t_gui_box *gui_box,
		const t_gui_box *main_gui_box,
		const t_gui_box *object_modification_gui_box)
{
	*gui_box = create_t_gui_box(engine, (t_gui_box_create){NULL,
			(t_vector2i){main_gui_box->position.x, \
						object_modification_gui_box->position.y},
			(t_vector2i){object_modification_gui_box->size.x / 4.f * 3.f, \
						object_modification_gui_box->size.y / 3},
			true});
	if (errno == EINVAL || errno == ENOMEM)
		return (-1);
	change_image_color(&gui_box->image, BASE_GUI_COLOR);
	round_image_corners(&gui_box->image, BOX_ROUNDING_RADIUS);
	engine->gui.object_list_box = gui_box;
	ft_vector_create(&engine->gui.object_boxes, sizeof(t_gui_box *), 0);
	return (0);
}

t_gui_box	*create_object_gui_box(t_engine *engine, const t_object *object)
{
	t_gui_box	*result;

	result = malloc(sizeof(*result));
	if (result == NULL)
		return (NULL);
	*result = create_t_gui_box(engine, (t_gui_box_create){
			engine->gui.object_list_box,
			(t_vector2i){4, 4},
			(t_vector2i){engine->gui.object_list_box->size.x - 8, \
						100},
			true});
	if (errno == EINVAL || errno == ENOMEM)
		return (NULL);
	change_image_color(&result->image, SUB_GUI_COLOR);
	round_image_corners(&result->image, BOX_ROUNDING_RADIUS);
	if (init_object_gui_box_children(engine, result, object) < 0)
		return (NULL); // TODO free
	return (result);
}

static int	init_object_gui_box_children(t_engine *engine, t_gui_box *parent,
				const t_object *object)
{
	if (create_n_horizontal_boxes(engine, parent, 1, (t_boxes_offsets){4, 4})
		< 0)
		return (-1);
	if (init_image(&parent->children.data->image, &engine->window,
			parent->children.data->size.x, parent->children.data->size.y) < 0)
		return (-1);
	(void)object;
	return (0);
}
