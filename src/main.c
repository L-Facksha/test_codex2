/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azebahad <azebahad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 23:01:50 by azebahad          #+#    #+#             */
/*   Updated: 2026/08/08 19:16:34 by azebahad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codixion.h"

static int	prs_argument(int ac, char **av, t_config *config)
{
	int	x;

	memset(config, 0, sizeof(t_config));
	x = parse_args(ac, av, config);
	if (x == -1 || x == -2)
	{
		if (x == -1)
			printf("Error: Invalid number of arguments\n");
		else if (x == -2)
			printf("Error: Invalid argument format\n");
		return (-1);
	}
	return (1);
}

int	main(int ac, char **av)
{
	t_config	config;
	t_dongle	*dongles;
	t_coder		*coders;

	if (prs_argument(ac, av, &config) == -1)
		return (1);
	config.start_time = get_time_ms();
	dongles = malloc(sizeof(t_dongle) * config.number_of_coders);
	coders = malloc(sizeof(t_coder) * config.number_of_coders);
	if (!dongles || !coders)
	{
		cleanup(&config, coders, dongles);
		return (1);
	}
	config.dongles = dongles;
	if (!init_dongles(dongles, config.number_of_coders) || !init_coders(coders,
			dongles, &config) || !create_threads(coders, &config))
	{
		cleanup(&config, coders, dongles);
		return (1);
	}
	cleanup(&config, coders, dongles);
	return (0);
}
