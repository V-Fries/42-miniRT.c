/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   absf.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 16:31:00 by vfries            #+#    #+#             */
/*   Updated: 2023/06/20 16:31:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

float	ft_absf(float i)
{
	return ((i < 0.f) * (-i) + (i >= 0.f) * i);
}
