/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_horizontal_boxes.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 18:00:00 by vfries            #+#    #+#             */
/*   Updated: 2023/06/02 18:00:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <errno.h>

#include "gui/box.h"
#include "gui/utils.h"

static void	create_boxes(t_engine *engine, t_gui_box *gui_box,
				t_boxes_to_create boxes_size, int side_offset);

///
/// \param engine
/// \param gui_box
/// \param boxes_setup "20 60 20" would mean having 3 boxes, 2 at the edges that
/// take 20 percent of available place, one in the center that takes 60 percent
/// of the available place. The sum of all numbers numbers needs to be > 0
///  && <= 100
/// \return
void	create_vertical_boxes(t_engine *engine, t_gui_box *gui_box,
		const char *boxes_setup, int side_offset)
{
	t_boxes_to_create	boxes_size;

	ft_bzero(&gui_box->children, sizeof(gui_box->children));
	if (side_offset * 2 >= gui_box->size.x)
		ft_fatal_error("create_vertical_boxes: side_offset too big");
	boxes_size = get_boxes_size(boxes_setup);
	gui_box->children.data = malloc(sizeof(*gui_box->children.data)
			* boxes_size.nb_of_boxes);
	if (gui_box->children.data == NULL)
		ft_fatal_error("create_vertical_boxes: malloc error");
	gui_box->children.size = boxes_size.nb_of_boxes;
	create_boxes(engine, gui_box, boxes_size, side_offset);
	free(boxes_size.box_size);
}

static void	create_boxes(t_engine *engine, t_gui_box *gui_box,
				t_boxes_to_create boxes_size, int side_offset)
{
	int		position;
	size_t	i;

	position = 0;
	i = -1;
	while (++i < boxes_size.nb_of_boxes)
	{
		gui_box->children.data[i] = create_t_gui_box(engine, \
			(t_gui_box_create){
				gui_box,
				(t_vector2i){\
					.x = side_offset, \
					.y = position}, \
				(t_vector2i){\
					.x = gui_box->size.x - side_offset * 2, \
					.y = (i + 1 == boxes_size.nb_of_boxes) \
							* (gui_box->size.y - position) \
						+ (i + 1 != boxes_size.nb_of_boxes) \
							* (gui_box->size.y \
							* (boxes_size.box_size[i] / 100.f))}, false});
		position += gui_box->children.data[i].size.y;
	}
}
