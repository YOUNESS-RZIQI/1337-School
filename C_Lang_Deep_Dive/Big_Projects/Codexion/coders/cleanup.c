#include "codexion.h"
#include <stdlib.h>

static void	cleanup_dongles(t_shared *shared)
{
	int	i;

	if (!shared->dongles)
		return ;
	i = 0;
	while (i < shared->args.num_coders)
	{
		pthread_mutex_destroy(&shared->dongles[i].mutex);
		pthread_cond_destroy(&shared->dongles[i].cond);
		i++;
	}
	free(shared->dongles);
	shared->dongles = NULL;
}

void	cleanup_simulation(t_shared *shared)
{
	cleanup_dongles(shared);
	if (shared->coders)
	{
		free(shared->coders);
		shared->coders = NULL;
	}
	pthread_mutex_destroy(&shared->log_mutex);
	pthread_mutex_destroy(&shared->state_mutex);
}
