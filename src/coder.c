#include "../include/codexion.h"

static int all_coders_done(t_coder *coder)
{
    int i;

    pthread_mutex_lock(&coder->config->state_mutex);
    i = 0;
    while (i < coder->config->number_of_coders)
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

    coder->last_compile_start = get_time_ms() - coder->config->start_time;

    while (!should_stop(coder->config))
        usleep(1000);
    return;
}

static int finish_compile_cycle(t_coder *coder, t_dongle *first, t_dongle *seconde)
{
    if (should_stop(coder->config))
    {
        relese_dongle(first);
        relese_dongle(seconde);
        return 0;
    }
    relese_dongle(first);
    relese_dongle(seconde);
    set_coder_state(coder, STATE_DEBUGING);
    print_status(coder, "is debugging");
    usleep(coder->config->time_to_debug * 1000);
    if (should_stop(coder->config))
        return 0;
    set_coder_state(coder, STATE_REFACTORING);
    print_status(coder, "is refactoring");
    usleep(coder->config->time_to_refactor * 1000);
    pthread_mutex_lock(&coder->config->state_mutex);
    coder->compiles_done++;
    pthread_mutex_unlock(&coder->config->state_mutex);
    if (all_coders_done(coder))
        set_simulation_stop(coder->config, 1);
    return 1;
}

static void helper_routing(t_coder *coder, t_dongle *first, t_dongle *seconde)
{

    // while (!should_stop(coder->config) && !all_coders_done(coder))
    // {
    //     coder->last_compile_start = get_time_ms() - coder->config->start_time;
    //     if (!request_dongles(coder, first, seconde))
    //         break;
    //     if (should_stop(coder->config))
    //     {
    //         relese_dongle(first);
    //         relese_dongle(seconde);
    //         break;
    //     }
    //     set_coder_state(coder, STATE_COMPILING);
    //     print_status(coder, "is compiling");
    //     usleep(coder->config->time_to_compile * 1000);
    //     if (!finish_compile_cycle(coder, first, seconde))
    //         break;
    // }
    while (!should_stop(coder->config) && !all_coders_done(coder))
    {
        set_coder_state(coder, STATE_WAITING);

        if (!request_dongles(coder, first, seconde))
            break;

        if (should_stop(coder->config))
        {
            relese_dongle(first);
            relese_dongle(seconde);
            break;
        }
        coder->last_compile_start =
            get_time_ms() - coder->config->start_time;
        set_coder_state(coder, STATE_COMPILING);
        print_status(coder, "is compiling");

        usleep(coder->config->time_to_compile * 1000);

        if (!finish_compile_cycle(coder, first, seconde))
            break;
    }
}

void *coder_routine(void *arg)
{
    t_coder *coder;
    t_dongle *first;
    t_dongle *seconde;

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
    helper_routing(coder, first, seconde);
    return NULL;
}
