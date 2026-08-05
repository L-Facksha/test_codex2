/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azebahad <azebahad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 23:01:14 by azebahad          #+#    #+#             */
/*   Updated: 2026/08/05 19:02:52 by azebahad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codixion.h"

int	create_threads(t_coder *coders, t_config *config)
{
	int i;
	t_runtime runtime;
	pthread_t monitor_thread;

	/* initialize flags */
	config->print_mutex_inited = 0;
	config->state_mutex_inited = 0;

	if (pthread_mutex_init(&config->print_mutex, NULL) != 0)
		return (0);
	config->print_mutex_inited = 1;

	if (pthread_mutex_init(&config->state_mutex, NULL) != 0)
	{
		/* rollback print mutex */
		if (config->print_mutex_inited)
			pthread_mutex_destroy(&config->print_mutex);
		return (0);
	}
	config->state_mutex_inited = 1;
	config->stop = 0;
	config->all_done = 0;
	runtime.coders = coders;
	runtime.config = config;
	if (pthread_create(&monitor_thread, NULL, monitor_routine, &runtime) != 0)
		return (0);
	i = 0;
	while (i < config->number_of_coders)
	{
		if (pthread_create(&coders[i].thread, NULL, coder_routine,
				&coders[i]) != 0)
			return (0);
		i++;
	}
	i = 0;
	while (i < config->number_of_coders)
	{
		if (pthread_join(coders[i].thread, NULL) != 0)
			return (0);
		i++;
	}
	pthread_join(monitor_thread, NULL);
	return (1);
}
