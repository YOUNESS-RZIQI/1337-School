#include "codexion.h"

void	release_dongle(t_dongle *d, t_shared *s)
{
	pthread_mutex_lock(&d->mutex);
	d->is_held = 0;
	d->available_at = get_time_ms() + s->args.cooldown;
	pthread_cond_broadcast(&d->cond);
	pthread_mutex_unlock(&d->mutex);
}
