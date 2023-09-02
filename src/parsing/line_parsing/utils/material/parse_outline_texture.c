#include "libft.h"

#include "object.h"

void	parse_outline_texture(t_object *object, char *file)
{
	if (set_outline_texture(&object->material, file) < 0)
	{
		ft_print_error("Failed to set outline texture ");
		ft_print_error(file);
		ft_print_error("During parsing\n");
	}
}
