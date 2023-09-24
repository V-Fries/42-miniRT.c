/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_user_interface.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 03:02:21 by tdameros          #+#    #+#             */
/*   Updated: 2023/09/24 03:02:22 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include "mlx_wrapper.h"
#include "engine.h"
#include "gui/box.h"
#include "gui/utils.h"

static float		get_gui_hidden_ration(t_gui *gui, uint64_t start_time);
static void			render_message(t_engine *engine);
static int16_t		get_message_transparency(const t_message *message,
						float time_elapsed);
static void			update_message_image_to_display(t_message *message,
						int16_t transparency);

void	render_user_interface(t_engine *engine, const uint64_t start_time)
{
	t_vector2i	mouse_position;

	engine->gui.hide_animation.current_hidden_ratio
		= get_gui_hidden_ration(&engine->gui, start_time);
	if (engine->gui.hide_animation.current_hidden_ratio >= 1.f)
		return ;
	mouse_position = engine->mouse_position;
	engine->gui.gui_boxes.data[0].draw(engine->gui.gui_boxes.data + 0, engine,
		(t_draw_data){\
			get_top_box_decal(engine, engine->gui.gui_boxes.data + 0), \
			mouse_position});
	engine->gui.gui_boxes.data[1].draw(engine->gui.gui_boxes.data + 1, engine,
		(t_draw_data){\
			get_right_box_decal(engine, engine->gui.gui_boxes.data + 1), \
			mouse_position});
	engine->gui.gui_boxes.data[2].draw(engine->gui.gui_boxes.data + 2, engine,
		(t_draw_data){\
			get_left_box_decal(engine, engine->gui.gui_boxes.data + 1), \
			mouse_position});
	if (engine->gui.current_optional_box >= 0
		&& engine->gui.current_optional_box < NUMBER_OF_OPTIONAL_BOXES
		&& engine->gui.optional_gui_boxes.data \
		[engine->gui.current_optional_box].draw != NULL)
		engine->gui.optional_gui_boxes.data[engine->gui.current_optional_box].\
		draw(engine->gui.optional_gui_boxes.data
			+ engine->gui.current_optional_box, engine, (t_draw_data){\
			get_left_box_decal(engine, engine->gui.gui_boxes.data + 1),
			mouse_position});
	render_message(engine);
}

#define TIME_TO_HIDE_GUI 180000.f

static float	get_gui_hidden_ration(t_gui *gui, const uint64_t start_time)
{
	const float	time_passed = start_time
		- gui->hide_animation.start_animation_time;
	const float	time_passed_squared = time_passed * time_passed;

	if (time_passed_squared > TIME_TO_HIDE_GUI)
	{
		gui->hide_animation.hide_animation_finished = true;
		if (gui->hide_animation.is_hidden == false)
			gui->draw_gui_image = &put_image_to_image_unsafe;
		return (gui->hide_animation.is_hidden);
	}
	if (gui->hide_animation.is_hidden)
	{
		return (time_passed_squared / TIME_TO_HIDE_GUI);
	}
	return (1.f - (time_passed_squared) / TIME_TO_HIDE_GUI);
}

static void	render_message(t_engine *engine)
{
	t_message		*message;
	const float		time_elapsed = ft_get_current_time_in_ms()
		- engine->gui.message.animation_start_time_ms;
	int16_t			transparency;

	message = &engine->gui.message;
	if (time_elapsed > message->time_to_appear_ms + message->time_to_display_ms
		+ message->time_to_disappear_ms)
		return ;
	transparency = get_message_transparency(message, time_elapsed);
	update_message_image_to_display(message, transparency);
	put_image(engine, &message->image_to_display, (t_vector2i){0, message->y_position});
}

static int16_t	get_message_transparency(const t_message *message,
					const float time_elapsed)
{
	if (time_elapsed < message->time_to_appear_ms)
		return (roundf((1.f - time_elapsed
					/ (message->time_to_appear_ms)) * 255.f));
	if (time_elapsed > message->time_to_appear_ms
		+ message->time_to_display_ms)
		return (roundf((time_elapsed - message->time_to_appear_ms
					- message->time_to_display_ms)
				/ message->time_to_display_ms * 255.f));
	return (0);
}

static void	update_message_image_to_display(t_message *message,
				const int16_t transparency)
{
	const unsigned int	*original;
	unsigned int		*display;
	int16_t				pixel_transparency;

	original = message->image_with_message.address;
	display = message->image_to_display.address;
	while (display < message->image_to_display.limit)
	{
		pixel_transparency = transparency + get_transparency(*original);
		pixel_transparency = ft_clamp(pixel_transparency, 0, 255);
		*display = (*original & 0x00FFFFFF) | (pixel_transparency << 24);
		original++;
		display++;
	}
}
