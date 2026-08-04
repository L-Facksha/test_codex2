#include "../include/codexion.h"

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