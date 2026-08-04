#include "../include/codexion.h"


void set_coder_state(t_coder *coder, t_state state)
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