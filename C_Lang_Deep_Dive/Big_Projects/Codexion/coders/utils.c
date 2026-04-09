#include "codexion.h"
#include <unistd.h>

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000LL + tv.tv_usec / 1000);
}

long long	get_timestamp(t_shared *shared)
{
	return (get_time_ms() - shared->start_time);
}

void	precise_sleep(long long ms, t_shared *shared)
{
	long long	target;

	target = get_time_ms() + ms;
	while (get_time_ms() < target)
	{
		if (!shared->running)
			break ;
		usleep(500);
	}
}

void	ft_putnbr_buf(long long n, char *buf, int *pos)
{
	if (n >= 10)
		ft_putnbr_buf(n / 10, buf, pos);
	buf[*pos] = '0' + (n % 10);
	(*pos)++;
}

int	is_sim_running(t_shared *shared)
{
	int	val;

	pthread_mutex_lock(&shared->state_mutex);
	val = shared->running;
	pthread_mutex_unlock(&shared->state_mutex);
	return (val);
}
