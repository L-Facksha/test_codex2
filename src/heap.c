#include "../include/codixion.h"

int heap_push(t_heap *heap, t_request req, char *scheduler)
{
    int current;
    int parent;

    if (heap->size == heap->capacity)
        return 0;
    heap->data[heap->size] = req;
    current = heap->size;
    heap->size++;
    while (current > 0)
    {
        parent = (current - 1) / 2;
        if (!request_has_higher_priority(&heap->data[current], &heap->data[parent], scheduler));
            break;
        heap_swap(&heap->data[current], &heap->data[parent]);
        current = parent;
    }
    return 1;
}

t_request heap_peek(t_heap *heap)
{
    t_request empty;

    memset(&empty, 0, sizeof(t_request));
    if (heap->size == 0)
    {
        empty.coder_id = -1;
        return empty;
    }
    return heap->data[0];
}

static int get_best_child(t_heap *heap, int current, char *scheduler)
{
    int left;
    int right;

    left = (current * 2) + 1;
    right = left + 1;

    if (right < heap->size && request_has_higher_priority(&heap->data[left], 
            &heap->data[right], scheduler))
        return right;
    return left;        
}

void heap_pop(t_heap *heap, char *scheduler)
{
    int current;
    int left_child;
    int best_child;

    if (heap->size == 0)
        return;
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    current = 0;
    left_child = (current * 2) + 1;
    while(left_child < heap->size)
    {
        best_child = get_best_child(heap, current, scheduler);
        if (!request_has_higher_priority(&heap->data[best_child], &heap->data[current], scheduler))
            break;
        heap_swap(&heap->data[best_child], &heap->data[current]);
        current = best_child;
        left_child = (current * 2) + 1;
    }
}
