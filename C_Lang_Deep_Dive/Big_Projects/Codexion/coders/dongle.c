#include "codexion.h"
#include <unistd.h>

int	get_left_dongle_id(t_coder *coder)
{
	return (coder->number - 1);
}

int	get_right_dongle_id(t_coder *coder, int total)
{
	return (coder->number % total);
}

static struct timespec	get_timeout(long long ms_from_now)
{
	struct timespec	ts;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + (ms_from_now / 1000);
	ts.tv_nsec = tv.tv_usec * 1000 + (ms_from_now % 1000) * 1000000;
	if (ts.tv_nsec >= 1000000000)
	{
		ts.tv_sec++;
		ts.tv_nsec -= 1000000000;
	}
	return (ts);
}

static long long	get_sort_key(t_coder *c, t_shared *s)
{
	if (s->args.scheduler == CDX_EDF)
		return (c->deadline);
	return (get_time_ms());
}

void	acquire_dongle(t_dongle *d, t_coder *c, t_shared *s)
{
	t_request		req;
	struct timespec	tout;

	req.coder_id = c->number;
	req.sort_key = get_sort_key(c, s);
	pthread_mutex_lock(&d->mutex);
	pq_push(&d->queue, req);
	while (s->running)
	{
		if (!d->is_held && get_time_ms() >= d->available_at
			&& !pq_is_empty(&d->queue)
			&& d->queue.data[0].coder_id == c->number)
			break ;
		tout = get_timeout(5);
		pthread_cond_timedwait(&d->cond, &d->mutex, &tout);
	}
	if (!pq_is_empty(&d->queue)
		&& d->queue.data[0].coder_id == c->number)
		pq_pop(&d->queue);
	d->is_held = 1;
	pthread_mutex_unlock(&d->mutex);
}
