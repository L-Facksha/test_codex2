/* name=src/scheduler.c */
#include "../include/codexion.h"

/* set_wait_time unchanged */
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

/* Helper: generate a unique request id (protected by state_mutex) */
static int generate_request_id(t_config *cfg)
{
    int id;
    pthread_mutex_lock(&cfg->state_mutex);
    id = ++cfg->next_request_id;
    pthread_mutex_unlock(&cfg->state_mutex);
    if (id == 0) /* avoid zero (used as "none") */
    {
        pthread_mutex_lock(&cfg->state_mutex);
        id = ++cfg->next_request_id;
        pthread_mutex_unlock(&cfg->state_mutex);
    }
    return id;
}

/* Presence check by request_id (no locking; caller should hold respective dongle mutex) */
static int presence_in_heap_by_request_id(t_heap *heap, int request_id)
{
    for (int i = 0; i < heap->size; i++)
        if (heap->data[i].request_id == request_id)
            return 1;
    return 0;
}

/* Remove by request_id while holding dongle->mutex; returns 1 if removed */
static int heap_remove_by_request(t_coder *coder, t_heap *heap, int request_id)
{
    int index = 0;
    while (index < heap->size)
    {
        if (heap->data[index].request_id == request_id)
            break;
        index++;
    }
    if (index >= heap->size)
        return 0;
    heap->data[index] = heap->data[heap->size - 1];
    heap->size--;
    if (heap->size > 0)
        heap_percolate_up(coder, heap, index), heap_percolate_down(coder, heap, index);
    return 1;
}

/* Insert a t_request into both heaps while holding both dongle mutexes */
static int insert_both_requests_with_req(t_coder *coder, t_dongle *left, t_dongle *right, t_request req)
{
    if (!heap_push(&left->scheduler.pending, req, coder->config->scheduler) ||
        !heap_push(&right->scheduler.pending, req, coder->config->scheduler))
    {
        /* rollback partial insertion */
        heap_remove_by_request(coder, &left->scheduler.pending, req.request_id);
        heap_remove_by_request(coder, &right->scheduler.pending, req.request_id);
        return 0;
    }
    pthread_cond_broadcast(&left->scheduler.cond);
    pthread_cond_broadcast(&right->scheduler.cond);
    return 1;
}

/* Push two requests atomically with request_id and set coder->pending_request_id before insertion.
   Ensures a coder cannot create multiple simultaneous requests. */
static int push_two_requests_with_id(t_coder *coder, t_dongle *first, t_dongle *second)
{
    t_dongle *left = first;
    t_dongle *right = second;
    t_config *cfg = coder->config;

    if (left->id > right->id)
    {
        left = second;
        right = first;
    }

    /* Ensure coder has no pending request; set pending_request_id atomically */
    pthread_mutex_lock(&cfg->state_mutex);
    if (coder->pending_request_id != 0)
    {
        /* Already has a pending request; nothing to do */
        pthread_mutex_unlock(&cfg->state_mutex);
        return 1;
    }
    int req_id = generate_request_id(cfg);
    coder->pending_request_id = req_id;
    pthread_mutex_unlock(&cfg->state_mutex);

    t_request req;
    req.request_id = req_id;
    req.coder_id = coder->id;
    req.timestamp = get_time_ms() - cfg->start_time;
    req.deadline = coder->last_compile_start + cfg->time_to_burnout;

    /* Try inserting while holding both dongle mutexes */
    pthread_mutex_lock(&left->mutex);
    pthread_mutex_lock(&right->mutex);

    /* Clean up stale presence if present on one side only */
    int present_left = presence_in_heap_by_request_id(&left->scheduler.pending, req_id);
    int present_right = presence_in_heap_by_request_id(&right->scheduler.pending, req_id);

    if (present_left && present_right)
    {
        pthread_mutex_unlock(&right->mutex);
        pthread_mutex_unlock(&left->mutex);
        return 1;
    }
    if (present_left && !present_right)
        heap_remove_by_request(coder, &left->scheduler.pending, req_id);
    if (present_right && !present_left)
        heap_remove_by_request(coder, &right->scheduler.pending, req_id);

    /* Capacity check */
    if (left->scheduler.pending.size == left->scheduler.pending.capacity ||
        right->scheduler.pending.size == right->scheduler.pending.capacity)
    {
        pthread_mutex_unlock(&right->mutex);
        pthread_mutex_unlock(&left->mutex);

        /* rollback pending_request_id so caller may retry later */
        pthread_mutex_lock(&cfg->state_mutex);
        if (coder->pending_request_id == req_id)
            coder->pending_request_id = 0;
        pthread_mutex_unlock(&cfg->state_mutex);
        return 0;
    }

    /* Insert both */
    if (!insert_both_requests_with_req(coder, left, right, req))
    {
        pthread_mutex_unlock(&right->mutex);
        pthread_mutex_unlock(&left->mutex);

        pthread_mutex_lock(&cfg->state_mutex);
        if (coder->pending_request_id == req_id)
            coder->pending_request_id = 0;
        pthread_mutex_unlock(&cfg->state_mutex);
        return 0;
    }

    pthread_mutex_unlock(&right->mutex);
    pthread_mutex_unlock(&left->mutex);
    return 1;
}

/* Push single request used for single-dongle (left==right) */
static int push_single_request_with_id(t_coder *coder, t_dongle *dongle)
{
    t_config *cfg = coder->config;

    pthread_mutex_lock(&cfg->state_mutex);
    if (coder->pending_request_id != 0)
    {
        pthread_mutex_unlock(&cfg->state_mutex);
        return 1;
    }
    int req_id = generate_request_id(cfg);
    coder->pending_request_id = req_id;
    pthread_mutex_unlock(&cfg->state_mutex);

    t_request req;
    req.request_id = req_id;
    req.coder_id = coder->id;
    req.timestamp = get_time_ms() - cfg->start_time;
    req.deadline = coder->last_compile_start + cfg->time_to_burnout;

    pthread_mutex_lock(&dongle->mutex);
    if (!heap_push(&dongle->scheduler.pending, req, coder->config->scheduler))
    {
        pthread_mutex_unlock(&dongle->mutex);
        pthread_mutex_lock(&cfg->state_mutex);
        if (coder->pending_request_id == req_id)
            coder->pending_request_id = 0;
        pthread_mutex_unlock(&cfg->state_mutex);
        return 0;
    }
    pthread_cond_broadcast(&dongle->scheduler.cond);
    pthread_mutex_unlock(&dongle->mutex);
    return 1;
}

/* Public: request_dongles. Will set per-coder pending_request_id and insert requests atomically.
   Retries when heaps are full; returns 1 on success (requests enqueued and later granted), 0 on stop/failure. */
int request_dongles(t_coder *coder, t_dongle *first, t_dongle *second)
{
    t_dongle *left = first;
    t_dongle *right = second;
    t_config *cfg = coder->config;

    if (left->id > right->id)
    {
        left = second;
        right = first;
    }

    if (left == right) /* single-dongle case */
    {
        while (!should_stop(cfg))
        {
            if (push_single_request_with_id(coder, left))
                break;
            usleep(1000);
        }
    }
    else
    {
        while (!should_stop(cfg))
        {
            if (push_two_requests_with_id(coder, left, right))
                break;
            usleep(1000);
        }
    }

    if (should_stop(cfg))
    {
        /* clear pending_request_id if present */
        pthread_mutex_lock(&cfg->state_mutex);
        coder->pending_request_id = 0;
        pthread_mutex_unlock(&cfg->state_mutex);
        return 0;
    }

    /* Now wait to be granted in get_dongles (which checks pending_request_id atomically) */
    if (get_dongles(coder, left, right))
        return 1;

    /* If get_dongles failed (stop), remove queued request if any */
    remove_queued_request(coder);
    return 0;
}

/* Remove a request from a heap by request_id while holding dongle mutex.
   This wraps the generic remove_by_request helper from remove_heap_req_dched.c but expects we have coder context. */
static int remove_request_from_heap(t_coder *coder, t_dongle *dongle, int request_id)
{
    int removed = 0;
    pthread_mutex_lock(&dongle->mutex);
    removed = heap_remove_by_request(coder, &dongle->scheduler.pending, request_id);
    if (removed)
        pthread_cond_broadcast(&dongle->scheduler.cond);
    pthread_mutex_unlock(&dongle->mutex);
    return removed;
}

/* Public wrapper to remove by request id on both dongles (used rarely) */
int remove_request_by_id(t_dongle *dongle, int request_id)
{
    /* This public function will lock the dongle mutex inside remove_request_by_id implementation
       in remove_heap_req_dched.c; to avoid duplication we can call that implementation directly.
       However, we provide this wrapper to satisfy the prototype. */
    return remove_request_by_id(dongle, request_id); /* will resolve to function implemented in remove_heap_req_dched.c */
}

/* release implementation unchanged here (reused from elsewhere) */
void relese_dongle(t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->mutex);
    dongle->taken = 0;
    dongle->last_released_at = get_time_ms();
    pthread_cond_broadcast(&dongle->scheduler.cond);
    pthread_mutex_unlock(&dongle->mutex);
}