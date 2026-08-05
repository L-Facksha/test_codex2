/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azebahad <azebahad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 23:01:34 by azebahad          #+#    #+#             */
/*   Updated: 2026/08/05 23:03:52 by azebahad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codixion.h"


int	request_dongles(t_coder *coder, t_dongle *first, t_dongle *second)
{
	t_dongle		*left;
	t_dongle		*right;
	struct timespec	timeout;
	long			wait_ms;
	long			left_last;
	long			right_last;
	long			now;
	long			left_wait;
	long			right_wait;

	left = first;
	right = second;
	if (left->id > right->id)
	{
		left = second;
		right = first;
	}
	if (!push_coder_request(coder, left) || !push_coder_request(coder, right))
	{
		if (left->scheduler.pending.size > 0)
			remove_request(coder, left);
		if (right->scheduler.pending.size > 0)
			remove_request(coder, right);
		return (0);
	}
	while (!should_stop(coder->config))
	{
		pthread_mutex_lock(&left->mutex);
		pthread_mutex_lock(&right->mutex);
		if (can_grant_pair(coder, left, right))
		{
			left->taken = 1;
			right->taken = 1;
			heap_pop(&left->scheduler.pending, coder->config->scheduler);
			heap_pop(&right->scheduler.pending, coder->config->scheduler);
			pthread_cond_broadcast(&left->scheduler.cond);
			pthread_cond_broadcast(&right->scheduler.cond);
			pthread_mutex_unlock(&right->mutex);
			pthread_mutex_unlock(&left->mutex);
			print_status(coder, "has taken a dongle");
			print_status(coder, "has taken a dongle");
			return (1);
		}
		pthread_mutex_unlock(&right->mutex);
		pthread_mutex_unlock(&left->mutex);
		/* Read last_released_at under each dongle mutex to avoid data race with release_dongle */
		left_last = 0;
		right_last = 0;
		pthread_mutex_lock(&left->mutex);
		left_last = left->last_released_at;
		pthread_mutex_unlock(&left->mutex);
		if (right != left)
		{
			pthread_mutex_lock(&right->mutex);
			right_last = right->last_released_at;
			pthread_mutex_unlock(&right->mutex);
		}
		else
			right_last = left_last; /* same dongle */
		/* compute remaining cooldown (ms). Keep your original semantics (minimal change) */
		now = get_time_ms();
		left_wait = 1;
		right_wait = 1;
		if (left_last != 0)
		{
			left_wait = coder->config->dongle_cooldown - (now - left_last);
			if (left_wait < 1)
				left_wait = 1;
		}
		if (right_last != 0)
		{
			right_wait = coder->config->dongle_cooldown - (now - right_last);
			if (right_wait < 1)
				right_wait = 1;
		}
		wait_ms = (left_wait < right_wait) ? left_wait : right_wait;
		set_wait_timeout(&timeout, wait_ms);
		pthread_mutex_lock(&left->mutex);
		pthread_cond_timedwait(&left->scheduler.cond, &left->mutex, &timeout);
		pthread_mutex_unlock(&left->mutex);
		pthread_mutex_lock(&right->mutex);
		pthread_cond_timedwait(&right->scheduler.cond, &right->mutex, &timeout);
		pthread_mutex_unlock(&right->mutex);
	}
	remove_request(coder, left);
	remove_request(coder, right);
	return (0);
}

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->taken = 0;
	dongle->last_released_at = get_time_ms();
	pthread_cond_broadcast(&dongle->scheduler.cond);
	pthread_mutex_unlock(&dongle->mutex);
}
