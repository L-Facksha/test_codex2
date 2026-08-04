/* name=src/fail_dongles.c */
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
}