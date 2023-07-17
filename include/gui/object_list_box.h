/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_list_box.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 11:50:00 by vfries            #+#    #+#             */
/*   Updated: 2023/07/16 11:50:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */
#ifndef OBJECT_LIST_BOX_H
# define OBJECT_LIST_BOX_H

# include "engine.h"
# include "gui/box.h"

int	init_object_list_box(t_engine *engine, t_gui_box *gui_box,
		const t_gui_box *main_gui_box,
		const t_gui_box *object_modification_gui_box);

#endif