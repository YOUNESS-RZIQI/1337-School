#include "codexion.h"
#include <unistd.h>

static void	acquire_both(t_coder *c, t_shared *s)
{
	acquire_dongle(&s->dongles[c->first_id], c, s);
	if (!s->running)
		return ;
	log_message(s, c->number, "has taken a dongle");
	if (c->first_id == c->second_id)
		return ;
	acquire_dongle(&s->dongles[c->second_id], c, s);
	if (!s->running)
	{
		release_dongle(&s->dongles[c->first_id], s);
		return ;
	}
	log_message(s, c->number, "has taken a dongle");
}

static void	release_both(t_coder *c, t_shared *s)
{
	release_dongle(&s->dongles[c->first_id], s);
	if (c->first_id != c->second_id)
		release_dongle(&s->dongles[c->second_id], s);
}

static int	coder_should_stop(t_coder *c, t_shared *s)
{
	if (!s->running)
		return (1);
	if (c->compile_count >= s->args.compiles_req)
		return (1);
	return (0);
}

static void	coder_loop(t_coder *c, t_shared *s)
{
	while (!coder_should_stop(c, s))
	{
		if (c->first_id == c->second_id)
		{
			usleep(1000);
			continue ;
		}
		acquire_both(c, s);
		if (!s->running)
			return ;
		do_compile(c, s);
		release_both(c, s);
		if (!s->running)
			return ;
		do_debug(c, s);
		if (!s->running)
			return ;
		do_refactor(c, s);
	}
}

void	*run_simulation(void *arg)
{
	t_coder		*c;
	t_shared	*s;

	c = (t_coder *)arg;
	s = c->shared;
	c->last_compile_start = s->start_time;
	c->deadline = s->start_time + s->args.ttb;
	coder_loop(c, s);
	return (NULL);
}
