#ifndef OPTIONAL_BOXES_H
# define OPTIONAL_BOXES_H

# include "engine.h"
# include "box.h"

void		init_settings_box(t_engine *engine, t_gui_box *gui_box,
				const t_gui_box *main_gui_box,
				const t_gui_box *object_list_box);
void		init_antialiasing_toggle_box(t_engine *engine, t_gui_box *gui_box,
				int *y, t_gui_box *parent);
void		init_cam_fov_box(t_engine *engine, t_gui_box *gui_box, int *y,
				t_gui_box *parent);
void		init_max_resolution_reduction_box(t_engine *engine,
				t_gui_box *gui_box, int *y, t_gui_box *parent);
void		init_min_resolution_reduction_box(t_engine *engine,
				t_gui_box *gui_box, int *y, t_gui_box *parent);
void		init_reset_camera_peaking_box(t_engine *engine, t_gui_box *gui_box,
				int *y, t_gui_box *parent);

void		init_texture_box(t_engine *engine, t_gui_box *gui_box,
				const t_gui_box *main_gui_box,
				const t_gui_box *object_list_box);
void		init_outline_cap_picker(t_engine *engine, t_gui_box *gui_box);
void		outline_picker_draw(t_gui_box *self, t_engine *engine,
				t_draw_data draw_data);
void		cap_picker_draw(t_gui_box *self, t_engine *engine,
				t_draw_data draw_data);
void		outline_picker_on_click(t_gui_box *self, t_engine *engine,
				t_click_data click_data);
void		cap_picker_on_click(t_gui_box *self, t_engine *engine,
				t_click_data click_data);
void		init_texture_bump_map_picker(t_engine *engine, t_gui_box *gui_box);
void		texture_picker_draw(t_gui_box *self, t_engine *engine,
				t_draw_data draw_data);
void		bump_map_picker_draw(t_gui_box *self, t_engine *engine,
				t_draw_data draw_data);
void		texture_picker_on_click(t_gui_box *self, t_engine *engine,
				t_click_data click_data);
void		bump_map_picker_on_click(t_gui_box *self, t_engine *engine,
				t_click_data click_data);

void		add_button_box(t_engine *engine, t_gui_box *gui_box, int *y,
				t_gui_box *parent);
void		add_plus_minus_box(t_engine *engine, t_gui_box *gui_box, int *y,
				t_gui_box *parent);

t_gui_box	create_optional_box(t_engine *engine, const t_gui_box *main_gui_box,
				const t_gui_box *object_list_box);

#endif //OPTIONAL_BOXES_H
