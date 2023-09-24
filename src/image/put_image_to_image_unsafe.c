/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_image_to_image_unsafe.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 03:05:10 by tdameros          #+#    #+#             */
/*   Updated: 2023/09/24 03:05:19 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "image.h"

void	put_image_to_image_unsafe(t_image *destination,
			const t_image *source, t_vector2i position)
{
	const unsigned int		*source_end
		= source->address + source->height * source->width;
	const int				x_max = source->width + position.x;
	register unsigned int	*source_curr;
	register unsigned int	*dest_curr;
	register int			x;

	dest_curr = destination->address + position.y * destination->width;
	x = position.x;
	source_curr = source->address;
	while (source_curr < source_end)
	{
		dest_curr[x] = mix_colors(*source_curr++, dest_curr[x]);
		x++;
		if (x >= x_max)
		{
			x = position.x;
			dest_curr += destination->width;
		}
	}
}
