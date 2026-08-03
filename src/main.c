#include "../include/codixion.h"

static void free_allocation(t_coder *coders, t_dongle *dongles)
{
    free(coders);
    free(dongles);
}

static int prs_argument(int ac, char **av, t_config *config)
{
    int x;

    memset(config, 0, sizeof(t_config));
    x = parse_args(ac, av, config);
    if (x != 1)
    {
        if (x == -1)
            printf("Error: Missing argument!\n");
		return (-1);
    }
    return 1;
}

static int initialization(t_coder *coders, t_config *config, t_dongle *dongles)
{
    if (!init_coders(coders, config, dongles))
    {
        free_allocation(coders, dongles);
        return (0);
    }

    if (!init_dongles(dongles, config->number_of_coders))
    {
        free_allocation(coders, dongles);
        return (0);
    }

    if (!creat_thread(coders, config))
    {
        cleanup(config, coders, dongles);
        return (0);
    }
    return 1;
}

int main(int ac, char **av)
{
    t_config config;
    t_dongle *dongles;
    t_coder *coders;

    if (prs_argument(ac, av, &config) != 1)
        return 1;
    config.start_time = get_time_ms();
    dongles = malloc(sizeof(t_dongle) * config.number_of_coders);
    coders = malloc(sizeof(t_coder) * config.number_of_coders);
    if (!dongles || !coders)
    {
        free_allocation(coders, dongles);
        return 1;
    }
    if (!initialization(coders, &config, dongles))
        return 1;
    cleanup(&config, coders, dongles);
    return 0;
}
