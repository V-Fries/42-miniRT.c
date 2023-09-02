#include <errno.h>

#include "libft.h"

#include "object.h"

void	parse_reflection(t_object *object, char *reflection)
{
	errno = 0;
	object->material.reflection = ft_atof(reflection);
	if (errno != 0 || object->material.reflection < 0.f
		|| object->material.reflection > 1.f)
	{
		ft_print_error("Warning: reflection value in rt file is abnormal ");
		ft_print_error(reflection);
		ft_print_error("\n");
	}
	if (object->material.reflection < 0.f)
		object->material.reflection = 0.f;
	if (object->material.reflection > 1.f)
		object->material.reflection = 1.f;
}
