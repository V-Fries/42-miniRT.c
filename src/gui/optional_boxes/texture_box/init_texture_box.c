#include <math.h>

#include "engine.h"
#include "gui/UI.h"
#include "gui/box.h"
#include "gui/utils.h"
#include "gui/optional_boxes.h"

static void	init_children_boxes(t_engine *engine, t_gui_box *gui_box);
static void	init_selection_boxes(t_engine *engine, t_gui_box *gui_box);
static void	get_paths_to_ppm_files(t_engine *engine);
static void	remove_name_of_executable(char *path);

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
	create_vertical_boxes(engine, gui_box, "1 6 1 6 1 84 1",
		roundf(gui_box->size.y / 100.f));
	init_outline_cap_picker(engine, gui_box->children.data + 1);
	init_texture_bump_map_picker(engine, gui_box->children.data + 3);
	init_selection_boxes(engine, gui_box->children.data + 5);
}

static void	init_selection_boxes(t_engine *engine, t_gui_box *gui_box)
{
	get_paths_to_ppm_files(engine);
	init_image(&gui_box->image, &engine->window, gui_box->size.x,
		gui_box->size.y);
	gui_box->draw = &textures_and_bump_maps_draw;
	engine->gui.color_and_material.textures_and_bump_maps.selection_box
		= gui_box;
	load_textures_and_bump_maps(engine);
}

static void	get_paths_to_ppm_files(t_engine *engine)
{
	t_textures_and_bump_maps	*textures_and_bump_maps;
	char						*path_to_executable;

	path_to_executable = realpath(engine->argv[0], NULL);
	if (path_to_executable == NULL)
		ft_fatal_error("Failed to get path to executable");
	remove_name_of_executable(path_to_executable);
	textures_and_bump_maps
		= &engine->gui.color_and_material.textures_and_bump_maps;
	textures_and_bump_maps->path_to_textures_folder
		= ft_strjoin(path_to_executable, "/data/textures");
	textures_and_bump_maps->path_to_bump_maps_folder
		= ft_strjoin(path_to_executable, "/data/bump maps");
	free(path_to_executable);
	if (textures_and_bump_maps->path_to_textures_folder == NULL
		|| textures_and_bump_maps->path_to_bump_maps_folder == NULL)
		ft_fatal_error("Failed to get path to textures and bump maps");
}

static void	remove_name_of_executable(char *path)
{
	char	*cursor;

	cursor = path + ft_strlen(path);
	if (*cursor == '/')
		*cursor-- = '\0';
	while (cursor >= path && *cursor != '/')
		cursor--;
	if (cursor > path)
		*cursor = '\0';
}
