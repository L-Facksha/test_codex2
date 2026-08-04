#include "../include/codexion.h"

void set_wait_time(struct timespec *timeout, long wait_ms)
{
    struct timeval current;

    gettimeofday(&current, NULL);
    timeout->tv_sec = current.tv_sec + (wait_ms / 1000);
    timeout->tv_nsec = (current.tv_usec * 1000) + ((wait_ms % 1000) * 1000000);
    while (timeout->tv_nsec >= 1000000000)
    {
        timeout->tv_nsec -= 1000000000;
        timeout->tv_sec++;
    }
}

static long get_request_deadline(t_coder *coder)
{
    long now;

    now = get_time_ms() - coder->config->start_time;
    if (coder->last_compile_start >= 0)
        return (coder->last_compile_start + coder->config->time_to_burnout);
    return (now + coder->config->time_to_burnout);
}

static int push_coder_request(t_coder *coder, t_dongle *dongle)
{
    t_request req;

    req.coder_id = coder->id;
    req.timestamp = get_time_ms() - coder->config->start_time;
    req.deadline = get_request_deadline(coder);
    pthread_mutex_lock(&dongle->mutex);
    if (!heap_push(&dongle->scheduler.pending, req, coder->config->scheduler))
    {
        pthread_mutex_unlock(&dongle->mutex);
        return 0;
    }
    pthread_cond_broadcast(&dongle->scheduler.cond);
    pthread_mutex_unlock(&dongle->mutex);
    return 1;
}

int request_dongles(t_coder *coder, t_dongle *first, t_dongle *second)
{
    t_dongle *left;
    t_dongle *right;

    left = first;
    right = second;
    if (left->id > right->id)
    {
        left = second;
        right = first;
    }
    if (!push_coder_request(coder, left) || !push_coder_request(coder, right))
    {
        if (left->scheduler.pending.size > 0)
            remove_request(coder, left);
        if (right->scheduler.pending.size > 0)
            remove_request(coder, right);
        return 0;
    }
    if (get_dongles(coder, left, right))
        return 1;
    remove_request(coder, left);
    remove_request(coder, right);
    return 0;
}

void relese_dongle(t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->mutex);
    dongle->taken = 0;
    dongle->last_released_at = get_time_ms();
    pthread_cond_broadcast(&dongle->scheduler.cond);
    pthread_mutex_unlock(&dongle->mutex);
}
