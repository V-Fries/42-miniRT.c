#include "events.h"
#include "gui/box.h"

void	outline_picker_on_click(t_gui_box *self, t_engine *engine,
			t_click_data click_data)
{
	(void)self;
	if (click_data.button != BUTTON_LEFT)
		return ;
	engine->gui.color_and_material.texture_being_changed = OUTLINE_TEXTURE;
}

void	cap_picker_on_click(t_gui_box *self, t_engine *engine,
			t_click_data click_data)
{
	(void)self;
	if (click_data.button != BUTTON_LEFT)
		return ;
	engine->gui.color_and_material.texture_being_changed = CAP_TEXTURE;
}
