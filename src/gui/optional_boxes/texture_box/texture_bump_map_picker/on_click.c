#include "events.h"
#include "gui/box.h"

void	texture_picker_on_click(t_gui_box *self, t_engine *engine,
			t_click_data click_data)
{
	(void)self;
	if (click_data.button != BUTTON_LEFT
		|| engine->gui.color_and_material.changing_bump_map_or_texture
		== TEXTURE)
		return ;
	engine->gui.color_and_material.textures_and_bump_maps.selection_box->scroll
		= 0;
	engine->gui.color_and_material.changing_bump_map_or_texture = TEXTURE;
}

void	bump_map_picker_on_click(t_gui_box *self, t_engine *engine,
			t_click_data click_data)
{
	(void)self;
	if (click_data.button != BUTTON_LEFT
		|| engine->gui.color_and_material.changing_bump_map_or_texture
		== BUMP_MAP)
		return ;
	engine->gui.color_and_material.textures_and_bump_maps.selection_box->scroll
		= 0;
	engine->gui.color_and_material.changing_bump_map_or_texture = BUMP_MAP;
}
