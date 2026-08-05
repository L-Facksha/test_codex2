#include "../include/codixion.h"

void	set_wait_timeout(struct timespec *timeout, long wait_ms)
{
	struct timeval	current;

	gettimeofday(&current, NULL);
	timeout->tv_sec = current.tv_sec + (wait_ms / 1000);
	timeout->tv_nsec = (current.tv_usec * 1000) + ((wait_ms % 1000) * 1000000);
	while (timeout->tv_nsec >= 1000000000)
	{
		timeout->tv_nsec -= 1000000000;
		timeout->tv_sec++;
	}
}

long	get_request_deadline(t_coder *coder)
{
	long	now;

	now = get_time_ms() - coder->config->start_time;
	if (coder->last_compile_start >= 0)
		return (coder->last_compile_start + coder->config->time_to_burnout);
	return (now + coder->config->time_to_burnout);
}

static int	can_grant_dongle(t_coder *coder, t_dongle *dongle)
{
	long	now;

	if (dongle->taken)
		return (0);
	now = get_time_ms();
	if (dongle->last_released_at != 0 && (now
			- dongle->last_released_at) < coder->config->dongle_cooldown)
		return (0);
	return (heap_peek(&dongle->scheduler.pending).coder_id == coder->id);
}

int	can_grant_pair(t_coder *coder, t_dongle *left, t_dongle *right)
{
	if (left->taken || right->taken)
		return (0);
	return (can_grant_dongle(coder, left) && can_grant_dongle(coder, right));
}

int	push_coder_request(t_coder *coder, t_dongle *dongle)
{
	t_request req;

	req.coder_id = coder->id;
	req.timestamp = get_time_ms() - coder->config->start_time;
	req.deadline = get_request_deadline(coder);
	pthread_mutex_lock(&dongle->mutex);
	if (!heap_push(&dongle->scheduler.pending, req, coder->config->scheduler))
	{
		pthread_mutex_unlock(&dongle->mutex);
		return (0);
	}
	pthread_cond_broadcast(&dongle->scheduler.cond);
	pthread_mutex_unlock(&dongle->mutex);
	return (1);
}