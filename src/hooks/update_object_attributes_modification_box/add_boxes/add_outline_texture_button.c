#include "engine.h"
#include "gui/box.h"
#include "font/render.h"
#include "events.h"
#include "gui/optional_boxes.h"

#define OUTLINE_TEXTURE_BUTTON_TEXT "Change outline texture"

static void	outline_texture_button_on_click(t_gui_box *self, t_engine *engine,
				t_click_data click_data);

void	add_outline_texture_button(t_engine *engine, t_gui_box *gui_box,
			int *y, t_gui_box *parent)
{
	add_button_box(engine, gui_box, y, parent);
	write_centered_string_to_image(&engine->gui.font,
		&gui_box->image, OUTLINE_TEXTURE_BUTTON_TEXT);
	write_centered_string_to_image(&engine->gui.font,
		&gui_box->on_hover_image, OUTLINE_TEXTURE_BUTTON_TEXT);
	gui_box->on_click = &outline_texture_button_on_click;
}

static void	outline_texture_button_on_click(t_gui_box *self, t_engine *engine,
				t_click_data click_data)
{
	(void)self;
	if (click_data.button != BUTTON_LEFT)
		return ;
	// TODO change texture box description to "Outline texture"
	engine->gui.current_optional_box = TEXTURE_BOX;
	engine->gui.color_and_material.texture_being_changed = OUTLINE_TEXTURE;
}
