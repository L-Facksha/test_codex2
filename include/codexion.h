#ifndef CODEXION_H
# define CODEXION_H

#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

typedef enum s_enum
{
    STATE_WAITING = 0,
    STATE_COMPILING,
    STATE_DEBUGGING,
    STATE_REFACTORING,
    STATE_BURNED_OUT
}   t_state;

typedef struct s_request
{
    int request_id;    /* unique id per logical request (same id on both heaps) */
    int coder_id;
    long timestamp;
    long deadline;
} t_request;

typedef struct s_heap
{
    t_request *data;
    int size;
    int capacity;
} t_heap;

typedef struct s_scheduler
{
    pthread_cond_t cond;
    t_heap pending;
} t_scheduler;

typedef struct s_dongle
{
    int id;
    pthread_mutex_t mutex;
    int taken;
    long last_released_at;
    t_scheduler scheduler;
} t_dongle;

typedef struct s_config
{
    int number_of_coders;
    int time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int number_of_compiles_required;
    int dongle_cooldown;
    char *scheduler;
    long start_time;

    /* synchronization primitives */
    pthread_mutex_t print_mutex;
    pthread_mutex_t state_mutex;

    /* init flags */
    int print_mutex_inited;
    int state_mutex_inited;

    /* pointer to dongles array */
    struct s_dongle *dongles;

    /* simulation control */
    int stop;
    int all_done;

    /* next request id counter (protected by state_mutex) */
    int next_request_id;
} t_config;

typedef struct s_coder
{
    int id;
    pthread_t thread;
    t_dongle *left;
    t_dongle *right;
    t_config *config;
    long last_compile_start; /* last compile FINISH time (ms relative to start_time) */
    int compiles_done;
    t_state state;
    int burned_out;
    struct s_coder *all_coders;

    /* pending request id if this coder currently has a queued request (0 if none).
       Protected by state_mutex when read/modified. */
    int pending_request_id;
} t_coder;

typedef struct runtime
{
    t_coder *coders;
    t_config * config;
} t_runtime;


void	cleanup(t_config *config, t_coder *coders, t_dongle *dongels);
void	cleanup_init_dongle(t_dongle *dongle, int count);

int	fail_mutex(t_dongle *dongles, int i);
int	fail_cond(t_dongle *dongles, int i);

long get_time_ms();

int	parse_args(int ac, char **av, t_config *config);

int init_resources(t_dongle *dongles, int i, int count);
int init_dongles(t_dongle *dongles, int count);
int init_coders(t_coder *coders, t_config *config, t_dongle *dongles);

int creat_thread(t_coder *coders, t_config *config);

void print_status(t_coder *coder, const char *status);
void print_burnout(t_coder *coder);
void set_coder_state(t_coder *coder, t_state state);
void set_simulation_stop(t_config *config, int all_done);
int should_stop(t_config *config);
void wake_all_dongles(t_dongle *dongles, int count);

void *monitor_routine(void *arg);

int request_has_higher_priority(t_request *f, t_request *s, char *scheduler);
void heap_swap(t_request *c1, t_request *c2);

int heap_push(t_heap *heap, t_request req, char *scheduler);
t_request heap_peek(t_heap *heap);
void heap_pop(t_heap *heap, char *scheduler);

/* scheduler API */
int request_dongles(t_coder *coder, t_dongle *first, t_dongle *second);

/* removal by request id (used internally by scheduler) */
int remove_request_by_id(t_dongle *dongle, int request_id);

/* remove any queued request for this coder on both dongles (reads pending_request_id) */
void remove_queued_request(t_coder *coder);

int get_dongles(t_coder *coder, t_dongle *left, t_dongle *right);

void set_wait_time(struct timespec *timeout, long wait_ms);

void relese_dongle(t_dongle *dongle);

void *coder_routine(void *arg);


# endif