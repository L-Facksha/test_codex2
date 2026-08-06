/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   release_dongle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azebahad <azebahad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 01:01:29 by azebahad          #+#    #+#             */
/*   Updated: 2026/08/06 01:01:58 by azebahad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codixion.h"

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->taken = 0;
	dongle->last_released_at = get_time_ms();
	pthread_cond_broadcast(&dongle->scheduler.cond);
	pthread_mutex_unlock(&dongle->mutex);
}
