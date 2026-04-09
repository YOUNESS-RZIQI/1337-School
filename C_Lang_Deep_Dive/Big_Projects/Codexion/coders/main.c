#include "codexion.h"
#include <stdlib.h>
#include <unistd.h>

static int	create_threads(t_shared *shared)
{
	int	i;

	i = 0;
	while (i < shared->args.num_coders)
	{
		if (pthread_create(&shared->coders[i].thread, NULL,
				run_simulation, &shared->coders[i]) != 0)
			return (0);
		i++;
	}
	if (pthread_create(&shared->monitor_thread, NULL,
			monitor_routine, shared) != 0)
		return (0);
	return (1);
}

static void	join_threads(t_shared *shared)
{
	int	i;

	pthread_join(shared->monitor_thread, NULL);
	i = 0;
	while (i < shared->args.num_coders)
	{
		pthread_join(shared->coders[i].thread, NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_args		data;
	t_shared	shared;

	data = convert_args(argc, argv);
	if (is_empty_args(data))
	{
		write(2, "Error\n", 6);
		return (1);
	}
	if (!init_simulation(&shared, &data))
	{
		write(2, "Error\n", 6);
		return (1);
	}
	if (!create_threads(&shared))
	{
		cleanup_simulation(&shared);
		write(2, "Error\n", 6);
		return (1);
	}
	join_threads(&shared);
	cleanup_simulation(&shared);
	return (0);
}
