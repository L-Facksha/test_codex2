/* name=src/get_dongels_sched.c */
#include "../include/codexion.h"
#include <errno.h>

static long read_last_released(t_dongle *d)
{
    long v;
    pthread_mutex_lock(&d->mutex);
    v = d->last_released_at;
    pthread_mutex_unlock(&d->mutex);
    return v;
}

/* compute_wait_ms and timed_wait_on same as before, but timed_wait_on checks should_stop right away */
static long compute_wait_ms(t_coder *coder, long left_last, long right_last)
{
    long wait_ms = 1;
    long right_wait;

    if (left_last != 0)
    {
        wait_ms = coder->config->dongle_cooldown - (get_time_ms() - left_last);
        if (wait_ms < 1)
            wait_ms = 1;
    }
    if (right_last != 0)
    {
        right_wait = coder->config->dongle_cooldown - (get_time_ms() - right_last);
        if (right_wait < wait_ms)
            wait_ms = right_wait < 1 ? 1 : right_wait;
    }
    return wait_ms;
}

static void timed_wait_on(t_coder *coder, t_dongle *d, struct timespec *timeout)
{
    int r;
    pthread_mutex_lock(&d->mutex);
    while (!should_stop(coder->config) &&
           heap_peek(&d->scheduler.pending).request_id != coder->pending_request_id &&
           (d->last_released_at != 0 && (get_time_ms() - d->last_released_at) < coder->config->dongle_cooldown))
    {
        r = pthread_cond_timedwait(&d->scheduler.cond, &d->mutex, timeout);
        if (r == ETIMEDOUT)
            break;
        if (should_stop(coder->config))
            break;
    }
    pthread_mutex_unlock(&d->mutex);
}

static void wait_cooldown(t_coder *coder, t_dongle *left, t_dongle *right, long wait_ms)
{
    struct timespec timeout;
    long left_last = read_last_released(left);
    long right_last = read_last_released(right);

    wait_ms = compute_wait_ms(coder, left_last, right_last);
    set_wait_time(&timeout, wait_ms);
    timed_wait_on(coder, left, &timeout);
    timed_wait_on(coder, right, &timeout);
}

/* Helper: pop root only if it matches given request_id. Must be called while holding dongle->mutex. */
static int heap_pop_root_if_id(t_heap *heap, int request_id, char *scheduler)
{
    if (heap->size == 0)
        return 0;
    if (heap->data[0].request_id != request_id)
        return 0;
    /* normal heap_pop code */
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    if (heap->size == 0)
        return 1;
    int current = 0;
    int left_child = (current * 2) + 1;
    while (left_child < heap->size)
    {
        int best_child = left_child;
        int right_child = left_child + 1;
        if (right_child < heap->size && request_has_higher_priority(&heap->data[right_child],
                &heap->data[left_child], scheduler))
            best_child = right_child;
        if (!request_has_higher_priority(&heap->data[best_child], &heap->data[current], scheduler))
            break;
        heap_swap(&heap->data[best_child], &heap->data[current]);
        current = best_child;
        left_child = (current * 2) + 1;
    }
    return 1;
}

/* public get_dongles */
int get_dongles(t_coder *coder, t_dongle *left, t_dongle *right)
{
    t_config *cfg = coder->config;

    /* single-dongle case */
    if (left == right)
    {
        while (!should_stop(cfg))
        {
            pthread_mutex_lock(&left->mutex);
            int can_take = 0;
            if (!left->taken &&
                !(left->last_released_at != 0 && (get_time_ms() - left->last_released_at) < cfg->dongle_cooldown) &&
                left->scheduler.pending.size > 0 &&
                left->scheduler.pending.data[0].request_id == coder->pending_request_id)
            {
                can_take = 1;
            }
            if (can_take)
            {
                left->taken = 1;
                heap_pop_root_if_id(&left->scheduler.pending, coder->pending_request_id, cfg->scheduler);
                pthread_mutex_unlock(&left->mutex);

                /* clear pending id */
                pthread_mutex_lock(&cfg->state_mutex);
                coder->pending_request_id = 0;
                pthread_mutex_unlock(&cfg->state_mutex);

                print_status(coder, "has taken a dongle");
                print_status(coder, "has taken a dongle");
                return 1;
            }
            pthread_mutex_unlock(&left->mutex);
            wait_cooldown(coder, left, right, 1);
        }
        return 0;
    }

    /* two-dongle case: atomically check both roots and pop both by request_id */
    while (!should_stop(cfg))
    {
        pthread_mutex_lock(&left->mutex);
        pthread_mutex_lock(&right->mutex);

        int can_take = 0;
        if (!left->taken && !right->taken)
        {
            if (left->scheduler.pending.size > 0 && right->scheduler.pending.size > 0 &&
                left->scheduler.pending.data[0].request_id == coder->pending_request_id &&
                right->scheduler.pending.data[0].request_id == coder->pending_request_id)
            {
                /* check cooldowns */
                long now = get_time_ms();
                int left_cool = (left->last_released_at == 0) || (now - left->last_released_at >= cfg->dongle_cooldown);
                int right_cool = (right->last_released_at == 0) || (now - right->last_released_at >= cfg->dongle_cooldown);
                if (left_cool && right_cool)
                    can_take = 1;
            }
        }

        if (can_take)
        {
            left->taken = 1;
            right->taken = 1;
            /* pop roots only if they match pending_request_id */
            heap_pop_root_if_id(&left->scheduler.pending, coder->pending_request_id, cfg->scheduler);
            heap_pop_root_if_id(&right->scheduler.pending, coder->pending_request_id, cfg->scheduler);

            pthread_mutex_unlock(&right->mutex);
            pthread_mutex_unlock(&left->mutex);

            /* clear pending id */
            pthread_mutex_lock(&cfg->state_mutex);
            coder->pending_request_id = 0;
            pthread_mutex_unlock(&cfg->state_mutex);

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