/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 00:02:00 by tdameros          #+#    #+#             */
/*   Updated: 2023/05/11 00:02:00 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef GUI_H
# define GUI_H

# include <sys/time.h>
# include <stdbool.h>

# include "gui/boxes.h"
# include "image.h"
# include "engine.h"

typedef struct s_fps
{
	struct timeval	last_update;
	char			*fps_count;
}	t_fps;

typedef struct s_gui
{
	bool		is_hidden;
	double		hidden_ratio;
	t_gui_boxes	gui_boxes;
	bool		color_picker_base_color_was_changed;
	t_color		color_picker_base_color;
	void		(*draw_gui_image)(t_image *destination, \
					const t_image *source, t_vector2i position);
	t_fps		fps;
}	t_gui;

#endif