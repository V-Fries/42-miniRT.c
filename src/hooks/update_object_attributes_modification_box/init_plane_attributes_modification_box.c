/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_plane_attributes_modification_box.c           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 11:32:00 by vfries            #+#    #+#             */
/*   Updated: 2023/06/01 11:32:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "gui/box.h"
#include "hooks.h"

int	init_plane_attributes_modification_box(t_engine *engine,
		t_gui_box *gui_box)
{
	int	i;

	gui_box->children.size = 1;
	gui_box->children.data = malloc(sizeof(*gui_box->children.data)
			* gui_box->children.size);
	if (gui_box->children.data == NULL)
	{
		gui_box->children.size = 0;
		return (-1);
	}
	i = 0;
	if (add_position_box(engine, gui_box->children.data, &i, gui_box) < 0)
	{
		gui_box->children.size = 0;
		free(gui_box->children.data);
		gui_box->children.data = NULL;
		return (-1);
	}
	return (0);
}
