#include "../include/codexion.h"

static int check_burnout(t_coder *coders, t_config *config)
{
    int i;
    long now;

    i = 0;
    while(i < config->number_of_coders && !should_stop(config))
    {
        now = get_time_ms() - config->start_time;
        if (!coders[i].burned_out && coders[i].state == STATE_WAITING
            && coders[i].last_compile_start >= 0
            && now - coders[i].last_compile_start >= config->time_to_burnout)
        {
            coders[i].burned_out = 1;
            set_coder_state(&coders[i], STATE_BOURN_OUT);
            print_burnout(&coders[i]);
            set_simulation_stop(config, 0);
            return -1;
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
    while(!should_stop(config))
    {
        if (check_burnout(coders, config) != 0)
            break;
        usleep(1000);
    }
    return NULL;
}
