#include "../include/codexion.h"


void	cleanup(t_config *config, t_coder *coders, t_dongle *dongels)
{
	int	i;

	if (config)
	{
		pthread_mutex_destroy(&config->print_mutex);
		pthread_mutex_destroy(&config->state_mutex);
	}
	if (coders)
		free(coders);
	if (dongels)
	{
		i = 0;
		while (i < config->number_of_coders)
		{
			if (dongels[i].scheduler.pending.data)
				free(dongels[i].scheduler.pending.data);
			pthread_mutex_destroy(&dongels[i].mutex);
			pthread_mutex_destroy(&dongels[i].scheduler.mutex);
			pthread_cond_destroy(&dongels[i].scheduler.cond);
			i++;
		}
		free(dongels);
	}
}

void	cleanup_init_dongle(t_dongle *dongle, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&dongle[i].mutex);
		pthread_mutex_destroy(&dongle[i].scheduler.mutex);
		pthread_cond_destroy(&dongle[i].scheduler.cond);
		if (dongle[i].scheduler.pending.data)
			free(dongle[i].scheduler.pending.data);
		i++;
	}
}