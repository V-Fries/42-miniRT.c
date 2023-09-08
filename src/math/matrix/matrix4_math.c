/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix4_math.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 18:25:00 by tdameros          #+#    #+#             */
/*   Updated: 2023/05/03 18:25:00 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "math/matrix.h"

static float	multiply_row_column(const t_matrix4 *m1, const t_matrix4 *m2,
					int y, int x);

t_matrix4	matrix4_multiply(const t_matrix4 *m1, const t_matrix4 *m2)
{
	t_matrix4	result;
	int			y;
	int			x;

	result = matrix4_create(0);
	y = 0;
	x = 0;
	while (y < 4)
	{
		while (x < 4)
		{
			result.matrix[y][x] = multiply_row_column(m1, m2, y, x);
			x++;
		}
		y++;
	}
	return (result);
}

static float	multiply_row_column(const t_matrix4 *m1, const t_matrix4 *m2,
					const int y, const int x)
{
	float	result;
	int		i;

	result = 0;
	i = 0;
	while (i < 4)
	{
		result += m1->matrix[y][i] * m2->matrix[i][x];
		i++;
	}
	return (result);
}

t_vector4f	matrix4_multiply_vector4(const t_matrix4 *matrix, t_vector4f vector)
{
	t_vector4f	result;

	result.x = matrix->matrix[0][0] * vector.x
		+ matrix->matrix[1][0] * vector.y
		+ matrix->matrix[2][0] * vector.z
		+ matrix->matrix[3][0] * vector.w;
	result.y = matrix->matrix[0][1] * vector.x
		+ matrix->matrix[1][1] * vector.y
		+ matrix->matrix[2][1] * vector.z
		+ matrix->matrix[3][1] * vector.w;
	result.z = matrix->matrix[0][2] * vector.x
		+ matrix->matrix[1][2] * vector.y
		+ matrix->matrix[2][2] * vector.z
		+ matrix->matrix[3][2] * vector.w;
	result.w = matrix->matrix[0][3] * vector.x
		+ matrix->matrix[1][3] * vector.y
		+ matrix->matrix[2][3] * vector.z
		+ matrix->matrix[3][3] * vector.w;
	return (result);
}

//	vector w value is 1
t_vector3f	matrix4_multiply_vector3(const t_matrix4 *matrix,
				t_vector3f vector)
{
	t_vector3f	result;

	result.x = matrix->matrix[0][0] * vector.x
		+ matrix->matrix[1][0] * vector.y
		+ matrix->matrix[2][0] * vector.z
		+ matrix->matrix[3][0];
	result.y = matrix->matrix[0][1] * vector.x
		+ matrix->matrix[1][1] * vector.y
		+ matrix->matrix[2][1] * vector.z
		+ matrix->matrix[3][1];
	result.z = matrix->matrix[0][2] * vector.x
		+ matrix->matrix[1][2] * vector.y
		+ matrix->matrix[2][2] * vector.z
		+ matrix->matrix[3][2];
	return (result);
}
