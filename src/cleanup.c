/* name=src/cleanup.c */
#include "../include/codexion.h"

void cleanup(t_config *config, t_coder *coders, t_dongle *dongels)
{
    int i;

    if (config)
    {
        if (config->print_mutex_inited)
            pthread_mutex_destroy(&config->print_mutex);
        if (config->state_mutex_inited)
            pthread_mutex_destroy(&config->state_mutex);
    }
    if (coders)
        free(coders);
    if (dongels)
    {
        i = 0;
        /* Only use config to know how many dongles to clean */
        int count = (config ? config->number_of_coders : 0);
        while (i < count)
        {
            if (dongels[i].scheduler.pending.data)
                free(dongels[i].scheduler.pending.data);
            pthread_mutex_destroy(&dongels[i].mutex);
            pthread_cond_destroy(&dongels[i].scheduler.cond);
            i++;
        }
        free(dongels);
    }
}

void cleanup_init_dongle(t_dongle *dongle, int count)
{
    int i;

    i = 0;
    while (i < count)
    {
        pthread_mutex_destroy(&dongle[i].mutex);
        pthread_cond_destroy(&dongle[i].scheduler.cond);
        if (dongle[i].scheduler.pending.data)
            free(dongle[i].scheduler.pending.data);
        i++;
    }
}