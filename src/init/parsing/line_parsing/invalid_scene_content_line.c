#include "libft.h"

void	invalid_scene_content_line(char **scene_content_line)
{
	size_t	i;

	ft_putstr_fd("Invalid scene_content line: ", STDERR_FILENO);
	i = 0;
	while (scene_content_line[i] != NULL)
	{
		ft_putstr_fd(scene_content_line[i], STDERR_FILENO);
		ft_putstr_fd(" ", STDERR_FILENO);
		i++;
	}
	ft_putstr_fd("\n", STDERR_FILENO);
}
