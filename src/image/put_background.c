/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_background.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 03:07:30 by tdameros          #+#    #+#             */
/*   Updated: 2023/09/24 03:07:31 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "image.h"

void	put_background(t_image *destination, const t_image *source)
{
	register const unsigned int	*dest_end
		= destination->address + destination->height * destination->width;
	register unsigned int		*dest_curr;
	register unsigned int		*source_curr;

	source_curr = source->address;
	dest_curr = destination->address;
	while (dest_curr < dest_end)
		*dest_curr++ = *source_curr++;
}
