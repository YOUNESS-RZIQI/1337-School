#include "codexion.h"
#include <unistd.h>

static void	ft_strcpy_buf(const char *src, char *buf, int *pos)
{
	int	i;

	i = 0;
	while (src[i])
	{
		buf[*pos] = src[i];
		(*pos)++;
		i++;
	}
}

void	log_message(t_shared *shared, int num, const char *msg)
{
	long long	ts;
	char		buf[256];
	int			pos;

	pthread_mutex_lock(&shared->log_mutex);
	if (!shared->running && strcmp(msg, "burned out") != 0)
	{
		pthread_mutex_unlock(&shared->log_mutex);
		return ;
	}
	ts = get_timestamp(shared);
	pos = 0;
	ft_putnbr_buf(ts, buf, &pos);
	buf[pos++] = ' ';
	ft_putnbr_buf((long long)num, buf, &pos);
	buf[pos++] = ' ';
	ft_strcpy_buf(msg, buf, &pos);
	buf[pos++] = '\n';
	write(1, buf, pos);
	pthread_mutex_unlock(&shared->log_mutex);
}
