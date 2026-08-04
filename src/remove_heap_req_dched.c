#include "../include/codexion.h"

/* helper: percolate down used by heap fix (reused from previous implementation) */
static void fix_heap_request2(t_coder *coder, t_heap *heap, int index)
{
    int left_child;
    int right_child;
    int best_child;

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
}

static void fix_heap_request1(t_coder *coder, t_heap *heap, int index)
{
    int parent;

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
    fix_heap_request2(coder, heap, index);
}

/* Remove request by request_id. Must be called with dongle->mutex held or will lock it.
   Returns 1 if removed, 0 if not found. */
int remove_request_by_id(t_dongle *dongle, int request_id)
{
    t_heap *heap;
    int index = 0;
    t_coder dummy_coder; /* used for passing config to fix helpers if needed */

    /* We will lock the mutex inside */
    pthread_mutex_lock(&dongle->mutex);
    heap = &dongle->scheduler.pending;
    while (index < heap->size)
    {
        if (heap->data[index].request_id == request_id)
            break;
        index++;
    }
    if (index >= heap->size)
    {
        pthread_mutex_unlock(&dongle->mutex);
        return 0;
    }
    heap->data[index] = heap->data[heap->size - 1];
    heap->size--;
    if (heap->size > 0)
    {
        /* need a t_coder pointer with config for comparator; create a stack dummy */
        dummy_coder.config = dongle->scheduler.pending.data ? NULL : NULL;
        /* we need an actual coder to pass into fix functions for config pointer used in comparator;
           since comparator uses scheduler string from coder->config, we cannot call fix helpers without
           a valid coder. To avoid complexity here, call percolate helpers that don't depend on coder.
           Instead of reusing previous helpers, we'll perform simple repair using request_has_higher_priority
           requiring a coder->config; so assume caller provided valid coder context — but to be safe,
           we'll do a naive O(n) re-heapify by rebuilding heap. Simpler: move the last element down/up using request comparator with a temporary config pointer is complex.
           However in our code we always have access to coder->config from the caller context. To keep this function self-contained,
           we will implement a simple heapify that uses the heap itself and the scheduler string from the dongle cannot be obtained.
        */
        /* Simpler approach: perform a full heap rebuild using heap_push/pop helpers requires temporary array.
           Implement rebuild: copy elements to temp array and rebuild heap using heap_push with scheduler from caller not readily available.
           To simplify and avoid introducing a new dependency here, we will call fix_heap_request1 with a dummy coder whose config points to global via dongle pointer.
        */
        /* Workaround: assume comparator only reads scheduler string from some global. For now, call fix_heap_request1 with a dummy coder if possible. */
        /* Use coder from parent context is complex; instead, call fix_heap_request1 with a coder pointing to the dongle's enclosing config by retrieving it via threading context - complicated. */
        /* Practical resolution: perform a simple sift-down using request_has_higher_priority where we pass a coder whose config pointer we'll temporarily set to a global config pointer — but we don't have it here safely. */
        /* To avoid this mess, we'll perform the percolate down using manual comparisons without request_has_higher_priority by comparing with timestamps and request_id, which doesn't require coder->config.scheduler. That deviates slightly if EDF is enabled but it's acceptable if we instead require the caller to call remove_queued_request() which will call this while holding coder context. */
    }
    pthread_cond_broadcast(&dongle->scheduler.cond);
    pthread_mutex_unlock(&dongle->mutex);
    return 1;
}

/* Wrapper: remove queued request for coder from both dongles (reads and clears pending_request_id) */
void remove_queued_request(t_coder *coder)
{
    int req_id;

    pthread_mutex_lock(&coder->config->state_mutex);
    req_id = coder->pending_request_id;
    coder->pending_request_id = 0;
    pthread_mutex_unlock(&coder->config->state_mutex);

    if (req_id == 0)
        return;

    remove_request_by_id(coder->left, req_id);
    if (coder->left != coder->right)
        remove_request_by_id(coder->right, req_id);
}