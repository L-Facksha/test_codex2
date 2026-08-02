#include "../include/codixion.h"

int	fail_mutex(t_dongle *dongles, int i)
{
	free(dongles[i].scheduler.pending.data);
	cleanup_init_dongle(dongles, i);
	return (0);
}

int	fail_scheduler_mutex(t_dongle *dongles, int i)
{
	free(dongles[i].scheduler.pending.data);
	pthread_mutex_destroy(&dongles[i].mutex);
	cleanup_init_dongle(dongles, i);
	return (0);
}

int	fail_cond(t_dongle *dongles, int i)
{
	free(dongles[i].scheduler.pending.data);
	pthread_mutex_destroy(&dongles[i].mutex);
	pthread_mutex_destroy(&dongles[i].scheduler.mutex);
	cleanup_init_dongle(dongles, i);
	return (0);
}