/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azebahad <azebahad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 23:01:56 by azebahad          #+#    #+#             */
/*   Updated: 2026/08/08 22:23:56 by azebahad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codixion.h"

static int	init_resources(t_dongle *dongles, int i, int count)
{
	dongles[i].id = i + 1;
	dongles[i].taken = 0;
	dongles[i].last_released_at = 0;
	dongles[i].scheduler.pending.capacity = count;
	dongles[i].scheduler.pending.size = 0;
	dongles[i].scheduler.pending.data = malloc(sizeof(t_request) * count);
	if (!dongles[i].scheduler.pending.data)
	{
		cleanup_init_dongle(dongles, i);
		return (0);
	}
	return (1);
}

int	init_dongles(t_dongle *dongles, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (!init_resources(dongles, i, count))
			return (0);
		if (pthread_mutex_init(&dongles[i].mutex, NULL) != 0)
			return (fail_mutex(dongles, i));
		i++;
	}
	return (1);
}

int	init_coders(t_coder *coders, t_dongle *dongles, t_config *config)
{
	int	i;

	i = 0;
	while (i < config->number_of_coders)
	{
		coders[i].id = i + 1;
		coders[i].left = &dongles[i];
		if (config->number_of_coders == 1)
			coders[i].right = &dongles[0];
		else
			coders[i].right = &dongles[(i + 1) % config->number_of_coders];
		coders[i].config = config;
		coders[i].last_compile_start = -1;
		coders[i].compiles_done = 0;
		coders[i].state = STATE_WAITING;
		coders[i].burned_out = 0;
		coders[i].all_coders = coders;
		i++;
	}
	return (1);
}
