#include "../include/codixion.h"

static int ft_atoi(char *nb)
{
	int res;

	res = atoi(nb);
	if (res <= 0)
	{
		printf("Error: Invalid argument format\n");
		return -1;
	}
	return res;
}

static int	parse_nb(char *nb)
{
	int		i;

	if (!nb)
		return (-1);
	i = 0;
	if (nb[i] == '+')
		i++;
	if (!nb[i])
		return (-1);
	while (nb[i])
	{
		if (nb[i] < '0' || nb[i] > '9')
		{
			printf("Error: Invalid number '%c'\n", nb[i]);
			return (-1);
		}
		i++;
	}
	return ft_atoi(nb);
}

static char	*parse_scheduler(char *sch)
{
	if (strcmp(sch, "fifo") == 0)
		return (sch);
	if (strcmp(sch, "edf") == 0)
		return (sch);
	printf("Error: Last argument must be 'fifo' or 'edf'\n");
	return (NULL);
}

int	parse_args(int ac, char **av, t_config *config)
{
	int	i;
	int	res;

	if (ac != 9)
		return (-1);
	i = 1;
	while (i < ac - 1)
	{
		res = parse_nb(av[i]);
		if (res == -1)
			return (0);
		i++;
	}
	config->number_of_coders = parse_nb(av[1]);
	config->time_to_burnout = parse_nb(av[2]);
	config->time_to_compile = parse_nb(av[3]);
	config->time_to_debug = parse_nb(av[4]);
	config->time_to_refactor = parse_nb(av[5]);
	config->number_of_compiles_required = parse_nb(av[6]);
	config->dongle_cooldown = parse_nb(av[7]);
	config->scheduler = parse_scheduler(av[8]);
	if (!config->scheduler)
		return (0);
	return (1);
}
