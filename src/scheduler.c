#include "../include/codixion.h"

static int 

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