#include "codexion.h"

static void	destroy_dongle_mutexes(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->args.number_of_coders)
	{
		pthread_mutex_destroy(&sim->dongles[i].lock);
		pthread_cond_destroy(&sim->dongles[i].cond);
		i++;
	}
}

// O(1) + 
static void	cleanup_sim(t_simulation *sim, pthread_t *th, short destroy_mutexes)
{
	if (sim->dongles)
	{
		if (destroy_mutexes)
			destroy_dongle_mutexes(sim);
		free(sim->dongles);
	}
	if (sim->coders)
		free(sim->coders);
	if (th)
		free(th);
	if (destroy_mutexes)
	{
		pthread_mutex_destroy(&sim->print_mutex);
		pthread_mutex_destroy(&sim->sim_mutex);
		pthread_cond_destroy(&sim->cond_lock);
	}
}

static short	start_threads(t_simulation *sim, pthread_t *th)
{
	pthread_t	monitor;
	int			i;

	i = 0;
	while (i < sim->args.number_of_coders)
	{
		if (pthread_create(&th[i], NULL, run_simulation, &sim->coders[i]) != 0)
			return (1);
		i++;
	}
	if (pthread_create(&monitor, NULL, run_monitor, sim) != 0)
		return (1);
	pthread_join(monitor, NULL);
	i = 0;
	while (i < sim->args.number_of_coders)
	{
		pthread_join(th[i], NULL);
		i++;
	}
	return (0);
}

static short	initialize_simulation(t_simulation *sim, pthread_t **th)
{
	sim->coders = malloc(sizeof(t_coder) * sim->args.number_of_coders);
	sim->dongles = malloc(sizeof(t_dongle) * sim->args.number_of_coders);
	if (!sim->coders || !sim->dongles)
		return (1);
	*th = malloc(sizeof(pthread_t) * sim->args.number_of_coders);
	if (!*th)
		return (1);
	sim->stop_simulation = 0;
	sim->start_time = 0;
	sim->threads_at_barrier = 0;
	init_dongles(sim);
	init_coders(sim);
	return (0);
}

int	main(int argc, char **argv)
{
	t_simulation	sim;
	pthread_t		*th;
	
	sim.args = convert_args(argc, argv);
	th = NULL;
	if (is_empty_args(sim.args))
		return (1);
	if (initialize_simulation(&sim, &th) != 0)
	{
		cleanup_sim(&sim, th, 0);
		return (1);
	}
	if (initialize_all_mutexes(&sim) == 0)
	{
		start_threads(&sim, th);
		cleanup_sim(&sim, th, 1);
	}
	else
		cleanup_sim(&sim, th, 0);
	return (0);
}


/*

	args.c					|done|
	codexion.h				|done|
	init.c					|done|
	main.c					|done|
	utils.c					|Not done|
	dongle_utils.c			|done|


	dongle.c				|Not done|
	heap.c					|Not done|
	Makefile				|Not done|
	monitor.c				|Not done|
	simulation_utils.c		|Not done|
	simulation.c			|Not done|
	utils_2.c				|Not done|

*/