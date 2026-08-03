#include "../include/codixion.h"

static long get_request_deadline(t_coder *coder)
{
    long now;

    now = get_time_ms() - coder->config->start_time;
    if (coder->last_compile_start >= 0)
        return (coder->last_compile_start + coder->config->time_to_burnout);
    return (now + coder->config->time_to_burnout);
}

static void remove_request(t_coder *coder, t_dongle *dongle)
{
    t_heap *heap;
    int index;
    int parent;
    int left_child;
    int right_child;
    int best_child;

    pthread_mutex_lock(&dongle->mutex);
    heap = &dongle->scheduler.pending;
    index = 0;
    while(index < heap->size)
    {
        if (heap->data[index].coder_id == coder->id)
            break;
        index++;
    }
    if (index >= heap->size)
    {
        pthread_mutex_unlock(&dongle->mutex);
        return ;
    }
    heap->data[index] = heap->data[heap->size - 1];
    heap->size--;
    if (heap->size == 0)
    {
        pthread_cond_broadcast(&dongle->scheduler.cond);
        pthread_mutex_unlock(&dongle->mutex);
        return;
    }
    if (index > 0)
    {
        parent = (index - 1) / 2;
        while (index > 0 && request_has_higher_priority(&heap->data[index],
                &heap->data[parent], coder->config->scheduler))
        {
            heap_swap(&heap->data[index], &heap->data[parent]);
            index = parent;
            parent = (index - 1) / 2;
        }
    }
    while(1)
    {
        left_child = (index * 2) + 1;
        right_child = left_child + 1;
        best_child = index;
        if (left_child < heap->size && request_has_higher_priority(
            &heap->data[left_child], &heap->data[best_child],
            coder->config->scheduler))
            best_child = left_child;
        if (right_child < heap->size && request_has_higher_priority(
            &heap->data[right_child], &heap->data[best_child],
            coder->config->scheduler))
            best_child = right_child;
        if (best_child == index)
            break;
        heap_swap(&heap->data[index], &heap->data[best_child]);
        index = best_child;
    }
    pthread_cond_broadcast(&dongle->scheduler.cond);
    pthread_mutex_unlock(&dongle->mutex);
}

static int push_coder_request(t_coder *coder, t_dongle *dongle)
{
    t_request req;

    req.coder_id = coder->id;
    req.deadline = get_time_ms() - coder->config->start_time;
    req.timestamp = get_request_deadline(coder);
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
    struct timespec timeout;
    long wait_ms;

    left = first;
    right = second;
    if (left->id > right->id)
    {
        left = second;
        right = first;
    }
    if (!)
}