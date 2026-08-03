#include "../include/codixion.h"

static int enqueue_request(t_coder *coder, t_dongle *dongle)
{
    
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
    if (!heap_push(&))
}