/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azebahad <azebahad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 23:01:26 by azebahad          #+#    #+#             */
/*   Updated: 2026/08/08 22:19:43 by azebahad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codixion.h"

void	set_coder_state(t_coder *coder, t_state state)
{
	pthread_mutex_lock(&coder->config->state_mutex);
	coder->state = state;
	pthread_mutex_unlock(&coder->config->state_mutex);
}

void	set_simulation_stop(t_config *config, int all_done)
{
	pthread_mutex_lock(&config->state_mutex);
	config->stop = 1;
	config->all_done = all_done;
	pthread_mutex_unlock(&config->state_mutex);
}

int	should_stop(t_config *config)
{
	int	stop;

	pthread_mutex_lock(&config->state_mutex);
	stop = config->stop;
	pthread_mutex_unlock(&config->state_mutex);
	return (stop);
}

int	coder_done(t_coder *coder)
{
	int	done;

	pthread_mutex_lock(&coder->config->state_mutex);
	done = coder->compiles_done >= coder->config->number_of_compiles_required;
	pthread_mutex_unlock(&coder->config->state_mutex);
	return (done);
}
