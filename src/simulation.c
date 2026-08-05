/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azebahad <azebahad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 23:01:14 by azebahad          #+#    #+#             */
/*   Updated: 2026/08/06 00:44:10 by azebahad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codixion.h"

static int	create_coder_threads(t_config *config, t_coder *coders,
		pthread_t monitor_thread)
{
	int	created;

	created = 0;
	while (created < config->number_of_coders)
	{
		if (pthread_create(&coders[created].thread, NULL, coder_routine,
				&coders[created]) != 0)
		{
			set_simulation_stop(config, 0);
			while (--created >= 0)
				pthread_join(coders[created].thread, NULL);
			pthread_join(monitor_thread, NULL);
			return (0);
		}
		created++;
	}
	return (1);
}

static int	join_threads(t_config *config, t_coder *coders,
		pthread_t monitor_thread)
{
	int	created;
	int	all_join;

	created = 0;
	all_join = 1;
	while (created < config->number_of_coders)
	{
		if (pthread_join(coders[created].thread, NULL) != 0)
			all_join = 0;
		created++;
	}
	if (pthread_join(monitor_thread, NULL) != 0)
		all_join = 0;
	return (all_join);
}

static int	full_routine(t_config *config, t_coder *coders)
{
	t_runtime	runtime;
	pthread_t	monitor_thread;

	runtime.coders = coders;
	runtime.config = config;
	if (pthread_create(&monitor_thread, NULL, monitor_routine, &runtime) != 0)
		return (0);
	if (!create_coder_threads(config, coders, monitor_thread))
		return (0);
	return (join_threads(config, coders, monitor_thread));
}

int	create_threads(t_coder *coders, t_config *config)
{
	config->print_mutex_inited = 0;
	config->state_mutex_inited = 0;
	if (pthread_mutex_init(&config->print_mutex, NULL) != 0)
		return (0);
	config->print_mutex_inited = 1;
	if (pthread_mutex_init(&config->state_mutex, NULL) != 0)
	{
		if (config->print_mutex_inited)
			pthread_mutex_destroy(&config->print_mutex);
		return (0);
	}
	config->state_mutex_inited = 1;
	config->stop = 0;
	config->all_done = 0;
	if (!full_routine(config, coders))
		return (0);
	return (1);
}
