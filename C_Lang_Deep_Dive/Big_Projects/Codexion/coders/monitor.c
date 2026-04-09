#include "codexion.h"
#include <unistd.h>

static int	check_all_done(t_shared *shared)
{
	int	i;

	i = 0;
	while (i < shared->args.num_coders)
	{
		if (shared->coders[i].compile_count < shared->args.compiles_req)
			return (0);
		i++;
	}
	return (1);
}

static int	check_burnout(t_shared *shared)
{
	int			i;
	long long	now;

	now = get_time_ms();
	i = 0;
	while (i < shared->args.num_coders)
	{
		if (shared->coders[i].deadline > 0
			&& now >= shared->coders[i].deadline
			&& shared->coders[i].compile_count < shared->args.compiles_req)
		{
			log_message(shared, shared->coders[i].number, "burned out");
			return (1);
		}
		i++;
	}
	return (0);
}

static void	wake_all_dongles(t_shared *shared)
{
	int	i;

	i = 0;
	while (i < shared->args.num_coders)
	{
		pthread_mutex_lock(&shared->dongles[i].mutex);
		pthread_cond_broadcast(&shared->dongles[i].cond);
		pthread_mutex_unlock(&shared->dongles[i].mutex);
		i++;
	}
}

void	*monitor_routine(void *arg)
{
	t_shared	*shared;

	shared = (t_shared *)arg;
	while (shared->running)
	{
		if (check_burnout(shared) || check_all_done(shared))
		{
			pthread_mutex_lock(&shared->state_mutex);
			shared->running = 0;
			pthread_mutex_unlock(&shared->state_mutex);
			wake_all_dongles(shared);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}
