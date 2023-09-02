#include <errno.h>

#include "libft.h"

#include "object.h"

void	parse_specular(t_object *object, char *specular)
{
	errno = 0;
	object->material.specular = ft_atof(specular);
	if (errno != 0 || object->material.specular < 0.f
		|| object->material.specular > 1.f)
	{
		ft_print_error("Warning: specular value in rt file is abnormal ");
		ft_print_error(specular);
		ft_print_error("\n");
	}
	if (object->material.specular < 0.f)
		object->material.specular = 0.f;
	if (object->material.specular > 1.f)
		object->material.specular = 1.f;
}
