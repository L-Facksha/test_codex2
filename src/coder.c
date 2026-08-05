/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azebahad <azebahad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 23:02:19 by azebahad          #+#    #+#             */
/*   Updated: 2026/08/05 23:03:15 by azebahad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codixion.h"

static int	all_coders_done(t_coder *coder)
{
	int	i;

	pthread_mutex_lock(&coder->config->state_mutex);
	i = 0;
	while (i < coder->config->number_of_coders)
	{
		if (coder->all_coders[i].compiles_done < coder->config->number_of_compiles_required)
		{
			pthread_mutex_unlock(&coder->config->state_mutex);
			return (0);
		}
		i++;
	}
	pthread_mutex_unlock(&coder->config->state_mutex);
	return (1);
}

static int	complet_routine(t_coder *coder, t_dongle *first, t_dongle *second)
{
	if (should_stop(coder->config))
	{
		release_dongle(first);
		release_dongle(second);
		return (-1);
	}
	release_dongle(first);
	release_dongle(second);
	set_coder_state(coder, STATE_DEBUGGING);
	print_status(coder, "is debugging");
	usleep(coder->config->time_to_debug * 1000);
	if (should_stop(coder->config))
		return (-1);
	set_coder_state(coder, STATE_REFACTORING);
	print_status(coder, "is refactoring");
	usleep(coder->config->time_to_refactor * 1000);
	pthread_mutex_lock(&coder->config->state_mutex);
	coder->compiles_done++;
	pthread_mutex_unlock(&coder->config->state_mutex);
	if (all_coders_done(coder))
		set_simulation_stop(coder->config, 1);
	return (0);
}

static int	start_routine(t_coder *coder, t_dongle *first, t_dongle *second)
{
	while (!should_stop(coder->config))
	{
		if (!request_dongles(coder, first, second))
			return (-1);
		pthread_mutex_lock(&coder->config->state_mutex);
		coder->last_compile_start = get_time_ms() - coder->config->start_time;
		pthread_mutex_unlock(&coder->config->state_mutex);
		if (should_stop(coder->config))
		{
			release_dongle(first);
			release_dongle(second);
			return (-1);
		}
		set_coder_state(coder, STATE_COMPILING);
		print_status(coder, "is compiling");
		usleep(coder->config->time_to_compile * 1000);
		if (complet_routine(coder, first, second) == -1)
			return (-1);
	}
	return (0);
}

static int	one_coder(t_coder *coder)
{
	if (!should_stop(coder->config) && !all_coders_done(coder))
	{
		pthread_mutex_lock(&coder->left->mutex);
		coder->left->taken = 1;
		pthread_mutex_unlock(&coder->left->mutex);
		print_status(coder, "has taken a dongle");
		pthread_mutex_lock(&coder->config->state_mutex);
		coder->last_compile_start = get_time_ms() - coder->config->start_time;
		pthread_mutex_unlock(&coder->config->state_mutex);
	}
	return (1);
}

void	*coder_routine(void *arg)
{
	t_coder		*coder;
	t_dongle	*first;
	t_dongle	*second;

	coder = (t_coder *)arg;
	first = coder->left;
	second = coder->right;
	if (first->id > second->id)
	{
		first = coder->right;
		second = coder->left;
	}
	if (coder->config->number_of_coders > 1 && (coder->id % 2) == 0)
		usleep(1000);
	if (coder->config->number_of_coders == 1)
	{
		one_coder(coder);
		return (NULL);
	}
	if (start_routine(coder, first, second) == -1)
		return (NULL);
	return (NULL);
}
