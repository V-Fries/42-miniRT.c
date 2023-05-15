/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_format4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 22:19:00 by vfries            #+#    #+#             */
/*   Updated: 2023/05/14 22:19:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ttf.h"

int	read_format4(const t_string *file, t_format4 **dest_format4, size_t i)
{
	const size_t	i_starting_value = i;
	uint16_t		length;
	t_format4		*format4;

	if (read_uint16(file, i + 2, &length) < 0)
		return (-1);
	format4 = ft_calloc(1, length + sizeof(uint16_t *) * 5); // TODO free in error case
	if (format4 == NULL)
		return (-1);
	format4->format = read_uint16_unsafe(file->data + i);
	i += sizeof(uint16_t);
	format4->length = length;
	i += sizeof(uint16_t);
	if (read_uint16_move(file, &i, &format4->language) < 0)
		return (-1);
	if (read_uint16_move(file, &i, &format4->segCountX2) < 0)
		return (-1);
	if (read_uint16_move(file, &i, &format4->searchRange) < 0)
		return (-1);
	if (read_uint16_move(file, &i, &format4->entrySelector) < 0)
		return (-1);
	if (read_uint16_move(file, &i, &format4->rangeShift) < 0)
		return (-1);

	format4->endCode = (uint16_t *)((uint8_t *)format4 + sizeof(t_format4));
	format4->startCode = format4->endCode + format4->segCountX2 / 2;
	format4->idDelta = format4->startCode + format4->segCountX2 / 2;
	format4->idRangeOffset = format4->idDelta + format4->segCountX2 / 2;
	format4->glyphIdArray = format4->idRangeOffset + format4->segCountX2 / 2;

	const size_t	start_code_start = i + format4->segCountX2 + sizeof(int16_t);
	const size_t	id_delta_start = i + format4->segCountX2 * 2 + sizeof(int16_t);
	const size_t	id_range_start = i + format4->segCountX2 * 3 + sizeof(int16_t);

	for (int j = 0; j < format4->segCountX2 / 2; ++j)
	{
		if (read_uint16(file, i + j * 2, format4->endCode + j) < 0)
			return (-1);
		if (read_uint16(file, start_code_start + j * 2,
				format4->startCode + j) < 0)
			return (-1);
		if (read_uint16(file, id_delta_start + j * 2,
				format4->idDelta + j) < 0)
			return (-1);
		if (read_uint16(file, id_range_start + j * 2,
				format4->idRangeOffset + j) < 0)
			return (-1);
	}

	i += format4->segCountX2 * 4 + sizeof(int16_t);


	size_t remaining_bytes_divided_by_2 = (format4->length
			- (i - i_starting_value)) / 2;
	uint	j;
	j = -1;
	while (++j < remaining_bytes_divided_by_2)
		if (read_uint16_move(file, &i, format4->glyphIdArray + j) < 0)
			return (-1);

	*dest_format4 = format4;
	return (0);
}
