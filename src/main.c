/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 20:41:35 by vfries            #+#    #+#             */
/*   Updated: 2023/04/12 20:46:20 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "libft.h"

#include "engine.h"
#include "path.h"

#define EXPECTED_EXECUTION_COMMAND "./miniRT *.rt"

static bool			arguments_are_bad(int argc, const char **argv);
static const char	*get_startup_scene_path(const int argc, const char **argv);

#include "ray_tracer/bvh.h"

int	main(const int argc, const char **argv)
{
	t_engine	minirt;
	const char	*start_up_scene = get_startup_scene_path(argc, argv);

	if (arguments_are_bad(argc, argv))
		return (1);
	if (init_engine(&minirt, start_up_scene, argv[0]) < 0)
	{
		ft_putstr_fd("Error: Failed to init miniRT\n", STDERR_FILENO);
		return (2);
	}
	mlx_loop(minirt.window.mlx);
//	(void) argc;
//	(void) argv;
//	t_objects objects;
//	initialize_objects_array(&objects, 10);
//	t_object 	test;
//	ft_bzero(&test, sizeof(test));
//
//	t_material mat = material_create((t_vector3f){0.f, 0.f, 0.f}, 0.f, 0.f);
//	mesh_object_initialize(&test, "assets/objects/cube.obj", mat);
//	add_object_in_objects(&objects, test);
////	ft_printf("%p\n", test);
//	free_objects(&objects);
//	(void) get_startup_scene_path;
//	(void) test;
//	(void) arguments_are_bad;
}

static bool	arguments_are_bad(const int argc, const char **argv)
{
	char	*file_extension;

	if (argc == 1)
		return (false);
	if (argc > 2)
	{
		ft_putstr_fd("Unexpected argument count, expected:\n\t"
			EXPECTED_EXECUTION_COMMAND"\n", STDERR_FILENO);
		return (true);
	}
	file_extension = ft_strrstr(argv[1], ".rt");
	if (file_extension == NULL || file_extension[3] != '\0')
	{
		ft_putstr_fd("Unexpected file extension, expected:\n\t"
			EXPECTED_EXECUTION_COMMAND"\n", STDERR_FILENO);
		return (true);
	}
	return (false);
}

static const char	*get_startup_scene_path(const int argc, const char **argv)
{
	if (argc == 1)
		return (DEFAULT_STARTUP_SCENE_PATH);
	else
		return (argv[1]);
}
