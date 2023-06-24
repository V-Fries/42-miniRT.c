/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_polygon_from_contours.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 13:31:00 by vfries            #+#    #+#             */
/*   Updated: 2023/06/20 13:31:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "font/render.h"

static	t_dlist	*add_main_contour_to_polygon_points(const t_vector2f *points,
					size_t number_of_points);
static int		insert_closest_contour_in_polygon_points(
					t_dlist *polygon_points, const t_vector2f *points,
					const size_t *contours_limits, int16_t number_of_contours,
					uint8_t *contour_done);
static int16_t	get_base_closest_point(const t_vector2f *points,
					const size_t *contours_limit, int16_t number_of_contours,
					const uint8_t *contour_done, float *closest_point_distance,
					size_t *closest_point_index,
					const t_vector2f *point_in_polygon);
static int		insert_contour(t_dlist *insert_position,
					const t_vector2f *points, const size_t *contours_limits,
					int16_t contour_of_closest_point,
					size_t index_of_closest_point);
static float	get_distance_between_two_points(const t_vector2f *v1,
					const t_vector2f *v2);

t_dlist	*get_polygon_from_contours(t_vector points,
			const int16_t number_of_contours, const size_t *contours_limits)
{
	t_dlist		*polygon_points;
	uint8_t		*contour_done;

	polygon_points = add_main_contour_to_polygon_points(points.data,
			contours_limits[0]);
	if (number_of_contours == 1 || polygon_points == NULL) // TODO need to check that contours have at least one elem in parsing
		return (polygon_points);
	contour_done = ft_calloc(number_of_contours - 1, sizeof(*contour_done));
	if (contour_done == NULL)
		return (ft_dlstclear(&polygon_points, &free), NULL);
	while ((uint16_t)ft_sum_uint8_arr(contour_done, number_of_contours - 1)
			< number_of_contours - 1)
	{
		if (insert_closest_contour_in_polygon_points(polygon_points,
				points.data, contours_limits, number_of_contours,
				contour_done) < 0)
		{
			free(contour_done);
			ft_dlstclear(&polygon_points, &free);
			return (NULL);
		}
	}
	free(contour_done);
	return (polygon_points);
}

static	t_dlist	*add_main_contour_to_polygon_points(const t_vector2f *points,
					size_t number_of_points)
{
	t_vector2f	*node_content;
	t_dlist		*node;
	t_dlist		*result;

	result = NULL;
	while (number_of_points--)
	{
		node_content = malloc(sizeof(t_vector2f));
		node = ft_dlstnew(node_content);
		if (node_content == NULL || node == NULL)
		{
			free(node_content);
			free(node);
			ft_dlstclear(&result, &free);
			return (NULL);
		}
		*node_content = points[number_of_points];
		ft_dlstadd_front(&result, node);
	}
	return (result);
}

static int	insert_closest_contour_in_polygon_points(t_dlist *polygon_points,
				const t_vector2f *points, const size_t *contours_limits,
				const int16_t number_of_contours, uint8_t *contour_done)
{
	size_t	i;
	int16_t	contour;
	float	closest_point_distance;
	size_t	closest_point_index;
	int16_t	contour_of_closest_point;
	t_dlist	*point_to_insert_at;
	float	current_distance;

	contour_of_closest_point = get_base_closest_point(points, contours_limits,
		number_of_contours, contour_done, &closest_point_distance,
		&closest_point_index, polygon_points->content);
	point_to_insert_at = polygon_points;
	while (polygon_points != NULL)
	{
		contour = 0;
		while (++contour < number_of_contours)
		{
			if (contour_done[contour - 1])
				continue ;
			i = contours_limits[contour - 1] - 1;
			while (++i < contours_limits[contour])
			{
				current_distance = get_distance_between_two_points(polygon_points->content,
						points + i);
				if (current_distance < closest_point_distance)
				{
					closest_point_distance = current_distance;
					closest_point_index = i;
					point_to_insert_at = polygon_points;
					contour_of_closest_point = contour;
				}
			}
		}
		polygon_points = polygon_points->next;
	}
	if (insert_contour(point_to_insert_at, points, contours_limits,
			contour_of_closest_point, closest_point_index) < 0)
		return (-1);
	contour_done[contour_of_closest_point - 1] = 1;
	return (0);
}

static int16_t	get_base_closest_point(const t_vector2f *points,
					const size_t *contours_limit,
					const int16_t number_of_contours,
					const uint8_t *contour_done, float *closest_point_distance,
					size_t *closest_point_index,
					const t_vector2f *point_in_polygon)
{
	int16_t	contour;

	contour = 0;
	while (++contour < number_of_contours)
	{
		if (contour_done[contour - 1])
			continue ;
		*closest_point_index = contours_limit[contour - 1];
		*closest_point_distance = get_distance_between_two_points(
			point_in_polygon, points + *closest_point_index);
		return (contour);
	}
	return (contour);
}

static int	insert_contour(t_dlist *insert_position, const t_vector2f *points,
				const size_t *contours_limits,
				const int16_t contour_of_closest_point,
				const size_t index_of_closest_point)
{
	t_dlist			*next_back_up;
	size_t			i;
	t_vector2f		*node_content;
	t_dlist			*node;
	t_dlist			*cursor;

	next_back_up = insert_position->next;
	insert_position->next = NULL;
	cursor = insert_position;
	i = index_of_closest_point - 1;
	while (++i < contours_limits[contour_of_closest_point])
	{
		node_content = malloc(sizeof(t_vector2f));
		node = ft_dlstnew(node_content);
		if (node_content == NULL || node == NULL)
		{
			free(node_content);
			free(node);
			return (-1);
		}
		*node_content = points[i];
		cursor->next = node;
		node->previous = cursor;
		cursor = node;
	}
	i = contours_limits[contour_of_closest_point - 1] - 1;
	while (++i <= index_of_closest_point) // TODO could cause infinite loop if index_of_closest_point == SIZE_MAX
	{
		node_content = malloc(sizeof(t_vector2f));
		node = ft_dlstnew(node_content);
		if (node_content == NULL || node == NULL)
		{
			free(node_content);
			free(node);
			return (-1);
		}
		*node_content = points[i];
		cursor->next = node;
		node->previous = cursor;
		cursor = node;
	}
	node_content = malloc(sizeof(t_vector2f));
	node = ft_dlstnew(node_content);
	if (node_content == NULL || node == NULL)
	{
		free(node_content);
		free(node);
		return (-1);
	}
	*node_content = *((t_vector2f *)insert_position->content);
	cursor->next = node;
	node->previous = cursor;
	node->next = next_back_up;
	next_back_up->previous = node;
	return (0);
}

static float	get_distance_between_two_points(const t_vector2f *v1,
					const t_vector2f *v2)
{
	return (ft_absf(vector2f_length(vector2f_subtract(*v2, *v1))));
}
