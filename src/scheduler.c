/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azebahad <azebahad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 23:01:34 by azebahad          #+#    #+#             */
/*   Updated: 2026/08/08 22:05:02 by azebahad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codixion.h"

static int	get_pair(t_coder *coder, t_dongle *left, t_dongle *right)
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
	return (0);
}

static void	check_cooldown(long left_last, long right_last, t_coder *coder,
		long *wait_ms)
{
	long	now;
	long	left_wait;
	long	right_wait;

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
	if (left_wait < right_wait)
		*wait_ms = left_wait;
	else
		*wait_ms = right_wait;
	if (*wait_ms > 50)
		*wait_ms = 50;
}

static void	wait_for_cooldown(t_dongle *left, t_dongle *right, long wait_ms)
{
	(void)left;
	(void)right;
	if (wait_ms > 5)
		wait_ms = 5;
	usleep(wait_ms * 1000);
}

static int	wait_for_dongles(t_coder *coder, t_dongle *left, t_dongle *right)
{
	long	left_last;
	long	right_last;
	long	wait_ms;

	while (!should_stop(coder->config))
	{
		if (get_pair(coder, left, right) == 1)
			return (1);
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
		check_cooldown(left_last, right_last, coder, &wait_ms);
		// usleep(1000);
		wait_for_cooldown(left, right, wait_ms);
	}
	return (0);
}

int	request_dongles(t_coder *coder, t_dongle *first, t_dongle *second)
{
	t_dongle	*left;
	t_dongle	*right;

	left = first;
	right = second;
	if (left->id > right->id)
	{
		left = second;
		right = first;
	}
	if (left == right)
		return (0);
	if (!push_coder_request(coder, left) || !push_coder_request(coder, right))
	{
		remove_request(coder, left);
		remove_request(coder, right);
		return (0);
	}
	if (wait_for_dongles(coder, left, right) == 1)
		return (1);
	remove_request(coder, left);
	remove_request(coder, right);
	return (0);
}
