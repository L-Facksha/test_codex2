#include "../include/codexion.h"

static int can_get_dongle(t_coder *coder, t_dongle *dongle)
{
    long now;

    if (dongle->taken)
        return 0;

    now = get_time_ms();
    if (dongle->last_released_at != 0 && (now - dongle->last_released_at) < coder->config->dongle_cooldown)
        return 0;
    return (heap_peek(&dongle->scheduler.pending).coder_id == coder->id);
}

static int can_get_pair(t_coder *coder, t_dongle *left, t_dongle *right)
{
    if (left->taken || right->taken)
        return 0;
    return (can_get_dongle(coder, left) && can_get_dongle(coder, right));
}

static void wait_cooldown(t_coder *coder, t_dongle *left, t_dongle *right, long wait_ms)
{
    struct timespec timeout;
    long right_wait;

    if (left->last_released_at != 0)
    {
        wait_ms = coder->config->dongle_cooldown - (get_time_ms() - left->last_released_at);
        if (wait_ms < 1)
            wait_ms = 1;
    }
    if (right->last_released_at != 0)
    {
        right_wait = coder->config->dongle_cooldown - (get_time_ms() - right->last_released_at);
        if (right_wait < wait_ms)
            wait_ms = right_wait < 1 ? 1 : right_wait;
    }
    set_wait_time(&timeout, wait_ms);
    pthread_mutex_lock(&left->mutex);
    pthread_cond_timedwait(&left->scheduler.cond, &left->mutex, &timeout);
    pthread_mutex_unlock(&left->mutex);
    pthread_mutex_lock(&right->mutex);
    pthread_cond_timedwait(&right->scheduler.cond, &right->mutex, &timeout);
    pthread_mutex_unlock(&right->mutex);
}

int get_dongles(t_coder *coder, t_dongle *left, t_dongle *right)
{
    while (!should_stop(coder->config))
    {
        pthread_mutex_lock(&left->mutex);
        pthread_mutex_lock(&right->mutex);
        if (can_get_pair(coder, left, right))
        {
            left->taken = 1;
            right->taken = 1;
            heap_pop(&left->scheduler.pending, coder->config->scheduler);
            heap_pop(&right->scheduler.pending, coder->config->scheduler);
            pthread_cond_broadcast(&left->scheduler.cond);
            pthread_cond_broadcast(&right->scheduler.cond);
            pthread_mutex_unlock(&right->mutex);
            pthread_mutex_unlock(&left->mutex);
            print_status(coder, "has taken a dongle");
            print_status(coder, "has taken a dongle");
            return 1;
        }
        pthread_mutex_unlock(&right->mutex);
        pthread_mutex_unlock(&left->mutex);
        wait_cooldown(coder, left, right, 1);
    }
    return 0;
}
