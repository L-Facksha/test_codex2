#include "../include/codexion.h"

void heap_swap(t_request *c1, t_request *c2)
{
    t_request tmp;

    tmp = *c1;
    *c1 = *c2;
    *c2 = tmp;
}

int request_has_higher_priority(t_request *f, t_request *s, char *scheduler)
{
    if (strcmp(scheduler, "edf") == 0)
    {
        if (f->deadline != s->deadline)
            return (f->deadline < s->deadline);
    }
    if (f->timestamp != s->timestamp)
        return (f->timestamp < s->timestamp);
    /* tie-breaker: lower request_id has higher priority (older request) */
    return (f->request_id < s->request_id);
}