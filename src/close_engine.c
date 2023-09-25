/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_miniRT.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 03:07:36 by tdameros          #+#    #+#             */
/*   Updated: 2023/09/24 03:07:37 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "mlx.h"

#include "engine.h"
#include "gui/box.h"

static void	destroy_gui(t_engine *minirt);

int	close_engine(t_engine *engine)
{
	destroy_gui(engine);
	destroy_t_image(&engine->window, &engine->main_image);
	if (engine->command_key_is_pressed)
		mlx_do_key_autorepeaton(engine->window.mlx);
	if (engine->window.mlx != NULL && engine->window.window != NULL)
		mlx_destroy_window(engine->window.mlx, engine->window.window);
	// TODO destroy font
	free_scene(&engine->scene);
	camera_free(&engine->camera);
	free(engine->start_up_scene);
//	ft_bzero(engine, sizeof(*engine));
	exit(0); // TODO: free everything
}

static void	destroy_gui(t_engine *minirt)
{
	while (minirt->gui.gui_boxes.size--)
		destroy_t_gui_box(&minirt->window,
			minirt->gui.gui_boxes.data + minirt->gui.gui_boxes.size);
	free(minirt->gui.gui_boxes.data);
	free(minirt->gui.fps.fps_count);
}