#include "../include/codixion.h"

static int all_coders_done(t_coder *coder)
{
    int i;

    pthread_mutex_lock(&coder->config->state_mutex);
    i = 0;
    while(i < coder->config->number_of_coders)
    {
        if (coder->all_coders[i].compiles_done < coder->config->number_of_compiles_required)
        {
            pthread_mutex_unlock(&coder->config->state_mutex);
            return 0;
        }
        i++;
    }
    pthread_mutex_unlock(&coder->config->state_mutex);
    return 1;
}

static void one_coder(t_coder *coder)
{
    pthread_mutex_lock(&coder->left->mutex);
    coder->left->taken = 1;
    pthread_mutex_unlock(&coder->left->mutex);

    print_status(coder, "has taken a dongle");

    coder->last_compile_start = 0;

    while(!should_stop(coder->config))
        usleep(1000);
    return -1;
}

void *coder_routine(void *arg)
{
    t_coder *coder;
    t_dongle *first;
    t_dongle *seconde;
    long now;

    coder = (t_coder *)arg;
    first = coder->left;
    seconde = coder->right;
    
    if (first->id > seconde->id)
    {
        first = coder->right;
        seconde = coder->left;
    }

    if (coder->config->number_of_coders == 1)
    {
        one_coder(coder);
        return NULL;
    }

    while (!should_stop(coder->config) && !all_coders_done(coder))
    {
        if (!should_stop(coder->config))
            break;
    }
}