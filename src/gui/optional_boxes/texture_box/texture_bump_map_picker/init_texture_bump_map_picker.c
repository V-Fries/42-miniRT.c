#include "gui/box.h"
#include "gui/UI.h"
#include "gui/utils.h"
#include "gui/optional_boxes.h"
#include "font/render.h"

#define OUTLINE_BOX_TEXT "  Texture  "
#define CAP_BOX_TEXT     "  Bump map  "

static void	init_texture_button(t_engine *engine, t_gui_box *gui_box);
static void	init_bump_map_button(t_engine *engine, t_gui_box *gui_box);

void	init_texture_bump_map_picker(t_engine *engine, t_gui_box *gui_box)
{
	init_image(&gui_box->image, &engine->window, gui_box->size.x,
		gui_box->size.y);
	change_image_color(&gui_box->image, SUB_GUI_COLOR);
	round_image_corners(&gui_box->image, BOX_ROUNDING_RADIUS);
	create_n_horizontal_boxes(engine, gui_box, 2, (t_vector2i){4, 4});
	init_texture_button(engine, gui_box->children.data + 0);
	init_bump_map_button(engine, gui_box->children.data + 1);
}

static void	init_texture_button(t_engine *engine, t_gui_box *gui_box)
{
	init_image(&gui_box->image, &engine->window, gui_box->size.x,
		gui_box->size.y);
	init_image(&gui_box->on_hover_image, &engine->window, gui_box->size.x,
		gui_box->size.y);
	change_image_color(&gui_box->image, COLOR_TRANSPARENT);
	change_image_color(&gui_box->on_hover_image, HOVER_GUI_COLOR);
	write_centered_string_to_image(&engine->gui.font, &gui_box->image,
		OUTLINE_BOX_TEXT);
	write_centered_string_to_image(&engine->gui.font, &gui_box->on_hover_image,
		OUTLINE_BOX_TEXT);
	round_image_corners(&gui_box->image, BOX_ROUNDING_RADIUS);
	round_image_corners(&gui_box->on_hover_image, BOX_ROUNDING_RADIUS);
	gui_box->draw = &texture_picker_draw;
	gui_box->on_click = &texture_picker_on_click;
}

static void	init_bump_map_button(t_engine *engine, t_gui_box *gui_box)
{
	init_image(&gui_box->image, &engine->window, gui_box->size.x,
		gui_box->size.y);
	init_image(&gui_box->on_hover_image, &engine->window, gui_box->size.x,
		gui_box->size.y);
	change_image_color(&gui_box->image, COLOR_TRANSPARENT);
	change_image_color(&gui_box->on_hover_image, HOVER_GUI_COLOR);
	write_centered_string_to_image(&engine->gui.font, &gui_box->image,
		CAP_BOX_TEXT);
	write_centered_string_to_image(&engine->gui.font, &gui_box->on_hover_image,
		CAP_BOX_TEXT);
	round_image_corners(&gui_box->image, BOX_ROUNDING_RADIUS);
	round_image_corners(&gui_box->on_hover_image, BOX_ROUNDING_RADIUS);
	gui_box->draw = &bump_map_picker_draw;
	gui_box->on_click = &bump_map_picker_on_click;
}
