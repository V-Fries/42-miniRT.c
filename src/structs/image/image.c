#include <stddef.h>

#include "mlx.h"
#include "libft.h"

#include "struct/t_image.h"
#include "struct/t_window.h"

int	init_image(t_image *image, t_window *window, int width, int height)
{
	image->data = mlx_new_image(window->mlx, width, height);
	if (image->data == NULL)
		return (-1);
	image->address = (unsigned int *)mlx_get_data_addr(image->data,
			&image->bits_per_pixel, &image->line_length, &image->endian);
	image->line_length /= 4;
	image->height = height;
	image->width = width;
	return (0);
}

int	init_image_from_xpm(t_image *image, t_window *window, char *xmp_file)
{
	image->data = mlx_xpm_file_to_image(window->mlx, xmp_file,
										&image->width, &image->height);
	if (image->data == NULL)
	{
		ft_putstr_fd("Error: mlx_xpm_file_to_image failed\n", STDERR_FILENO);
		return (1);
	}
	image->address = (unsigned int *)mlx_get_data_addr(image->data,
			&image->bits_per_pixel, &image->line_length, &image->endian);
	image->line_length /= 4;
	return (0);
}

inline void    put_pixel_on_image(t_image *image, int y, int x,
					unsigned int color)
{
	image->address[y * image->line_length + x] = color;
}

inline unsigned int	get_image_pixel_color(t_image *image, int y, int x)
{
	return (image->address[y * image->line_length + x]);
}

void	change_image_color(t_image *image, unsigned int color)
{
	int	i;
	int	j;

	i = image->height;
	while (i--)
	{
		j = image->width;
		while (j--)
			put_pixel_on_image(image, i, j, color);
	}
}
