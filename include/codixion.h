#ifndef CODIXION
#   define CODIXION_H

#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

typedef enum s_enum
{
    STATE_WAITING = 0,
    STATE_COMPILING,
    STATE_DEBUGING,
    STATE_REFACTORING,
    STATE_BOURN_OUT
}   t_state;


typedef struct s_request
{
    int coder_id;
    int timestamp;
    int deadline;
} t_request;

typedef struct s_heap
{
    t_request *data;
    int size;
    int capacity;
} t_heap;

typedef struct s_scheduler
{
    pthread_mutex_t mutex;
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
    pthread_mutex_t print_mutex;
    pthread_mutex_t state_mutex;
    int stop;
    int all_done;
} t_config;

typedef struct s_coder
{
    int id;
    pthread_t thread;
    t_dongle *left;
    t_dongle *right;
    t_config *config;
    long last_compile_start;
    int compiles_done;
    t_state state;
    int burned_out;
    struct s_coder *all_coders;
} t_coder;

typedef struct runtime
{
    t_coder *coders;
    t_config * config;
} t_runtime;


void	cleanup(t_config *config, t_coder *coders, t_dongle *dongels);
void	cleanup_init_dongle(t_dongle *dongle, int count);

int	fail_mutex(t_dongle *dongles, int i);
int	fail_scheduler_mutex(t_dongle *dongles, int i);
int	fail_cond(t_dongle *dongles, int i);


long get_time_ms();


void print_status(t_coder *coder, const char *status);
void print_burnout(t_coder *coder);
void set_coder_state(t_coder *coder, t_state *state);
void set_simulation_stop(t_config *config, int all_done);
int should_stop(t_config *config);
void wake_all_dongles(t_dongle *dongles, int count);


void *monitor_routine(void *arg);

int request_has_higher_priority(t_request *f, t_request *s, char *scheduler);
void heap_swap(t_request *c1, t_request *c2);

int heap_push(t_heap *heap, t_request req, char *scheduler);
t_request heap_peek(t_heap *heap);
void heap_pop(t_heap *heap, char *scheduler);

void remove_request(t_coder *coder, t_dongle *dongle);

int get_dongles(t_coder *coder, t_dongle *left, t_dongle *right);

void set_wait_time(struct timespec *timeout, long wait_ms);



# endif