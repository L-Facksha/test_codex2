#include "../include/codexion.h"

static int routing_for_each_thread(t_coder *coders, t_config *config)
{
    int i;

    i = 0;
    while(i < config->number_of_coders)
    {
        if (pthread_create(&coders[i].thread, NULL, coder_routine, &coders[i]) != 0)
        {
            set_simulation_stop(config, 0);
            while (i > 0)
            {
                i--;
                pthread_join(coders[i].thread, NULL);
            }
            return 0;
        }
        i++;
    }
    i = 0;
    while(i < config->number_of_coders)
    {
        if (pthread_join(coders[i].thread, NULL) != 0)
            return 0;
        i++;
    }
    return 1;
}

int creat_thread(t_coder *coders, t_config *config)
{
    t_runtime runtime;
    pthread_t monitor_thread;

    config->print_mutex_inited = 0;
    config->state_mutex_inited = 0;

    if (pthread_mutex_init(&config->print_mutex, NULL) == 0)
        config->print_mutex_inited = 1;
    else
        return 0;

    if (pthread_mutex_init(&config->state_mutex, NULL) == 0)
        config->state_mutex_inited = 1;
    else
    {
        if (config->print_mutex_inited)
            pthread_mutex_destroy(&config->print_mutex);
        return 0;
    }

    config->stop = 0;
    config->all_done = 0;
    runtime.coders = coders;
    runtime.config = config;

    if (pthread_create(&monitor_thread, NULL, monitor_routine, &runtime) != 0)
    {
        /* Clean up initialized mutexes before returning */
        if (config->state_mutex_inited)
            pthread_mutex_destroy(&config->state_mutex);
        if (config->print_mutex_inited)
            pthread_mutex_destroy(&config->print_mutex);
        return 0;
    }

    if (!routing_for_each_thread(coders, config))
    {
        set_simulation_stop(config, 0);
        pthread_join(monitor_thread, NULL);
        return (0);
    }
    pthread_join(monitor_thread, NULL);
    return 1;
}