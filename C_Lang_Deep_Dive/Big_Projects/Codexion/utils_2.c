#include "codexion.h"

long long	get_current_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

short	null_error_message(void)
{
	write(2, "! ! ! Error (type: NULL return) ! ! !\n", 38);
	return (1);
}

short	input_error_message(void)
{
	write(2, "! ! ! Error in Input ! ! !\n", 28);
	return (1);
}

void	destroy_dongle_mutexes(t_simulation *sim)
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
