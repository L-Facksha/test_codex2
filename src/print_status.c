/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_status.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azebahad <azebahad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 00:59:57 by azebahad          #+#    #+#             */
/*   Updated: 2026/08/06 01:01:54 by azebahad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codixion.h"

void	print_status(t_coder *coder, const char *status)
{
	long	timestamp;

	pthread_mutex_lock(&coder->config->print_mutex);
	timestamp = get_time_ms() - coder->config->start_time;
	printf("%ld %d %s\n", timestamp, coder->id, status);
	pthread_mutex_unlock(&coder->config->print_mutex);
}

void	print_burnout(t_coder *coder)
{
	long	timestamp;

	pthread_mutex_lock(&coder->config->print_mutex);
	timestamp = get_time_ms() - coder->config->start_time;
	printf("%ld %d burned out\n", timestamp, coder->id);
	pthread_mutex_unlock(&coder->config->print_mutex);
}
