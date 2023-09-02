#include "libft.h"

#include "object.h"

void	parse_outline_normal_map(t_object *object, char *file)
{
	if (set_outline_normals_map(&object->material, file) < 0)
	{
		ft_print_error("Failed to set outline normal map ");
		ft_print_error(file);
		ft_print_error("During parsing\n");
	}
}
