/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix3_rotation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 15:12:00 by tdameros          #+#    #+#             */
/*   Updated: 2023/05/09 15:12:00 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

#include "matrix.h"
#include "conversion.h"

t_matrix3	matrix3_rodrigues_rotation(t_vector3 axe, double degrees)
{
	const double	radians = convert_degrees_to_radians(degrees);
	const double	cos_result = cos(radians);
	const double	sin_result = sin(radians);
	t_matrix3		result;

	result = matrix3_create(0);
	result.matrix[0][0] = cos_result + axe.x * axe.x * (1 - cos_result);
	result.matrix[0][1] = axe.x * axe.y * (1 - cos_result) - axe.z * sin_result;
	result.matrix[0][2] = axe.x * axe.z * (1 - cos_result) + axe.y * sin_result;

	result.matrix[1][0] = axe.y * axe.x * (1 - cos_result) + axe.z * sin_result;
	result.matrix[1][1] = cos_result + axe.y * axe.y * (1 - cos_result);
	result.matrix[1][2] = axe.y * axe.z * (1 - cos_result) - axe.x * sin_result;

	result.matrix[2][0] = axe.z * axe.x * (1 - cos_result) - axe.y * sin_result;
	result.matrix[2][1] = axe.z * axe.y * (1 - cos_result) - axe.x * sin_result;
	result.matrix[2][2] = cos_result + axe.z * axe.z * (1 - cos_result);

	return (result);
}
