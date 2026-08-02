#include "../include/codixion.h"


void print_status(t_coder *coder, const char *status)
{
    long timestamp;

    pthread_mutex_lock(&coder->config->print_mutex);
    timestamp = get_time_ms() - coder->config->start_time;
    printf("%ld %d %s\n", timestamp, coder->id, status);
    pthread_mutex_unlock(&coder->config->print_mutex);
}

void print_burnout(t_coder *coder)
{
    long timestamp;

    pthread_mutex_lock(&coder->config->print_mutex);
    timestamp = get_time_ms() - coder->config->start_time;
    printf("%ld %d burned out\n", timestamp, coder->id);
    pthread_mutex_unlock(&coder->config->print_mutex);
}

void set_coder_state(t_coder *coder, t_state *state)
{
    pthread_mutex_lock(&coder->config->state_mutex);
    coder->state = state;
    pthread_mutex_unlock(&coder->config->state_mutex);
}

void set_simulation_stop(t_config *config, int all_done)
{
    pthread_mutex_lock(&config->state_mutex);
    config->stop = 1;
    config->all_done = all_done;
    pthread_mutex_unlock(&config->state_mutex);
}

int should_stop(t_config *config)
{
    int stop;
    pthread_mutex_lock(&config->state_mutex);
    stop = config->stop;
    pthread_mutex_unlock(&config->state_mutex);
    return stop;
}

void wake_all_dongles(t_dongle *dongles, int count)
{
    int i;

    i = 0;
    while(i < count)
    {
        pthread_mutex_lock(&dongles[i].mutex);
        pthread_cond_broadcast(&dongles[i].scheduler.cond);
        pthread_mutex_unlock(&dongles[i].mutex);
        i++;
    }
}