#include "codexion.h"
#include <stdlib.h>

static int	init_one_dongle(t_dongle *d, int id)
{
	d->id = id;
	d->is_held = 0;
	d->available_at = 0;
	pq_init(&d->queue);
	if (pthread_mutex_init(&d->mutex, NULL) != 0)
		return (0);
	if (pthread_cond_init(&d->cond, NULL) != 0)
	{
		pthread_mutex_destroy(&d->mutex);
		return (0);
	}
	return (1);
}

static int	init_dongles(t_shared *shared)
{
	int	i;
	int	n;

	n = shared->args.num_coders;
	shared->dongles = malloc(sizeof(t_dongle) * n);
	if (!shared->dongles)
		return (0);
	i = 0;
	while (i < n)
	{
		if (!init_one_dongle(&shared->dongles[i], i))
		{
			while (--i >= 0)
			{
				pthread_mutex_destroy(&shared->dongles[i].mutex);
				pthread_cond_destroy(&shared->dongles[i].cond);
			}
			free(shared->dongles);
			return (0);
		}
		i++;
	}
	return (1);
}

static void	init_one_coder(t_shared *shared, int i)
{
	t_coder	*c;

	c = &shared->coders[i];
	c->number = i + 1;
	c->compile_count = 0;
	c->last_compile_start = 0;
	c->deadline = 0;
	c->left_id = get_left_dongle_id(c);
	c->right_id = get_right_dongle_id(c, shared->args.num_coders);
	if (c->left_id <= c->right_id)
	{
		c->first_id = c->left_id;
		c->second_id = c->right_id;
	}
	else
	{
		c->first_id = c->right_id;
		c->second_id = c->left_id;
	}
	c->shared = shared;
}

static int	init_coders(t_shared *shared)
{
	int	i;

	shared->coders = malloc(sizeof(t_coder) * shared->args.num_coders);
	if (!shared->coders)
		return (0);
	i = 0;
	while (i < shared->args.num_coders)
	{
		init_one_coder(shared, i);
		i++;
	}
	return (1);
}

int	init_simulation(t_shared *shared, t_args *args)
{
	shared->args = *args;
	shared->running = 1;
	shared->finished_count = 0;
	shared->dongles = NULL;
	shared->coders = NULL;
	if (pthread_mutex_init(&shared->log_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&shared->state_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&shared->log_mutex);
		return (0);
	}
	if (!init_dongles(shared) || !init_coders(shared))
	{
		cleanup_simulation(shared);
		return (0);
	}
	shared->start_time = get_time_ms();
	return (1);
}
