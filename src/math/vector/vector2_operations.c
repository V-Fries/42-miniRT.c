/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2_operations.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 17:54:48 by tdameros          #+#    #+#             */
/*   Updated: 2023/05/03 17:54:49 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include "math/vector.h"

float	vector2_length_squared(t_vector2 vector)
{
	return (vector.x * vector.x + vector.y * vector.y);
}

float	vector2_length(t_vector2 vector)
{
	return (sqrt(vector2_length_squared(vector)));
}

t_vector2	vector2_unit(t_vector2 vector)
{
	return (vector2_divide(vector, vector2_length(vector)));
}

float	vector2_dot(t_vector2 v1, t_vector2 v2)
{
	return (v1.x * v2.x + v1.y * v2.y);
}
