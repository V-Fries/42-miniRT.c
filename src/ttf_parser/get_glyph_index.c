/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_glyph_index.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 14:41:00 by vfries            #+#    #+#             */
/*   Updated: 2023/05/15 14:41:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

#include "ttf.h"

static int	get_index(uint16_t code_point, t_format4 *format4);

int get_glyph_index(uint16_t code_point, t_format4 *format4)
{
	const int	index = get_index(code_point, format4);
	uint16_t	*ptr;

	// TODO read documentation to check this function
	if (index == -1)
		return (0);
	if (format4->startCode[index] < code_point)
	{
		if (format4->idRangeOffset[index] == 0)
			return (code_point + format4->idDelta[index]);
		ptr = format4->idRangeOffset + index + format4->idRangeOffset[index]
			/ 2;
		ptr += code_point - format4->startCode[index];
		if (*ptr == 0)
			return (0);
		return (*ptr + format4->idDelta[index]);
	}
	return (0);
}

static int	get_index(uint16_t code_point, t_format4 *format4)
{
	uint16_t		i;
	const uint16_t	i_limit = format4->segCountX2 / 2;

	i = -1;
	while (++i < i_limit)
	{
		if (format4->endCode[i] > code_point)
			return (i);
	}
	return (-1);
}
