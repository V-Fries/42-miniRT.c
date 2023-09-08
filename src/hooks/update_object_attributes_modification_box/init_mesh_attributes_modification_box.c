#include "hooks.h"

static void	add_transformation_boxes(t_engine *engine, t_gui_box *gui_box,
				int *i);

void	init_mesh_attributes_modification_box(t_engine *engine,
			t_gui_box *gui_box)
{
	int	i;

	i = 0;
	gui_box->children.size = 3;
	gui_box->children.data = ft_calloc(gui_box->children.size,
			sizeof(*gui_box->children.data));
	if (gui_box->children.data == NULL)
		ft_fatal_error("init_cone_attributes_modification_box: "
			"malloc failed");
	add_delete_box(engine, gui_box->children.data + i++, gui_box);
	add_transformation_boxes(engine, gui_box, &i);
}

static void	add_transformation_boxes(t_engine *engine, t_gui_box *gui_box,
				int *i)
{
	int	y;

	y = get_transformations_boxes_index(gui_box);
	add_position_box(engine, gui_box->children.data + (*i)++, &y, gui_box);
	add_normal_box(engine, gui_box->children.data + (*i)++, &y, gui_box);
}
