#include "mlx.h"

#include "gui/box.h"

void	outline_picker_draw(t_gui_box *self, t_engine *engine,
			t_draw_data draw_data)
{
	if (engine->gui.color_and_material.texture_being_changed == OUTLINE_TEXTURE)
		return ((void)mlx_put_image_to_window(engine->window.mlx,
				engine->window.window, self->on_hover_image.data,
				self->position.x + draw_data.offset.x,
				self->position.y + draw_data.offset.y));
	draw_gui_box_image(self, engine, draw_data);
}

void	cap_picker_draw(t_gui_box *self, t_engine *engine,
			t_draw_data draw_data)
{
	if (engine->gui.color_and_material.texture_being_changed == CAP_TEXTURE)
		return ((void)mlx_put_image_to_window(engine->window.mlx,
				engine->window.window, self->on_hover_image.data,
				self->position.x + draw_data.offset.x,
				self->position.y + draw_data.offset.y));
	draw_gui_box_image(self, engine, draw_data);
}
