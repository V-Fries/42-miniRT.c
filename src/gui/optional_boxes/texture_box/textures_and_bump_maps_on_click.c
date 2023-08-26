/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures_and_bump_maps_on_click.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 23:26:00 by vfries            #+#    #+#             */
/*   Updated: 2023/07/30 17:45:32 by vfries           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "gui/box.h"
#include "events.h"
#include "gui/object_list_box.h"
#include "hooks.h"
#include "gui/optional_boxes.h"

static void		handle_scroll(t_gui_box *self, int button,
					const t_gui_boxes *gui_boxes);
static void		click_ppm_box(t_engine *engine, size_t index);
static size_t	get_selected_ppm_box_index(t_gui_box *self, t_engine *engine,
					t_vector2i click_position);
static void		set_normal_map(const t_color_and_material *color_and_material,
					t_object *object, size_t index, bool is_cap);
static void		set_texture(const t_color_and_material *color_and_material,
					t_object *object, size_t index, bool is_cap);

void	textures_and_bump_maps_on_click(t_gui_box *self, t_engine *engine,
			t_click_data click_data)
{
	size_t		selected_file_index;
	t_gui_boxes	*gui_boxes;

	if (engine->gui.color_and_material.changing_bump_map_or_texture == BUMP_MAP)
		gui_boxes = &engine->gui.color_and_material.textures_and_bump_maps.\
			bump_maps_boxes;
	else
		gui_boxes = &engine->gui.color_and_material.textures_and_bump_maps.\
			textures_boxes;
	handle_scroll(self, click_data.button, gui_boxes);
	if (click_data.button != BUTTON_LEFT)
		return ;
	selected_file_index = get_selected_ppm_box_index(self, engine,
			click_data.click_position);
	if (selected_file_index == SIZE_MAX)
		return ;
	click_ppm_box(engine, selected_file_index);
}

static void	handle_scroll(t_gui_box *self, const int button,
				const t_gui_boxes *gui_boxes)
{
	int		size_diff_sub_boxes_main_box;

	if (button == SCROLL_UP)
	{
		self->scroll -= 10;
		size_diff_sub_boxes_main_box = self->size.y - \
			((int)gui_boxes->size
				* (gui_boxes->data->size.y + TEXTURE_BOX_PPM_OFFSET));
		if (size_diff_sub_boxes_main_box < 0
			&& self->scroll < size_diff_sub_boxes_main_box)
			self->scroll = size_diff_sub_boxes_main_box;
		else if (size_diff_sub_boxes_main_box >= 0
			&& self->scroll < TEXTURE_BOX_PPM_OFFSET)
			self->scroll = TEXTURE_BOX_PPM_OFFSET;
	}
	if (button != SCROLL_DOWN)
		return ;
	self->scroll += 10;
	if (self->scroll > TEXTURE_BOX_PPM_OFFSET)
		self->scroll = TEXTURE_BOX_PPM_OFFSET;
}

static size_t	get_selected_ppm_box_index(t_gui_box *self, t_engine *engine,
					t_vector2i click_position)
{
	int			y;
	size_t		i;
	t_gui_boxes	*gui_boxes;

	if (engine->gui.color_and_material.changing_bump_map_or_texture == BUMP_MAP)
		gui_boxes = &engine->gui.color_and_material.textures_and_bump_maps.\
			bump_maps_boxes;
	else
		gui_boxes = &engine->gui.color_and_material.textures_and_bump_maps.\
			textures_boxes;
	if (click_position.x < TEXTURE_BOX_PPM_OFFSET
		|| click_position.x >= self->size.y - TEXTURE_BOX_PPM_OFFSET * 2)
		return (SIZE_MAX);
	y = self->scroll;
	i = -1;
	while (++i < gui_boxes->size)
	{
		if (y <= click_position.y
			&& y + gui_boxes->data[i].size.y >= click_position.y)
			return (i);
		y += gui_boxes->data[i].size.y + TEXTURE_BOX_PPM_OFFSET;
	}
	return (SIZE_MAX);
}

static void	click_ppm_box(t_engine *engine, size_t index)
{
	t_object					*object;
	const t_color_and_material	*color_and_material
		= &engine->gui.color_and_material;
	const bool					is_normal_map
		= color_and_material->changing_bump_map_or_texture == BUMP_MAP;
	const bool					is_cap
		= color_and_material->texture_being_changed == CAP_TEXTURE;

	object = engine->gui.selected_object.object;
	if (object == NULL)
		return ;
	if (is_normal_map)
		set_normal_map(color_and_material, object, index, is_cap);
	else
		set_texture(color_and_material, object, index, is_cap);
	engine->scene_changed = true;
}

static void	set_normal_map(const t_color_and_material *color_and_material,
				t_object *object, size_t index, const bool is_cap)
{
	char	**files;

	files = color_and_material->textures_and_bump_maps.bump_maps_files;
	if (is_cap)
		set_cap_normals_map(&object->material, files[index]);
	else
		set_outline_normals_map(&object->material, files[index]);
}

static void	set_texture(const t_color_and_material *color_and_material,
				t_object *object, size_t index, const bool is_cap)
{
	char	**files;

	files = color_and_material->textures_and_bump_maps.textures_files;
	if (is_cap)
		set_cap_texture(&object->material, files[index]);
	else
		set_outline_texture(&object->material, files[index]);
}
