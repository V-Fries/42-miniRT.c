#include "gui/box.h"

t_vector2i	get_top_box_decal(const t_engine *engine, const t_gui_box *gui_box)
{
	const float	hidden_ratio = engine->gui.hide_animation.current_hidden_ratio;

	return ((t_vector2i){
		0,
		-((gui_box->size.y + gui_box->position.y) * hidden_ratio)});
}

t_vector2i	get_right_box_decal(const t_engine *engine,
				const t_gui_box *gui_box)
{
	const float	hidden_ratio = engine->gui.hide_animation.current_hidden_ratio;

	return ((t_vector2i){
		(engine->window.size.x - gui_box->position.x) * hidden_ratio,
		0});
}

t_vector2i	get_left_box_decal(const t_engine *engine, const t_gui_box *gui_box)
{
	const float	hidden_ratio = engine->gui.hide_animation.current_hidden_ratio;

	return ((t_vector2i){
		-((engine->window.size.x - gui_box->position.x) * hidden_ratio),
		0});
}
