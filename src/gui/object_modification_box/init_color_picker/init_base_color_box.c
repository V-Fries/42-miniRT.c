/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_base_color_box.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 01:50:32 by vfries            #+#    #+#             */
/*   Updated: 2023/07/30 18:24:44 by vfries           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>

#include "mlx.h"

#include "engine.h"
#include "gui/box.h"
#include "colors.h"
#include "gui/object_modification_box.h"
#include "gui/utils.h"
#include "hooks.h"
#include "events.h"

typedef struct s_color_getter
{
	int	red;
	int	(*get_red)(float color, float x, float decal, float ratio);
	int	green;
	int	(*get_green)(float color, float x, float decal, float ratio);
	int	blue;
	int	(*get_blue)(float color, float x, float decal, float ratio);
}	t_color_getter;

typedef struct s_color_separator
{
	int	color_segment_width;
	int	max;
	int	previous_max;
}	t_color_separator;

static void	base_color_box_draw(t_gui_box *self, t_engine *engine,
				t_draw_data draw_data);
static void	put_color_segment(t_image *image, t_vector2i *position,
				t_color_separator *color_separator,
				t_color_getter color_getter);
static void	init_color_getters(t_color_getter *color_getters);
static void	write_color_row(t_image *image, int y);
static void	base_color_picker_on_click(t_gui_box *self, t_engine *engine,
				t_click_data click_data);

void	init_base_color_box(t_engine *engine, t_gui_box *gui_box,
			t_gui_box *parent)
{
	// TODO the struct above should be in a header !! NORM ERROR
	int	y;

	*gui_box = create_t_gui_box(engine, (t_gui_box_create){parent, \
		(t_vector2i){.x = 0,
			.y = parent->size.y - parent->size.y / 2 + 4}, \
		(t_vector2i){.y = parent->size.y / 2 - 4, \
			.x = parent->size.x}, true});
	init_image(&gui_box->on_hover_image, &engine->window, parent->size.x,
		parent->size.y / 2 - 4);
	gui_box->draw = &base_color_box_draw;
	gui_box->on_click = &base_color_picker_on_click;
	engine->gui.color_picker_base_color = vector3f_multiply(
			engine->gui.material_to_assign_to_new_objects.albedo, 255.f);
	engine->gui.color_picker_base_color_was_changed = true;
	y = -1;
	while (++y < gui_box->size.y)
		write_color_row(&gui_box->image, y);
	round_image_corners(&gui_box->image, 10);
}

#ifdef __linux__

static void	base_color_box_draw(t_gui_box *self, t_engine *engine,
				int x_offset, int y_offset)
{
	engine->gui.draw_gui_image(&engine->main_image, &self->image,
		(t_vector2i){\
			.x = self->position.x + x_offset, \
			.y = self->position.y + y_offset}
	);
	if (mouse_is_hovering_box(&self->image, get_mouse_position(self, engine,
				x_offset, y_offset)) == false)
		return ;
	add_hover_color_circle(self, engine, x_offset, y_offset);
	engine->gui.draw_gui_image(&engine->main_image, &self->on_hover_image,
		(t_vector2i){\
			.x = self->position.x + x_offset, \
			.y = self->position.y + y_offset});
}
#endif
#ifdef __APPLE__

static void	base_color_box_draw(t_gui_box *self, t_engine *engine,
				t_draw_data draw_data)
{
	mlx_put_image_to_window(engine->window.mlx, engine->window.window,
		self->image.data, self->position.x + draw_data.offset.x,
		self->position.y + draw_data.offset.y);
	if (is_mouse_hovering_box(self, draw_data.offset, &self->image,
			draw_data.mouse_position) == false)
		return ;
	add_hover_color_circle(self, draw_data.offset, draw_data.mouse_position);
	mlx_put_image_to_window(engine->window.mlx, engine->window.window,
		self->on_hover_image.data, self->position.x + draw_data.offset.x,
		self->position.y + draw_data.offset.y);
}
#endif

static void	write_color_row(t_image *image, int y)
{
	t_color_separator	color_separator;
	t_vector2i			position;
	t_color_getter		color_getters[6];

	init_color_getters(color_getters);
	position = (t_vector2i){.x = 0, .y = y};
	color_separator.color_segment_width = image->width / 6;
	color_separator.max = color_separator.color_segment_width;
	color_separator.previous_max = 0;
	put_color_segment(image, &position, &color_separator, color_getters[0]);
	put_color_segment(image, &position, &color_separator, color_getters[1]);
	put_color_segment(image, &position, &color_separator, color_getters[2]);
	put_color_segment(image, &position, &color_separator, color_getters[3]);
	put_color_segment(image, &position, &color_separator, color_getters[4]);
	put_color_segment(image, &position, &color_separator, color_getters[5]);
	while (position.x < image->width)
	{
		put_pixel_on_image(image, y, position.x, rgb_to_uint(\
			(t_color){\
				.x = 255, \
				.y = 0, \
				.z = 0}));
		position.x++;
	}
}

static void	init_color_getters(t_color_getter *color_getters)
{
	color_getters[0] = (t_color_getter){
		255, &get_base_color,
		0, &get_base_color,
		255, &get_increasing_color};
	color_getters[1] = (t_color_getter){
		255, &get_decreasing_color,
		0, &get_base_color,
		255, &get_base_color};
	color_getters[2] = (t_color_getter){
		0, &get_base_color,
		255, &get_increasing_color,
		255, &get_base_color};
	color_getters[3] = (t_color_getter){
		0, &get_base_color,
		255, &get_base_color,
		255, &get_decreasing_color};
	color_getters[4] = (t_color_getter){
		255, &get_increasing_color,
		255, &get_base_color,
		0, &get_base_color};
	color_getters[5] = (t_color_getter){
		255, &get_base_color,
		255, &get_decreasing_color,
		0, &get_base_color};
}

static void	put_color_segment(t_image *image, t_vector2i *position,
				t_color_separator *color_separator, t_color_getter color_getter)
{
	while (position->x < color_separator->max)
	{
		put_pixel_on_image(image, position->y, position->x,
			rgb_to_uint((t_color){
				.x = color_getter.get_red(color_getter.red,
					position->x, color_separator->previous_max,
					color_separator->color_segment_width),
				.y = color_getter.get_green(color_getter.green,
					position->x, color_separator->previous_max,
					color_separator->color_segment_width),
				.z = color_getter.get_blue(color_getter.blue,
					position->x, color_separator->previous_max,
					color_separator->color_segment_width)}));
		position->x++;
	}
	color_separator->previous_max = color_separator->max;
	color_separator->max += color_separator->color_segment_width;
}

static void	base_color_picker_on_click(t_gui_box *self, t_engine *engine,
				t_click_data click_data)
{
	const unsigned int	uint_color = get_image_pixel_color(&self->image,
			click_data.click_position.y, click_data.click_position.x);
	t_color				albedo;

	if (click_data.button != BUTTON_LEFT || uint_color == COLOR_TRANSPARENT)
		return ;
	engine->gui.color_picker_base_color = get_t_color_from_uint(uint_color);
	engine->gui.color_picker_base_color_was_changed = true;
	albedo = vector3f_divide(engine->gui.color_picker_base_color, 255.f);
	if (engine->gui.selected_object.object == NULL
		&& engine->gui.selected_object.light == NULL)
		return (redraw_icons(engine, material_create(albedo, 0, 0)));
	engine->scene_changed = true;
	if (engine->gui.selected_object.object == NULL)
	{
		light_set_color(engine->gui.selected_object.light, albedo);
		return (redraw_icons(engine, material_create(albedo, 0, 0)));
	}
	if (engine->gui.color_being_changed_is_checked_pattern)
		engine->gui.selected_object.object->material.texture.\
			outline_checkerboard.albedo = albedo;
	else
		engine->gui.selected_object.object->material.albedo = albedo;
	redraw_icons(engine, engine->gui.selected_object.object->material);
}
