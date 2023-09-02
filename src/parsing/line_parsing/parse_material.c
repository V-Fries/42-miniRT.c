#include "parsing.h"

int	parse_material(t_engine *engine, char **scene_content_line,
		t_rt_file_requirements *rt_file_requirements)
{
//	size_t	i;

	(void)rt_file_requirements;
	(void)engine;(void)scene_content_line;
//	i = -1;
//	while (scene_content_line[++i] != NULL)
//		change_material(engine, scene_content_line[i]);
	return (0);
}

//static void	change_material(t_engine *engine, char *instructions)
//{
//	char	**instructions_split = ft_split(instructions, ':');
//
//	if (instructions_split == NULL || ft_split_len(instructions_split) != 2)
//	{
//		ft_print_error("Warning: Failed to change material ");
//		ft_print_error(instructions);
//		return (ft_free_split(instructions_split));
//	}
//
//}


