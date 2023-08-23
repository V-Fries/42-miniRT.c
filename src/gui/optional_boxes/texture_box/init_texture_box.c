#include <math.h>

#include "engine.h"
#include "gui/UI.h"
#include "gui/box.h"
#include "gui/utils.h"
#include "gui/optional_boxes.h"

static void	init_children_boxes(t_engine *engine, t_gui_box *gui_box);

void	init_texture_box(t_engine *engine, t_gui_box *gui_box,
			const t_gui_box *main_gui_box, const t_gui_box *object_list_box)
{
	*gui_box = create_optional_box(engine, main_gui_box, object_list_box);
	change_image_color(&gui_box->image, BASE_GUI_COLOR);
	round_image_corners(&gui_box->image, BOX_ROUNDING_RADIUS);
	init_children_boxes(engine, gui_box);
}

static void	init_children_boxes(t_engine *engine, t_gui_box *gui_box)
{
	create_vertical_boxes(engine, gui_box, "1 10 1 10 1 76 1",
		roundf(gui_box->size.y / 100.f));
	init_outline_cap_picker(engine, gui_box->children.data + 1);
}
