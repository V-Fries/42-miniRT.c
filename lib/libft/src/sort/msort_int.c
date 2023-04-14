/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msort_int.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 18:11:00 by tdameros          #+#    #+#             */
/*   Updated: 2023/01/27 18:11:00 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	merge(int *tab, int index_start, int index_middle, int index_end);
static int	*duplicate_tab(int *tab, int index_start, int index_end);
static int	fill_tab(int *dst, int *src, int index_start, int index_end);
static	int	free_two_tab(int *tab1, int *tab2);

int	ft_msort_int(int *tab, int index_left, int index_right)
{
	int	middle;

	if (index_left < index_right)
	{
		middle = (index_left + index_right) / 2;
		if (ft_msort_int(tab, index_left, middle) < 0)
			return (-1);
		if (ft_msort_int(tab, middle + 1, index_right) < 0)
			return (-1);
		if (merge(tab, index_left, middle, index_right) < 0)
			return (-1);
	}
	return (1);
}

static int	merge(int *tab, int start, int middle, int end)
{
	int	*left_tab;
	int	*right_tab;
	int	index_left_tab;
	int	index_right_tab;
	int	index;

	left_tab = duplicate_tab(tab, start, middle);
	right_tab = duplicate_tab(tab, middle + 1, end);
	if (right_tab == NULL || left_tab == NULL)
		return (free_two_tab(right_tab, left_tab) - 1);
	index_left_tab = 0;
	index_right_tab = 0;
	index = start;
	while (index_left_tab < middle - start + 1
		&& index_right_tab < end - middle)
	{
		if (left_tab[index_left_tab] <= right_tab[index_right_tab])
			tab[index++] = left_tab[index_left_tab++];
		else
			tab[index++] = right_tab[index_right_tab++];
	}
	index += fill_tab(tab + index, right_tab, index_right_tab, end - middle);
	fill_tab(tab + index, left_tab, index_left_tab, middle - start + 1);
	return (free_two_tab(left_tab, right_tab) + 1);
}

static int	*duplicate_tab(int *tab, int index_start, int index_end)
{
	int	index;
	int	*new_tab;

	index = 0;
	new_tab = malloc(sizeof(int) * (index_end - index_start + 1));
	if (new_tab == NULL)
		return (NULL);
	while (index_start <= index_end)
	{
		new_tab[index] = tab[index_start];
		index_start++;
		index++;
	}
	return (new_tab);
}

static int	fill_tab(int *dst, int *src, int index_start, int index_end)
{
	while (index_start < index_end)
	{
		*dst = src[index_start];
		index_start++;
		dst++;
	}
	return (index_end - index_start);
}

static	int	free_two_tab(int *tab1, int *tab2)
{
	free(tab1);
	free(tab2);
	return (0);
}
