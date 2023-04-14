#include "mlx.h"

#include "image.h"
#include "struct/window.h"

void	init_image(t_image *image, t_window *window, int height, int width)
{
	image->image = mlx_new_image(window->mlx, width, height); // TODO can this fail?
	image->address = mlx_get_data_addr(image->image, &image->bits_per_pixel,
			&image->line_length, &image->endian);
}
`