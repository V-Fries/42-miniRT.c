#include <stdlib.h>
#include <math.h>

#include "gui/box.h"
#include "gui/main_gui_box.h"
#include "engine.h"
#include "colors.h"
#include "gui/UI.h"

static void	init_menu_gui_box_children(t_engine *minirt, t_gui_box *gui_box);
static void	init_camera_icon_box(t_engine *minirt, t_gui_box *gui_box,
				t_gui_box *parent);
static void	init_saving_icon_box(t_engine *minirt, t_gui_box *gui_box,
				t_gui_box *parent);
static void	init_menu_settings_icon_box(t_engine *minirt, t_gui_box *gui_box,
				t_gui_box *parent);

void	init_menu_gui_box(t_engine *minirt, t_gui_box *gui_box,
			t_gui_box *parent)
{
	*gui_box = create_t_gui_box(minirt, (t_gui_box_create){NULL, \
		(t_vector2i){
			.x = parent->size.x - parent->size.x / 4, \
			.y = 8}, \
		(t_vector2i){
			.x = parent->size.x / 4 - 8, \
			.y = parent->size.y - 16}, true});
	change_image_color(&gui_box->image, SUB_GUI_COLOR);
	round_image_corners(&gui_box->image, BOX_ROUNDING_RADIUS);
	init_menu_gui_box_children(minirt, gui_box);
}

static void	init_menu_gui_box_children(t_engine *minirt, t_gui_box *gui_box)
{
	gui_box->children.size = 3;
	gui_box->children.data = malloc(sizeof(*gui_box->children.data)
			* gui_box->children.size);
	if (gui_box->children.data == NULL)
		ft_fatal_error("init_menu_gui_box_children: malloc failed");
	init_camera_icon_box(minirt, gui_box->children.data + 0, gui_box);
	init_saving_icon_box(minirt, gui_box->children.data + 1, gui_box);
	init_menu_settings_icon_box(minirt, gui_box->children.data + 2, gui_box);
}

static void	init_camera_icon_box(t_engine *minirt, t_gui_box *gui_box,
				t_gui_box *parent)
{
	const int	box_width = roundf(
			((float)parent->size.x - ICON_BOX_SEPARATOR * 4) / 3.0);

	*gui_box = create_t_gui_box(minirt, (t_gui_box_create){NULL, \
		(t_vector2i){
			.x = ICON_BOX_SEPARATOR, \
			.y = ICON_BOX_SEPARATOR}, \
		(t_vector2i){
			.x = box_width, \
			.y = parent->size.y - ICON_BOX_SEPARATOR * 2}, true});
	init_image(&gui_box->on_hover_image, &minirt->window, gui_box->size.x,
		gui_box->size.y);
	change_image_color(&gui_box->image, COLOR_TRANSPARENT);
	round_image_corners(&gui_box->image, BOX_ROUNDING_RADIUS);
	change_image_color(&gui_box->on_hover_image, HOVER_GUI_COLOR);
	round_image_corners(&gui_box->on_hover_image, BOX_ROUNDING_RADIUS);
}

static void	init_saving_icon_box(t_engine *minirt, t_gui_box *gui_box,
				t_gui_box *parent)
{
	const int	box_width = roundf(
			((float)parent->size.x - ICON_BOX_SEPARATOR * 4) / 3.0);

	*gui_box = create_t_gui_box(minirt, (t_gui_box_create){NULL, \
		(t_vector2i){
			.x = ICON_BOX_SEPARATOR * 2 + box_width, \
			.y = ICON_BOX_SEPARATOR}, \
		(t_vector2i){
			.x = box_width, \
			.y = parent->size.y - ICON_BOX_SEPARATOR * 2}, true});
	init_image(&gui_box->on_hover_image, &minirt->window, gui_box->size.x,
		gui_box->size.y);
	change_image_color(&gui_box->image, COLOR_TRANSPARENT);
	round_image_corners(&gui_box->image, BOX_ROUNDING_RADIUS);
	change_image_color(&gui_box->on_hover_image, HOVER_GUI_COLOR);
	round_image_corners(&gui_box->on_hover_image, BOX_ROUNDING_RADIUS);
}

static void	init_menu_settings_icon_box(t_engine *minirt, t_gui_box *gui_box,
				t_gui_box *parent)
{
	const int	box_width = roundf(
			((float)parent->size.x - ICON_BOX_SEPARATOR * 4) / 3.f);

	*gui_box = create_t_gui_box(minirt, (t_gui_box_create){NULL, \
		(t_vector2i){
			.x = ICON_BOX_SEPARATOR * 3 + box_width * 2, \
			.y = ICON_BOX_SEPARATOR}, \
		(t_vector2i){
			.x = box_width, \
			.y = parent->size.y - ICON_BOX_SEPARATOR * 2}, true});
	init_image(&gui_box->on_hover_image, &minirt->window, gui_box->size.x,
		gui_box->size.y);
	init_settings_icon(gui_box);
}
