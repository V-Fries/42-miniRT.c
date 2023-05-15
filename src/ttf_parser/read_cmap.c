/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_cmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 14:21:00 by vfries            #+#    #+#             */
/*   Updated: 2023/05/12 14:21:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"

#include "ttf.h"

static int	read_cmap_encoding_subtable(const t_string *file, t_cmap *cmap,
				size_t i);
static int	error_read_cmap_encoding_subtable(t_cmap *cmap);
static bool	unicode_with_platform_specific_id_3_found_in_cmap(t_cmap *cmap);

int	read_cmap(const t_string *file, size_t i, t_cmap *cmap)
{
	if (read_uint16_move(file, &i, &cmap->version) < 0)
		return (-1);
	if (read_uint16_move(file, &i, &cmap->number_subtables) < 0)
		return (-1);
	return (read_cmap_encoding_subtable(file, cmap, i));
}

static int	read_cmap_encoding_subtable(const t_string *file, t_cmap *cmap,
				size_t i)
{
	size_t						j;
	t_cmap_encoding_subtable	*current_subtable;

	cmap->subtables = ft_calloc(cmap->number_subtables,
			sizeof(*cmap->subtables));
	if (cmap->subtables == NULL)
		return (-1);
	j = -1;
	while (++j < cmap->number_subtables)
	{
		current_subtable = cmap->subtables + j;
		if (read_uint16_move(file, &i, &current_subtable->platform_id) < 0)
			return (error_read_cmap_encoding_subtable(cmap));
		if (read_uint16_move(file, &i, &current_subtable->platform_specific_id)
			< 0)
			return (error_read_cmap_encoding_subtable(cmap));
		if (read_uint32_move(file, &i, &current_subtable->offset) < 0)
			return (error_read_cmap_encoding_subtable(cmap));
	}
	if (unicode_with_platform_specific_id_3_found_in_cmap(cmap))
		return (0);
	free(cmap->subtables);
	return (-1);
}

static int	error_read_cmap_encoding_subtable(t_cmap *cmap)
{
	free(cmap->subtables);
	cmap->subtables = NULL;
	return (-1);
}

static bool	unicode_with_platform_specific_id_3_found_in_cmap(t_cmap *cmap)
{
	uint16_t	i;

	i = cmap->number_subtables;
	while (i--)
	{
		if (cmap->subtables[i].platform_id == 0
			&& cmap->subtables[i].platform_specific_id == 3)
			return (true);
	}
	ft_putstr_fd("Error: font needs to have a unicode cmap with platform "
		"specific id == 3\n", STDERR_FILENO);
	return (false);
}
