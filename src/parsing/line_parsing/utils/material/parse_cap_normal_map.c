#include "libft.h"

#include "object.h"

void	parse_cap_normal_map(t_object *object, char *file)
{
	if (set_cap_normals_map(&object->material, file) < 0)
	{
		ft_print_error("Failed to set cap normal map ");
		ft_print_error(file);
		ft_print_error("During parsing\n");
	}
}
