/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_press.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 18:37:00 by vfries            #+#    #+#             */
/*   Updated: 2023/07/30 18:37:01 by vfries           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"

#include "events.h"
#include "engine.h"
#include "hooks.h"
#include "export.h"

static bool	handle_command_hooks(int key_code, t_engine *engine);
static bool	handle_normal_hooks(int key_code, t_engine *engine);

int	key_press_handler(int key_code, t_engine *engine)
{
	if (handle_command_hooks(key_code, engine))
		return (0);
	if (handle_normal_hooks(key_code, engine))
		return (0);
	add_key_to_pressed_keys(engine, key_code);
	return (0);
}

static bool	handle_command_hooks(int key_code, t_engine *engine)
{
	if (key_code != KEY_COMMAND && engine->command_key_is_pressed == false)
		return (false);
	if (key_code == KEY_COMMAND)
	{
		engine->command_key_is_pressed = true;
		clear_pressed_keys(engine);
		mlx_do_key_autorepeatoff(engine->window.mlx);
	}
	else if (key_code == KEY_S)
	{
		if (export_scene(engine, engine->start_up_scene) < 0)
			ft_print_error("Warning: Failed to export scene.\n");
	}
	return (true);
}

static bool	handle_normal_hooks(int key_code, t_engine *engine)
{
	if (key_code == KEY_B)
	{
		ft_reverse_bool(&engine->is_black_and_white_render);
		engine->scene_changed = true;
	}
	else if (key_code == KEY_ESC)
		close_engine(engine);
	else if (key_code == KEY_H)
		toggle_gui(&engine->gui);
	else if (key_code == KEY_T)
	{
		if (take_screenshot(&engine->ray_traced_image) < 0)
			ft_print_error("Warning: Failed to take screenshot.\n");
		else
			engine->gui.screen_shot.last_screen_shot
				= ft_get_current_time_in_ms();
	}
	else
		return (false);
	return (true);
}
