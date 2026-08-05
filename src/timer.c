/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azebahad <azebahad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 23:01:21 by azebahad          #+#    #+#             */
/*   Updated: 2026/08/05 14:02:57 by azebahad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codixion.h"

long	get_time_ms(void)
{
	struct timeval	current;

	if (gettimeofday(&current, NULL) != 0)
		return (-1);
	return ((current.tv_sec * 1000) + (current.tv_usec / 1000));
}
