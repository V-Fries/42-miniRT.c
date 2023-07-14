#include <sys/errno.h>
#include <stdlib.h>
#include <math.h>

#include "gui/box.h"
#include "colors.h"
#include "gui/object_modification_box.h"
#include "gui/utils.h"
#include "hooks.h"
#include "gui/UI.h"

static int	init_object_modification_gui_box_children(t_engine *engine,
				t_gui_box *parent);
static int	init_box_deleter(t_engine *engine, t_gui_box *gui_box);
static void	delete_box_on_click(t_gui_box *self, t_engine *engine, int y,
				int x);

int	init_object_modification_gui_box(t_engine *engine, t_gui_box *gui_box,
		const t_gui_box *object_creation_gui_box)
{
	*gui_box = create_t_gui_box(engine, (t_gui_box_create){NULL, \
		(t_vector2i){
			.x = engine->window.size.x - engine->window.size.x / 4 \
				- object_creation_gui_box->position.x,
			.y = object_creation_gui_box->size.y \
				+ object_creation_gui_box->position.y * 2}, \
		(t_vector2i){
			.x = engine->window.size.x / 4, \
			.y = engine->window.size.y \
				- (object_creation_gui_box->size.y \
				+ object_creation_gui_box->position.y * 3)}, false});
	if (errno == EINVAL)
		return (-1);
	change_image_color(&gui_box->image, BASE_GUI_COLOR);
	round_image_corners(&gui_box->image, BOX_ROUNDING_RADIUS);
	gui_box->draw = &default_gui_box_draw;
	gui_box->on_click = &default_gui_box_on_click;
	if (init_object_modification_gui_box_children(engine, gui_box))
		return (-1); // TODO free image
	return (0);
}

static int	init_object_modification_gui_box_children(t_engine *engine,
				t_gui_box *parent)
{
	if (create_vertical_boxes(engine, parent, "1 12 1 69 1 15 1",
			roundf(parent->size.y / 100.f)) < 0)
		return (-1);
	engine->gui.object_attributes_modification_box = parent->children.data + 3;
	if (init_box_deleter(engine, parent->children.data + 1) < 0)
		return (-1); // TODO free stuff
	if (init_rgb_picker(engine, parent->children.data + 5) < 0)
	{
		free(parent->children.data);
		return (-1); // TODO free stuff
	}
	parent->children.data[0].draw = NULL;
	parent->children.data[0].on_click = NULL;
	parent->children.data[2].draw = NULL;
	parent->children.data[2].on_click = NULL;
	parent->children.data[4].draw = NULL;
	parent->children.data[4].on_click = NULL;
	parent->children.data[6].draw = NULL;
	parent->children.data[6].on_click = NULL;
	return (0);
}

static int	init_box_deleter(t_engine *engine, t_gui_box *gui_box)
{
	if (create_n_horizontal_boxes(engine, gui_box, 3, (t_boxes_offsets){4, 4})
		< 0)
		return (-1);

	change_image_color(&gui_box->image, SUB_GUI_COLOR);
	round_image_corners(&gui_box->image, 10);
	gui_box->children.data[0].on_click = NULL;
	gui_box->children.data[0].draw = NULL;
	gui_box->children.data[1].on_click = NULL;
	gui_box->children.data[1].draw = NULL;
	gui_box->children.data[1].on_click = NULL;
	gui_box->children.data[2].on_click = delete_box_on_click;
	change_image_color(&gui_box->children.data[2].image, COLOR_RED);
	round_image_corners(&gui_box->children.data[2].image, 10);
	return (0);
}

static void	delete_box_on_click(t_gui_box *self, t_engine *engine, int y,
				int x)
{
	size_t	index;

	(void)y;
	(void)x;
	(void)self;
	if (engine->gui.selected_object.object == NULL
		&& engine->gui.selected_object.light == NULL)
		return ;
	engine->scene_changed = true;
	if (engine->gui.selected_object.object == NULL)
	{
		index = engine->gui.selected_object.light - engine->scene.lights.data;
		remove_light_in_lights(&engine->scene.lights, index);
		engine->gui.selected_object.light = NULL;
		update_object_attributes_modification_box(engine);
		return ;
	}
	index = engine->gui.selected_object.object - engine->scene.objects.data;
	remove_object_in_objects(&engine->scene.objects, index);
	engine->gui.selected_object.object = NULL;
	update_object_attributes_modification_box(engine);
}
