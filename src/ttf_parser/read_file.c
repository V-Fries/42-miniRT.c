/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 06:07:00 by vfries            #+#    #+#             */
/*   Updated: 2023/05/12 06:07:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	read_uint16(const t_string *file, const size_t i, uint16_t *dest)
{
	char	*read_address;

	if (i + sizeof(uint16_t) > file->len)
		return (-1);
	read_address = file->data + i;
	*dest = (read_address[0] << 8) | read_address[1];
	return (0);
}

int	read_uint32(const t_string *file, const size_t i, uint32_t *dest)
{
	char	*read_address;

	if (i + sizeof(uint32_t) > file->len)
		return (-1);
	read_address = file->data + i;
	*dest = (read_address[0] << 24) | (read_address[1] << 16)
		| (read_address[2] << 8) | read_address[3];
	return (0);
}

int	read_uint_16_move(const t_string *file, size_t *i, uint16_t *dest)
{
	if (read_uint16(file, *i, dest) < 0)
		return (-1);
	*i += sizeof(uint16_t);
	return (0);
}

int	read_uint_32_move(const t_string *file, size_t *i, uint32_t *dest)
{
	if (read_uint32(file, *i, dest) < 0)
		return (-1);
	*i += sizeof(uint32_t);
	return (0);
}
