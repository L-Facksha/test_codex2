/* name=src/cleanup.c */
#include "../include/codexion.h"

void cleanup(t_config *config, t_coder *coders, t_dongle *dongels)
{
    int i;

    if (config)
    {
        if (config->print_mutex_inited)
            pthread_mutex_destroy(&config->print_mutex);
        if (config->state_mutex_inited)
            pthread_mutex_destroy(&config->state_mutex);
    }
    if (coders)
        free(coders);
    if (dongels)
    {
        i = 0;
        /* Only use config to know how many dongles to clean */
        int count = (config ? config->number_of_coders : 0);
        while (i < count)
        {
            if (dongels[i].scheduler.pending.data)
                free(dongels[i].scheduler.pending.data);
            pthread_mutex_destroy(&dongels[i].mutex);
            pthread_cond_destroy(&dongels[i].scheduler.cond);
            i++;
        }
        free(dongels);
    }
}

void cleanup_init_dongle(t_dongle *dongle, int count)
{
    int i;

    i = 0;
    while (i < count)
    {
        pthread_mutex_destroy(&dongle[i].mutex);
        pthread_cond_destroy(&dongle[i].scheduler.cond);
        if (dongle[i].scheduler.pending.data)
            free(dongle[i].scheduler.pending.data);
        i++;
    }
}/* name=src/coder.c */
#include "../include/codexion.h"

/* small helper: initial stagger */
static void initial_stagger(t_coder *coder)
{
    if (coder->config->number_of_coders > 1)
    {
        if ((coder->id % 2) == 0)
            usleep(1000); /* 1 ms */
    }
}

static int check_stop_after_get(t_coder *coder)
{
    t_config *cfg = coder->config;
    if (should_stop(cfg))
    {
        relese_dongle(coder->left);
        if (coder->left != coder->right)
            relese_dongle(coder->right);
        return 1;
    }
    return 0;
}

static void do_compile_and_release(t_coder *coder)
{
    t_config *cfg = coder->config;
    print_status(coder, "is compiling");
    usleep(cfg->time_to_compile * 1000);

    /* Record the time after the compile so burnout is measured from compile finish */
    pthread_mutex_lock(&cfg->state_mutex);
    coder->last_compile_start = get_time_ms() - cfg->start_time;
    pthread_mutex_unlock(&cfg->state_mutex);

    relese_dongle(coder->left);
    if (coder->left != coder->right)
        relese_dongle(coder->right);
}

static void do_debug_and_refactor(t_coder *coder)
{
    t_config *cfg = coder->config;
    set_coder_state(coder, STATE_DEBUGGING);
    print_status(coder, "is debugging");
    usleep(cfg->time_to_debug * 1000);

    set_coder_state(coder, STATE_REFACTORING);
    print_status(coder, "is refactoring");
    usleep(cfg->time_to_refactor * 1000);

    /* after refactor, go back to waiting */
    set_coder_state(coder, STATE_WAITING);
}

static int everyone_done(t_config *cfg, t_coder *coders)
{
    int i;
    for (i = 0; i < cfg->number_of_coders; i++)
    {
        if (coders[i].compiles_done < cfg->number_of_compiles_required)
            return 0;
    }
    return 1;
}

static int check_and_stop_if_done(t_coder *coder)
{
    t_config *cfg = coder->config;
    int stop = 0;

    pthread_mutex_lock(&cfg->state_mutex);
    coder->compiles_done++;
    if (cfg->number_of_compiles_required > 0 && everyone_done(cfg, coder->all_coders))
    {
        cfg->stop = 1;
        cfg->all_done = 1;
        stop = 1;
    }
    pthread_mutex_unlock(&cfg->state_mutex);

    if (stop)
    {
        /* wake everyone */
        if (cfg->dongles)
            wake_all_dongles(cfg->dongles, cfg->number_of_coders);
    }
    return stop;
}

void *coder_routine(void *arg)
{
    t_coder *coder = (t_coder *)arg;
    t_config *cfg = coder->config;

    initial_stagger(coder);

    while (!should_stop(cfg))
    {
        if (!request_dongles(coder, coder->left, coder->right))
            break;

        if (check_stop_after_get(coder))
            break;

        set_coder_state(coder, STATE_COMPILING);
        do_compile_and_release(coder);

        if (should_stop(cfg))
            break;

        do_debug_and_refactor(coder);

        if (check_and_stop_if_done(coder))
            break;
    }
    return NULL;
}/* name=src/fail_dongles.c */
#include "../include/codexion.h"

/* Called when pthread_mutex_init on dongles[i] fails.
   cleanup_init_dongle(dongles, i) will destroy/ free indices 0..i-1. */
int	fail_mutex(t_dongle *dongles, int i)
{
	cleanup_init_dongle(dongles, i);
	return (0);
}

/* Called when pthread_cond_init on dongles[i] fails. */
int	fail_cond(t_dongle *dongles, int i)
{
	/* cleanup_init_dongle will destroy mutex/cond for previous indices */
	cleanup_init_dongle(dongles, i);
	return (0);
}/* name=src/get_dongels_sched.c */
#include "../include/codexion.h"
#include <errno.h>

static int can_get_dongle(t_coder *coder, t_dongle *dongle)
{
    long now;

    if (dongle->taken)
        return 0;

    now = get_time_ms();
    if (dongle->last_released_at != 0 && (now - dongle->last_released_at) < coder->config->dongle_cooldown)
        return 0;
    return (heap_peek(&dongle->scheduler.pending).coder_id == coder->id);
}

static int can_get_pair(t_coder *coder, t_dongle *left, t_dongle *right)
{
    if (left->taken || right->taken)
        return 0;
    return (can_get_dongle(coder, left) && can_get_dongle(coder, right));
}

static long read_last_released(t_dongle *d)
{
    long v;
    pthread_mutex_lock(&d->mutex);
    v = d->last_released_at;
    pthread_mutex_unlock(&d->mutex);
    return v;
}

static long compute_wait_ms(t_coder *coder, long left_last, long right_last)
{
    long wait_ms = 1;
    long right_wait;

    if (left_last != 0)
    {
        wait_ms = coder->config->dongle_cooldown - (get_time_ms() - left_last);
        if (wait_ms < 1)
            wait_ms = 1;
    }
    if (right_last != 0)
    {
        right_wait = coder->config->dongle_cooldown - (get_time_ms() - right_last);
        if (right_wait < wait_ms)
            wait_ms = right_wait < 1 ? 1 : right_wait;
    }
    return wait_ms;
}

static void timed_wait_on(t_coder *coder, t_dongle *d, struct timespec *timeout)
{
    int r;
    pthread_mutex_lock(&d->mutex);
    while (!should_stop(coder->config) &&
           heap_peek(&d->scheduler.pending).coder_id != coder->id &&
           (d->last_released_at != 0 && (get_time_ms() - d->last_released_at) < coder->config->dongle_cooldown))
    {
        r = pthread_cond_timedwait(&d->scheduler.cond, &d->mutex, timeout);
        if (r == ETIMEDOUT)
            break;
        if (should_stop(coder->config))
            break;
    }
    pthread_mutex_unlock(&d->mutex);
}

static void wait_cooldown(t_coder *coder, t_dongle *left, t_dongle *right, long wait_ms)
{
    struct timespec timeout;
    long left_last = read_last_released(left);
    long right_last = read_last_released(right);

    wait_ms = compute_wait_ms(coder, left_last, right_last);
    set_wait_time(&timeout, wait_ms);
    timed_wait_on(coder, left, &timeout);
    timed_wait_on(coder, right, &timeout);
}

/* public, non-static */
int get_dongles(t_coder *coder, t_dongle *left, t_dongle *right)
{
    /* Special-case single-dongle (left == right) to avoid locking same mutex twice */
    if (left == right)
    {
        while (!should_stop(coder->config))
        {
            pthread_mutex_lock(&left->mutex);
            if (!left->taken &&
                !(left->last_released_at != 0 && (get_time_ms() - left->last_released_at) < coder->config->dongle_cooldown) &&
                heap_peek(&left->scheduler.pending).coder_id == coder->id)
            {
                left->taken = 1;
                heap_pop(&left->scheduler.pending, coder->config->scheduler);
                pthread_mutex_unlock(&left->mutex);
                print_status(coder, "has taken a dongle");
                print_status(coder, "has taken a dongle");
                return 1;
            }
            pthread_mutex_unlock(&left->mutex);
            wait_cooldown(coder, left, right, 1);
        }
        return 0;
    }

    while (!should_stop(coder->config))
    {
        pthread_mutex_lock(&left->mutex);
        pthread_mutex_lock(&right->mutex);
        if (can_get_pair(coder, left, right))
        {
            left->taken = 1;
            right->taken = 1;
            heap_pop(&left->scheduler.pending, coder->config->scheduler);
            heap_pop(&right->scheduler.pending, coder->config->scheduler);
            pthread_mutex_unlock(&right->mutex);
            pthread_mutex_unlock(&left->mutex);
            print_status(coder, "has taken a dongle");
            print_status(coder, "has taken a dongle");
            return 1;
        }
        pthread_mutex_unlock(&right->mutex);
        pthread_mutex_unlock(&left->mutex);
        wait_cooldown(coder, left, right, 1);
    }
    return 0;
}#include "../include/codexion.h"

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
        if (!request_has_higher_priority(&heap->data[current], &heap->data[parent], scheduler))
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

    if (right < heap->size && request_has_higher_priority(&heap->data[right], 
            &heap->data[left], scheduler))
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
    if (heap->size == 0)
        return;
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
    return (f->coder_id < s->coder_id);
}/* name=src/init.c */
#include "../include/codexion.h"

int init_resources(t_dongle *dongles, int i, int count)
{
    dongles[i].id = i + 1;
    dongles[i].taken = 0;
    dongles[i].last_released_at = 0;
    dongles[i].scheduler.pending.capacity = count;
    dongles[i].scheduler.pending.size = 0;
    dongles[i].scheduler.pending.data = malloc(sizeof(t_request) * count);
    if (!dongles[i].scheduler.pending.data)
    {
        cleanup_init_dongle(dongles, i);
        return 0;
    }
    return 1;
}

int init_dongles(t_dongle *dongles, int count)
{
    int i;

    i = 0;
    while(i < count)
    {
        if (!init_resources(dongles, i, count))
            return 0;
        if (pthread_mutex_init(&dongles[i].mutex, NULL) != 0)
            return (fail_mutex(dongles, i));
        if (pthread_cond_init(&dongles[i].scheduler.cond, NULL) != 0)
            return (fail_cond(dongles, i));
        i++;
    }
    return 1;
}

int init_coders(t_coder *coders, t_config *config, t_dongle *dongles)
{
    int i;

    i = 0;
    while(i < config->number_of_coders)
    {
        coders[i].id = i + 1;
        coders[i].left = &dongles[i];
        coders[i].right = &dongles[(i + 1) % config->number_of_coders];
        coders[i].last_compile_start = 0; /* initialize to simulation start (relative) */
        coders[i].state = STATE_WAITING;
        coders[i].config = config;
        coders[i].burned_out = 0;
        coders[i].all_coders = coders;
        coders[i].compiles_done = 0; /* initialize counter to 0 */
        i++;
    }
    return 1;
}#include "../include/codexion.h"

static void free_allocation(t_coder *coders, t_dongle *dongles)
{
    free(coders);
    free(dongles);
}

static int prs_argument(int ac, char **av, t_config *config)
{
    int x;

    memset(config, 0, sizeof(t_config));
    x = parse_args(ac, av, config);
    if (x != 1)
    {
        if (x == -1)
            printf("Error: Missing argument!\n");
		return (-1);
    }
    return 1;
}

static int initialization(t_coder *coders, t_config *config, t_dongle *dongles)
{
    if (!init_coders(coders, config, dongles))
    {
        free_allocation(coders, dongles);
        return (0);
    }

    if (!init_dongles(dongles, config->number_of_coders))
    {
        free_allocation(coders, dongles);
        return (0);
    }

    if (!creat_thread(coders, config))
    {
        cleanup(config, coders, dongles);
        return (0);
    }
    return 1;
}

int main(int ac, char **av)
{
    t_config config;
    t_dongle *dongles;
    t_coder *coders;

    if (prs_argument(ac, av, &config) != 1)
        return 1;
    config.start_time = get_time_ms();
    dongles = malloc(sizeof(t_dongle) * config.number_of_coders);
    coders = malloc(sizeof(t_coder) * config.number_of_coders);
    config.dongles = dongles; /* NEW: expose dongles array via config */
    if (!dongles || !coders)
    {
        free_allocation(coders, dongles);
        return 1;
    }
    if (!initialization(coders, &config, dongles))
        return 1;
    cleanup(&config, coders, dongles);
    return 0;
}
/* name=src/monitor.c */
#include "../include/codexion.h"

static int check_burnout(t_coder *coders, t_config *config)
{
    int i;
    long now;

    i = 0;
    while (i < config->number_of_coders && !should_stop(config))
    {
        now = get_time_ms() - config->start_time;

        pthread_mutex_lock(&config->state_mutex);
        if (config->stop)
        {
            pthread_mutex_unlock(&config->state_mutex);
            return 0;
        }

        int state = coders[i].state;
        long last_start = coders[i].last_compile_start;
        int burned = coders[i].burned_out;
        pthread_mutex_unlock(&config->state_mutex);

        if (!burned && state == STATE_WAITING)
        {
            if (now - last_start >= config->time_to_burnout)
            {
                pthread_mutex_lock(&config->state_mutex);
                coders[i].burned_out = 1;
                pthread_mutex_unlock(&config->state_mutex);

                set_coder_state(&coders[i], STATE_BURNED_OUT);
                print_burnout(&coders[i]);
                set_simulation_stop(config, 0);
                return -1;
            }
        }
        i++;
    }
    return 0;
}

void *monitor_routine(void *arg)
{
    t_runtime *runtime;
    t_coder *coders;
    t_config *config;

    runtime = (t_runtime *)arg;
    coders = runtime->coders;
    config = runtime->config;
    while (!should_stop(config))
    {
        if (check_burnout(coders, config) != 0)
            break;
        usleep(1000);
    }
    return NULL;
}#include "../include/codexion.h"

static int ft_atoi(char *nb)
{
	int res;

	res = atoi(nb);
	if (res <= 0)
	{
		printf("Error: Invalid argument format\n");
		return -1;
	}
	return res;
}

static int	parse_nb(char *nb)
{
	int		i;

	if (!nb)
		return (-1);
	i = 0;
	if (nb[i] == '+')
		i++;
	if (!nb[i])
		return (-1);
	while (nb[i])
	{
		if (nb[i] < '0' || nb[i] > '9')
		{
			printf("Error: Invalid number '%c'\n", nb[i]);
			return (-1);
		}
		i++;
	}
	return ft_atoi(nb);
}

static char	*parse_scheduler(char *sch)
{
	if (strcmp(sch, "fifo") == 0)
		return (sch);
	if (strcmp(sch, "edf") == 0)
		return (sch);
	printf("Error: Last argument must be 'fifo' or 'edf'\n");
	return (NULL);
}

int	parse_args(int ac, char **av, t_config *config)
{
	int	i;
	int	res;

	if (ac != 9)
		return (-1);
	i = 1;
	while (i < ac - 1)
	{
		res = parse_nb(av[i]);
		if (res == -1)
			return (0);
		i++;
	}
	config->number_of_coders = parse_nb(av[1]);
	config->time_to_burnout = parse_nb(av[2]);
	config->time_to_compile = parse_nb(av[3]);
	config->time_to_debug = parse_nb(av[4]);
	config->time_to_refactor = parse_nb(av[5]);
	config->number_of_compiles_required = parse_nb(av[6]);
	config->dongle_cooldown = parse_nb(av[7]);
	config->scheduler = parse_scheduler(av[8]);
	if (!config->scheduler)
		return (0);
	return (1);
}
#include "../include/codexion.h"

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

void remove_request(t_coder *coder, t_dongle *dongle)
{
    t_heap *heap;
    int index;

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
    if (heap->size > 0)
        fix_heap_request1(coder, heap, index);
    pthread_cond_broadcast(&dongle->scheduler.cond);
    pthread_mutex_unlock(&dongle->mutex);
}/* name=src/scheduler.c */
#include "../include/codexion.h"

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

static long get_request_deadline(t_coder *coder)
{
    return (coder->last_compile_start + coder->config->time_to_burnout);
}

/* Percolate up helper for heap fix */
static void heap_percolate_up(t_coder *coder, t_heap *heap, int index)
{
    int parent;
    while (index > 0)
    {
        parent = (index - 1) / 2;
        if (!request_has_higher_priority(&heap->data[index], &heap->data[parent], coder->config->scheduler))
            break;
        heap_swap(&heap->data[index], &heap->data[parent]);
        index = parent;
    }
}

/* Percolate down helper for heap fix */
static void heap_percolate_down(t_coder *coder, t_heap *heap, int index)
{
    int left_child;
    int right_child;
    int best_child;
    while (1)
    {
        left_child = (index * 2) + 1;
        right_child = left_child + 1;
        best_child = index;
        if (left_child < heap->size && request_has_higher_priority(
            &heap->data[left_child], &heap->data[best_child], coder->config->scheduler))
            best_child = left_child;
        if (right_child < heap->size && request_has_higher_priority(
            &heap->data[right_child], &heap->data[best_child], coder->config->scheduler))
            best_child = right_child;
        if (best_child == index)
            break;
        heap_swap(&heap->data[index], &heap->data[best_child]);
        index = best_child;
    }
}

/* Replace-and-fix helper used by removal */
static void heap_fix_after_replacement(t_coder *coder, t_heap *heap, int index)
{
    if (index > 0)
        heap_percolate_up(coder, heap, index);
    heap_percolate_down(coder, heap, index);
}

/* Remove a request by coder_id while holding the heap's mutex */
static int heap_remove_by_coder(t_heap *heap, int coder_id, t_coder *coder)
{
    int index = 0;
    while (index < heap->size)
    {
        if (heap->data[index].coder_id == coder_id)
            break;
        index++;
    }
    if (index >= heap->size)
        return 0;
    heap->data[index] = heap->data[heap->size - 1];
    heap->size--;
    if (heap->size > 0)
        heap_fix_after_replacement(coder, heap, index);
    return 1;
}

/* Linear search: presence in heap */
static int presence_in_heap(t_heap *heap, int coder_id)
{
    int i = 0;
    while (i < heap->size)
    {
        if (heap->data[i].coder_id == coder_id)
            return 1;
        i++;
    }
    return 0;
}

/* Simple capacity check */
static int capacity_available(t_dongle *left, t_dongle *right)
{
    if (left->scheduler.pending.size == left->scheduler.pending.capacity ||
        right->scheduler.pending.size == right->scheduler.pending.capacity)
        return 0;
    return 1;
}

/* Insert request objects into both heaps while holding both dongle mutexes */
static int insert_both_requests(t_coder *coder, t_dongle *left, t_dongle *right)
{
    t_request req;
    req.coder_id = coder->id;
    req.timestamp = get_time_ms() - coder->config->start_time;
    req.deadline = get_request_deadline(coder);

    if (!heap_push(&left->scheduler.pending, req, coder->config->scheduler) ||
        !heap_push(&right->scheduler.pending, req, coder->config->scheduler))
    {
        /* rollback any partial insert */
        heap_remove_by_coder(&left->scheduler.pending, coder->id, coder);
        heap_remove_by_coder(&right->scheduler.pending, coder->id, coder);
        return 0;
    }
    pthread_cond_broadcast(&left->scheduler.cond);
    pthread_cond_broadcast(&right->scheduler.cond);
    return 1;
}

/* Push a single request into one dongle (used for single-coder case) */
static int push_single_request(t_coder *coder, t_dongle *dongle)
{
    t_request req;
    pthread_mutex_lock(&dongle->mutex);
    /* prevent duplicate */
    for (int i = 0; i < dongle->scheduler.pending.size; i++)
    {
        if (dongle->scheduler.pending.data[i].coder_id == coder->id)
        {
            pthread_mutex_unlock(&dongle->mutex);
            return 1;
        }
    }
    req.coder_id = coder->id;
    req.timestamp = get_time_ms() - coder->config->start_time;
    req.deadline = get_request_deadline(coder);
    if (!heap_push(&dongle->scheduler.pending, req, coder->config->scheduler))
    {
        pthread_mutex_unlock(&dongle->mutex);
        return 0;
    }
    pthread_cond_broadcast(&dongle->scheduler.cond);
    pthread_mutex_unlock(&dongle->mutex);
    return 1;
}

/* Atomically push requests into both dongles (holds both mutexes in id order) */
static int push_two_requests(t_coder *coder, t_dongle *first, t_dongle *second)
{
    t_dongle *left = first;
    t_dongle *right = second;
    if (left->id > right->id)
    {
        left = second;
        right = first;
    }

    pthread_mutex_lock(&left->mutex);
    pthread_mutex_lock(&right->mutex);

    int present_left = presence_in_heap(&left->scheduler.pending, coder->id);
    int present_right = presence_in_heap(&right->scheduler.pending, coder->id);

    if (present_left && present_right)
    {
        pthread_mutex_unlock(&right->mutex);
        pthread_mutex_unlock(&left->mutex);
        return 1;
    }

    if (present_left && !present_right)
        heap_remove_by_coder(&left->scheduler.pending, coder->id, coder);
    if (present_right && !present_left)
        heap_remove_by_coder(&right->scheduler.pending, coder->id, coder);

    if (!capacity_available(left, right))
    {
        pthread_mutex_unlock(&right->mutex);
        pthread_mutex_unlock(&left->mutex);
        return 0;
    }

    if (!insert_both_requests(coder, left, right))
    {
        pthread_mutex_unlock(&right->mutex);
        pthread_mutex_unlock(&left->mutex);
        return 0;
    }

    pthread_mutex_unlock(&right->mutex);
    pthread_mutex_unlock(&left->mutex);
    return 1;
}

/* Public API: request the two dongles (will retry when heaps are full or wait) */
int request_dongles(t_coder *coder, t_dongle *first, t_dongle *second)
{
    t_dongle *left = first;
    t_dongle *right = second;
    if (left->id > right->id)
    {
        left = second;
        right = first;
    }

    /* Single-dongle case */
    if (left == right)
    {
        while (!should_stop(coder->config))
        {
            if (push_single_request(coder, left))
                break;
            usleep(1000);
        }
    }
    else
    {
        while (!should_stop(coder->config))
        {
            if (push_two_requests(coder, left, right))
                break;
            usleep(1000);
        }
    }

    if (should_stop(coder->config))
        return 0;

    if (get_dongles(coder, left, right))
        return 1;

    /* remove queued requests if we failed to get dongles */
    remove_request(coder, left);
    if (left != right)
        remove_request(coder, right);
    return 0;
}

/* Release dongle (public) */
void relese_dongle(t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->mutex);
    dongle->taken = 0;
    dongle->last_released_at = get_time_ms();
    pthread_cond_broadcast(&dongle->scheduler.cond);
    pthread_mutex_unlock(&dongle->mutex);
}#include "../include/codexion.h"

static int routing_for_each_thread(t_coder *coders, t_config *config)
{
    int i;

    i = 0;
    while(i < config->number_of_coders)
    {
        if (pthread_create(&coders[i].thread, NULL, coder_routine, &coders[i]) != 0)
        {
            set_simulation_stop(config, 0);
            while (i > 0)
            {
                i--;
                pthread_join(coders[i].thread, NULL);
            }
            return 0;
        }
        i++;
    }
    i = 0;
    while(i < config->number_of_coders)
    {
        if (pthread_join(coders[i].thread, NULL) != 0)
            return 0;
        i++;
    }
    return 1;
}

int creat_thread(t_coder *coders, t_config *config)
{
    t_runtime runtime;
    pthread_t monitor_thread;

    config->print_mutex_inited = 0;
    config->state_mutex_inited = 0;

    if (pthread_mutex_init(&config->print_mutex, NULL) == 0)
        config->print_mutex_inited = 1;
    else
        return 0;

    if (pthread_mutex_init(&config->state_mutex, NULL) == 0)
        config->state_mutex_inited = 1;
    else
    {
        if (config->print_mutex_inited)
            pthread_mutex_destroy(&config->print_mutex);
        return 0;
    }

    config->stop = 0;
    config->all_done = 0;
    runtime.coders = coders;
    runtime.config = config;

    if (pthread_create(&monitor_thread, NULL, monitor_routine, &runtime) != 0)
    {
        /* Clean up initialized mutexes before returning */
        if (config->state_mutex_inited)
            pthread_mutex_destroy(&config->state_mutex);
        if (config->print_mutex_inited)
            pthread_mutex_destroy(&config->print_mutex);
        return 0;
    }

    if (!routing_for_each_thread(coders, config))
    {
        set_simulation_stop(config, 0);
        pthread_join(monitor_thread, NULL);
        return (0);
    }
    pthread_join(monitor_thread, NULL);
    return 1;
}#include "../include/codexion.h"

long get_time_ms()
{
    struct timeval current;
    if (gettimeofday(&current, NULL) != 0)
        return -1;
    return ((current.tv_sec * 1000) + (current.tv_usec / 1000));
}#include "../include/codexion.h"

void set_coder_state(t_coder *coder, t_state state)
{
    pthread_mutex_lock(&coder->config->state_mutex);
    coder->state = state;
    pthread_mutex_unlock(&coder->config->state_mutex);
}

/* Set stop flag and wake all dongles so waiting threads exit immediately.
   We set the flag under state_mutex and then wake outside the mutex. */
void set_simulation_stop(t_config *config, int all_done)
{
    pthread_mutex_lock(&config->state_mutex);
    config->stop = 1;
    config->all_done = all_done;
    pthread_mutex_unlock(&config->state_mutex);

    if (config->dongles)
        wake_all_dongles(config->dongles, config->number_of_coders);
}

int should_stop(t_config *config)
{
    int stop = 0;
    pthread_mutex_lock(&config->state_mutex);
    stop = config->stop;
    pthread_mutex_unlock(&config->state_mutex);
    return stop;
}

void wake_all_dongles(t_dongle *dongles, int count)
{
    int i;

    i = 0;
    while (i < count)
    {
        pthread_mutex_lock(&dongles[i].mutex);
        pthread_cond_broadcast(&dongles[i].scheduler.cond);
        pthread_mutex_unlock(&dongles[i].mutex);
        i++;
    }
}#include "../include/codexion.h"

void print_status(t_coder *coder, const char *status)
{
    long timestamp;

    pthread_mutex_lock(&coder->config->print_mutex);
    timestamp = get_time_ms() - coder->config->start_time;
    printf("%ld %d %s\n", timestamp, coder->id, status);
    pthread_mutex_unlock(&coder->config->print_mutex);
}

void print_burnout(t_coder *coder)
{
    long timestamp;

    pthread_mutex_lock(&coder->config->print_mutex);
    timestamp = get_time_ms() - coder->config->start_time;
    printf("%ld %d burned out\n", timestamp, coder->id);
    pthread_mutex_unlock(&coder->config->print_mutex);
}