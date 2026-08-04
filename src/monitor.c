/* name=src/monitor.c */
#include "../include/codexion.h"

static int check_burnout(t_coder *coders, t_config *config)
{
    int i;
    long now;

    i = 0;
    while (i < config->number_of_coders && !should_stop(config))
    {
        now = get_time_ms() - config->start_time;

        pthread_mutex_lock(&config->state_mutex);
        if (config->stop)
        {
            pthread_mutex_unlock(&config->state_mutex);
            return 0;
        }

        int state = coders[i].state;
        long last_start = coders[i].last_compile_start;
        int burned = coders[i].burned_out;
        pthread_mutex_unlock(&config->state_mutex);

        if (!burned && state == STATE_WAITING)
        {
            if (now - last_start >= config->time_to_burnout)
            {
                pthread_mutex_lock(&config->state_mutex);
                coders[i].burned_out = 1;
                pthread_mutex_unlock(&config->state_mutex);

                set_coder_state(&coders[i], STATE_BURNED_OUT);
                print_burnout(&coders[i]);
                set_simulation_stop(config, 0);
                return -1;
            }
        }
        i++;
    }
    return 0;
}

void *monitor_routine(void *arg)
{
    t_runtime *runtime;
    t_coder *coders;
    t_config *config;

    runtime = (t_runtime *)arg;
    coders = runtime->coders;
    config = runtime->config;
    while (!should_stop(config))
    {
        if (check_burnout(coders, config) != 0)
            break;
        usleep(1000);
    }
    return NULL;
}