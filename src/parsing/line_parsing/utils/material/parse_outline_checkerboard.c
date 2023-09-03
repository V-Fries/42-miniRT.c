#include "object.h"
#include "parsing.h"

void	parse_outline_checkerboard(t_object *object, char *raw_checkerboard_data)
{
	const t_checkerboard_parsing_data	checkerboard_data
		= get_checkerboard_parsing_data(raw_checkerboard_data);

	free_outline_texture(&object->material);
	if (checkerboard_data.size.x + checkerboard_data.size.y < .01f)
		return ;
	set_outline_checkerboard(&object->material, checkerboard_data.size,
		vector3f_divide(checkerboard_data.color, 255.f));
}
