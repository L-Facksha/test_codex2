/* name=src/coder.c */
#include "../include/codexion.h"

/* small helper: initial stagger */
static void initial_stagger(t_coder *coder)
{
    if (coder->config->number_of_coders > 1)
    {
        if ((coder->id % 2) == 0)
            usleep(1000); /* 1 ms */
    }
}

static int check_stop_after_get(t_coder *coder)
{
    t_config *cfg = coder->config;
    if (should_stop(cfg))
    {
        relese_dongle(coder->left);
        if (coder->left != coder->right)
            relese_dongle(coder->right);
        return 1;
    }
    return 0;
}

static void do_compile_and_release(t_coder *coder)
{
    t_config *cfg = coder->config;
    print_status(coder, "is compiling");
    usleep(cfg->time_to_compile * 1000);

    /* Record the time after the compile so burnout is measured from compile finish */
    pthread_mutex_lock(&cfg->state_mutex);
    coder->last_compile_start = get_time_ms() - cfg->start_time;
    pthread_mutex_unlock(&cfg->state_mutex);

    relese_dongle(coder->left);
    if (coder->left != coder->right)
        relese_dongle(coder->right);
}

static void do_debug_and_refactor(t_coder *coder)
{
    t_config *cfg = coder->config;
    set_coder_state(coder, STATE_DEBUGGING);
    print_status(coder, "is debugging");
    usleep(cfg->time_to_debug * 1000);

    set_coder_state(coder, STATE_REFACTORING);
    print_status(coder, "is refactoring");
    usleep(cfg->time_to_refactor * 1000);

    /* after refactor, go back to waiting */
    set_coder_state(coder, STATE_WAITING);
}

static int everyone_done(t_config *cfg, t_coder *coders)
{
    int i;
    for (i = 0; i < cfg->number_of_coders; i++)
    {
        if (coders[i].compiles_done < cfg->number_of_compiles_required)
            return 0;
    }
    return 1;
}

static int check_and_stop_if_done(t_coder *coder)
{
    t_config *cfg = coder->config;
    int stop = 0;

    pthread_mutex_lock(&cfg->state_mutex);
    coder->compiles_done++;
    if (cfg->number_of_compiles_required > 0 && everyone_done(cfg, coder->all_coders))
    {
        cfg->stop = 1;
        cfg->all_done = 1;
        stop = 1;
    }
    pthread_mutex_unlock(&cfg->state_mutex);

    if (stop)
    {
        /* wake everyone */
        if (cfg->dongles)
            wake_all_dongles(cfg->dongles, cfg->number_of_coders);
    }
    return stop;
}

void *coder_routine(void *arg)
{
    t_coder *coder = (t_coder *)arg;
    t_config *cfg = coder->config;

    initial_stagger(coder);

    while (!should_stop(cfg))
    {
        if (!request_dongles(coder, coder->left, coder->right))
            break;

        if (check_stop_after_get(coder))
            break;

        set_coder_state(coder, STATE_COMPILING);
        do_compile_and_release(coder);

        if (should_stop(cfg))
            break;

        do_debug_and_refactor(coder);

        if (check_and_stop_if_done(coder))
            break;
    }
    return NULL;
}