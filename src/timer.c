#include "../include/codixion.h"

long get_time_ms()
{
    struct timeval current;
    if (gettimeofday(&current, NULL) != 0)
        return -1;
    return ((current.tv_sec * 1000) + (current.tv_usec / 1000));
}