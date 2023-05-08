#include "struct/t_image.h"
#include "struct/t_point_int_2d.h"

static unsigned int	mix_colors(unsigned int added_color,
						unsigned int base_color);

void	put_image_to_image(register t_image *destination,
			const t_image *source, t_point_int_2d position)
{
	const int		y_max = source->height * source->line_length;
	const int		position_y_max = destination->height
		* destination->line_length;
	int				position_x_backup;
	register int	y;
	register int	x;

	position_x_backup = position.x;
	y = (position.y < 0) * -position.y * source->line_length;
	position.y = (position.y > 0) * position.y * destination->line_length;
	while (position.y < position_y_max && y < y_max)
	{
		x = (position.x < 0) * -position.x;
		position.x = (position.x > 0) * position.x;
		while (position.x < destination->width && x < source->width)
		{
			destination->address[position.y + position.x]
				= mix_colors(source->address[y + x++],
					destination->address[position.y + position.x]);
			position.x++;
		}
		position.x = position_x_backup;
		position.y += destination->line_length;
		y += source->line_length;
	}
}

void	put_background(t_image *destination, const t_image *source)
{
	register const unsigned int	*dest_end
		= destination->address + destination->height * destination->line_length;
	register unsigned int		*dest_curr;
	register unsigned int		*source_curr;

	source_curr = source->address;
	dest_curr = destination->address;
	while (dest_curr < dest_end)
		*dest_curr++ = *source_curr++;
}

void	put_image_to_image_unsafe(t_image *destination,
			const t_image *source, t_point_int_2d position)
{
	const unsigned int	*source_end
		= source->address + source->height * source->line_length;
	const int			x_max = source->width + position.x;
	register unsigned int		*source_curr;
	register unsigned int		*dest_curr;
	register int				x;

	dest_curr = destination->address + position.y * destination->line_length;
	x = position.x;
	source_curr = source->address;
	while (source_curr < source_end)
	{
		dest_curr[x]
			= mix_colors(*source_curr++, dest_curr[x]);
		x++;
		if (x >= x_max)
		{
			x = position.x;
			dest_curr += destination->line_length;
		}
	}
}

static unsigned int	mix_colors(unsigned int added_color,
						unsigned int base_color)
{
	const unsigned int	added_color_transparency = added_color >> 24;
//	float				inverse_transparency;
//	float				transparency;

	if (added_color_transparency == 255)
		return (base_color);
	if (added_color_transparency == 0)
		return (added_color);

	// TODO check how it looks with this (Applies 0.25 transparency instead of calculating actual transparency)
	//  								 (is faster but doesn't allow precise transparency)
	return (((((added_color & 0x00FF0000) >> 16) / 4 * 3 + ((base_color & 0x00FF0000) / 4 >> 16))) << 16
		| ((((added_color & 0x0000FF00) >> 8) / 4 * 3 + ((base_color & 0x0000FF00) >> 8) / 4)) << 8
		| (((added_color & 0x000000FF) / 4 * 3 + (base_color & 0x000000FF) / 4)));


//	inverse_transparency = added_color_transparency / 255.f;
//	transparency = 1.f - inverse_transparency;
//	return (((unsigned int)(transparency * ((added_color & 0x00FF0000) >> 16)
//			+ inverse_transparency * ((base_color & 0x00FF0000) >> 16)))
//				<< 16
//		| ((unsigned int)(transparency * ((added_color & 0x0000FF00) >> 8)
//			+ inverse_transparency * ((base_color & 0x0000FF00) >> 8)))
//				<< 8
//		| (unsigned int)(transparency * (added_color & 0x000000FF)
//			+ inverse_transparency * (base_color & 0x000000FF)));
}
